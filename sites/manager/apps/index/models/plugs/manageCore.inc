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
 
class manageCore extends EnsKa_PlateformPlugin
{
	Public
		Function import($data=null)
		{
			$this->_store->setDatas($data);
			$this->_store->setData('file_size_limit', 5);
			
			$_line = __LINE__;
			if (strcmp(($res = $this->doImport()), 'ok') == 0) {
				$_line = __LINE__;
				if (strcmp(($res = $this->installArchive()), 'ok') == 0) {
					unset($data, $res, $_line);
					rollBackFolders($this->_store->getData('pathTemp'));
					if ($this->_store->getData('wizard')) {
						return ('{"success": true, "msg":{"msgInfo":"The core settings has been successfully imported !"}}');
					}
					else {
						echo '{"success": true, "msg":{"msgInfo":"The core settings has been successfully uploaded !"}}';
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
			trigger_error('Unable to import the core settings****1x2****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $res, $_line);
			return;
		}
		
		Function updateEnviron($data=null)
		{
			GLOBAL $_alias;
			$env = $data[1];
			
			if (strcmp($env, 'none') == 0) {
				unset($data, $env);
				return (false);
			}
			
			$_line = __LINE__;
			$conf = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'settings.ini');
			if (!$conf->getError())
			{
				$conf->addLine('env', $env, true);
				$_alias['env'] = $env;
				echo 'success';
				unset($data, $env, $_line, $conf);
				return (true);
			}
			
			echo 'UNABLE TO UPDATE THE PLATEFORM ENVIRONMENT -- '.$conf->getError();
			trigger_error('Unable to update the plateform environment -- '.$conf->getError().'****1x22001****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $env, $_line, $conf);
			return (false);
		}
		
		Function edit($data=null)
		{
			GLOBAL $_alias;
			$config = $data[1];
			
			$_line = __LINE__;
			$conf = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'settings.ini');
			if (!$conf->getError())
			{
				$conf->fileBuff($config);
				$_alias['path'] = EnsKa_Path::get()->getPath();
				$_alias['env'] = $_alias['path']['global']['env'];
				echo 'success';
				unset($data, $config, $_line, $conf);
				return (true);
			}
			
			echo 'UNABLE TO UPDATE THE CORE SETTINGS -- '.$conf->getError();
			trigger_error('Unable to update the core settings -- '.$conf->getError().'****1x22002****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $config, $_line, $conf);
			return (false);
		}
		
		Function export($data=null)
		{
			$tmp = explode(',', rtrim($data[1], ','));
			$params = Array
						(
							'core'		=>	(int)$tmp[0],
							'users'		=>	(int)$tmp[1],
							'acl'		=>	(int)$tmp[2],
							'rules'		=>	(int)$tmp[3],
							'corePath'	=>	''
						);
			$this->_store->setDatas($params);
			
			$options = Array
			(
				'archiveExtension'	=>	'cor',
				'exportSchema'		=>	'core',
				'archiveName'		=>	'EnsKa_core-settings',
				'replace'			=>	Array()
			);
			
			$_line = __LINE__;
			if (!$this->exportOptions($options)) {
				trigger_error('Unable to set the core settings export options****1x2****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}
			
			if($this->_store->getData('core')) {
				$conf = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_conf'].'settings.ini');
				$exceptions = Array('load.db', 'load.version', 'load.libs', 'load.auth');
				$save;
				foreach($conf->getLines() as $key => $value) {
					if (strncmp($key, 'load.', strlen('load.')) == 0) {
						if (!in_array($key, $exceptions)) {
							$save[$key] = $value;
						}
					}
				}
				if (is_array($save)) {
					$this->_store->setData('corePath', $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_conf'].md5(create_id(5)).'/');
					$coreFile = $this->_store->getData('corePath').'settings.ini';
					mkdir($this->_store->getData('corePath'), 0777);
					$coreConf = new EnsKa_INIReader($coreFile, 'create');
					foreach($save as $key => $value) {
						$coreConf->addLine($key, $value);
						$fileConfNew = new EnsKa_INIReader($this->_store->getData('corePath').$value, 'create');
						$fileConfOld = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_conf'].$value);
						$conf = '';
						foreach($fileConfOld->getLines() as $lkey => $lvalue) {
							$conf .= $lkey.'='.$lvalue."\n";
						}
						$fileConfNew->fileBuff($conf);
					}
					$coreConf->save();
				}
			}
			
			if($this->_store->getData('users')) {
				$db = EnsKa_Config::get()->getConfig();
				$db = $db['global']['db'];
				$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $db['dbUser'], $db['dbPassword']);
				
				if ($sql->connect()) {
					if ($sql->export(Array($db['dbPrefix'].'groups_infos', $db['dbPrefix'].'users_auth', $db['dbPrefix'].'users_groups', $db['dbPrefix'].'users_infos'), true)) {
						rename($sql->_exportFile, $sql->_exportFile.'.users');
						$this->addCPY('rename', $sql->_exportFile.'.users', $this->_store->getData('tmpFolder').'EnsKa_core-settings/groups/plateform_groups-users.sql');
					}
				}
			}
			
			if($this->_store->getData('acl')) {
				$db = EnsKa_Config::get()->getConfig();
				$db = $db['global']['db'];
				$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $db['dbUser'], $db['dbPassword']);
				
				if ($sql->connect()) {
					if ($sql->export(Array($db['dbPrefix'].'acl_infos'), true)) {
						rename($sql->_exportFile, $sql->_exportFile.'.acl');
						$this->addCPY('rename', $sql->_exportFile.'.acl', $this->_store->getData('tmpFolder').'EnsKa_core-settings/acl/plateform_acl.sql');
					}
				}
			}
			
			if($this->_store->getData('rules')) {
				$db = EnsKa_Config::get()->getConfig();
				$db = $db['global']['db'];
				$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $db['dbUser'], $db['dbPassword']);
				
				if ($sql->connect()) {
					if ($sql->export(Array($db['dbPrefix'].'params_rules'), true)) {
						rename($sql->_exportFile, $sql->_exportFile.'.rules');
						$this->addCPY('rename', $sql->_exportFile.'.rules', $this->_store->getData('tmpFolder').'EnsKa_core-settings/rules/plateform_rules.sql');
					}
				}
			}
			
			if($this->_store->getData('core')) {
				$this->addCPY('rename', $this->_store->getData('corePath'), $this->_store->getData('tmpFolder').'EnsKa_core-settings/settings/');
			}
			
			echo $this->doExport();
		}
		
	Private
		Function installArchive()
		{
			$pathAcl = $this->_store->getData('pathTemp').'EnsKa_core-settings/acl/';
			$pathGroups = $this->_store->getData('pathTemp').'EnsKa_core-settings/groups/';
			$pathRules = $this->_store->getData('pathTemp').'EnsKa_core-settings/rules/';
			$pathSettings = $this->_store->getData('pathTemp').'EnsKa_core-settings/settings/';
			$pathConf = $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_conf'];
			
			$err = '';
			
			$db = EnsKa_Config::get()->getConfig();
			$db = $db['global']['db'];
			$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $this->_store->getData('login'), $this->_store->getData('password'));
			if (!$sql->connect()) {
				return ('Unable to connect to database');
			}
			
			if (file_exists($pathAcl.'plateform_acl.sql')) {					
				if (!$sql->scriptRequest($pathAcl.'plateform_acl.sql', false, true)) {
					$err .= ('Unable to import ACL settings -- ');
				}
			}
			
			if (file_exists($pathGroups.'plateform_groups-users.sql')) {					
				if (!$sql->scriptRequest($pathGroups.'plateform_groups-users.sql', false, true)) {
					$err .= ('Unable to import Users/Groups settings -- ');
				}
			}
			
			if (file_exists($pathRules.'plateform_rules.sql')) {					
				if (!$sql->scriptRequest($pathRules.'plateform_rules.sql', false, true)) {
					$err .= ('Unable to import rules settings -- ');
				}
			}
			
			$files = get_file_list($pathSettings);
			foreach($files as $file) {
				if ((strcmp($file, 'settings.ini') != 0) && (!file_exists($pathConf.$file))) {
					copy($pathSettings.$file, $pathConf.$file);
				}
			}
			
			$conf = new EnsKa_INIReader($pathSettings.'settings.ini');
			$settings = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'settings.ini');
			$lines = $conf->getLines();
			foreach($lines as $key => $value) {
				if (!$settings->getLine($key)) {
					$settings->addLine($key, $value);
				}
			}
			$settings->save();
			
			if ($err == '') {
				return ('ok');
			}
			return ($err);
		}
}

?>