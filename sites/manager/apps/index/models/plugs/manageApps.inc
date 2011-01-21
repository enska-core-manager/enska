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
 
class manageApps extends EnsKa_PlateformPlugin
{
	Public
		Function import($data=null)
		{
			$this->_store->setDatas($data);
			$this->_store->setData('file_size_limit', 500);
			
			$_line = __LINE__;
			if (strcmp(($res = $this->doImport()), 'ok') == 0) {
				$_line = __LINE__;
				if (strcmp(($res = $this->installArchive()), 'ok') == 0) {
					unset($data, $res, $_line);
					rollBackFolders($this->_store->getData('pathTemp'));
					if ($this->_store->getData('wizard')) {
						return ('{"success": true, "msg":{"msgInfo":"The application has been successfully imported !"}}');
					}
					else {
						echo '{"success": true, "msg":{"msgInfo":"The application has been successfully imported !"}}';
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
			trigger_error('Unable to import the application****1x2****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $res, $_line);
			return;
		}
		
		Function export($data=null)
		{
			$website = $data[1];
			$app = $data[2];
			$release = $data[3];
			$pathApp = $this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$website.'/'.get_app_path($website).$app.'/';
			
			$options = Array
			(
				'archiveExtension'	=>	'aaf',
				'exportSchema'		=>	'application',
				'archiveName'		=>	$app,
				'release'			=>	$release,
				'release_path'		=>	$this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_updates'].'apps/'.$website.'/'.$app.'/',
				'infos_path'		=>	$pathApp.'/app.settings.ini',
				'replace'			=>	Array
										(
											'%app%'	=>	$app
										)
			);
			
			$this->_store->setData('pathRessources', $this->_alias['path']['global']['users_ressources'].$website.'/'.$app.'/');
			if (!$this->exportOptions($options)) {
				trigger_error('Unable to set the applicatino export options****1x2901****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}
			
			$this->addCPY('copyDir', $this->_store->getData('pathRessources'), $this->_store->getData('tmpFolder').$app.'/ressources/');
			$this->addCPY('copyDir', $pathApp, $this->_store->getData('tmpFolder').$app.'/app/');
			
			echo $this->doExport();
			unset($app, $website, $options, $pathApp, $release, $data);
		}
		
		Function edit($data=null)
		{
			$params = Array
					(
						'app'			=>		$data[1],
						'website'		=>		$data[2],
						'name'			=>		$data[3],
						'description'	=>		$data[4],
						'version'		=>		$data[5],
						'authUsers' 	=>		$data[6],
						'UnAuthUsers' 	=>		$data[7],
						'upFrom'		=>		$data[8],
						'upCheck'		=>		$data[9],
						'upAuto'		=>		$data[10]
					);
			$this->_store->setDatas($params);
			
			$authorizations = $this->getRights($this->_store);
			
			$pathApps = get_app_path($this->_store->getData('website'));
			$pathApp = $this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('website').'/'.$pathApps.$this->_store->getData('app').'/';
			$this->_store->setData('pathApp', $pathApp);
			$_line = __LINE__;
			$settings = new EnsKa_INIReader($this->_store->getData('pathApp').'app.settings.ini');
			
			if (!$settings->getError()) {
				$settings->addLine('name', $this->_store->getData('name'));
				$settings->addLine('version', $this->_store->getData('version'));
				$settings->addLine('description', trim($this->_store->getData('description'), "\n"));
				$settings->addLine('rights', $authorizations);
				$settings->addLine('updateFrom', $this->_store->getData('upFrom'));
				$settings->addLine('updateCheck', $this->_store->getData('upCheck'));
				$settings->addLine('updateAuto', $this->_store->getData('upAuto'));
				$settings->save();
				
				echo 'success';
				unset($params, $authorizations, $pathsApps, $pathApp, $settings, $_line);
				return (true);
			}
			
			echo 'UNABLE TO SAVE THE APPLICATION<br>'.$settings->getError().'<br>'.$this->_store->getData('pathApp').'app.settings.ini';
			trigger_error('Unable to save the application -- '.$settings->getError().' -- '.$this->_store->getData('pathApp').'app.settings.ini****1x2902****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($params, $authorizations, $pathsApps, $pathApp, $settings, $_line);
			return (false);
		}
		
		Function delete($data=null)
		{
			$params = Array
					(
						'app' 		=> $data[1],
						'website' 	=> $data[2]
					);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if ((strcmp($this->_store->getData('website'), 'manager') == 0) || (strcmp($this->_store->getData('website'), 'login') == 0) || (strcmp($this->_store->getData('website'), 'esandbox') == 0)) {
				echo 'YOU CAN NOT DELETE A SYSTEM APPLICATION !';
				trigger_error('Unable to delete a system application****1x2903****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($params, $data, $_line);
				return (false);
			}
			
			$_line = __LINE__;
			if (strcmp($this->_store->getData('app'), 'index') == 0) {
				echo 'YOU CAN NOT DELETE THE INDEX CONTROLLER !<br>If is the last module on the website, delete the website.';
				trigger_error('Unable to delete the index controller****1x2904****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($params, $data, $_line);
				return (false);
			}
			
			rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('website').'/'.get_app_path($this->_store->getData('website')).$this->_store->getData('app').'/');
			rollBackFolders($this->_alias['path']['global']['users_ressources'].$this->_store->getData('website').'/'.$this->_store->getData('app').'/');
			
			echo 'success';
			unset($params, $data, $_line);
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
			unset($params);
		}
		
	Private
		Function installArchive()
		{
			$root = get_folder_list($this->_store->getData('pathTemp'));
			
			$idName = $root[0];
			$sitesPath = $this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'];
			$ressourcesPath = $this->_alias['path']['global']['users_ressources'].$this->_store->getData('sOptions').'/'.$idName.'/';
			
			$appsPath = $sitesPath.$this->_store->getData('sOptions').'/'.get_app_path($this->_store->getData('sOptions'));
			
			if (!is_dir($sitesPath.$this->_store->getData('sOptions'))) {
				return ('Website destination not exists');
			}
			
			if (is_dir($appsPath.$idName)) {
				return ('Application already exists in '.$this->_store->getData('sOptions'));
			}
			
			$conf = new EnsKa_INIReader($this->_store->getData('pathTemp').$idName.'/app/app.settings.ini');
			if ($conf->getError()) {
				return ('Unable to read the application configuration. - '.$conf->getError());
			}
			
			$req = getComplexArray(dbSelect('pb.site_infos', Array('framework'), Array('id_name' => $this->_store->getData('sOptions'))));
			if (!$req) {
				return ('Unable to get the website destination framework');
			}
			
			$req = getComplexArray(dbSelect('pb.frameworks_infos', Array('id_name'), Array('id' => $req['framework'])));
			if (!$req) {
				return ('You must install the correct framework system before importing this application');
			}
			
			if (strcmp($conf->getLine('framework'), $req['id_name']) != 0) {
				return ('The system framework of destination website is uncompatible with this application');
			}
			
			if (!@rename($this->_store->getData('pathTemp').$idName.'/ressources/', $ressourcesPath)) {
				return('Unable to create the application ressources');
			}
			
			if (!@rename($this->_store->getData('pathTemp').$idName.'/app/', $appsPath.$idName)) {
				rollBackFolders($ressourcesPath);
				return('Unable to create the application path');
			}
			
			return ('ok');
		}
}

?>