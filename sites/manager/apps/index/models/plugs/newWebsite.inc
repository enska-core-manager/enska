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
 
imports ('enska.enskaXMLReader');

class newWebsite extends EnsKa_PlateformPlugin
{
	Public
		Function create($data=null)
		{
			$datas = Array
					(
						'authUsers' 	=>		$data[1],
						'UnAuthUsers' 	=>		$data[2],
						'config' 		=>		$data[3],
						'pathApp' 		=>		$data[4],
						'description'	=>		$data[5],
						'dplName'		=>		$data[6],
						'framework'		=>		$data[7],
						'alias'			=>		$data[8],
						'idName'		=>		$data[9],
						'cur'			=>		$this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$data[9].'/'
					);
			$this->_store->setDatas($datas);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'new_website')) {
				$_line = __LINE__;
				if (is_writable($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'])) {
					$_line = __LINE__;
					if (@mkdir($this->_store->getData('cur'), 0755)) {
						$_line = __LINE__;
						if ($this->createPathApps()) {
							$_line = __LINE__;
							if (@mkdir($this->_store->getData('cur').'conf/', 0755)) {
								$_line = __LINE__;
								if ($this->createConfig()) {
									$_line = __LINE__;
									if ($this->createFolders()) {
										$_line = __LINE__;
										if ($this->createPanel()) {
											$_line = __LINE__;
											if ($this->createAlias()) {
												$_line = __LINE__;
												if ($this->createWebsite()) {
													echo 'success';
													return;
												}
												else {
													$err = 'UNABLE TO CREATE THE WEBSITE';
													$notice = $err.'****1x1101****'.($_line + 1).'****'.__FILE__;
												}
											}
											else {
												$err = 'UNABLE TO CREATE THE ALIAS';
												$notice = $err.'****1x1102****'.($_line + 1).'****'.__FILE__;
											}
										}
										else {
											$err = 'UNABLE TO CREATE THE PANEL';
											$notice = $err.'****1x1103****'.($_line + 1).'****'.__FILE__;
										}
									}
									else {
										$err = 'UNABLE TO CREATE PATH APPS (2)';
										$notice = $err.'****1x1104****'.($_line + 1).'****'.__FILE__;
									}
								}
								else {
									$err ='UNABLE TO CREATE CONFIG';
									$notice = $err.'****1x1105****'.($_line + 1).'****'.__FILE__;
								}
							}
							else {
								$err = 'UNABLE TO CREATE CONFIG PATH';
								$notice = $err.'****1x1106****'.($_line + 1).'****'.__FILE__;
							}
						}
						else {
							$err = 'UNABLE TO CREATE PATH APPS (1)';
							$notice = $err.'****1x1107****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {	
						$err = 'UNABLE TO CREATE ROOT FOLDER';
						$notice = $err.'****1x1108****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'NON WRITABLE';
					$notice = $err.'****1x1109****'.($_line + 1).'****'.__FILE__;
				}
				
				if ($this->_store->getData('idName') != "") {
					rollBackFolders($this->_store->getData('cur'));
				}
			}
			else {
				$err = 'WRONG PARAMETERS';
				$notice = $err.'****1x1110****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($data, $datas, $err, $notice);
			return (false);
		}
		
	Private
		Function createPathApps()
		{
			$folders = explode('/', $this->_store->getData('pathApp'));
			if (is_array($folders)) {
				$nb = count($folders);
				$i = 0;
				$cur = $this->_store->getData('cur');
				while ($i < $nb) {
					if (trim($folders[$i]) != '') {
						$cur .= '/'.$folders[$i];
						@mkdir($cur, 0755);
					}
					$i++;
				}
				$this->_store->setData('pathApp', substr($cur, strlen($this->_store->getData('cur')) + 1, strlen($cur) - strlen($this->_store->getData('cur')) + 1).'/');
				return (true);
			}
			return (false);
		}
		
		Function createConfig()
		{
			$conf = new EnsKa_INIReader($this->_store->getData('cur').'conf/settings.ini', 'create');
			if ($conf->getError()) {
				echo $conf->getError().'<br>';
				return (false);
			}
			
			$conf->addLine('path_apps', $this->_store->getData('pathApp'));
			$conf->addLine('load.libs', 'conf/libs.ini');
			$conf->save();
			
			$libsFile = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'libs.ini');
			if ($libsFile->getError()) {
				echo $libsFile->getError().'<br>';
				return (false);
			}
			$libs = $libsFile->getLines();
			
			$confLibs = new EnsKa_INIReader($this->_store->getData('cur').'conf/libs.ini', 'create');
			if ($confLibs->getError()) {
				echo $confLibs->getError().'<br>';
				return (false);
			}
			
			foreach($libs as $lib => $mod) {
				$confLibs->addLine($lib, 'disable');
			}
			$confLibs->save();
			
			if (!@copy($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/site/cache.ini', $this->_store->getData('cur').'conf/cache.ini')) {
				echo 'Unable to copy cache.ini';
				return (false);
			}
			if (!@copy($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/site/cache_exclude.ini', $this->_store->getData('cur').'conf/cache_exclude.ini')) {
				echo 'Unable to copy cache_exclude.ini';
				return (false);
			}
			if (!@copy($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/site/logs.ini', $this->_store->getData('cur').'conf/logs.ini')) {
				echo 'Unable to copy logs.ini';
				return (false);
			}
			
			unset($conf, $libsFile, $confLibs);
			return (true);
		}
		
		Function createFolders()
		{
			$infos = Array('field' => 'id_name', 'where' => 'id', 'id' => $this->_store->getData('framework'));
			$frm = $this->getFrameworkInfos(&$infos);
			
			if (strncmp($frm, 'enskacore', strlen('enskacore')) != 0) {
				$struct = new EnsKa_XMLReader($this->_alias['path']['global']['frmPath'].$frm.'/schemas/site.xml');
				if ($struct->getError()) {
					echo $struct->getError().'<br>';
					return (false);
				}
				
				$tree = $struct->getTree();
				
				$infos = Array
						(
							'path' => '',
							'pathOld' => '',
							'levelOld' => 0,
						);
				
				$this->parseSiteStructure($tree, &$infos);
				
				if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/', 0755)) {
					if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/index/', 0755)) {
						if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/_commons/', 0755)) {
							if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/_commons/css/', 0755)) {
								if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/_commons/js/', 0755)) {
									if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/_commons/images/', 0755)) {
										if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/index/css/', 0755)) {
											if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/index/js/', 0755)) {
												if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/index/images/', 0755)) {
												
													$replace = Array
																(
																	'%framework%'	=>	$this->getFrameworkInfos(Array('field' => 'id_name', 'where' => 'id', 'id' => $this->_store->getData('framework'))),
																	'%description%'	=>	$this->_store->getData('description')
																);
													
													if (!$this->strReplaceInFile($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/application/app.settings.ini', $this->_store->getData('cur').$this->_store->getData('pathApp').'index/app.settings.ini', $replace)) {
														echo 'Unable to create app.settings.ini';
														return (false);
													}
													
													return (true);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else {
				if (@mkdir($this->_store->getData('cur').$this->_store->getData('pathApp').'_commons/', 0755)) {
					if (@mkdir($this->_store->getData('cur').$this->_store->getData('pathApp').'index/', 0755)) {
						if (@mkdir($this->_store->getData('cur').$this->_store->getData('pathApp').'index/models/', 0755)) {
							if (@mkdir($this->_store->getData('cur').$this->_store->getData('pathApp').'index/views/', 0755)) {
								if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/', 0755)) {
									if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/_commons/', 0755)) {
										if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/_commons/css/', 0755)) {
											if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/_commons/js/', 0755)) {
												if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/_commons/images/', 0755)) {
													if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/index/', 0755)) {
														if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/index/css/', 0755)) {
															if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/index/js/', 0755)) {
																if (@mkdir($this->_alias['path']['global']['users_ressources'].$this->_store->getData('idName').'/index/images/', 0755)) {
																	if (@copy($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/site/controller.php', $this->_store->getData('cur').$this->_store->getData('pathApp').'index/controller.php')) {
																	
																		$replace = Array
																					(
																						'%framework%'	=>	$this->getFrameworkInfos(Array('field' => 'id_name', 'where' => 'id', 'id' => $this->_store->getData('framework'))),
																						'%description%'	=>	$this->_store->getData('description')
																					);
																		
																		if (!$this->strReplaceInFile($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/application/app.settings.ini', $this->_store->getData('cur').$this->_store->getData('pathApp').'index/app.settings.ini', $replace)) {
																			echo 'Unable to create app.settings.ini';
																			return (false);
																		}
																		
																		return (true);
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			
			return (false);
		}
		
		Function parseSiteStructure($nodes, $infos, $level=0)
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
						@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('idName').'/'.$infos['path'], 0, true);
					}
					
					$this->parseSiteStructure($node->children, &$infos, $level + 1);
				}
				else {
				
					$infos['path'] .= $node->attributes['name'];
					
					if (strcmp($node->name, 'folder') == 0) {
						mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('idName').'/'.$infos['path'], 0, true);
					}
					else if (strcmp($node->name, 'file') == 0) {
						copy(
							$this->_alias['path']['global']['frmPath'].$infos['framework'].'/content/'.$node->attributes['name'],
							$this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('idName').'/'.$infos['path']
						);
					}
					
					$infos['levelOld'] = $level;
					$infos['pathOld'] = $infos['path'];
					$infos['path'] = substr($infos['path'], 0, strlen($infos['path']) - strlen($node->attributes['name']));
				}
			}
		}
		
		Function createPanel()
		{
			if (@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].'index/views/plugs/'.$this->_store->getData('idName'), 0755)) {
				if (@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].'index/views/plugs/'.$this->_store->getData('idName').'/'.$this->_store->getData('idName'), 0755)) {
				
					$files = Array('id', 'mod', 'panel');
					$cPath = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].'index/views/plugs/'.$this->_store->getData('idName').'/'.$this->_store->getData('idName').'/';	
					$replace = new EnsKa_DataStore();
					$replace->setData('%idName%', $this->_store->getData('idName'));
					
					foreach($files as $file) {
						$src = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/site/%idName%-'.$file.'.phtml';
						$dest = $cPath.$this->_store->getData('idName').'-'.$file.'.phtml';
						if (!$this->strReplaceInFile($src, $dest, $replace->getDatas())) {
							rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].'index/views/plugs/'.$this->_store->getData('idName'));
							return (false);
						}
					}
					
					return (true);
				}
			}
			
			rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].'index/views/plugs/'.$this->_store->getData('idName'));
			return (false);
			
		}
		
		Function createAlias()
		{
			$alias = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'alias.ini');
			if ($alias->getError()) {
				echo $alias->getError().'<br>';
				return (false);
			}
			$alias->addLine($this->_store->getData('alias'), $this->_store->getData('idName'), true);
			return (true);
		}
		
		Function createWebsite()
		{
			$nparams = Array('table' => get_table('pb.site_infos'), 'id' => 'id');
			$nid = create_new_id($nparams);
			
			$req = 	dbInsert('pb.site_infos', 
				Array(
					'id' => $nid, 
					'dpl_name' => $this->_store->getData('dplName'), 
					'id_name' => $this->_store->getData('idName'), 
					'description' => $this->_store->getData('description'), 
					'framework' => $this->_store->getData('framework')
				)
			);
			
			if (!$req) {
				return (false);
			}
			
			$authorizations = $this->getRights(&$this->_store);
			$req = dbInsert('pb.site_auth', Array('id' => $nid, 'rights' => $authorizations));
			
			if (!$req) {
				return (false);
			}
			return (true);
		}
}

?>