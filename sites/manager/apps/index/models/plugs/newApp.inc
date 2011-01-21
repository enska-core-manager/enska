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
 
class newApp extends EnsKa_PlateformPlugin
{
	Public
		Function create($data=null)
		{
			$params = Array
						(
							'authUsers' 	=>		$data[1],
							'UnAuthUsers' 	=>		$data[2],
							'name'	 		=>		$data[3],
							'idName'	 	=>		$data[4],
							'description'	=>		$data[5],
							'target'	 	=>		$data[6]
						);
			$this->_store->setDatas($params);

			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'new_app')) {
				$_line = __LINE__;
				$pathApps = get_app_path($this->_store->getData('target'));
				if ($pathApps) {
					$_line = __LINE__;
					if (!is_app_exists($this->_store->getData('idName'), $this->_store->getData('target'))) {
						$pathApp = $this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('target').'/'.$pathApps.$this->_store->getData('idName').'/';
						$this->_store->setData('pathApp', $pathApp);
						$_line = __LINE__;
						if (mkdir($this->_store->getData('pathApp'), 0755)) {
							$_line = __LINE__;
							if ($this->createFolders($this->_store)) {
								echo 'success';
								unset($data, $params, $_line, $pathApps, $pathApp);
								return (true);
							}
							else {
								@rollBackFolders($pathApp);
								@rollBackFolders($this->_alias['path']['global']['users_ressources'].$this->_store->getData('target').'/'.$this->_store->getData('idName'));
								$err = 'UNABLE TO CREATE THE NEW APPLICATION';
								$notice = $err.'****1x1801****'.($_line + 1).'****'.__FILE__;
							}
						}
						else {
							$err = 'UNABLE TO CREATE THE APPLICATION PATH';
							$notice = $err.'****1x1802****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						$err = 'APPLICATION ALREADY EXISTS!';
						$notice = $err.'****1x1803****'.($_line + 1).'****'.__FILE__;
					}
				}	
				else {
					$err = 'UNABLE TO OPEN WEBSITE\'S APPLICATIONS PATH :'.$this->_store->getData('target').$pathApps;
					$notice = $err.'****1x1804****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters';
				$notice = $err.'****1x1805****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notie, E_USER_NOTICE);
			unset($data, $params, $_line, $pathApps, $pathApp, $err, $notice);
			return (false);
		}
	
	Private
		Function createFolders()
		{
			$frm = $this->getFrameworkNameBySite($this->_store->getData('target'));
			if (strncmp($frm, 'enskacore', strlen('enskacore')) != 0) {
				$struct = new EnsKa_XMLReader($this->_alias['path']['global']['frmPath'].$frm.'/schemas/application.xml');
				if ($struct->getError()) {
					echo $struct->getError().'<br>';
					return (false);
				}
				
				$tree = $struct->getTree();
				$infos = Array(
						'path' => '',
						'pathOld' => '',
						'levelOld' => 0,
					);
				$this->parseApplicationStructure($tree, &$infos);
				
				if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('target').'/'.$this->_store->getData('idName').'/', 0755)) {
					if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('target').'/'.$this->_store->getData('idName').'/css/', 0755)) {
						if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('target').'/'.$this->_store->getData('idName').'/js/', 0755)) {
							if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('target').'/'.$this->_store->getData('idName').'/images/', 0755)) {
								$_line = __LINE__;
								$app = new EnsKa_INIReader($this->_store->getData('pathApp').'app.settings.ini', 'create');
								if (!$app->getError()) {
									$app->addLine('name', $this->_store->getData('name'));
									$app->addLine('idName', $this->_store->getData('idName'));
									$app->addLine('framework', $frm);
									$app->addLine('version', '1.0.0');
									$app->addLine('versionId', '1');
									$app->addLine('description', trim($this->_store->getData('description'), "\n"));	
									$app->addLine('rights', $this->getRights($this->_store));
									$app->addLine('updateFrom', 'none');
									$app->addLine('updateCheck', 'D');
									$app->addLine('updateAuto', '0');
									$app->save();
									return (true);
								}
								else {
									echo $app->getError().'<br>';
									trigger_error($app->getError().'****1x1808****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
									unset($_line, $app, $frm, $struct, $tree, $infos);
									return (false);
								}
							}
						}
					}
				}
			}
			else {
				if (@mkdir($this->_store->getData('pathApp').'/models/', 0755)) {
					if (@mkdir($this->_store->getData('pathApp').'/views/', 0755)) {
						if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('target').'/'.$this->_store->getData('idName').'/', 0755)) {
							if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('target').'/'.$this->_store->getData('idName').'/css/', 0755)) {
								if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('target').'/'.$this->_store->getData('idName').'/js/', 0755)) {
									if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('target').'/'.$this->_store->getData('idName').'/images/', 0755)) {
										$_line = __LINE__;
										if ($this->createController()) {
											$_line = __LINE__;
											$app = new EnsKa_INIReader($this->_store->getData('pathApp').'app.settings.ini', 'create');
											if (!$app->getError()) {
												$app->addLine('name', $this->_store->getData('name'));
												$app->addLine('idName', $this->_store->getData('idName'));
												$app->addLine('framework', $frm);
												$app->addLine('version', '1.0.0');
												$app->addLine('versionId', '1');
												$app->addLine('description', trim($this->_store->getData('description'), "\n"));
												$app->addLine('rights', $this->getRights($this->_store));
												$app->addLine('updateFrom', 'none');
												$app->addLine('updateCheck', 'D');
												$app->addLine('updateAuto', '0');
												$app->save();
												return (true);
											}
											else {
												echo $app->getError().'<br>';
												trigger_error($app->getError().'****1x1807****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
												unset($_line, $app, $frm);
												return (false);
											}
										}
										else {
											echo 'UNABLE TO CREATE THE NEW APPLICATION CONTROLLER<br>';
											trigger_error('UNABLE TO CREATE THE NEW APPLICATION CONTROLLER****1x1806****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
										}
									}
								}
							}
						}
					}
				}
			}
			unset($_line, $app, $frm);
			return (false);
		}
		
		Function parseApplicationStructure($nodes, $infos, $level=0)
		{	
			foreach ($nodes as $node) {
				
				if ($level == 1) {
					$infos['path'] = '';
				}
				else {
					if ($level < $infos['levelOld']) {
						$pos = $infos['levelOld'] - $level;
						$tmp = explode('/', $infos['path']);
						$nb = (count($tmp) - $pos) - 1;
						$i = 0;
						$infos['path'] = '';
						while ($i < $nb) {
							$infos['path'] .= $tmp[$i].'/';
							$i++;
						}
					}
				}
				
				if (isset($node->children)) {
				
					if (strcmp($node->name, 'framework') == 0) {
						$infos['framework'] = $node->attributes['name'];
					}
					
					if (strcmp(@$node->attributes['name'], @$infos['framework']) != 0) {
						$infos['path'] .= $node->attributes['name'].'/';
					}
					
					if (trim($infos['path']) != '') {
						@mkdir($this->_store->getData('pathApp').$infos['path'], 0, true);
					}
					
					$this->parseApplicationStructure($node->children, &$infos, $level + 1);
				}
				else {
				
					$infos['path'] .= $node->attributes['name'];
					
					if (strcmp($node->name, 'folder') == 0) {
						mkdir($this->_store->getData('pathApp').$infos['path'], 0, true);
					}
					else if (strcmp($node->name, 'file') == 0) {
						if (strncmp($node->attributes['name'], '%name%', 6) == 0) {
							$file = strReplace($node->attributes['name'], '%name%', ucfirst(strtolower($this->_store->getData('idName'))));
							$file = $this->_store->getData('pathApp').'/'.$infos['pathOld'].'/'.$file;
							$fd = fopen($file, 'w');
							fclose($fd);
							$this->createController($infos['framework'], $file, $this->_alias['path']['global']['frmPath'].$infos['framework'].'/content/'.$node->attributes['name']);
						}
						else {
							copy(
								$this->_alias['path']['global']['frmPath'].$infos['framework'].'/content/'.$node->attributes['name'],
								$this->_store->getData('pathApp').'/'.$infos['path']
							);
						}
					}
					
					$infos['levelOld'] = $level;
					$infos['pathOld'] = $infos['path'];
					$infos['path'] = substr($infos['path'], 0, strlen($infos['path']) - strlen($node->attributes['name']));
				}
			}
		}

		Function createController($frm='enska', $file=null, $default=null)
		{	
			if (strcmp($frm, 'enska') == 0) {
				$replace = Array('%name%' => ucfirst(strtolower($this->_store->getData('idName'))));
				$this->strReplaceInFile($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/application/controller.php', $this->_store->getData('pathApp').'controller.php', $replace);
			}
			else {
				$replace = Array('%name%' => $this->_store->getData('idName'));
				$this->strReplaceInFile($default, $file, $replace);
			}
			
			unset($replace, $frm, $file, $default);
			return (true);
		}
}

?>