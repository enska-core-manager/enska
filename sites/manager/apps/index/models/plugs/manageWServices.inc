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
 * @package    models/plugs
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */
 
define ('ADD', 1);
define ('DEL', 2);

class manageWServices extends EnsKa_PlateformPlugin
{
	Public
		Function import($data=null)
		{
			$this->_store->setDatas($data);
			$this->_store->setData('file_size_limit', 10);
			
			$_line = __LINE__;
			if (strcmp(($res = $this->doImport()), 'ok') == 0) {
				$_line = __LINE__;
				if (strcmp(($res = $this->installArchive()), 'ok') == 0) {
					unset($data, $res, $_line);
					rollBackFolders($this->_store->getData('pathTemp'));
					if ($this->_store->getData('wizard')) {
						return ('{"success": true, "msg":{"msgInfo":"The web service has been successfully imported !"}}');
					}
					else {
						echo '{"success": true, "msg":{"msgInfo":"The web service has been successfully imported !"}}';
					}
					return;
				}
			}
			
			rollBackFolders($this->_store->getData('pathTemp'));
			if ($this->_store->getData('wizard')) {
				return ('{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}');
			}
			else {
				echo '{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}';
			}
			trigger_error('Unable to import the web service****1x2****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $res, $_line);
			return;
		}
		
		Function export($data=null)
		{
			$id = $data[1];
			$release = $data[2];
			$keys = $data[3];
			$req = $this->getWServiceInfos(Array('key' => 'id', 'val' => $id));
			
			$options = Array
			(
				'archiveExtension'	=>	'saf',
				'exportSchema'		=>	'wservice',
				'archiveName'		=>	$req['name'],
				'release'			=>	$release,
				'release_path'		=>	$this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_updates'].'wservices/'.$req['website'].'/'.$req['name'].'/',
				'infos_path'		=>	$this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$req['website'].'/'.$req['name'].'/ws/ws.update.ini',
				'replace'			=>	Array
										(
											'%wservice%'	=>	$req['name']
										)
			);
			
			$_line = __LINE__;
			if (!$this->exportOptions($options)) {
				trigger_error('Unable to set the WEBService export options****1x2001****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}
			
			$this->_store->setData('keys', $keys);
			if ($this->_store->getData('keys') == 1) {
				$keys = dbSelect('pb.ws_keys', Array('public_key', 'private_key'), Array('api' => $req['id']));
				if ($keys) {
					$infos = '';
					foreach ($keys as $key) {
						$infos .= $key['public_key'].'::'.$key['private_key']."\n";
					}
					unset($keys);
					mkdir($this->_store->getData('tmpFolder'), 0777);
					$keys = new EnsKa_INIReader($this->_store->getData('tmpFolder').'keys.ini', 'create');
					$keys->fileBuff($infos);
				}
			}
			
			$this->addCPY('copyDir', $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$req['website'].'/'.$req['name'].'/ws/', $this->_store->getData('tmpFolder').$req['name'].'/ws/');
			if ($this->_store->getData('keys') == 1) {
				$this->addCPY('copy', $this->_store->getData('tmpFolder').'keys.ini', $this->_store->getData('tmpFolder').$req['name'].'/keys.ini');
			}
			
			echo $this->doExport();
			unset($id, $release, $keys, $req, $options, $key, $infos);
		}
		
		Function generateSDK($data=null)
		{
			$id = $data[1];
			$req = $this->getWServiceInfos(Array('key' => 'id', 'val' => $id));
			
			$options = Array
			(
				'archiveExtension'	=>	'zip',
				'exportSchema'		=>	'wservicesdk',
				'archiveName'		=>	$req['name'].'_sdk_'.$req['version'].'_'.$req['revision'],
				'replace'			=>	Array
										(
											'%sdkname%'	=>	$req['name'].'_sdk_'.$req['version'].'_'.$req['revision']
										)
			);
			
			$_line = __FILE__;
			if (!$this->exportOptions($options)) {
				trigger_error('Unable to set the SDK export options****1x2002****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}
			$name = $req['name'].'_sdk_'.$req['version'].'_'.$req['revision'];
			
			$this->addCPY('copyDir', $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$req['website'].'/'.$req['name'].'/sdk/api/', $this->_store->getData('tmpFolder').$name.'/api/');
			$this->addCPY('copyDir', $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$req['website'].'/'.$req['name'].'/sdk/app/', $this->_store->getData('tmpFolder').$name.'/app/');
			$this->addCPY('copyDir', $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$req['website'].'/'.$req['name'].'/sdk/user/', $this->_store->getData('tmpFolder').$name.'/user/');
			
			echo $this->doExport();
			unset($id, $name, $req, $options);
		}
		
		Function editKeys($data=null)
		{
			$this->$data[1]($data);
			unset($data);
		}
		
		Function editByGrid($data=null)
		{
			$params = Array
					(
						'id' 			=>	$data[1],
						'description'	=>	$data[2]
					);
			$this->_store->setDatas($params);
			
			$_line = __FILE__;
			$req = dbUpdate
			(
				'pb.ws_infos',
				Array
				(
					'description' 	=> 		$this->_store->getData('description')
				),
				Array
				(
					'id' 			=> 		$this->_store->getData('id')
				)
			);
			
			if (!$req)
			{
				echo 'Manager couldn\'t update the web service informations!<br>';
				trigger_error('Manager couldn\'t update the web service informations****1x2003****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($data, $params, $req);
				return (false);
			}
			
			echo 'success';
			unset($data, $params, $req);
			return (true);
		}

		Function editByEditor($data=null)
		{
			$params = Array
					(
						'authUsers' 	=>		$data[1],
						'UnAuthUsers' 	=>		$data[2],
						'description'	=>		$data[3],
						'id'			=>		$data[4],
						'updateFrom'	=>		$data[5],
						'updateCheck'	=>		$data[6],
						'updateAuto'	=>		$data[7],
						'version'		=>		$data[8],
						'revision'		=>		$data[9]
					);
			$this->_store->setDatas($params);
			$infos = $this->getWServiceInfos(Array('key' => 'id', 'val' => $this->_store->getData('id')));
			
			if (trim($this->_store->getData('updateFrom')) == '') {
				$this->_store->setData('updateFrom', 'none');
			}
			
			$_line = __FILE__;
			$req = dbUpdate
			(
				'pb.ws_infos',
				Array
				(
					'description'	=>	$this->_store->getData('description'),
					'maj_link'		=>	$this->_store->getData('updateFrom'),
					'maj_check'		=>	$this->_store->getData('updateCheck'),
					'maj_auto'		=>	$this->_store->getData('updateAuto'),
					'version'		=>	$this->_store->getData('version'),
					'revision'		=>	$this->_store->getData('revision')
				),
				Array
				(
					'id'			=>	$this->_store->getData('id')
				)
			);
			
			if (!$req) {
				echo 'Manager couldn\'t update the web service informations!<br>';
				trigger_error('Manager couldn\'t update the web service informations****1x2004****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($data, $params, $req);
				return (false);
			}
			
			$_line = __FILE__;
			$update = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$infos['website'].'/'.$infos['name'].'/ws/ws.update.ini');
			if($update->getError()) {
				echo 'Manager couldn\'t update the web service informations!<br>'.$update->getError();
				trigger_error('Manager couldn\'t update the web service informations -- '.$update->getError().'****1x2005****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($data, $params, $req, $update);
				return (false);
			}
			$update->addLine('version', $this->_store->getData('version'));
			$update->addLine('versionId', $this->_store->getData('revision'));
			$update->addLine('updateAuto', $this->_store->getData('updateAuto'));
			$update->addLine('updateCheck', $this->_store->getData('updateCheck'));
			$update->addLine('updateFrom', $this->_store->getData('updateFrom'));
			$update->save();
			
			$authorizations = $this->getRights($this->_store);
			
			$_line = __FILE__;
			$req = dbUpdate
			(
				'pb.ws_auth',
				Array
				(
					'rights'	=>	$authorizations
				),
				Array
				(
					'id'		=>	$this->_store->getData('id')
				)
			);
			
			if (!$req) {
				echo 'Manager couldn\'t update the web service informations!<br>';
				trigger_error('Manager couldn\'t update the web service informations****1x2006****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($data, $params, $req, $update);
				return (false);
			}
			
			echo 'success';
			unset($data, $params, $req, $update);
			return (true);
		}

		Function checkUpdate($data=null)
		{
			$params = Array
						(
							'version'	=>		$data[1],
							'url'		=>		$data[2]
						);
				
			echo $this->doCheckUpdate(&$params);
			unset($params, $data);
		}
		
		Function delete($data=null)
		{
			$id = $data[1];
			$infos = $this->getWServiceInfos(Array('key' => 'id', 'val' => $id));

			if (!is_array($infos)) {
				echo 'UNABLE TO GET WEB SERVICE INFOS';
				trigger_error('Unable to get webservice infos****1x2007****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}
			
			rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$infos['website'].'/'.$infos['name'].'/');
			
			$req = dbDelete('pb.ws_infos', Array('id' => $id));
			$req = dbDelete('pb.ws_keys', Array('api' => $id));
			$req = dbDelete('pb.ws_auth', Array('id' => $id));
			
			echo 'success';
			unset($id, $infos, $req, $fata);
			return (true);
		}
		
	Private
		Function installArchive()
		{
			$root = get_folder_list($this->_store->getData('pathTemp'));
			$idName = $root[0];
			$pathWS = $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('sOptions').'/';
			
			if (is_dir($pathWS.$idName)) {
				return('The webs service already exists');
			}
			
			$conf = new EnsKa_INIReader($this->_store->getData('pathTemp').$idName.'/ws/ws.update.ini');
			if ($conf->getError()) {
				return ('Unable to read the web service configuration. - '.$conf->getError());
			}
			
			$version = $conf->getLine('version');
			$revision = $conf->getLine('versionId');
			$maj = $conf->getLine('updateFrom');
			$auto = $conf->getLine('updateAuto');
			$check = $conf->getLine('updateCheck');
			
			if (@file_exists($this->_store->getData('pathTemp').$idName.'/keys.ini')) {
				if (!@mkdir($pathWS.$idName.'/', 0777, true)) {
					return ('Unable to create the web service path (1)');
				}
				if (!@rename($this->_store->getData('pathTemp').$idName.'/ws/', $pathWS.$idName.'/ws/')) {
					return('Unable to create the web service path (4)');
				}
			}
			else {
				if (!is_dir($pathWS)) {
					if (!@mkdir($pathWS, 0777, true)) {
						return('Unable to create the web service path (2)');
					}
				}
				if (!@rename($this->_store->getData('pathTemp').$idName.'/', $pathWS.$idName)) {
					return('Unable to create the web service path (3)');
				}
			}
			
			if (!@mkdir($pathWS.$idName.'/sdk/', 0777)) {
				rollBackFolders($pathWS.$idName);
				return ('Unable to create the SDK path (1)');
			}
			if (!@mkdir($pathWS.$idName.'/sdk/api/', 0777)){
				rollBackFolders($pathWS.$idName);
				return ('Unable to create the SDK path (2)');
			}
			if (!@mkdir($pathWS.$idName.'/sdk/app/', 0777)) {
				rollBackFolders($pathWS.$idName);
				return ('Unable to create the SDK path (3)');
			}
			if (!@mkdir($pathWS.$idName.'/sdk/user/', 0777)){
				rollBackFolders($pathWS.$idName);
				return ('Unable to create the SDK path (4)');
			}
			
			$this->_store->setData('pathAPI', $pathWS.$idName.'/');
			$this->_store->setData('idName', $idName);
			$this->_store->setData('host', getHost);
			
			/*echo '{"success": true, "msg":{"msgInfo":"Step: OK !"}}';
			return;*/
			
			if (!$this->_createSDK()) {
				rollBackFolders($pathWS.$idName);
				return ('Unable to create the SDK files');
			}
			
			$nparams = Array('table' => get_table('pb.ws_infos'), 'id' => 'id');
			$nid = create_new_id($nparams);
			
			$nparams = Array('table' => get_table('pb.ws_infos'), 'id' => 'id');
			$nid = create_new_id($nparams);
			
			$req = 	dbInsert('pb.ws_infos', 
					Array
						(
							'id' => $nid, 
							'name' => $this->_store->getData('idName'), 
							'website' => $this->_store->getData('sOptions'), 
							'description' => '', 
							'domain' => $this->_store->getData('host'), 
							'version' => $version,
							'revision' => $revision,
							'maj_link' => $maj, 
							'maj_auto' => $auto, 
							'maj_check' => $check, 
							'maj_date' => '0000-00-00'
						)
			);
			
			if (!$req) {
				rollBackFolders($pathWS.$idName);
				return ('Unable to install the web service');
			}
			
			if (@file_exists($this->_store->getData('pathTemp').$idName.'/keys.ini')) {
				$fd = fopen($this->_store->getData('pathTemp').$idName.'/keys.ini', 'r');
				if (!$fd) {
					rollBackFolders($pathWS.$idName);
					return ('Unable to import the API keys');
				}
				
				$i = 0;
				while(!feof($fd)){
					$tmp = trim(fgets($fd));
					if ($tmp != '') {
						$line = explode('::', $tmp);
						if ((isset($line[0])) && (isset($line[1]))) {
							$req = 	dbInsert('pb.ws_keys', 
													Array
														(
															'id' => Array('create_new_id' => '%t,id'), 
															'api' => $nid, 
															'public_key' => $line[0], 
															'private_key' => $line[1], 
															'status' => 'revoked'
														)
											);
						}
						unset($line);
					}
					unset($tmp);
				}
			}
			return ('ok');
		}
		
		Function _createSDK()
		{
			$pathTPL = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_templates'].'/ws/sdk/';
			$pathServer = $this->_store->getData('pathAPI').'sdk/';
			
			$replace = Array
							(
								'%name%'		=>	$this->_store->getData('idName'),
								'%website%'		=>	$this->_store->getData('sOptions'),
								'%host%'		=>	$this->_store->getData('host'),
								'%pathAuth%'	=>	$this->_alias['path']['global']['path_webservices_auth']
							);
			
			/**
			 * Creatting API files
			 */
				if (!@copy($pathTPL.'api/lib.inc', $pathServer.'api/lib.inc')) {
					return (false);
				}
				
				$this->strReplaceInFile($pathTPL.'api/%name%.php', $pathServer.'api/'.$this->_store->getData('idName').'.php', $replace);
				$this->strReplaceInFile($pathTPL.'app/index.php', $pathServer.'app/index.php', $replace);
				$this->strReplaceInFile($pathTPL.'user/config.inc', $pathServer.'user/config.inc', $replace);
				
			unset($pathTPL, $pathServer, $replace);
			return (true);
		}
		
		Function getWServiceInfos($where)
		{
			$_line = __LINE__;
			$req = getComplexArray(dbSelect('pb.ws_infos', '*', Array($where['key'] => $where['val'])));
			if ($req) {
				return ($req);
			}
			trigger_error('Unable to get webservice infos****1x2008****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			return (false);
		}
		
		Function createKeys($data=null)
		{
			$id = $data[2];
			$key_pub = md5(create_id(100));
			$key_pri = md5(create_id(100));
			$_line = __LINE__;
			$req = dbInsert('pb.ws_keys', Array('id' => Array('create_new_id' => '%t,id'), 'api' => $id, 'public_key' => $key_pub, 'private_key' => $key_pri, 'status' => 'revoked'));
			
			switch ($req)
			{
				case 1:
					echo 'success';
					return (true);
					break;
					
				default:
					echo 'UNABLE TO CREATE THE PAIR OF KEY';
					trigger_error('Unable to create the pair of key****1x2009****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					return (false);
					break;
			}
		}

		Function updateKeys($data=null)
		{
			$id = $data[2];
			$key_pub = md5(create_id(100));
			$key_pri = md5(create_id(100));
			$req1 = getComplexArray(dbSelect('pb.ws_keys', Array('public_key', 'private_key', 'api'), Array('id' => $id)));
			
			$_line = __LINE__;
			$req = dbUpdate('pb.ws_keys', Array('public_key' => $key_pub, 'private_key' => $key_pri), Array('id' => $id));
			if ($req) {
				if ($this->isActivedKeys($id)) {
					$infos = $this->getWServiceInfos(Array('key' => 'id', 'val' => $req1['api']));
					$req = getComplexArray(dbSelect('pb.ws_keys', Array('public_key', 'private_key'), Array(Array('id' => $id))));
					$this->manageAuth($req1['public_key'], $req1['private_key'], $infos['name'], DEL);
					$this->manageAuth($req['public_key'], $req['private_key'], $infos['name'], ADD);
				}
				echo 'success';
				return (true);
			}
			else {
				echo 'UNABLE TO RE-GENERATE THE PAIR OF KEY';
				trigger_error('Unable to re-generate the pair of key****1x2010****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}
		}
		
		Function isActivedKeys($kid)
		{
			$req = getComplexArray(dbSelect('pb.ws_keys', Array('status'), Array('id' => $kid)));
			if (strcmp($req['status'], 'actived') == 0) {
				return (true);
			}
			return (false);
		}

		Function activateKeys($data=null)
		{
			$id = $data[2];
			
			$req = getComplexArray(dbSelect('pb.ws_keys', Array('public_key', 'private_key', 'api'), Array(Array('id' => $id))));
			$infos = $this->getWServiceInfos(Array('key' => 'id', 'val' => $req['api']));
			$this->manageAuth($req['public_key'], $req['private_key'], $infos['name'], DEL);
			$err = 'UNABLE TO ACTIVATE THE KEY';
			$_line = __LINE__;
			if ($this->manageAuth($req['public_key'], $req['private_key'], $infos['name'], ADD)) {
				$_line = __LINE__;
				if ($req = dbUpdate('pb.ws_keys', Array('status' => 'actived'), Array('id' => $id))) {
					echo 'success';
					return (true);
				}
			}
			echo $err;
			trigger_error('Unable to activate the key****1x2018****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			return (false);
		}

		Function revokeKeys($data=null)
		{
			$id = $data[2];
			
			$req = getComplexArray(dbSelect('pb.ws_keys', Array('public_key', 'private_key', 'api'), Array(Array('id' => $id))));
			$infos = $this->getWServiceInfos(Array('key' => 'id', 'val' => $req['api']));
			$err = 'UNABLE TO REVOKE THE KEY';
			$_line = __LINE__;
			if ($this->manageAuth($req['public_key'], $req['private_key'], $infos['name'], DEL)) {
				$_line = __LINE__;
				if ($req = dbUpdate('pb.ws_keys', Array('status' => 'revoked'), Array('id' => $id))) {
					echo 'success';
					return (true);
				}
			}
			echo $err;
			trigger_error('Unable to revoke the key****1x2012****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			return (false);
		}

		Function deleteKeys($data=null)
		{
			$id = $data[2];
			$req1 = getComplexArray(dbSelect('pb.ws_keys', Array('public_key', 'private_key', 'api'), Array(Array('id' => $id))));
			$_line = __LINE__;
			$req = dbDelete('pb.ws_keys', Array('id' => $id));
			
			switch ($req)
			{
				case 1:
					$infos = $this->getWServiceInfos(Array('key' => 'id', 'val' => $req1['api']));
					$this->manageAuth($req1['public_key'], $req1['private_key'], $infos['name'], DEL);
					echo 'success';
					return (true);
					break;
					
				default:
					echo 'UNABLE TO DELETE THE PAIR OF KEY';
					trigger_error('Unable to delete the pair of key****1x2013****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					return (false);
					break;
			}
		}
		
		Function manageAuth($puk, $prk, $api, $mod)
		{
			$auth = $puk.':'.$prk;
			$htpasswd = $this->_alias['path']['global']['path_webservices_auth'].$api.'\\.htpasswd';

			if (file_exists($htpasswd)) {
				$_line = __LINE__;
				$fd = fopen($htpasswd, 'r');
			}
			else {
				mkdir($this->_alias['path']['global']['path_webservices_auth'].$api, true);
				$_line = __LINE__;
				$fd = fopen($htpasswd, 'w');
				
				if(!$fd) {
					echo 'UNABLE TO OPEN THE AUTH PATH (1)<br>';
					trigger_error('Unable to open the auth path (1)****1x2014****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					return (false);
				}
				
				if ($mod == DEL) {
					fclose($fd);
					return (true);
				}
				
				fputs($fd, $auth);
				fclose($fd);
				return (true);
			}

			if(!$fd){
				echo 'UNABLE TO OPEN THE AUTH PATH (2)';
				trigger_error('Unable to open the auth path (2)****1x2015****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}

			if ($mod == ADD) {
				fclose($fd);
				
				$_line = __LINE__;
				$fd = fopen($htpasswd, 'a');
				if(!$fd){
					echo 'UNABLE TO UPDATE THE AUTH FILE (1)';
					trigger_error('Unable to update the auth file (1)****1x2016****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					return (false);
				}
				
				fputs($fd, "\r");
				fputs($fd, $auth);
				fclose($fd);
				return (true);
			}

			$line = '';
			while(!feof($fd)){
				$line .= fgets($fd);
			}
			fclose($fd);

			$tmp = explode("\r", $line);
			$i = 0;
			$j = 0;
			$nb = count($tmp);
			while($i < $nb){
				if (strcmp(trim($tmp[$i]), trim($auth)) != 0) {
					$tab[$j] = $tmp[$i];
					$j++;
				}
				$i++;
			}
			
			if (is_array($tab)){
				$_line = __LINE__;
				$fd = fopen($htpasswd, 'w');
				if(!$fd){
					echo 'UNABLE TO UPDATE THE AUTH FILE (2)';
					trigger_error('Unable to update the auth file (2)****1x2017****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					return (false);
				}
				$i = 0;
				$nb = count($tab);
				while($i < $nb){
					fputs($fd, $tab[$i]);
					if (($i  + 1) != $nb){
						fputs($fd, "\r");
					}
					$i++;
				}
				fclose($fd);
			}
			return (true);
		}
}

?>