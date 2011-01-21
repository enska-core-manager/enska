<?php

/**
 *Copyright (C) 2008 - 2011  EnsKa-System.
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @category   Model
 * @package    models
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */
 
DEFINE ('MENU', 1);
DEFINE ('BROWSER', 2);
DEFINE ('BROWSERANDMENU', 3);
DEFINE ('NONE', 4);

Function loadBrowser($data=null)
{
	GLOBAL $_alias;
	
	$requestNode = "";
	
	if (isset($_REQUEST["node"])) {
		$requestedNode = $_REQUEST["node"];
	}

	$nodes = new EnsKa_EXTTreeNodes();
	
	if (strcmp($requestedNode, 'node-root') == 0) {
	
		$paramsBrowser = Array(
				'id' 		=> 		'node-browser',
				'text' 		=> 		'Websites',
				'iconCls' 	=> 		'tree-browser'
		);
		
		$paramsPlugins = Array(
				'id' 		=> 		'node-plugins',
				'text' 		=> 		'Plug-ins',
				'iconCls' 	=> 		'tree-plugins'
		);
		
		$nodes->add($paramsBrowser);
		$nodes->add($paramsPlugins);
		unset($paramsBrowser, $paramsPlugins);
	}
		
	if (strcmp($requestedNode, 'node-browser') == 0) {
	
		$req = dbSelect('pb.site_infos', Array('dpl_name', 'id_name'));
		
		foreach($req as $node)
		{			
			$params = Array(
				'id' 		=> 		$node['id_name'],
				'text' 		=> 		$node['dpl_name'],
				'iconCls' 	=> 		'tree-website',
				'leaf'		=>		true
			);
			
			$nodes->add($params);
			unset($params);
		}
	}
		
	if (strcmp($requestedNode, 'node-plugins') == 0) {
		
		$req = dbSelect('pb.site_infos', Array('dpl_name', 'id_name'));

		foreach($req as $node) {
			
			$path = $_alias['path']['global']['point'].$_alias['path']['local']['path_plugsViews'].$node['id_name'].'/';
			$dirs = get_folder_list($path);
			
			if (is_array($dirs)) {
				foreach($dirs as $dir) {
					if (file_exists($path.$dir.'/'.'settings.ini')) {
						$settings = new EnsKa_INIReader($path.$dir.'/'.'settings.ini');
						if (($settings->getLine('display') == BROWSER) || ($settings->getLine('display') == BROWSERANDMENU)) {
							$params = Array(
								'id' 		=> 		$settings->getLine('id'),
								'text' 		=> 		$settings->getLine('name'),
								'iconCls' 	=> 		'tree-plugin',
								'leaf'		=>		true
							);
							
							$nodes->add($params);
							unset($params);
						}
						unset($settings);
					}
				}
			}
		}
	}
	
	return ($nodes->toJson());
}

Function loadAvailableUsersOptions($data=null)
{
	$requestNode = "";
	
	if (isset($_REQUEST["node"]))
	{
		$requestedNode = $_REQUEST["node"];
	}

	$nodes = new EnsKa_EXTTreeNodes();
	
	if (strcmp($requestedNode, 'node-root') == 0)
	{
		$req = getUserProperties();
		
		foreach($req as $node)
		{
			$params = Array(
				'id' 		=> 		$node.'-id',
				'text' 		=> 		$node,
				'iconCls' 	=> 		'tree-tag-blue',
				'leaf'		=>		true
			);
			
			$nodes->add($params);
			unset($params);
		}
	}
	
	return ($nodes->toJson());
}

Function loadTreeRulesParams($data=null) 
{
	$nodes = new EnsKa_EXTTreeNodes();
	$req = getComplexArray(dbSelect('pb.params_rules', Array('rule'), Array('id' => $data)));	
	
	if (is_array($req)) {
		$rules = unserialize($req['rule']);
		foreach($rules as $rule) {
			$params = Array
					(
						'id' 		=> 		$rule['id'],
						'text' 		=> 		'<b>'.$rule['field'].':</b><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$rule['value'],
						'iconCls' 	=> 		'tree-rule',
						'leaf'		=>		true
					);
			$nodes->add($params);
			unset($params);
		}
	}
	return ($nodes->toJson());
}

Function loadGroups($data=null)
{
	$requestNode = "";
	$nodes = new EnsKa_EXTTreeNodes();
	
	if ($data == null) {
		if (isset($_REQUEST["node"])) {
			$requestedNode = $_REQUEST["node"];
		}

		if (strcmp($requestedNode, 'node-root') == 0) {
			$paramsGroups = Array(
					'id' 		=> 		'node-groups',
					'text' 		=> 		'Groups',
					'iconCls' 	=> 		'tree-groups'
			);
			
			$nodes->add($paramsGroups);
			unset($paramsGroups);
		}
		
		if (strcmp($requestedNode, 'node-groups') == 0) {
			
			$params = Array(
					'id' 		=> 		'gid-0-all',
					'text' 		=> 		'All groups',
					'iconCls' 	=> 		'tree-group',
					'leaf'		=>		true,
					'draggable'	=>		true
			);
			$nodes->add($params);
			unset($params);
			
			$req = dbSelect('pb.groups_infos', Array('gid', 'name'));
			
			foreach($req as $node) {
				$params = Array(
					'id' 		=> 		'gid-'.$node['gid'].'-'.$node['name'],
					'text' 		=> 		$node['name'],
					'iconCls' 	=> 		'tree-group',
					'leaf'		=>		true,
					'draggable'	=>		true
				);
				
				$nodes->add($params);
				unset($params);
			}
		}
	}
	
	return ($nodes->toJson());
}
	
Function loadUsers($data=null) 
{
	$requestNode = "";
	$nodes = new EnsKa_EXTTreeNodes();
	
	if ($data == null) {
		if (isset($_REQUEST["node"])) {
			$requestedNode = $_REQUEST["node"];
		}

		if (strcmp($requestedNode, 'node-root') == 0) {
			$paramsUsers = Array(
					'id' 		=> 		'node-users',
					'text' 		=> 		'Users',
					'iconCls' 	=> 		'tree-users'
			);
			
			$paramsGroups = Array(
					'id' 		=> 		'node-groups',
					'text' 		=> 		'Groups',
					'iconCls' 	=> 		'tree-groups'
			);
			
			$nodes->add($paramsUsers);
			$nodes->add($paramsGroups);
			unset($paramsUsers, $paramsGroups);
		}
		
		if (strcmp($requestedNode, 'node-users') == 0) {
			$params = Array(
					'id' 		=> 		'uid-0-all',
					'text' 		=> 		'All users',
					'iconCls' 	=> 		'tree-group',
					'leaf'		=>		true,
					'draggable'	=>		true
			);
			$nodes->add($params);
			unset($params);
			
			$req = dbSelect('pb.users_auth', Array('uid', 'login'));	
			
			foreach($req as $node) {
				$params = Array(
					'id' 		=> 		'uid-'.$node['uid'].'-'.$node['login'],
					'text' 		=> 		$node['login'],
					'iconCls' 	=> 		'tree-user',
					'leaf'		=>		true,
					'draggable'	=>		true
				);
				
				$nodes->add($params);
				unset($params);
			}
		}
		
		if (strcmp($requestedNode, 'node-groups') == 0) {
			
			$params = Array(
					'id' 		=> 		'gid-0-all',
					'text' 		=> 		'All groups',
					'iconCls' 	=> 		'tree-group',
					'leaf'		=>		true,
					'draggable'	=>		true
			);
			$nodes->add($params);
			unset($params);
			
			$req = dbSelect('pb.groups_infos', Array('gid', 'name'));
			
			foreach($req as $node) {
				$params = Array(
					'id' 		=> 		'gid-'.$node['gid'].'-'.$node['name'],
					'text' 		=> 		$node['name'],
					'iconCls' 	=> 		'tree-group',
					'leaf'		=>		true,
					'draggable'	=>		true
				);
				
				$nodes->add($params);
				unset($params);
			}
		}
	}
	else {
		$dat = explode(';', $data);
		
		if ($dat[1] == 1) {
			$req = getComplexArray(dbSelect($dat[2], Array('rights'), Array('id' => $dat[0])));
			$authorization = unserialize($req['rights']);
		
			if (isset($authorization['uid']['auth'])) {
				$UIDauth = $authorization['uid']['auth'];
				foreach ($UIDauth as $val) {
					if ($val != 0) {
						$login = EnsKa_Users::get()->getUserLogin($val);
						$params = Array(
							'id' 		=> 		'uid-'.$val.'-'.$login,
							'text' 		=> 		$login,
							'iconCls' 	=> 		'tree-user',
							'leaf'		=>		true,
							'draggable'	=>		true
						);
					}
					else {
						$login = 'All users';
						$params = Array(
							'id' 		=> 		'uid-0-'.$login,
							'text' 		=> 		$login,
							'iconCls' 	=> 		'tree-group',
							'leaf'		=>		true,
							'draggable'	=>		true
						);
					}
					
					$nodes->add($params);
					unset($params);
				}
			}
			
			if (isset($authorization['gid']['auth'])) {
				$GIDauth = $authorization['gid']['auth'];
				foreach ($GIDauth as $val) {
					if ($val != 0) {
						$name = EnsKa_Groups::get()->getGroupName($val);
						$params = Array(
							'id' 		=> 		'gid-'.$val.'-'.$name,
							'text' 		=> 		$name,
							'iconCls' 	=> 		'tree-group',
							'leaf'		=>		true,
							'draggable'	=>		true
						);
					}
					else {
						$name = 'All groups';
						$params = Array(
							'id' 		=> 		'gid-0-'.$name,
							'text' 		=> 		$name,
							'iconCls' 	=> 		'tree-group',
							'leaf'		=>		true,
							'draggable'	=>		true
						);
					}
					
					$nodes->add($params);
					unset($params);
				}
			}
		}
		elseif ($dat[1] == 2) {
			$req = getComplexArray(dbSelect($dat[2], Array('rights'), Array('id' => $dat[0])));
			$authorization = unserialize($req['rights']);
			
			if (isset($authorization['uid']['unAuth'])) {
				$UIDunAuth = $authorization['uid']['unAuth'];
				foreach ($UIDunAuth as $val) {
					if ($val != 0) {
						$login = EnsKa_Users::get()->getUserLogin($val);
						$params = Array(
							'id' 		=> 		'uid-'.$val.'-'.$login,
							'text' 		=> 		$login,
							'iconCls' 	=> 		'tree-user',
							'leaf'		=>		true,
							'draggable'	=>		true
						);
					}
					else {
						$login = 'All users';
						$params = Array(
							'id' 		=> 		'uid-0-'.$login,
							'text' 		=> 		$login,
							'iconCls' 	=> 		'tree-group',
							'leaf'		=>		true,
							'draggable'	=>		true
						);
					}
					
					$nodes->add($params);
					unset($params);
				}
			}
			
			if (isset($authorization['gid']['unAuth'])) {
				$GIDunAuth = $authorization['gid']['unAuth'];
				foreach ($GIDunAuth as $val) {
					if ($val != 0) {
						$name = EnsKa_Groups::get()->getGroupName($val);
						$params = Array(
							'id' 		=> 		'gid-'.$val.'-'.$name,
							'text' 		=> 		$name,
							'iconCls' 	=> 		'tree-group',
							'leaf'		=>		true,
							'draggable'	=>		true
						);
					}
					else {
						$name = 'All groups';
						$params = Array(
							'id' 		=> 		'gid-0-'.$name,
							'text' 		=> 		$name,
							'iconCls' 	=> 		'tree-group',
							'leaf'		=>		true,
							'draggable'	=>		true
						);
					}
					
					$nodes->add($params);
					unset($params);
				}
			}
		}
		elseif (strcmp($dat[1],'getUserGroups') == 0) {
			$req = dbSelect('pb.users_groups', Array('gid'), Array('AND' => Array('uid' => $dat[0], 'status' => 'ok')));
			
			foreach ($req as $group) {
				$groupName = EnsKa_Groups::get()->getGroupName($group['gid']);
				$params = Array(
					'id' 		=> 		'gid-'.$group['gid'].'-'.$groupName,
					'text' 		=> 		$groupName,
					'iconCls' 	=> 		'tree-group',
					'leaf'		=>		true,
					'draggable'	=>		true
				);
				
				$nodes->add($params);
				unset($params);
			}
		}
	}
	
	return ($nodes->toJson());
}

Function loadAppsUsers($data=null)
{
	GLOBAL $_alias;
	
	$requestNode = "";
	$nodes = new EnsKa_EXTTreeNodes();

	if (isset($_REQUEST["node"])) {
		$requestedNode = $_REQUEST["node"];
	}

	/*
	$dat[]
	|----
		| 0 => app
		| 1 => website
		| 2 => 1->AUTH ; 2->UNAUTH
	*/
	
	$dat = explode(';', $data);
	$settings = new EnsKa_INIReader($_alias['path']['global']['point'].$_alias['path']['global']['sites'].$dat[1].'/'.get_app_path($dat[1]).$dat[0].'/app.settings.ini');
	$authorization = unserialize(strReplace($settings->getLine('rights'), '\\"', '"'));
	unset($settings);
	
	if ($dat[2] == 1) {
		if (isset($authorization['uid']['auth'])) {
			$UIDauth = $authorization['uid']['auth'];
			foreach ($UIDauth as $val) {
				if ($val != 0) {
					$login = EnsKa_Users::get()->getUserLogin($val);
					$params = Array(
						'id' 		=> 		'uid-'.$val.'-'.$login,
						'text' 		=> 		$login,
						'iconCls' 	=> 		'tree-user',
						'leaf'		=>		true,
						'draggable'	=>		true
					);
				}
				else {
					$login = 'All users';
					$params = Array(
						'id' 		=> 		'uid-0-'.$login,
						'text' 		=> 		$login,
						'iconCls' 	=> 		'tree-group',
						'leaf'		=>		true,
						'draggable'	=>		true
					);
				}
				
				$nodes->add($params);
				unset($params);
			}
		}
		
		if (isset($authorization['gid']['auth'])) {
			$GIDauth = $authorization['gid']['auth'];
			foreach ($GIDauth as $val) {
				if ($val != 0) {
					$name = EnsKa_Groups::get()->getGroupName($val);
					$params = Array(
						'id' 		=> 		'gid-'.$val.'-'.$name,
						'text' 		=> 		$name,
						'iconCls' 	=> 		'tree-group',
						'leaf'		=>		true,
						'draggable'	=>		true
					);
				}
				else {
					$name = 'All groups';
					$params = Array(
						'id' 		=> 		'gid-0-'.$name,
						'text' 		=> 		$name,
						'iconCls' 	=> 		'tree-group',
						'leaf'		=>		true,
						'draggable'	=>		true
					);
				}
				
				$nodes->add($params);
				unset($params);
			}
		}
	}
	elseif ($dat[2] == 2) {
		if (isset($authorization['uid']['unAuth'])) {
			$UIDunAuth = $authorization['uid']['unAuth'];
			foreach ($UIDunAuth as $val) {
				if ($val != 0) {
					$login = EnsKa_Users::get()->getUserLogin($val);
					$params = Array(
						'id' 		=> 		'uid-'.$val.'-'.$login,
						'text' 		=> 		$login,
						'iconCls' 	=> 		'tree-user',
						'leaf'		=>		true,
						'draggable'	=>		true
					);
				}
				else {
					$login = 'All users';
					$params = Array(
						'id' 		=> 		'uid-0-'.$login,
						'text' 		=> 		$login,
						'iconCls' 	=> 		'tree-group',
						'leaf'		=>		true,
						'draggable'	=>		true
					);
				}
				
				$nodes->add($params);
				unset($params);
			}
		}
		
		if (isset($authorization['gid']['unAuth'])) {
			$GIDunAuth = $authorization['gid']['unAuth'];
			foreach ($GIDunAuth as $val) {
				if ($val != 0) {
					$name = EnsKa_Groups::get()->getGroupName($val);
					$params = Array(
						'id' 		=> 		'gid-'.$val.'-'.$name,
						'text' 		=> 		$name,
						'iconCls' 	=> 		'tree-group',
						'leaf'		=>		true,
						'draggable'	=>		true
					);
				}
				else {
					$name = 'All groups';
					$params = Array(
						'id' 		=> 		'gid-0-'.$name,
						'text' 		=> 		$name,
						'iconCls' 	=> 		'tree-group',
						'leaf'		=>		true,
						'draggable'	=>		true
					);
				}
				
				$nodes->add($params);
				unset($params);
			}
		}
	}
	
	return ($nodes->toJson());
}

Function loadAUTHUsers($data=null)
{
	$requestNode = "";
	$nodes = new EnsKa_EXTTreeNodes();
	
	if (isset($_REQUEST["node"]))
	{
		$requestedNode = $_REQUEST["node"];
	}

	if (strcmp($requestedNode, 'node-root') == 0)
	{
		$params = Array(
				'id' 		=> 		'node-authUsers',
				'text' 		=> 		'Users',
				'iconCls' 	=> 		'tree-users'
			);
			
		$nodes->add($params);
		unset($params);
	}
	else
	{
		$type = explode('-', $requestedNode);
		if (strcmp($type[0], 'uid') == 0)
		{
			$params = Array(
				'id' 		=> 		$requestedNode,
				'text' 		=> 		$type[2],
				'iconCls' 	=> 		'tree-user',
				'leaf'		=>		true,
				'draggable'	=>		true
			);
			
			$nodes->add($params);
			unset($params);
		}
		else if (strcmp($type[0], 'gid') == 0)
		{
			$params = Array(
				'id' 		=> 		$requestedNode,
				'text' 		=> 		$type[2],
				'iconCls' 	=> 		'tree-users',
				'leaf'		=>		true,
				'draggable'	=>		true
			);
			
			$nodes->add($params);
			unset($params);
		}
	}
	
	return ($nodes->toJson());
}

Function loadUNAUTHUsers($data=null)
{
	$requestNode = "";
	$nodes = new EnsKa_EXTTreeNodes();
	
	if (isset($_REQUEST["node"]))
	{
		$requestedNode = $_REQUEST["node"];
	}

	if (strcmp($requestedNode, 'node-root') == 0)
	{
		$params = Array(
				'id' 		=> 		'node-unAuthUsers',
				'text' 		=> 		'Users',
				'iconCls' 	=> 		'tree-users'
			);
			
		$nodes->add($params);
		unset($params);
	}
	else
	{
		$type = explode('-', $requestedNode);
		if (strcmp($type[0], 'uid') == 0)
		{
			$params = Array(
				'id' 		=> 		$requestedNode,
				'text' 		=> 		$type[2],
				'iconCls' 	=> 		'tree-user',
				'leaf'		=>		true,
				'draggable'	=>		true
			);
			
			$nodes->add($params);
			unset($params);
		}
		else if (strcmp($type[0], 'gid') == 0)
		{
			$params = Array(
				'id' 		=> 		$requestedNode,
				'text' 		=> 		$type[2],
				'iconCls' 	=> 		'tree-users',
				'leaf'		=>		true,
				'draggable'	=>		true
			);
			
			$nodes->add($params);
			unset($params);
		}
	}
	
	return ($nodes->toJson());
}

?>