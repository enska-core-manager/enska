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
 * @category   Library
 * @package    libs
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

imports ('enska.enskaXMLReader');
imports ('enska.enskaRequest');
imports ('enska.enskaFTP');

class EnsKa_PlateformPlugin
{
	Protected	$_alias;
	Protected	$_store;
	Protected	$_cmd;
	
	Public
		Function __construct()
		{
			GLOBAL	$_alias;
			$this->_alias = $_alias;
			$this->_store = new EnsKa_DataStore();
		}
		
	Protected
		Function getFrameworkInfos($infos)
		{
			$req = getComplexArray(dbSelect('pb.frameworks_infos', Array($infos['field']), Array($infos['where'] => $infos['id'])));
			return ($req[$infos['field']]);
		}
		
		Function getFrameworkNameBySite($name)
		{
			$req = getComplexArray(dbSelect('pb.site_infos', Array('framework'), Array('id_name'=>$name)));
			$req = getComplexArray(dbSelect('pb.frameworks_infos', Array('id_name'), Array('id'=>$req['framework'])));
			return ($req['id_name']);
		}
		
		Function getRights(&$store)
		{	
			$tmpAuth = $store->getData('authUsers');
			$tmpUnAuth = $store->getData('UnAuthUsers');
			
			$auth = explode('/', $tmpAuth);
			$unAuth = explode('/', $tmpUnAuth);
			
			$nbAuth = count($auth);
			$nbUnAuth = count($unAuth);
			
			if ($nbAuth > 1) {
				$i = 1;
				$j = 0;
				
				while ($i < $nbAuth) {
				
					unset($rights);
					$rights = explode('-', $auth[$i]);
					
					if (strcmp($rights[0], 'uid') == 0) {
						$authorization['uid']['auth'][$j] = "$rights[1]";
						$j++;
					}
					elseif (strcmp($rights[0], 'gid') == 0) {
						$authorization['gid']['auth'][$j] = "$rights[1]";
						$j++;
					}
					
					$i++;
				}
			}
			else {
				$authorization['gid']['auth'][0] = '2'; // Authorize only administrators and websitesOperators groups
				$authorization['gid']['auth'][1] = '3';
			}
			
			if ($nbUnAuth > 1) {
				$i = 1;
				$j = 0;
				
				while ($i < $nbUnAuth) {
				
					unset($rights);
					$rights = explode('-', $unAuth[$i]);
					
					if (strcmp($rights[0], 'uid') == 0) {
						$authorization['uid']['unAuth'][$j] = "$rights[1]";
						$j++;
					}
					elseif (strcmp($rights[0], 'gid') == 0) {
						$authorization['gid']['unAuth'][$j] = "$rights[1]";
						$j++;
					}
					
					$i++;
				}
			}
			else {
				if ($nbAuth <= 1) {
					$authorization['gid']['unAuth'][0] = '0'; //Unauthorize all groups
				}
			}
			
			if (!isset($authorization)){
				$authorization['gid']['auth'][0] = '2'; // Authorize only administrators and websitesOperators groups
				$authorization['gid']['auth'][1] = '3';
				$authorization['gid']['unAuth'][0] = '0'; //Unauthorize all groups
			}
			
			unset($tmpAuth, $tmpUnAuth, $auth, $unAuth, $nbAuth, $nbUnAuth, $rights);
			return (serialize($authorization));
		}
		
		Function strReplaceInFile($source, $target, $replace)
		{
			if (!($fs = @fopen($source, 'r'))) {
				echo 'No source file: '.$source.'<br>';
				trigger_error('No source file: '.$source.'****0x2501', E_USER_NOTICE);
				return (false);
			}
			
			if (!($fd = fopen($target, 'w'))) {
				echo 'No destination file: '.$target.'<br>';
				fclose($fs);
				trigger_error('No destination file: '.$target.'****0x2502', E_USER_NOTICE);
				return (false);
			}
			
			$buff = '';
			while (!(feof($fs))) {
				$buff .= fgets($fs);
			}
			fclose($fs);
			
			foreach($replace as $key => $val) {
				$buff = strReplace($buff, $key, $val);
			}
			
			fputs($fd, $buff);
			fclose($fd);
			
			unset($buff, $fd, $fs, $source, $target, $key, $val, $replace);
			return (true);
		}
		
		Function doImport($mod='zip')
		{
			$this->_store->setData('pathTemp', $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_temp'].md5(create_id(5)).'/');
			$this->_store->setData('temp', $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_temp']);
			
			if ($this->_store->getData('file_size_limit')) {
				$this->_store->setData('file_size_limit', $this->_store->getData('file_size_limit')* 1024 * 1204);
			}
			else {
				$this->_store->setData('file_size_limit', 1 * 1024 * 1204);
			}
			
			$err = 'NO FILE';
			foreach($_FILES as $file) {
				$_line = __LINE__;
				if(!empty($file['tmp_name'])) {
					$_line = __LINE__;
					if(is_uploaded_file($file['tmp_name'])) {
						$_line = __LINE__;
						if($file['size'] <= $this->_store->getData('file_size_limit')) {
							$this->_store->setData('fileName', md5(create_id(5)).'.tmp');
							$_line = __LINE__;
							if(move_uploaded_file($file['tmp_name'], $this->_store->getData('temp').$this->_store->getData('fileName'))) {
								$this->_store->setData('realName', $file['name']);
								if (strcmp($mod, 'zip') == 0) {
									if (strcmp(($res = $this->extractArchive()), 'ok') == 0) {
										return ('ok');
									}
									else {
										$err = $res;
									}
								}
								elseif (strcmp($mod, 'file') == 0) {
									return ('ok');
								}
								else {
									$err = 'Unknown extracting mod';
									$notice = $err.'****0x2503****'.($_line + 1).'****'.__FILE__;
								}
							}
							else {
								$err = 'Unable to create the TEMP file';
								$notice = $err.'****0x2503****'.($_line + 1).'****'.__FILE__;
							}
						}
						else {
							$err = 'File too big';
							$notice = $err.'****0x2504****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						$err = 'Error on upload';
						$notice = $err.'****0x2505****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'No file for uploading';
					$notice = $err.'****0x2506****'.($_line + 1).'****'.__FILE__;
				}
			}
			
			trigger_error($notice, E_USER_NOTICE);
			return ('{"success": true, "msg":{"msgInfo":"err;'.$err.'"}}');
		}
		
		Function exportOptions($opts)
		{
			$tmp = substr(md5(create_id(10)), 5, 5);
			
			$data = Array
			(
				'temp'			=> 	$this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_temp'],
				'archive' 		=> 	new EnsKa_ZIP(),
				'archiveName' 	=> 	$opts['archiveName'].'.'.$opts['archiveExtension'],
				'name'		 	=> 	$opts['archiveName'],
				'tmpFolder'		=>	$this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_temp'].$tmp.'/',
				'replace'		=>	$opts['replace'],
			);
			
			if (isset($opts['exportSchema'])) {
				$data['exportSchema'] = $opts['exportSchema'];
			}
			
			if (isset($opts['release'])) {
				$data['release'] = $opts['release'];
				$data['release_path'] = $opts['release_path'];
				$data['infos_path'] = $opts['infos_path'];
			}
			
			$this->_store->setDatas($data);
			
			if (isset($opts['exportSchema'])) {
				$schemasPath = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/schemas/export/';
				$struct = new EnsKa_XMLReader($schemasPath.$opts['exportSchema'].'.xml');
				if ($struct->getError()) {
					echo 'structSchema: '.$struct->getError().'<br>File: '.$schemasPath.$opts['exportSchema'].'.xml';
					trigger_error('structSchema: '.$struct->getError().' - File: '.$schemasPath.$opts['exportSchema'].'.xml****0x2507', E_USER_NOTICE);
					return (false);
				}
				
				$structInfos = Array
				(
					'path' => '',
					'pathOld' => '',
					'levelOld' => 0,
				);
					
				$this->_store->setData('structTree', $struct->getTree());
				$this->_store->setData('structInfos', $structInfos);
				$this->_store->setData('schemasPath', $schemasPath);
				unset($structInfos, $schemasPath, $struct);
			}
			
			unset($data, $tmp, $opts);
			return (true);
		}
		
		Function doExport()
		{
			$_line = __LINE__;
			if (@mkdir($this->_store->getData('tmpFolder'), 0777) || (is_dir($this->_store->getData('tmpFolder')))) {
				$_line = __LINE__;
				if ($this->_store->getData('archive')->open($this->_store->getData('temp').$this->_store->getData('archiveName'), ZipArchive::CREATE)) {
					$res = true;
					if ($this->_store->getData('exportSchema')) {
						$_line = __LINE__;
						$res = $this->parseSchema($this->_store->getData('structTree'), $this->_store->getData('structInfos'));
					}
					if ($res) {
						$this->runCPY();
						if (strcmp(($res = $this->makeArchive()), 'ok') == 0) {
							return ('success');
						}
						else {
							$err = 'Unable to make the archive -- '.$res;
							$notice = $err.'****0x2511****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						$err = 'Unable to create the folder tree';
						$this->_store->getData('archive')->close();
						rollBackFolders($this->_store->getData('tmpFolder'));
						$notice = $err.'****0x2510****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = $this->_store->getData('archive')->getError();
					$notice = $err.'****0x2509****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = $this->_store->getData('name').', access denied! (CREATE)';
				$notice = $err.'****0x2508****'.($_line + 1).'****'.__FILE__;
			}
			
			trigger_error($notice, E_USER_NOTICE);
			return ($err);
		}
		
		Function makeArchive()
		{
			$this->_store->getData('archive')->addDir($this->_store->getData('tmpFolder').$this->_store->getData('name'));
			$this->_store->getData('archive')->close();
			rollBackFolders($this->_store->getData('tmpFolder'));
			
			if ($this->_store->getData('release')) {
				if ($this->_store->getData('release') == 1){
					$r = substr(md5(create_id(10)), 5, 5).'.ini';
					$inf = new EnsKa_INIReader($this->_store->getData('infos_path'));
					$release = new EnsKa_INIReader($this->_store->getData('temp').$r, 'create');
					$release->addLine('versionId', $inf->getLine('versionId') + 1);
					$release->addLine('version', $inf->getLine('version'));
					$release->addLine('size', get_size(filesize($this->_store->getData('temp').$this->_store->getData('archiveName')), true));
					$release->addLine('release', date('Y-m-d'));
					$release->save();
					$inf->addLine('versionId', $release->getLine('versionId'), true);
					
					if (!is_dir($this->_store->getData('release_path'))) {
						if (!@mkdir($this->_store->getData('release_path'), 0755, true)); {
							return('EXPORTATION IS OK, BUT UNABLE TO RELEASE!<br>TRY TO RE-EXPORT');
						}
					}
					
					@unlink($this->_store->getData('release_path').$this->_store->getData('archiveName'));
					@unlink($this->_store->getData('release_path').'release.ini');
					@rename($this->_store->getData('temp').$this->_store->getData('archiveName'), $this->_store->getData('release_path').$this->_store->getData('archiveName'));
					@rename($this->_store->getData('temp').$r, $this->_store->getData('release_path').'release.ini');
					unset($r, $release, $inf);
					return ('ok');
				}
			}
			
			//$export = 'export_'.$this->_store->getData('exportSchema').'_destination';
			//rename($this->_store->getData('temp').$this->_store->getData('archiveName'), $this->_alias['path']['local'][$export].$this->_store->getData('archiveName'));
			
			return ('ok');
		}
		
		Function parseSchema(&$nodes, &$infos, $level=0)
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
					@$infos['path'] .= $node->attributes['name'].'/';
					if (trim($infos['path']) != '') {
						$path = strReplace($infos['path'], $this->_store->getData('replace'));
						@mkdir($this->_store->getData('tmpFolder').$path, 0, true);
					}
					
					$this->parseSchema(&$node->children, &$infos, $level + 1);
				}
				else {
					$infos['path'] .= $node->attributes['name'];
					if (strcmp($node->name, 'folder') == 0) {
						$path = strReplace($infos['path'], $this->_store->getData('replace'));
						@mkdir($this->_store->getData('tmpFolder').$path, 0, true);
					}
					elseif (strcmp($node->name, 'file') == 0) {
						$path = strReplace($infos['path'], $this->_store->getData('replace'));
						$this->strReplaceInFile($this->_store->getData('schemasPath').$this->_store->getData('exportSchema').'.ini', $this->_store->getData('tmpFolder').$path, $this->_store->getData('replace'));
					}
					
					$infos['levelOld'] = $level;
					$infos['pathOld'] = $infos['path'];
					$infos['path'] = substr($infos['path'], 0, strlen($infos['path']) - strlen($node->attributes['name']));
				}
			}
			return (true);
		}
		
		Function addCPY($command, $argv1, $argv2)
		{
			$cmd = Array($command, $argv1, $argv2);
			$this->_cmd []= $cmd;
			unset($cmd);
		}
		
		Function doCheckUpdate(&$params)
		{
			$this->_store->setDatas($params);
			
			$proto = strstr($this->_store->getData('url'), '://', true);
			$err = 'err;UNABLE TO GET UPDATE';
			
			switch ($proto) {
				case 'http':
					$tmpFile = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_temp'].md5(create_id(5)).'.ini';
					$curl = new EnsKa_Request($this->_store->getData('url').'/release.ini');
					$_line = __LINE__;
					if ($curl->isUrl()) {
						$_line = __LINE__;
						$res = $curl->getFile($tmpFile);
						if (!$curl->getError()) {
							$update = new EnsKa_INIReader($tmpFile);
							$_line = __LINE__;
							if (!$update->getError()) {
								$_line = __LINE__;
								if ($this->_store->getData('version') < $update->getLine('versionId')) {
									$curl->close();
									//unlink($tmpFile);
									unset($err, $tmpFile, $proto, $curl);	
									return('success;'.$update->getLine('versionId').';'.$update->getLine('version').';'.$update->getLine('size').';'.$update->getLine('release'));
								}
								else {
									unlink($tmpFile);
									$err = 'noUpdate;noUpdate';
								}
							}
							else {
								$err = $update->getError();
								$notice = $err.'****0x2512****'.($_line + 1).'****'.__FILE__;
								trigger_error($notice, E_USER_NOTICE);
							}
						}
						else {
							$err = 'err;'.$curl->getError();
							$notice = $curl->getError().'****0x2513****'.($_line + 1).'****'.__FILE__;
							trigger_error($notice, E_USER_NOTICE);
						}
					}
					else {
						$err = 'err;'.$curl->getError().'<br>URL NOT EXISTS';
						$notice = $curl->getError().' -- URL not exists****0x2514****'.($_line + 1).'****'.__FILE__;
						trigger_error($notice, E_USER_NOTICE);
					}
					$curl->close();
					
					break;
				case 'https':
					
					break;
				case 'ftp':
					$tmpFile = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_temp'].md5(create_id(5)).'.ini';
					$curl = new EnsKa_FTP();
					$curl->getFile($this->_store->getData('url').'/release.ini', $tmpFile);
					$_line = __LINE__;
					if (!$curl->getError()) {
						$update = new EnsKa_INIReader($tmpFile);
						$_line = __LINE__;
						if (!$update->getError()) {
							if ($this->_store->getData('version') < $update->getLine('versionId')) {
								echo 'success;'.$update->getLine('versionId').';'.$update->getLine('version').';'.$update->getLine('size').';'.$update->getLine('release');
								unlink($tmpFile);
								return (true);
							}
							else {
								$err = 'noUpdate;noUpdate';
							}
						}
						else {
							$err = 'err;'.$update->getError();
							$notice = $update->getError().'****0x2515****'.($_line + 1).'****'.__FILE__;
							trigger_error($notice, E_USER_NOTICE);
						}
					}
					else {
						$err = 'err;'.$curl->getError();
						$notice = $curl->getError().'****0x2516****'.($_line + 1).'****'.__FILE__;
						trigger_error($notice, E_USER_NOTICE);
					}
					$curl->close();
					
					break;
				default:
					$err = 'err;Error on update!<br>UNKNOWN PROTOCOL!';
					$notice = 'Error on update, unknown protocol****0x2517****0****'.__FILE__;
					trigger_error($notice, E_USER_NOTICE);
					break;
			}
			return($err);
			unset($err, $tmpFile, $proto, $curl, $update);
		}
		
	Private
		Function extractArchive()
		{
			$this->_store->setData('archive', new ZipArchive());
			
			$tmp = explode('.', $this->_store->getData('fileName'));
			$idName = $tmp[0];
			
			$_line = __LINE__;
			if (@$this->_store->getData('archive')->open($this->_store->getData('temp').$this->_store->getData('fileName'))) {
				$_line = __LINE__;
				if (@$this->_store->getData('archive')->extractTo($this->_store->getData('pathTemp'))) {
						@$this->_store->getData('archive')->close();
						@unlink($this->_store->getData('temp').$this->_store->getData('fileName'));
						unset($tmp, $idName);
						return ('ok');
				}
				else {
					$err = 'Unable to extract the archive file';
					$notice = $err.'****0x2518****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Unable to open the backup file';
				$notice = $err.'****0x2519****'.($_line + 1).'****'.__FILE__;
			}
				
			rollBackFolders($this->_store->getData('pathTemp'));
			@unlink($this->_store->getData('temp').$this->_store->getData('fileName'));
			unset($tmp, $idName);
			trigger_error($notice, E_USER_NOTICE);
			return ($err);
		}
		
		Function runCPY()
		{
			foreach($this->_cmd as $command) {
				@$command[0]($command[1], $command[2]);
			}
			unset($command);
		}
}

?>