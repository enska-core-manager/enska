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
 
class managePlugins extends EnsKa_PlateformPlugin
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
						return ('{"success": true, "msg":{"msgInfo":"The plug-in has been successfully imported !"}}');
					}
					else {
						echo '{"success": true, "msg":{"msgInfo":"The plug-in has been successfully imported !"}}';
					}
					return;
				}
			}
		}
		
		Function export($data=null)
		{
			$plug = $data[1];
			$website = $data[2];
			$release = $data[3];
			
			
			$options = Array
			(
				'archiveExtension'	=>	'pex',
				'exportSchema'		=>	'plugin',
				'archiveName'		=>	$plug,
				'release'			=>	$release,
				'release_path'		=>	$this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_updates'].'plugins/'.$plug.'/',
				'infos_path'		=>	$this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$website.'/'.$plug.'/settings.ini',
				'replace'			=>	Array
										(
											'%idName%'		=>	$plug
										)
			);
			
			$_line = __LINE__;
			if (!$this->exportOptions($options)) {
				trigger_error('Unable to set the plug-in export options****1x2501****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				return (false);
			}
			
			$this->addCPY('copyDir',$this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$website.'/'.$plug.'/', $this->_store->getData('tmpFolder').$plug.'/view/');
			$this->addCPY('copy', $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsModels'].'/'.$plug.'.inc', $this->_store->getData('tmpFolder').$plug.'/model/'.$plug.'.inc');
			
			echo $this->doExport();
			unset($plug, $website, $options, $release, $data);
		}
		
		Function editByGrid($data=null)
		{
			$datas = Array(
						'idPlugin' 		=>		$data[1],
						'idWebsite'		=>		$data[2],
						'plgType'		=>		$data[3],
						'display' 		=>		$data[4]
			);		
			$this->_store->setDatas($datas);
			
			$file = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$this->_store->getData('idWebsite').'/'.$this->_store->getData('idPlugin').'/settings.ini';
			$_line = __LINE__;
			if (file_exists($file)) {
				$_line = __LINE__;
				if ((($this->_store->getData('display') == BROWSER) || ($this->_store->getData('display') == BROWSERANDMENU)) && ($this->_store->getData('plgType') == 'Window')) {
					echo 'Window type is uncompatible with the browser. Select menu !';
					trigger_error('Window type is uncompatible with the browser. Select menu****1x2502****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					unset($datas, $file, $settings, $_line);
					return (false);
				}
				else {
					$settings = new EnsKa_INIReader($file);
					$settings->deleteLine('display');
					$settings->addLine('display', $this->_store->getData('display'));
					$settings->save();
				}
			}
			else {
				echo 'UNABLE TO UPDATE THE PLUG-IN';
				trigger_error('Unable to update the plug-in****1x2503****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($datas, $file, $settings, $_line);
				return (false);
			}
			
			echo 'success';
			unset($datas, $file, $settings, $_line);
			return (true);
		}
		
		Function editByEditor($data=null)
		{
			$editType = Array
						(
							'win', 
							'panel', 
							'mod'
						);
			
			$params = Array
						(
							'plugin'		=>		$data[1],
							'website'		=>		$data[2],
							'editpanel'		=>		$data[3],
							'editwin'		=>		$data[4],
							'editmod'		=>		$data[5],
							'editModel'		=>		$data[6],
							'editType'		=>		$editType,
							'updateFrom'	=>		$data[7],
							'updateCheck'	=>		$data[8],
							'updateAuto'	=>		$data[9],
							'version'		=>		$data[10],
							'versionId'		=>		$data[11]
						);
			$this->_store->setDatas($params);
			
			$path = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$this->_store->getData('website').'/'.$this->_store->getData('plugin').'/';
			$model = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsModels'].$this->_store->getData('plugin').'.inc';
			$err = '';
			
			$_line = __LINE__;
			$settings = new EnsKa_INIReader($path.'settings.ini');
			if (!$settings->getError()) {
				$settings->addLine('version', $this->_store->getData('version'));
				$settings->addLine('versionId', $this->_store->getData('versionId'));
				$settings->addLine('updateFrom', $this->_store->getData('updateFrom'));
				$settings->addLine('updateCheck', $this->_store->getData('updateCheck'));
				$settings->addLine('updateAuto', $this->_store->getData('updateAuto'));
				$settings->save();
			}
			else {
				echo 'UNABLE TO SAVE PLUG-IN SETTINGS<br>'.$settings->getError();
				trigger_error('Unable to save plug-in settings -- '.$settings->getError().'****1x2504****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($settings, $path, $model, $params, $editType, $err);
				return (false);
			}
			
			$_line = __LINE__;
			if ($fd = fopen($model.'.cp', 'w')) {
				if (trim($this->_store->getData('editModel')) != '') {
					$_line = __LINE__;
					if (!fwrite($fd, $this->_store->getData('editModel'))) {
						$err .= 'UNABLE TO SAVE PHP MODEL (1)<br>';
						trigger_error('Unable to save PHP model (1)****1x2505****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
						@unlink ($model.'.cp');
					}
				}
				fclose($fd);
				$_line = __LINE__;
				if (!@copy($model.'.cp', $model)) {
					$err .= 'UNABLE TO SAVE PHP MODEL (2)<br>';
					trigger_error('Unable to save PHP model (2)****1x2506****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				}
				else {
					@unlink($model.'.cp');
				}
			}
			else {
				$err .= 'UNABLE TO OPEN THE PHP MODEL SOURCE CODE';
				trigger_error('Unable to open the PHP model source code****1x2507****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			}
				
			foreach($this->_store->getData('editType') as $type) {
				$file = $path.$this->_store->getData('plugin').'-'.$type.'.phtml';
				if (file_exists($file)) {
					$_line = __LINE__;
					if ($fd = fopen($file.'.cp', 'w')) {
						$_line = __LINE__;
						if (!fputs($fd, $this->_store->getData('edit'.$type))) {
							fclose($fd);
							$err .= 'UNABLE TO SAVE THE '.strtoupper($type).' SOURCE (1)<br>';
							trigger_error('Unable to save the '.strtoupper($type).' source (1)****1x2508****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
							@unlink ($file.'.cp');
						}
					}
					else {
						fclose($fd);
						$err .= 'UNABLE TO OPEN THE '.strtoupper($type).' SOURCE (1)<br>';
						trigger_error('Unable to open the '.strtoupper($type).' source (1)****1x2509****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					}
				}
				else {
					if (trim($this->_store->getData('edit'.$type)) != '') {
						$_line = __LINE__;
						if ($fd = fopen($file.'.cp', 'w')) {
							$_line = __LINE__;
							if (!fputs($fd, $this->_store->getData('edit'.$type))) {
								fclose($fd);
								$err .= 'UNABLE TO SAVE THE '.strtoupper($type).' SOURCE (3)<br>';
								trigger_error('Unable to save the '.strtoupper($type).' source (3)****1x2510****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
								@unlink ($file.'.cp');
							}
						}
						else {
							fclose($fd);
							$err .= 'UNABLE TO OPEN THE '.strtoupper($type).' SOURCE (2)<br>';
							trigger_error('Unable to open the '.strtoupper($type).' source (2)****1x2511****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
						}
					}
				}
			}
			
			foreach($this->_store->getData('editType') as $type) {
				$source = $path.$this->_store->getData('plugin').'-'.$type.'.phtml.cp';
				$target = $path.$this->_store->getData('plugin').'-'.$type.'.phtml';
				if (file_exists($source)) {
					$_line = __LINE__;
					if (!@copy($source, $target)) {
						$err .= 'UNABLE TO SAVE THE '.strtoupper($type).' SOURCE (2)<br>';
						trigger_error('Unable to save the '.strtoupper($type).' source (2)****1x2512****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					}
					else {
						@unlink($source);
					}
				}
			}
			
			unset($params, $path, $model, $source, $target, $fd, $type, $file, $editType, $conf, $_line);
			
			if ($err != '') {
				echo $err;
				unset($err, $_line);
				return (false);
			}
			
			echo 'success';
			return (true);
		}
		
		Function delete($data=null)
		{
			$params = Array(
				'plugin' => $data[1],
				'website' => $data[2]
			);
			$this->_store->setDatas($params);
			
			$path = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$this->_store->getData('website').'/'.$this->_store->getData('plugin').'/';
			$model = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsModels'].$this->_store->getData('plugin').'.inc';
			@copy($model, $model.'.bkp');
			
			$_line = __LINE__;
			if (@unlink($model)) {
				$_line = __LINE__;
				if (rollBackFolders($path)) {
					@unlink($model.'.bkp');
					echo 'success';
					unset($params, $model, $path, $_line);
					return (true);
				}
				else {
					$err = 'UNABLE TO DELETE THE PLUG-IN\'S (2)';
					trigger_error('Unable to delete the plug-in (2)****1x2513****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					@copy($model.'.bkp', $model);
				}
			}
			else {
				$err = 'UNABLE TO DELETE THE PLUG-IN\'S (1)';
				trigger_error('Unable to delete the plug-in (1)****1x2514****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			}
			
			echo $err."<br>$model";
			unset($err, $params, $model, $path, $_line);
			return (false);
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
			
			if (!@rename($this->_store->getData('pathTemp').$idName.'/view/', $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$this->_store->getData('sOptions').'/'.$idName.'/')) {
				return ('Unable to create the plug-in view');
			}
			
			if (!@rename($this->_store->getData('pathTemp').$idName.'/model/'.$idName.'.inc', $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsModels'].$idName.'.inc')) {
				rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$this->_store->getData('sOptions').'/'.$idName.'/');
				return ('Unable to create the plug-in model');
			}
			
			return ('ok');
		}
}

?>