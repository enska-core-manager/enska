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

DEFINE ('PANEL', 1);
DEFINE ('WINDOW', 2);
DEFINE ('MOD', 3);

Function is_available_framework($frameworkID)
{
	$req = dbSelect('pb.frameworks_infos', Array('id'), Array('id' => $frameworkID));
	
	if ((!$req) || ($req == -1)) {
		return (false);
	}
	
	return (true);
}

Function is_website_exists($idName)
{
	$req = dbSelect('pb.site_infos', Array('id'), Array('id_name' => $idName));
	
	if ((!$req) || ($req == -1)) {
		return (true);
	}
	
	return (false);
}

Function is_website_exists_rev($idName)
{
	$req = dbSelect('pb.site_infos', Array('id'), Array('id_name' => $idName));
	
	if ((!$req) || ($req == -1)) {
		return (false);
	}
	
	return (true);
}

Function is_exists_group_rev($name)
{
	if (trim($name) == '') {
		return (true);
	}
	
	$req = dbSelect('pb.groups_infos', Array('gid'), Array('name' => $name));
	
	if ((!$req) || ($req == -1)) {
		return (false);
	}
	
	return (true);
}

Function is_sys_group($gid)
{
	$req = dbSelect('pb.groups_infos', Array('gid'), Array('AND' => Array('gid' => $gid, 'gType' => 'sys')));
	
	if ((!$req) || ($req == -1)) {
		return (false);
	}
	
	return (true);
}

Function is_rule_exists_rev($name)
{
	$req = dbSelect('pb.rules_params', Array('id'), Array('name' => $name));
	
	if ($req) {
		return (false);
	}
	return (true);
}

Function is_acl_exists($roleName)
{
	$req = dbSelect('pb.acl_infos', Array('id'), Array('role' => $roleName));
	
	if ((!$req) || ($req == -1)) {
		return (true);
	}
	
	return (false);
}

Function is_user_exists($login)
{
	$req = dbSelect('pb.users_auth', Array('id'), Array('login' => $login));
	
	if ((!$req) || ($req == -1)) {
		return (true);
	}
	
	return (false);
}

Function is_plugin_exists($plgName, $website)
{
	GLOBAL $_alias;
	
	if (is_dir($_alias['path']['global']['point'].$_alias['path']['local']['path_plugsViews'].$website.'/'.$plgName.'/')) {
		return (true);
	}
	
	return (false);
}

Function is_app_exists($appName, $website)
{
	GLOBAL $_alias;
	
	if (is_dir($_alias['path']['global']['point'].$_alias['path']['global']['sites'].$website.'/'.get_app_path($website).$appName.'/')) {
		return (true);
	}
	
	return (false);
}

Function loadPlgs($view, $type)
{
	GLOBAL $_controller;
	
	$req = dbSelect('pb.site_infos', Array('dpl_name', 'id_name'));

	foreach($req as $node) {
		GLOBAL $_alias;
		
		$path = $_alias['path']['global']['point'].$_alias['path']['local']['path_apps'].$_alias['controller']."/views/plugs/".$node['id_name'].'/';
		$dirs = get_folder_list($path);
		
		if (is_array($dirs)) {
			foreach($dirs as $dir) {
				$view->addView('plugs/'.$node['id_name'].'/'.$dir.'/'.$dir.'-'.$type);
			}
		}
	}
}

Function Compress()
{
	GLOBAL $_controller;
	
	$req = dbSelect('pb.site_infos', Array('dpl_name', 'id_name'));
	$fileTypes = Array('mod', 'win');
	
	foreach($fileTypes as $fileType) {
		foreach($req as $node) {
			GLOBAL $_alias;
			
			$path = $_alias['path']['global']['point'].$_alias['path']['local']['path_apps'].$_alias['controller']."/views/plugs/".$node['id_name'].'/';
			$dirs = get_folder_list($path);
			
			if (is_array($dirs)) {
				foreach($dirs as $dir) {

					$src = $_alias['path']['global']['point'].$_alias['path']['local']['path_apps'].$_alias['controller']."/views/plugs/manager/".$dir.'/'.$dir.'-'.$fileType.'.phtml';
					
					$fs = fopen($src, 'r');
					if (!$fs){
						echo 'NO SOURCE<br>';
					}
					
					$buff = '';
					while (!(feof($fs))) {
						$buff .= trim(fgets($fs));
					}
					fclose($fs);

					if (!($fd = fopen($src, 'w'))) {
						echo 'NO DEST<br>';
					}

					fputs($fd, $buff);
					fclose($fd);
				}
			}
		}
	}
}

Function loadSitesGrid($data=null)
{
	$req = dbSelect('pb.site_infos');
	
	if (is_array($req)) {
		$store = new EnsKa_DataStore($req);
		echo '({"total":"'.count($store->getDatas()).'","results":'.json_encode($store->getDatas()).'})';
		return;
	}
	
	echo '({"total":"0", "results":""})';
}

Function loadFrameworksGrid($data=null)
{
	$req = dbSelect('pb.frameworks_infos');
	
	if (is_array($req)) {
		$store = new EnsKa_DataStore($req);
		echo '({"total":"'.count($store->getDatas()).'","results":'.json_encode($store->getDatas()).'})';
		return;
	}
	
	echo '({"total":"0", "results":""})';
}

Function loadWServicesSelect($data=null)
{
	$req = dbSelect('pb.ws_infos', Array('id', 'name'));
	
	if (is_array($req)) {
		$tmps = $req;
		$i = 0;
		foreach($tmps as $tmp) {
			$res[$i]['id'] = $tmp['id'].';2';
			$res[$i++]['name'] = $tmp['name'];
			$res[$i]['id'] = $tmp['id'].';1';
			$res[$i++]['name'] = $tmp['name'].' - With keys';
		}
		
		$store = new EnsKa_DataStore($res);
		echo "{ items: ".json_encode($store->getDatas())." }";
	}
}

Function loadWServicesGrid($data=null)
{
	GLOBAL $_alias;
	$req = dbSelect('pb.ws_infos');
	
	$nb = count($req);
	$i = 0;
	while ($i < $nb) {
		$update = new EnsKa_INIReader($_alias['path']['global']['point'].$_alias['path']['global']['path_webservices'].$req[$i]['website'].'/'.$req[$i]['name'].'/ws/ws.update.ini');
		if (!$update->getError()) {
			$req[$i]['version'] = $update->getLine('version');
			$req[$i]['revision'] = $update->getLine('versionId');
		}
		else {
			echo '({"total":"0", "results":""})';
			return;
		}
		$i++;
	}
	
	if (is_array($req)) {
		$store = new EnsKa_DataStore($req);
		echo '({"total":"'.count($store->getDatas()).'","results":'.json_encode($store->getDatas()).'})';
		return;
	}
	echo '({"total":"0", "results":""})';
}

Function loadWSKeysGrid($data=null)
{
	$req = dbSelect('pb.ws_keys', Array('id', 'api', 'public_key', 'private_key', 'status'), Array('api' => $data));
	
	if (is_array($req)) {
		$store = new EnsKa_DataStore($req);
		echo '({"total":"'.count($store->getDatas()).'","results":'.json_encode($store->getDatas()).'})';
		return;
	}
	
	echo '({"total":"0", "results":""})';
}

Function loadFrameworksStructureGrid($data=null)
{
	GLOBAL $_alias;
	
	$path = $_alias['path']['global']['frmPath'].$data.'/content/';
	if (!is_dir($path)) {
		echo '({"total":"0", "results":""})';
		return (false);
	}
	$files = get_file_list($path);
	
	if (is_array($files)) {
		$i = 1;
		foreach($files as $file) {
			$req['id'] = $i;
			$req['file'] = $file;
			$res[] = $req;
			$i++;
		}
		echo '({"total":"'.count($res).'","results":'.json_encode($res).'})';
		return;
	}
	echo '({"total":"0", "results":""})';
}

Function loadGetFrameworkContentFile($data=null)
{
	GLOBAL $_alias;
	
	$datas = explode('::', $data);
	$framework = $datas[0];
	$file = $datas[1];
	$path = $_alias['path']['global']['frmPath'].$framework.'/content/'.$file;
	
	if (file_exists($path)) {
		$fd = fopen($path, 'r');
		if ($fd) {
			$line = '';
			while (!FEOF($fd)) {
				$line .= strReplace(strReplace(fgets($fd), "\\'", "'"), '\\"', '"');
			}
			fclose($fd);
			echo $line;
			return;
		}
		else {
			echo 'Unable to open the file';
			return(false);
		}
	}
	else {
		echo 'The content file not exists';
		return (false);
	}
}

Function loadGetFrameworkSiteSchema($data=null)
{
	GLOBAL $_alias;
	
	$path = $_alias['path']['global']['frmPath'].$data.'/schemas/site.xml';
	
	if (file_exists($path)) {
		$fd = fopen($path, 'r');
		if ($fd) {
			$line = '';
			while (!FEOF($fd)) {
				$line .= strReplace(strReplace(fgets($fd), "\\'", "'"), '\\"', '"');
			}
			fclose($fd);
			echo $line;
			return;
		}
		else {
			echo 'Unable to open the file';
			return(false);
		}
	}
	else {
		$fd = fopen($path, 'w');
		if ($fd) {
			fputs($fd, '<?xml version="1.0" encoding="UTF-8"?>'."\n");
			fputs($fd, '<framework name="'.$data.'">'."\n");
			fputs($fd, "\n".'</framework>');
			fclose($fd);
			loadGetFrameworkSiteSchema($data);
			return;
		}
		else {
			echo 'Unable to create the site schema';
			return (false);
		}
	}
}

Function loadGetFrameworkApplicationSchema($data=null)
{
	GLOBAL $_alias;
	
	$path = $_alias['path']['global']['frmPath'].$data.'/schemas/application.xml';
	
	if (file_exists($path)) {
		$fd = fopen($path, 'r');
		if ($fd) {
			$line = '';
			while (!FEOF($fd)) {
				$line .= strReplace(strReplace(fgets($fd), "\\'", "'"), '\\"', '"');
			}
			fclose($fd);
			echo $line;
			return;
		}
		else {
			echo 'Unable to open the file';
			return(false);
		}
	}
	else {
		$fd = fopen($path, 'w');
		if ($fd) {
			fputs($fd, '<?xml version="1.0" encoding="UTF-8"?>'."\n");
			fputs($fd, '<framework name="'.$data.'">'."\n");
			fputs($fd, "\n".'</framework>');
			fclose($fd);
			loadGetFrameworkApplicationSchema($data);
			return;
		}
		else {
			echo 'Unable to create the application schema';
			return (false);
		}
	}
}

Function loadLibrariesGrid($data=null)
{
	GLOBAL $_alias;
	
	if (strcmp($data, 'default') == 0) {
		$libsFile = new EnsKa_INIReader($_alias['path']['global']['point'].$_alias['path']['global']['path_conf'].'libs.ini');
	}
	else {
		$libsFile = new EnsKa_INIReader($_alias['path']['global']['point'].$_alias['path']['global']['sites'].$data.'/conf/libs.ini');
	}
	
	if ($libsFile->getError()) {
		echo '({"total":"0", "results":""})';
		return;
	}
	
	$libs = $libsFile->getLines();
	
	foreach($libs as $key => $value)
	{
		$params = Array(
					'name'		=>	ltrim($key, 'lib_'),
					'loaded'	=>	$value
				);
		$_plugins[] = $params;
		unset($params);
	}
	
	if (is_array($_plugins)) {
		$store = new EnsKa_DataStore($_plugins);
		echo '({"total":"'.count($store->getDatas()).'","results":'.json_encode($store->getDatas()).'})';
		return;
	}
	
	echo '({"total":"0", "results":""})';
}

Function loadLibrariesSelect($data=null)
{
	GLOBAL $_alias;
	
	$libsFile = new EnsKa_INIReader($_alias['path']['global']['point'].$_alias['path']['global']['path_conf'].'libs.ini');
	if ($libsFile->getError()) {
		return;
	}
	
	$libs = $libsFile->getLines();
	$i = 0;
	
	foreach($libs as $key => $value)
	{
		$params = Array
				(
					'id_name'	=>	ltrim($key, 'lib_'),
					'dpl_name'	=>	$key
				);
		$_plugins[] = $params;
		unset($params);
	}
	
	if (is_array($_plugins)) {
		$store = new EnsKa_DataStore($_plugins);
		echo "{ items: ".json_encode($store->getDatas())." }";
	}
}

Function loadPlugsGrid($data=null)
{
	GLOBAL $_alias;
	
	$req = dbSelect('pb.site_infos', Array('dpl_name', 'id_name'));

	foreach($req as $node) {
		
		$path = $_alias['path']['global']['point'].$_alias['path']['local']['path_plugsViews'].$node['id_name'].'/';
		$dirs = get_folder_list($path);
		
		foreach($dirs as $dir) {
			if (file_exists($path.$dir.'/'.'settings.ini')) {
			
				$settings = new EnsKa_INIReader($path.$dir.'/'.'settings.ini');
				
				if (strcmp($data, 'export') != 0) {
					$params = Array
								(
									'id' 			=> 		$settings->getLine('id'),
									'name' 			=> 		$settings->getLine('name'),
									'type' 			=> 		getPluginType($settings->getLine('type')),
									'display' 		=> 		getPluginDisplay($settings->getLine('display')),
									'versionId' 	=> 		$settings->getLine('versionId'),
									'version' 		=> 		$settings->getLine('version'),
									'websiteId'		=>		$node['id_name'],
									'websiteName'	=>		$node['dpl_name'],
									'updateFrom'	=>		$settings->getLine('updateFrom'),
									'updateCheck'	=>		$settings->getLine('updateCheck'),
									'updateAuto'	=>		$settings->getLine('updateAuto')
								);
				}
				else {
					$params = Array
								(
									'id_name' 		=> 		$node['id_name'].';'.$settings->getLine('id'),
									'dpl_name' 		=> 		$node['dpl_name'].' - '.$settings->getLine('name'),
								);
				}
				
				$_plugins[] = $params;
				unset($params);
				unset($settings);

			}					
		}
	}
	
	if (isset($_plugins)) {
		if (is_array($_plugins)) {
			$store = new EnsKa_DataStore($_plugins);
			if (strcmp($data, 'export') != 0) {
				echo '({"total":"'.count($store->getDatas()).'","results":'.json_encode($store->getDatas()).'})';
			}
			else {
				echo "{ items: ".json_encode($store->getDatas())." }";
			}
			return;
		}
	}
	echo '({"total":"0", "results":""})';
}

Function getFrameworkNameByNameId($frmId)
{
	$req = getComplexArray(dbSelect('pb.frameworks_infos', Array('name'), Array('id_name'=>$frmId)));
	return ($req['name']);
}

Function getFrameworkIdNameById($frmId)
{
	$req = getComplexArray(dbSelect('pb.frameworks_infos', Array('id_name'), Array('id'=>$frmId)));
	return ($req['id_name']);
}

Function loadAppsGrid($data=null)
{
	GLOBAL $_alias;
	
	$req = dbSelect('pb.site_infos', Array('dpl_name', 'id_name'));

	foreach($req as $node) {
		
		$path = $_alias['path']['global']['point'].$_alias['path']['global']['sites'].$node['id_name'].'/'.get_app_path($node['id_name']);
		$dirs = get_folder_list($path, true);
		
		foreach($dirs as $dir) {
			if (file_exists($dir.'/'.'app.settings.ini')) {
				$settings = new EnsKa_INIReader($dir.'/'.'app.settings.ini');
				
				if (strcmp($data, 'export') != 0) {
					$params = Array
								(
									'idName' 		=> 		$settings->getLine('idName'),
									'name' 			=> 		$settings->getLine('name'),
									'frameworkId' 	=> 		$settings->getLine('framework'),
									'framework' 	=> 		getFrameworkNameByNameId($settings->getLine('framework')),
									'description' 	=> 		$settings->getLine('description'),
									'versionId' 	=> 		$settings->getLine('versionId'),
									'version' 		=> 		$settings->getLine('version'),
									'websiteId'		=>		$node['id_name'],
									'websiteName'	=>		$node['dpl_name'],
									'updateFrom' 	=> 		$settings->getLine('updateFrom'),
									'updateAuto' 	=> 		$settings->getLine('updateAuto'),
									'updateCheck' 	=> 		$settings->getLine('updateCheck')
								);
				}
				else {
					$params = Array
								(
									'id_name' 		=> 		$node['id_name'].';'.$settings->getLine('idName'),
									'dpl_name' 		=> 		$node['dpl_name'].' - '.$settings->getLine('name'),
								);
				}
				
				$_apps[] = $params;
				unset($params);
				unset($settings);
			}					
		}
	}
	
	if (is_array($_apps)) {
		$store = new EnsKa_DataStore($_apps);
		if (strcmp($data, 'export') != 0) {
			echo '({"total":"'.count($store->getDatas()).'","results":'.json_encode($store->getDatas()).'})';
		}
		else {
			echo "{ items: ".json_encode($store->getDatas())." }";
		}
		return;
	}
	if (strcmp($data, 'export') != 0) {
		echo '({"total":"0", "results":""})';
	}
}

Function getPluginType($type)
{
	if ($type == PANEL) {
		return ('Panel');
	}
	elseif ($type == WINDOW) {
		return ('Window');
	}
	
	return ('Source code');
}

Function getPluginDisplay($display)
{
	if ($display == MENU) {
		return ('Menu');
	}
	elseif ($display == BROWSER) {
		return ('Browser');
	}
	elseif ($display == BROWSERANDMENU) {
		return ('Browser & Menu');
	}
	
	return ('Not displaying');
}

Function loadGetPluginSource($data=null)
{
	GLOBAL $_alias;
	
	$data = explode('::', $data);
	$plugin = $data[0];
	$website = $data[1];
	$type = $data[2];
	
	if (strcmp($type, 'php') == 0) {
		$file = $_alias['path']['global']['point'].$_alias['path']['local']['path_plugsModels'].$plugin.'.inc';
	}
	else {
		$path = $_alias['path']['global']['point'].$_alias['path']['local']['path_plugsViews'].$website.'/'.$plugin.'/';
		$file = $path.$plugin.'-'.$type.'.phtml';
	}
	
	if (file_exists($file)) {
		if ($fd = fopen($file, 'r'))  {
			$line = '';
			while (!(feof($fd))) {
				$line .= fgets($fd);
			}
			fclose($fd);
			echo $line;
			return;
		}
		else {
			echo 'ERROR!<br>UNABLE TO OPEN THE PLUG-IN\'S SOURCE CODE !';
			return (false);
		}
	}
}

Function loadUsersGrid($data=null)
{
	$tableAuth = get_table('pb.users_auth');
	$tableInf = get_table('pb.users_infos');
	
	$req = dbQuery("SELECT `".$tableAuth."`.`uid`,`".$tableAuth."`.`login`,`".$tableAuth."`.`mail`,`".$tableInf."`.* FROM `".$tableAuth."` LEFT JOIN `".$tableInf."` ON `".$tableInf."`.`uid`=`".$tableAuth."`.`uid` WHERE 1;");
	
	if (is_array($req)) {
		$store = new EnsKa_DataStore($req);
		echo '({"total":"'.count($store->getDatas()).'","results":'.json_encode($store->getDatas()).'})';
		return;
	}
	
	echo '({"total":"0", "results":""})';
}

Function loadGroupsGrid($data=null)
{
	$req = dbSelect('pb.groups_infos');
	
	if (is_array($req)) {
		$store = new EnsKa_DataStore($req);
		echo '({"total":"'.count($store->getDatas()).'","results":'.json_encode($store->getDatas()).'})';
		return;
	}
	
	echo '({"total":"0", "results":""})';
}

Function loadRolesData($data=null)
{
	$req = dbSelect('pb.acl_infos', Array('id', 'role', 'description'));
	
	if (is_array($req)) {
		echo "{ items: ".json_encode($req)." }";
	}
}

Function loadRulesData($data=null)
{
	$req = dbSelect('pb.params_rules', Array('id', 'name', 'description'));
	
	if (is_array($req)) {
		echo "{ items: ".json_encode($req)." }";
	}
}

Function loadFrmData($data=null)
{
	$req = dbSelect('pb.frameworks_infos', Array('id', 'name', 'version'));
	
	if (is_array($req)) {
		echo "{ items: ".json_encode($req)." }";
	}
}

Function loadFrmDataExp($data=null)
{
	$req = dbSelect('pb.frameworks_infos', Array('name', 'id_name'));
	
	if (is_array($req)) {
		echo "{ items: ".json_encode($req)." }";
	}
}

Function loadWebsiteData($data=null)
{
	$req = dbSelect('pb.site_infos', Array('id', 'dpl_name', 'id_name'));
	
	if (is_array($req)) {
		if (strcmp($data, 'libs') == 0) {
			$ind = count($req);
			$req[$ind] = Array('id' => $ind, 'dpl_name' => 'Default', 'id_name' => 'default');
		}
		echo "{ items: ".json_encode($req)." }";
		return;
	}
		
	echo '({"total":"0", "results":""})';
}

Function loadAuthEngine($data=null)
{
	GLOBAL $_alias;
	
	$path = $_alias['path']['global']['point'].$_alias['path']['global']['libs'].'Auth/';
	$folders = get_folder_list($path);
	
	foreach($folders as $folder) {
		$auth = new EnsKa_INIReader($path.$folder.'/auth.ini');
		$res['engine'] = $folder;
		$res['name'] = $auth->getLine('name');
		$result []= $res;
	}
	
	if (is_array($result)) {
		echo "{ items: ".json_encode($result)." }";
	}
	else {
		echo '({"total":"0", "results":""})';
	}
}

Function loadGetAuthEngine($data=null)
{
	GLOBAL	$_alias;
	
	$config = EnsKa_Config::get()->getConfig();
	$buff = '';
	
	if(strcmp($data, 'start') != 0) {
		$options = new EnsKa_INIReader($_alias['path']['global']['point'].$_alias['path']['global']['libs'].'Auth/'.$data.'/auth.ini');
		$lines = $options->getLines();
		
		foreach($lines as $key => $val) {
			if ((strcmp($key, 'name') != 0 ) && (strcmp($key, 'class') != 0 )) {
				$buff .= $key.'='.$val."\n";
			}
		}
		$result = $config['global']['auth']['auth_default_redirect'].';'.$config['global']['auth']['auth_cookie_time'].';'.$buff;
	}
	else {
		$options = new EnsKa_INIReader($_alias['path']['global']['point'].$_alias['path']['global']['libs'].'Auth/'.$config['global']['auth']['auth_lib'].'/auth.ini');
		$lines = $options->getLines();
		$buff = '';
		foreach($lines as $key => $val) {
			if ((strcmp($key, 'name') != 0 ) && (strcmp($key, 'class') != 0 )) {
				$buff .= $key.'='.$val."\n";
			}
		}
		$result = $config['global']['auth']['auth_default_redirect'].';'.$config['global']['auth']['auth_cookie_time'].';'.$config['global']['auth']['auth_name'].';'.$config['global']['auth']['auth_lib'].';'.$buff;
	}
	return($result);
}


Function loadLogsSettings($data=null)
{
	GLOBAL $_alias;
	$website = $data;
	
	if ($data != null) {
		$conf = new EnsKa_INIReader($_alias['path']['global']['point'].$_alias['path']['global']['sites'].$website.'/conf/logs.ini');
		
		$res['status'] = $conf->getLine('status');
		$res['max_size'] = $conf->getLine('max_size');
		$res['filename'] = $conf->getLine('filename');
		$res['path'] = $conf->getLine('path');
		
		$res['tError'] = $conf->getLine('tError');
		$res['tWarning'] = $conf->getLine('tWarning');
		$res['tNotice'] = $conf->getLine('tNotice');
		$res['tParse'] = $conf->getLine('tParse');
		$res['tStrict'] = $conf->getLine('tStrict');
		$res['tDeprecated'] = $conf->getLine('tDeprecated');
		$res['tAll'] = $conf->getLine('tAll');
		
		echo $res['status'].';'.$res['max_size'].';'.$res['filename'].';'.$res['path'].';'.$res['tError'].';'.$res['tWarning'].';'.$res['tNotice'].';'.$res['tParse'].';'.$res['tStrict'].';'.$res['tDeprecated'].';'.$res['tAll'];
		return;
	}
	echo '{ items: "" }';
}

Function loadCacheSettings($data=null)
{
	GLOBAL $_alias;
	$website = $data;
	
	if ($data != null) {
		$conf = new EnsKa_INIReader($_alias['path']['global']['point'].$_alias['path']['global']['sites'].$website.'/conf/cache.ini');
		$res['status'] = 0;
		if(strcmp($conf->getLine('status'), 'enable') == 0) {
			$res['status'] = 1;
		}
		$res['cache_path'] = $conf->getLine('cache_path');
		$res['time_to_cache'] = $conf->getLine('time_to_cache');
		$res['session_cache_expire'] = $conf->getLine('session_cache_expire');
		$res['session_cache_limiter'] = $conf->getLine('session_cache_limiter');
		
		echo $res['status'].';'.$res['cache_path'].';'.$res['time_to_cache'].';'.$res['session_cache_expire'].';'.$res['session_cache_limiter'];
		return;
	}
	echo '{ items: "" }';
}

Function loadCacheExcludes($data=null)
{
	GLOBAL $_alias;
	$website = $data;
	
	$fd = fopen($_alias['path']['global']['point'].$_alias['path']['global']['sites'].$website.'/conf/cache_exclude.ini', 'r');
	if ($fd) {
		while (!(feof($fd))) {
			$line = trim(fgets($fd));
			$tmp = explode('*', $line);
			$exclude['id'] = $tmp[0];
			$exclude['url'] = $tmp[1];
			if (trim($tmp[1]) != '') {
				$excludes []= $exclude;
			}
		}
		fclose($fd);
		echo '({"total":"'.count($excludes).'","results":'.json_encode($excludes).'})';
	}
	else {
		echo '({"total":"0", "results":""})';
	}
}

Function loadDatabasesGrid($data=null)
{
	$db = EnsKa_Config::get()->getConfig();
	$db = $db['global']['db'];
	$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], 'root', '');
	
	if ($sql->connect()) {
		$tables = $sql->listTables();
		foreach ($tables as $table) {
			$res['name'] = $table;
			$tmp = $sql->getNbRecords($table);
			$res['records'] = $tmp[0]['COUNT(*)'];
			$result[] = $res;
		}
		echo '({"total":"'.count($result).'","results":'.json_encode($result).'})';
	}
	else {
		echo '({"total":"0", "results":""})';
	}
}

Function loadGetTablesFields($data=null)
{
	$db = EnsKa_Config::get()->getConfig();
	$db = $db['global']['db'];
	$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], 'root', '');
	$ds = '';
	
	if ($sql->connect()) {
		$tables = $sql->listTables();
		foreach ($tables as $table) {
			$fields = $sql->getTableFields($table);
			foreach($fields as $field) {
				$res['name'] = ucfirst($field['Field']);
				if (strncmp($field['Type'], 'int', 3) == 0) {
					$res['type'] = 'int';
				}
				else {
					$res['type'] = 'string';
				}
				$res['mapping'] = $field['Field'];
				$result[] = $res;
			}
			$ds .= $table.'.'.json_encode($result).';';
			unset($result);
		}
		$ds = rtrim($ds, ';');
		echo $ds;
	}
	else {
		echo '';	
	}
}

Function loadGetTablesColumnsModel($data=null)
{
	$db = EnsKa_Config::get()->getConfig();
	$db = $db['global']['db'];
	$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], 'root', '');
	$cm = '';
	
	if ($sql->connect()) {
		$tables = $sql->listTables();
		foreach ($tables as $table) {
			$fields = $sql->getTableFields($table);
			foreach($fields as $field) {
				$res['header'] = $field['Field'];
				$res['dataIndex'] = ucfirst($field['Field']);
				$res['width'] = 100;
				$result []= $res;
			}
			$cm .= $table.'.'.json_encode($result).';';
			unset($result);
		}
		$cm = rtrim($cm, ';');
		echo $cm;
	}
	else {
		echo '';	
	}
}

Function loadDBFields($data=null)
{
	$db = EnsKa_Config::get()->getConfig();
	$db = $db['global']['db'];
	$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], 'root', '');
	
	if ($sql->connect()) {
		$fields = $sql->getTableFields($data);
		foreach($fields as $field) {
			$res['key'] = $field['Field'];
			$res['value'] = $field['Field'];
			$result[] = $res;
		}
		echo "{ items: ".json_encode($result)." }";
	}
	else {
		echo '';	
	}
}

Function loadTableDatasGrid($data=null)
{
	$db = EnsKa_Config::get()->getConfig();
	$db = $db['global']['db'];
	$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], 'root', '');
	
	if ($sql->connect()) {
		$datas = $sql->request('SELECT * FROM '.$data);
		$nbrows = count($datas);
		$start = (integer) (isset($_POST['start']) ? $_POST['start'] : $_GET['start']);
		$end = (integer) (isset($_POST['limit']) ? $_POST['limit'] : $_GET['limit']);
		$datas = $sql->request('SELECT * FROM '.$data.' LIMIT '.$start.','.$end);
		echo '({"total":"'.$nbrows.'","results":'.json_encode($datas).'})';
	}
	else {
		echo '({"total":"0", "results":""})';
	}
}

function loadDBTables($data=null)
{
	$db = EnsKa_Config::get()->getConfig();
	$db = $db['global']['db'];
	$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], 'root', '');
	
	if ($sql->connect()) {
		$tables = $sql->listTables();
		foreach ($tables as $table) {
			$res['key'] = $table;
			$res['value'] = $table;
			$result[] = $res;
		}
		echo "{ items: ".json_encode($result)." }";
	}
	else {
		echo '';
	}
}

Function loadLogsGrid($data=null)
{
	GLOBAL $_alias;
	
	$datas = explode('::', $data);
	$store = new EnsKa_DataStore
			(
				Array
					(
						'path'		=>	$datas[0],
						'file'		=>	$datas[1]
					)
			);
	$path = $_alias['path']['global']['point'].$_alias['path']['global']['path_logs'].$store->getData('path').'/';
	
	if (!is_dir($path)) {
		@mkdir($path, 0777, true);
	}
	
	$file = $path.$store->getData('file').'.log';
	
	if (!file_exists($file)) {
		fclose(fopen($file, 'w'));
		echo '({"total":"0", "results":""})';
		return;
	}
	else {
		$fd = @fopen($file, 'r');
	}
	
	if ($fd) {
		while (!(feof($fd))) {
			$line = trim(fgets($fd));
			$tmp = explode('*', $line);
			$err['id'] = $tmp[0];
			$err['type'] = $tmp[1];
			$err['errId'] = $tmp[2];
			$err['errMsg'] = $tmp[3];
			$err['where'] = $tmp[4];
			$err['when'] = $tmp[5];
			$err['who'] = $tmp[6];
			$errs []= $err;
		}
		fclose($fd);
		
		$start = (integer) (isset($_POST['start']) ? $_POST['start'] : $_GET['start']);
		$end = (integer) (isset($_POST['limit']) ? $_POST['limit'] : $_GET['limit']);
		
		$total = count($errs);
		$cpt = 0;
		unset($err);
		
		foreach($errs as $err) {
			if (($cpt >= $start) && ($cpt <= ($start + $end) - 1)) {
				$res[] = $err;
			}
			$cpt++;
		}
		echo '({"total":"'.$total.'","results":'.json_encode($res).'})';
	}
	else {
		echo '({"total":"0", "results":""})';
	}
}

Function loadGetEnviron($data=null)
{
	GLOBAL $_alias;
	echo $_alias['env'];
}

Function loadGetCoreSettings($data=null)
{
	$file = '../'.ENSKA_GLOBAL_CONF.'settings.ini';
	$fd = fopen($file, 'r');
	$buff = '';
	while (!FEOF($fd)) {
		$buff .= fgets($fd);
	}
	echo $buff;
}

Function loadGetPathApps($data=null)
{
	GLOBAL $_alias;
	
	$iniReader = new EnsKa_INIReader($_alias['path']['global']['sites'].trim($data).'/conf/settings.ini');
	
	if (!$iniReader->getError()) {
		$pathApps = $iniReader->getLine('path_apps');
		
		if ($pathApps != '') {
			echo $pathApps;
			return;
		}
		else {
			echo 'Error, unconfigured "path_apps" !';
		}
	}
	else {
		echo $iniReader->getError();
	}
}

Function loadGetLocalConfig($data=null)
{
	GLOBAL $_alias;
	
	$iniReader = new EnsKa_INIReader($_alias['path']['global']['sites'].trim($data).'/conf/settings.ini');
	
	if (!$iniReader->getError()) {
		$lines = $iniReader->getLines();
		$buff = '';
		foreach($lines as $key => $value) {
		
			if (($value == '#com') || ($value == '#head')) {
				$line = $key."\n";
			}
			else {
				$line = $key.' = '.$value."\n";
			}
			
			if (strcmp($key, 'path_apps') != 0) {
				$buff .= $line;
			}
		}
		
		echo $buff;
		return;
	}
	else
	{
		echo $iniReader->getError();
	}
}

Function loadGetFrameworkName($data=null)
{
	$req = getComplexArray(dbSelect('pb.frameworks_infos', Array('name'), Array('id' => $data)));
	
	if (is_array($req)) {
		echo $req['name'];
		return;
	}
	
	echo 'ERROR ON GETTING FRAMEWORK NAME !';
}

Function loadGetSiteAlias($data=null)
{
	$alias = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'alias.ini');
	$aliases = $alias->getLines();
	
	foreach($aliases as $key => $val) {
	
		if (strcmp(trim($val), $data) == 0) {
			echo trim($key);
			return;
		}
	}
	
	echo 'ERROR ON GETTING SITE ALIAS !';
}

Function loadGetFrameworkLoader($data=null)
{
	GLOBAL $_alias;
	
	$frm = $data;
	
	if (strncmp('enskacore', $data, strlen('enskacore')) == 0)
	{
		echo 'You can\'t access to a system loader';
		return;
	}
	
	$loader = $_alias['path']['global']['frmPath'].$data.'/loader.php';
	$line = '';
	
	$fd = fopen($loader, 'r');
	if ($fd) {
		while (!FEOF($fd)) {
			$line .= strReplace(fgets($fd), "\\'", "'");
		}
		fclose($fd);
		echo $line;
	}
	else {
		echo 'ERROR ON GETTING THE FRAMEWORK LOADER !';
	}
}

Function loadGetSelectedRoleinfos($data=null)
{
	$req = dbSelect('pb.acl_infos', Array('id', 'role', 'description'), Array('id' => $data));
	
	if (is_array($req)) {
		$res = json_encode($req);
		echo substr($res, 1, strlen($res) - 2);
		return;
	}
	
	echo 'ERROR ON GETTING ROLE !';
}

Function loadGetSelectedRuleinfos($data=null)
{
	$req = dbSelect('pb.params_rules', Array('id', 'name', 'description'), Array('id' => $data));
	
	if (is_array($req)) {
	
		$req = getComplexArray($req);
		$res = json_encode($req);
		echo $res;
		return;
	}
	
	echo 'ERROR ON GETTING RULE !';
}

Function getPluginsMenu()
{
	GLOBAL $_alias;
	
	$req = dbSelect('pb.site_infos', Array('dpl_name', 'id_name'));

	foreach($req as $node) {
		
		$path = $_alias['path']['global']['point'].$_alias['path']['local']['path_plugsViews'].$node['id_name'].'/';		
		$dirs = get_folder_list($path);
		
		foreach($dirs as $dir) {
			if (file_exists($path.$dir.'/'.'settings.ini')) {
				$settings = new EnsKa_INIReader($path.$dir.'/'.'settings.ini');
				if (($settings->getLine('display') == MENU) || ($settings->getLine('display') == BROWSERANDMENU)) {
					if ($settings->getLine('type') == WINDOW) {
						$params = Array(
							'id' 		=> 		'win-'.$settings->getLine('id'),
							'text' 		=> 		$settings->getLine('name')
						);
					}
					else {
						$params = Array(
							'id' 		=> 		$settings->getLine('id'),
							'text' 		=> 		$settings->getLine('name')
						);
					}
					
					$_plugins[] = $params;
					unset($params);
				}
				unset($settings);
			}
		}
	}
	
	if (isset($_plugins)) {
		return ($_plugins);
	}
	else {
		return (false);
	}
}

?>