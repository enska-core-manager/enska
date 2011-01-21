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
 * @category   Engine
 * @package    core
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

class EnsKa_ACL
{	
	const	ROLE = 1;
	const	SITE = 2;
	const	APPS = 3;
	
	Private	static	$_instance;
	
	Public
		static Function get()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
			}
			
			return self::$_instance;
		}
		
		Function checkACL($role, $mod=self::ROLE)
		{
			if (!is_array($role)) {
				$role = Array('role' => $role, 'type' => 'uid', 'id' => EnsKa_Sessions::get()->getSession('uid'));
			}
			
			if (strcmp($role['type'], 'uid') == 0) {
				if (EnsKa_Rights::get()->is_root($role['id'])) {
					return (true);
				}
			}
			
			$rights = @unserialize(self::getRights($role['role'], $mod));
			$right = false;
			
			if(isset($rights['gid'])) {
				$tab = $rights['gid'];
				if(isset($tab['auth'])) {
					foreach($tab['auth'] as $rid) {
						if (strcmp($role['type'], 'uid') == 0) {
							if ($rid == 0) {
								$right = true;
								break;
							}
							if (EnsKa_Rights::get()->is_in_group($role['id'], $rid)) {
								$right = true;
								break;
							}
						}
						elseif (strcmp($role['type'], 'gid') == 0) {
							if ($rid == 0) {
								$right = true;
								break;
							}
							if ($rid == $role['id']) {
								$right = true;
								break;
							}
						}
						else {
							return (false);
						}
					}
				}
				
				if(isset($tab['unAuth'])) {
					foreach($tab['unAuth'] as $rid) {
						if (strcmp($role['type'], 'uid') == 0) {
							if ($rid == 0) {
								$right = false;
								if (isset($tab['auth'])) {
									foreach($tab['auth'] as $ridTmp) {
										if (strcmp($role['type'], 'uid') == 0) {
											if (EnsKa_Rights::get()->is_in_group($role['id'], $ridTmp)) {
												$right = true;
												break;
											}
										}
										elseif (strcmp($role['type'], 'gid') == 0) {
											if ($ridTmp == $role['id']) {
												$right = true;
												break;
											}
										}
										else {
											return (false);
										}
									}
								}
							}
							elseif(EnsKa_Rights::get()->is_in_group($role['id'], $rid)) {
								$right = false;
								foreach($tab['auth'] as $ridTmp) {
									if (strcmp($role['type'], 'uid') == 0) {
										if (EnsKa_Rights::get()->is_in_group($role['id'], $ridTmp)) {
											$right = true;	
											break;
										}
									}
								}
							}
						}
						elseif (strcmp($role['type'], 'gid') == 0) {
							if ($rid == $role['id']) {
								$right = false;
								break;
							}
						}
						else {
							return (false);
						}
					}
				}
			}
			
			if(isset($rights['uid'])) {
				$tab = $rights['uid'];
				if(isset($tab['auth'])) {
					foreach($tab['auth'] as $rid) {
						if (strcmp($role['type'], 'uid') == 0) {
							if ($rid == 0) {
								$right = true;
								break;
							}
							if($rid == $role['id']) {
								$right = true;
								break;
							}
						}
						else {
							break;
						}
					}
				}
				
				if(isset($tab['unAuth'])) {
					foreach($tab['unAuth'] as $rid) {
						if (strcmp($role['type'], 'uid') == 0) {
							if ($rid == 0) {
								$right = false;
								break;
							}
							if($rid == $role['id']) {
								$right = false;
								break;
							}
						}
						else {
							break;
						}
					}
				}
			}
			return ($right);
		}
		
	Private
		Function __construct()
		{
			/*Constructor*/
		}
		
		Function __destruct()
		{
			/*Destructor*/
		}
		
		Function getRights($role, $mod=self::ROLE)
		{
			GLOBAL $_alias;
			
			try
			{
				if ($mod == self::ROLE) {
					$req = dbSelect('pb.acl_infos', Array('rights'), Array('role' => $role));
					
					if ($req) {
						$res = getComplexArray($req);
						return($res['rights']);
					}
					else {
						throw new EnsKa_Exception('Unable to get the rights for role', '0x1101', E_NOTICE);
					}
				}
				elseif ($mod == self::SITE) {
					$req = getComplexArray(dbSelect('pb.site_infos', Array('id'), Array('id_name' => $role)));
					$req = dbSelect('pb.site_auth', Array('rights'), Array('id' => $req['id']));
					
					if ($req) {
						$res = getComplexArray($req);
						return($res['rights']);
					}
					else {
						throw new EnsKa_Exception('Unable to get the rights for role', '0x1102', E_NOTICE);
					}
				}
				elseif ($mod == self::APPS) {
					$pathApps = get_app_path($_alias['site']);
					$pathApp = $_alias['path']['global']['point'].$_alias['path']['global']['sites'].$_alias['site'].'/'.$pathApps.$_alias['controller'].'/';
					$settings = new EnsKa_INIReader($pathApp.'app.settings.ini');
					if ($settings->getError()) {
						throw new EnsKa_Exception('Unable to get the rights for role', '0x1103', E_NOTICE);
					}
					return(strReplace($settings->getLine('rights'), '\\"', '"'));
				}
			}
			catch(EnsKa_Exception $e)
			{
				return (-1);
			}
		}
}

?>