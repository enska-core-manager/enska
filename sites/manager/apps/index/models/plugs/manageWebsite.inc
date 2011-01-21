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
 
class manageWebsite extends EnsKa_PlateformPlugin
{
	Public
		Function import($data=null)
		{
			$this->_store->setDatas($data);
			$this->_store->setData('file_size_limit', 100);
			
			$_line = __LINE__;
			if (strcmp(($res = $this->doImport()), 'ok') == 0) {
				$_line = __LINE__;
				if (strcmp(($res = $this->installArchive()), 'ok') == 0) {
					unset($data, $res, $_line);
					rollBackFolders($this->_store->getData('pathTemp'));
					if ($this->_store->getData('wizard')) {
						return ('{"success": true, "msg":{"msgInfo":"The website has been successfully imported !"}}');
					}
					else {
						echo '{"success": true, "msg":{"msgInfo":"The website has been successfully imported !"}}';
						return;
					}
				}
			}
			
			rollBackFolders($this->_store->getData('pathTemp'));
			if ($this->_store->getData('wizard')) {
				return ('{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}');
			}
			else {
				echo '{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}';
			}
			trigger_error('Unable to import the website****1x2****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $res, $_line);
			return;
		}
		
		Function export($data=null)
		{
			if (($data[1] == 1) || ($data[1] == 2)) {
				echo 'You can not export a system website !';
				return (false);
			}
			
			$req = getComplexArray(dbSelect('pb.site_infos', Array('dpl_name', 'id_name', 'description', 'framework'), Array('id' => $data[1])));
			
			$options = Array
			(
				'archiveExtension'	=>	'waf',
				'exportSchema'		=>	'website',
				'archiveName'		=>	$req['id_name'],
				'replace'			=>	Array
										(
											'%dplName%'		=>	$req['dpl_name'],
											'%idName%'		=>	$req['id_name'],
											'%description%'	=>	$req['description'],
											'%alias%'		=>	EnsKa_Alias::get()->getAlias($req['id_name']),
											'%framework%'	=>	$this->getFrameworkInfos(Array('field' => 'id_name', 'where' => 'id', 'id' => $req['framework'])),
										)
			);
			
			$_line = __LINE__;
			if (!$this->exportOptions($options)) {
				trigger_error('Unable to set the website export options****1x2101****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}

			$this->addCPY('copyDir', $this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$req['id_name'].'/', $this->_store->getData('tmpFolder').$req['id_name'].'/website/');
			$this->addCPY('copyDir', $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$req['id_name'].'/', $this->_store->getData('tmpFolder').$req['id_name'].'/plugs/views/');
			$this->addCPY('copyDir', $this->_alias['path']['global']['users_ressources'].$req['id_name'].'/', $this->_store->getData('tmpFolder').$req['id_name'].'/ressources/');
			$this->addCPY('copy', $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsModels'].$req['id_name'].'.inc', $this->_store->getData('tmpFolder').$req['id_name'].'/plugs/models/'.$req['id_name'].'.inc');
			
			echo $this->doExport();
			unset($options, $req);
		}
		
		Function editByGrid($data=null)
		{
			$datas = Array(
						'idWebsite' 	=>		$data[1],
						'dplName'	 	=>		$data[2],
						'idName' 		=>		$data[3],
						'description'	=>		$data[4]
			);	
			$this->_store->setDatas($datas);
			
			$_line = __LINE__;
			$req = dbUpdate
			(
				'pb.site_infos',
				Array
				(
					'dpl_name' 		=> 		$this->_store->getData('dplName'),
					'id_name'		=> 		$this->_store->getData('idName'),
					'description' 	=> 		$this->_store->getData('description')
				),
				Array
				(
					'id' 			=> 		$this->_store->getData('idWebsite')
				)
			);
			
			if ($req) {
				unset($req, $data);
				echo 'success';
				return (true);
			}
			else {
				unset($req, $data);
				echo 'UNABLE TO EDIT THE WEBSITE INFORMATIONS';
				trigger_error('Unable to edit the website informations****1x2102****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}
		}
		
		Function editByEditor($data=null)
		{
			$datas = Array(
						'authUsers' 	=>		$data[1],
						'UnAuthUsers' 	=>		$data[2],
						'config' 		=>		$data[3],
						'pathApp' 		=>		$data[4],
						'description'	=>		$data[5],
						'dplName'		=>		$data[6],
						'idName'		=>		$data[7],
						'alias'			=>		trim($data[8]),
						'idWebsite'		=>		$data[9],
						'cur'			=>		$this->_alias['path']['global']['point'].$data[7].'/'
			);
			$this->_store->setDatas($datas);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'edit_website'))
			{
				$_line = __LINE__;
				$req = dbUpdate
				(
					'pb.site_infos',
					Array
					(
						'dpl_name' 		=> 		$this->_store->getData('dplName'),
						'description' 	=> 		$this->_store->getData('description')
					),
					Array
					(
						'id' 			=> 		$this->_store->getData('idWebsite')
					)
				);
				
				if ($req) {
					$fd = fopen($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('idName').'/conf/settings.ini', 'w');
					$_line = __LINE__;
					if ($fd) {
						fputs($fd, $this->_store->getData('config'));
						fclose($fd);
						$_line = __LINE__;
						$conf = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('idName').'/conf/settings.ini');
						if (!$conf->getError()) {
							$conf->addLine('path_apps', $this->_store->getData('pathApp'), true);
							$_line = __LINE__;
							if (EnsKa_Alias::get()->deleteAlias($this->_store->getData('idName'))) {
								EnsKa_Alias::get()->AddAlias($this->_store->getData('idName'), $this->_store->getData('alias'));
								$authorizations = $this->getRights($this->_store);			
								$_line = __LINE__;
								$req = dbUpdate('pb.site_auth', Array('rights' => $authorizations), Array('id' => $this->_store->getData('idWebsite')));
								if ($req) {
									unset($fd, $req, $data, $_line);
									echo 'success';
									return (true);
								}
								else {
									$err = 'UNABLE TO UPDATE THE WEBSITE RIGHTS';
									$notice = 'Unable to update the website rights****1x2103****'.($_line + 1).'****'.__FILE__;
								}
							}
							else {
								$err = 'UNABLE TO UPDATE THE WEBSITE ALIAS ROUTE';
								$notice = 'Unable to upddate the website alias route****1x2104****'.($_line + 1).'****'.__FILE__;
							}
						}
						else {
							$err = $conf->getError();
							$notice = 'Unable to edit the website informations -- '.$conf->getError().'****1x2105****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						$err = 'UNABLE TO EDIT THE WEBSITE SETTINGS';
						$notice = 'Unable to edit the website settings****1x2106****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'UNABLE TO UPDATE DATABASE INFORMATIONS';
					$notice = 'Unable to edit the database informations****1x2107****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters';
				$notice = 'Unable to edit the website informations****1x2108****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($fd, $req, $data, $err, $_line, $notice);
			return (false);
		}
		
		Function delete($data=null)
		{
			$id = $data[1];
			
			if (($id == 1) || ($id == 2)) {
				echo 'YOU CAN NOT DELETE A SYSTEM WEBSITE!';
				trigger_error('Unable to delete a system website****1x2109****'.__LINE__.'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}
			
			$_line = __LINE__;
			$path = getComplexArray(dbSelect('pb.site_infos', Array('id_name'), Array('id' => $id)));
			$req = dbDelete('pb.site_infos', Array('id' => $id));
			
			if ($req) {
				$req = dbDelete('pb.site_auth', Array('id' => $id));
				rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$path['id_name'].'/');
				$pviews = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'];
				$pmodels = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsModels'];
				$prsrc = $this->_alias['path']['global']['users_ressources'];
				@unlink($pmodels.$path['id_name'].'.inc');
				rollBackFolders($pviews.$path['id_name'].'/');
				rollBackFolders($prsrc.$path['id_name'].'/');
				EnsKa_Alias::get()->deleteAlias($path['id_name']);
				unset($id, $path, $req, $_line);
				echo 'success';
				return (true);
			}
			else {
				echo 'UNABLE TO DELETE THE WEBSITE';
				trigger_error('Unable to delete the website****1x2110****'.__LINE__.'****'.__FILE__, E_USER_NOTICE);
				unset($id, $path, $req, $_line);
				return (false);
			}
		}
		
	Private
		Function installArchive()
		{
			$root = get_folder_list($this->_store->getData('pathTemp'));
			
			$idName = $root[0];
			$sitesPath = $this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'];
			$ressourcesPath = $this->_alias['path']['global']['users_ressources'].$idName.'/';
			
			if (is_dir($sitesPath.$idName)) {
				return ('Website already exist');
			}
			
			$conf = new EnsKa_INIReader($this->_store->getData('pathTemp').$idName.'/'.$idName.'.ini');
			if ($conf->getError()) {
				return ('Unable to read the website configuration. - '.$conf->getError());
			}
			
			$req = getComplexArray(dbSelect('pb.frameworks_infos', Array('id'), Array('id_name' => $conf->getLine('framework'))));
			if (!$req) {
				return ('You must install the correct framework system before importing this website');
			}
			
			$frmId = $req['id'];
			
			if (!@rename($this->_store->getData('pathTemp').$idName.'/website/', $sitesPath.$idName.'/')) {
				return('Unable to create the website path');
			}
			
			if (!@rename($this->_store->getData('pathTemp').$idName.'/ressources/', $ressourcesPath)) {
				rollBackFolders($sitesPath.$idName.'/');
				return('Unable to create the ressources path');
			}
			
			if (!@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$idName.'/', 0777)) {
				rollBackFolders($sitesPath.$idName.'/');
				rollBackFolders($ressourcesPath);
				return('Unable to create the plug-ins folder view (1)');
			}
			
			if (!@rename($this->_store->getData('pathTemp').$idName.'/plugs/views/'.$idName.'/', $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$idName.'/'.$idName.'/')) {
				rollBackFolders($sitesPath.$idName.'/');
				rollBackFolders($ressourcesPath);
				rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$idName.'/');
				return('Unable to create the plug-ins folder view (2)');
			}
			
			$files = null;
			$files = get_file_list($this->_store->getData('pathTemp').$idName.'/plugs/models/');
			if (is_array($files)) {
				foreach($files as $file) {
					@rename($this->_store->getData('pathTemp').$idName.'/plugs/models/'.$file, $this->_alias['path']['local']['path_plugsModels'].$idName.'_'.$file);
				}
			}
			
			$nparams = Array('table' => get_table('pb.site_infos'), 'id' => 'id');
			$nid = create_new_id($nparams);
			
			$req = 	dbInsert('pb.site_infos', 
				Array(
					'id' => $nid, 
					'dpl_name' => $conf->getLine('dplName'), 
					'id_name' => $conf->getLine('idName'), 
					'description' => $conf->getLine('description'), 
					'framework' => $frmId
				)
			);
			
			if (!$req) {
				rollBackFolders($sitesPath.$idName.'/');
				rollBackFolders($ressourcesPath);
				rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$idName.'/');
				if (is_array($files)) {
					foreach($files as $file) {
						@unlink($this->_alias['path']['local']['path_plugsModels'].$idName.'_'.$file);
					}
				}
				return('Unable to save the website');
			}
			
			$this->_store->setData('authUsers', true);
			$this->_store->setData('UnAuthUsers', true);
			$authorizations = $this->getRights(&$this->_store);
			$req = dbInsert('pb.site_auth', Array('id' => $nid, 'rights' => $authorizations));
			
			if (!$req) {
				rollBackFolders($sitesPath.$idName.'/');
				rollBackFolders($ressourcesPath);
				rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$idName.'/');
				if (is_array($files)) {
					foreach($files as $file) {
						@unlink($this->_alias['path']['local']['path_plugsModels'].$idName.'_'.$file);
					}
				}
				$err = 'Unable to set the website rights.';
				$req = dbDelete('pb.sites_infos', Array('id' => $nid));
				if (!req) {
					$err .= ' Check your database to clean up the corrupt data (sites_infos).';
				}
				return($err);
			}
			
			if (!EnsKa_Alias::get()->addAlias($idName, $this->_store->getData('sOptions'))) {
				rollBackFolders($sitesPath.$idName.'/');
				rollBackFolders($ressourcesPath);
				rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$idName.'/');
				if (is_array($files)) {
					foreach($files as $file) {
						@unlink($this->_alias['path']['local']['path_plugsModels'].$idName.'_'.$file);
					}
				}
				$err = 'Unable to read the alias file. - '.$alias->getError().' - ';
				$req = dbDelete('pb.sites_infos', Array('id' => $nid));
				if (!req) {
					$err .= ' Check your database (pb.sites_infos) to clean up the corrupt data.';
				}
				$req = dbDelete('pb.site_auth', Array('id' => $nid));
				if (!req) {
					$err .= ' Check your database (pb.site_auth) to clean up the corrupt data.';
				}
				return ($err);
			}
			
			return ('ok');
		}
}

?>