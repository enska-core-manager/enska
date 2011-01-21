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
 
class newPlugin extends EnsKa_PlateformPlugin
{
	Public
		Function create($data=nulll)
		{	
			$datas = Array(
						'plgIDName'			=>  $data[5],
						'plgName'		 	=>	$data[1],
						'plgType'	 		=>	$data[2],
						'websiteName' 		=>	$data[3],
						'display'	 		=>	$data[4]
					);
			$this->_store->setDatas($datas);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'new_plugin')) {
				$_line = __LINE__;
				if (!is_plugin_exists($this->_store->getData('plgIDName'), $this->_store->getData('websiteName'))) {
					$plgPath = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsViews'].$this->_store->getData('websiteName').'/'.$this->_store->getData('plgIDName').'/';
					$this->_store->setData('plgPath', $plgPath);
					$_line = __LINE__;
					if ((($this->_store->getData('display') == BROWSER) || ($this->_store->getData('display') == BROWSERANDMENU)) && ($this->_store->getData('plgType') == WINDOW)) {
						$err = 'Window type is uncompatible with the browser. Select menu';
						$notice = $err.'****1x1608****'.($_line + 1).'****'.__FILE__;
					}
					else {
						$_line = __LINE__;
						if (mkdir($plgPath, 0755)) {
							$_line = __LINE__;
							$settings = new EnsKa_INIReader($plgPath.'settings.ini', 'create');
							if (!$settings->getError()) {
								$settings->addLine('name', $this->_store->getData('plgName'));
								$settings->addLine('id', $this->_store->getData('plgIDName'));
								$settings->addLine('type', $this->_store->getData('plgType'));
								$settings->addLine('display', $this->_store->getData('display'));
								$settings->addLine('versionId', '1');
								$settings->addLine('version', '1.0.0');
								$settings->addLine('updateFrom', '');
								$settings->addLine('updateCheck', 'D');
								$settings->addLine('updateAuto', '0');
								$_line = __LINE__;
								$settings->save();
								if (!$settings->getError()) {
									$_line = __LINE__;
									if ($this->createPanel()) {
										$_line = __LINE__;
										$model = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_plugsModels'].$this->_store->getData('plgIDName').'.inc', 'create');					
										if (!$model->getError()) {
											echo 'success';
											unset($data, $datas, $_line, $settings, $model);
											return (true);
										}
										else {
											$err = 'UNABLE TO CREATE PLUG-IN MODEL<br>';
											$err .= $model->getError();
											$notice = $err.'****1x1601****'.($_line + 1).'****'.__FILE__;
											rollBackFolders($this->_store->getData('plgPath'));
										}
									}
									else {
										$err = 'UNBALE TO CREATE THE PLUG-IN FILES';
										$notice = $err.'****1x1602****'.($_line + 1).'****'.__FILE__;
										rollBackFolders($this->_store->getData('plgPath'));
									}
								}
								else {
									$err = 'UNBALE TO CREATE THE CONFIGURATION FILE! (2)';
									$notice = $err.'****1x1603****'.($_line + 1).'****'.__FILE__;
									rollBackFolders($this->_store->getData('plgPath'));
								}
							}
							else {
								$err = 'UNABLE TO CREATE THE CONFIGURATION FILE! (1)';
								$notice = $err.'****1x1604****'.($_line + 1).'****'.__FILE__;
								rollBackFolders($this->_store->getData('plgPath'));
							}
						}
						else {
							$err = 'UNBALE TO CREATE THE PLUG-IN\'S FOLDER';
							$notice = $err.'****1x1605****'.($_line + 1).'****'.__FILE__;
						}
					}
				}
				else {
					$err = 'PLUG-IN ALREADY EXISTS!';
					$notice = $err.'****1x1606****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters!';
				$notice = $err.'****1x1607****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($data, $datas, $_line, $settings, $model, $notice, $err);
			return (false);
		}
	
	Private
		Function createPanel()
		{
			if ($this->_store->getData('plgType') == PANEL) {
				$mods = Array('mod', 'id', 'panel');
				$replace = Array('%name%' => $this->_store->getData('plgIDName'), '%DPLName%' => $this->_store->getData('plgName'));
				
				foreach($mods as $mod) {
					$source = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/plugin/%name%-'.$mod.'.phtml';
					$target = $this->_store->getData('plgPath').$this->_store->getData('plgIDName').'-'.$mod.'.phtml';
					$this->strReplaceInFile($source, $target, $replace);
				}
				
				unset($mods, $replace, $source, $target);
				return (true);
			}
			elseif ($this->_store->getData('plgType') == WINDOW) {
				$replace = Array('%name%' => $this->_store->getData('plgIDName'), '%DPLName%' => $this->_store->getData('plgName'));
				$this->strReplaceInFile($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/plugin/%name%-win.phtml', $this->_store->getData('plgPath').$this->_store->getData('plgIDName').'-win.phtml', $replace);
				
				unset($replace);
				return (true);
			}
		}
}

?>