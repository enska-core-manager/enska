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
 
class manageFrameworks extends EnsKa_PlateformPlugin
{
	Public
		Function import($data=null)
		{
			$this->_store->setDatas($data);
			$this->_store->setData('file_size_limit', 50);
			
			$_line = __LINE__;
			if (strcmp(($res = $this->doImport()), 'ok') == 0) {
				$_line = __LINE__;
				if (strcmp(($res = $this->installImport()), 'ok') == 0) {
					unset($data, $res, $_line);
					rollBackFolders($this->_store->getData('pathTemp'));
					if ($this->_store->getData('wizard')) {
						return ('{"success": true, "msg":{"msgInfo":"The framework has been successfully imported !"}}');
					}
					else {
						echo '{"success": true, "msg":{"msgInfo":"The framework has been successfully imported !"}}';
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
			trigger_error('Unable to import the system framework****1x2****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $res, $_line);
			return;
		}
		
		Function export($data=null)
		{
			$framework = $data[1];
			
			$options = Array
			(
				'archiveExtension'	=>	'frm',
				'archiveName'		=>	$framework,
				'exportSchema'		=>	'framework',
				'replace'			=>	Array
										(
											'%idName%'	=>	$framework
										)
			);
			
			$_line = __LINE__;
			if (!$this->exportOptions($options)) {
				trigger_error('Unable to set the framework export option****1x23020****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($framework, $options, $data, $_line);
				return (false);
			}
			
			$settings = new EnsKa_INIReader($this->_alias['path']['global']['frmPath'].$framework.'/settings.ini', 'create');
			$settings->addLine('name', $framework, true);
			
			$this->addCPY('copyDir', $this->_alias['path']['global']['frmPath'].$framework.'/', $this->_store->getData('tmpFolder').$framework.'/');
			$this->addCPY('copy', $this->_alias['path']['global']['frmPath'].$framework.'/settings.ini', $this->_store->getData('tmpFolder').$framework.'/settings.ini');
			
			echo $this->doExport();
			unlink($this->_alias['path']['global']['frmPath'].$framework.'/settings.ini');
			unset($framework, $options, $data, $_line);
		}
		
		Function editByGrid($data=null)
		{
			$datas = Array
					(
						'id' 			=>		$data[1],
						'dplName'	 	=>		$data[2],
						'idName' 		=>		$data[3],
						'version'		=>		$data[4],
						'revision'		=>		$data[5]
					);	
			$this->_store->setDatas($datas);
			
			$_line = __LINE__;
			$req = dbUpdate
			(
				'pb.frameworks_infos',
				Array
				(
					'name' 			=> 		$this->_store->getData('dplName'),
					'id_name'		=> 		$this->_store->getData('idName'),
					'version' 		=> 		$this->_store->getData('version'),
					'revision' 		=> 		$this->_store->getData('revision')
				),
				Array
				(
					'id' 			=> 		$this->_store->getData('id')
				)
			);
			
			if ($req) {
				unset($req, $data, $_line, $datas);
				echo 'success';
				return (true);
			}
			else {
				echo 'UNABLE TO EDIT THE FRAMEWORK INFORMATIONS';
				trigger_error('Unable to edit the framework informations****1x23001****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($req, $data, $_line, $datas);
				return (false);
			}
		}
		
		Function editByEditor($data=null)
		{
			$datas = Array
					(
						'name'		=>	$data[1],
						'version'	=>	$data[2],
						'revision'	=>	$data[3],
						'loader'	=>	$data[4],
						'framework'	=>	$data[5]
					);
			$this->_store->setDatas($datas);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'edit_framework'))
			{
				$_line = __LINE__;
				$req = dbUpdate
				(
					'pb.frameworks_infos',
					Array
					(
						'name' 		=>	$this->_store->getData('name'),
						'version' 	=>	$this->_store->getData('version'),
						'revision' 	=>	$this->_store->getData('revision')
					),
					Array
					(
						'id_name'	=>	$this->_store->getData('framework')
					)
				);
				
				if ($req) {
					$_line = __LINE__;
					$fd = fopen($this->_alias['path']['global']['frmPath'].$this->_store->getData('framework').'/loader.php', 'w');
					if ($fd) {
						$this->_store->setData('loader', strReplace($this->_store->getData('loader'), ':-:', '::'));
						$_line = __LINE__;
						if (fputs($fd, $this->_store->getData('loader'))) {
							fclose($fd);
							echo 'success';
							unset($fd, $req, $data, $datas, $err, $_line, $notice);
							return (true);
						}
						else {
							$err = 'UNABLE TO EDIT THE LOADER FILE';
							$notice = 'Unable to edit the loader file****1x23007****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						$err = 'UNABLE TO OPEN THE LOADER FILE'.'<br>'.$this->_alias['path']['global']['frmPath'].$this->_store->getData('framework').'/loader.php';
						$notice = 'Unable to open the loader file****1x23006****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'UNABLE TO UPDATE FRAMEWORK INFORMATIONS';
					$notice = 'Unable to edit the framework informations****1x23005****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters';
				$notice = $err.'****1x23004****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($fd, $req, $data, $datas, $err, $_line, $notice);
			return (false);
		}
		
		Function delete($data=null)
		{
			$id = $data[1];
			
			if (($id == 1)) {
				echo 'YOU CAN NOT DELETE A SYSTEM FRAMEWORK!';
				trigger_error('Unable to delete a system framework****1x23002****'.__LINE__.'****'.__FILE__, E_USER_NOTICE);
				unset($data, $id);
				return (false);
			}
			
			$frm = getComplexArray(dbSelect('pb.frameworks_infos', Array('id_name'), Array('id' => $id)));
			$_line = __LINE__;
			$req = dbDelete('pb.frameworks_infos', Array('id' => $id));
			
			if ($req) {
				rollBackFolders($this->_alias['path']['global']['frmPath'].$frm['id_name']);
				unset($id, $frm, $req, $_line);
				echo 'success';
				return (true);
			}
			else {
				echo 'UNABLE TO DELETE THE FRAMEWORK';
				trigger_error('Unable to delete the framework****1x23003****'.$_line.'****'.__FILE__, E_USER_NOTICE);
				unset($id, $frm, $req, $_line, $data);
				return (false);
			}
		}
		
		Function createContentFile($data=null)
		{
			$params = Array
						(
							'fileName'	=>	$data[1],
							'filePath'	=>	$this->_alias['path']['global']['frmPath'].$data[2].'/content/'.$data[1],
							'framework'	=>	$data[2]
						);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (!file_exists($this->_store->getData('filePath'))) {
				$_line = __LINE__;
				$fd = fopen($this->_store->getData('filePath'), 'w');
				if ($fd) {
					fclose($fd);
					echo 'success';
					unset($data, $params, $_line, $fd);
					return (true);
				}
				else {
					$err = 'Unable to create the new content file !';
					$notice = $err.' File: '.$this->_store->getData('filePath').'****1x23008****'.$_line.'****'.__FILE__;
				}
			}
			else {
				$err = 'The content file '.$this->_store->getData('fileName').' already exists !';
				$notice = $err.'****1x23009****'.$_line.'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($data, $params, $_line, $notice, $fd, $err);
			return (false);
		}
		
		Function importContentFile($data=null)
		{
			$this->_store->setData('contentPath', $this->_alias['path']['global']['frmPath'].$data[1].'/content/');
			$this->_store->setData('file_size_limit', 3);
			
			$_line = __LINE__;
			if (strcmp(($res = $this->doImport('file')), 'ok') == 0) {
				$_line = __LINE__;
				if (strcmp(($res = $this->addContentFile()), 'ok') == 0) {
					unset($data, $res, $_line);
					rollBackFolders($this->_store->getData('pathTemp'));
					unlink($this->_store->getData('temp').$this->_store->getData('fileName'));
					echo '{"success": true, "msg":{"msgInfo":"The content file has been uploaded !"}}';
					return;
				}
			}
			
			echo '{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}';
			trigger_error('Unable import the content file****1x23010****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $res, $_line);
			return;
		}
		
		Function editContentFile($data=null)
		{
			$params = Array
						(
							'content'	=>	$data[1],
							'fileName'	=>	$data[2],
							'filePath'	=>	$this->_alias['path']['global']['frmPath'].$data[3].'/content/'.$data[2],
							'framework'	=>	$data[3]
						);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (file_exists($this->_store->getData('filePath'))) {
				$fd = fopen($this->_store->getData('filePath'), 'w');
				$_line = __LINE__;
				if ($fd) {
					fputs($fd, $this->_store->getData('content'));
					fclose($fd);
					echo 'success';
					unset($params, $data, $_line, $fd);
					return(true);
				}
				else {
					$err = 'Unable to open file';
					$notice = $err.'****1x23011****'.$_line.'****'.__FILE__;
				}
			}
			else {
				$err = 'The content file not exists';
				$notice = $err.'****1x23012****'.$_line.'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($params, $data, $_line, $fd, $err, $notice);
			return (false);
		}
		
		Function deleteContentFile($data=null)
		{
			$params = Array
						(
							'fileName'	=>	$data[1],
							'filePath'	=>	$this->_alias['path']['global']['frmPath'].$data[2].'/content/'.$data[1],
							'framework'	=>	$data[2]
						);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (file_exists($this->_store->getData('filePath'))) {
				if (unlink($this->_store->getData('filePath'))) {
					echo 'success';
					unset($data, $_line, $params);
					return (true);
				}
				else {
					$err = 'Unable to delete the selected file !';
					$notice = $err.' File: '.$this->_store->getData('filePath').'****1x23013****'.$_line.'****'.__FILE__;
				}
			}
			else {
				$err = 'Unable to delete the selected file !';
				$notice = $err.' File: '.$this->_store->getData('filePath').'****1x23014****'.$_line.'****'.__FILE__;
			}
			
			echo $err;
			unset($data, $params, $_line, $notice, $err);
			return (false);
		}
		
		Function editSchemas($data=null)
		{
			$params = Array
						(
							'site'			=>	$data[1],
							'application'	=>	$data[2],
							'schemasPath'	=>	$this->_alias['path']['global']['frmPath'].$data[3].'/schemas/',
						);
			$this->_store->setDatas($params);
			$site = $this->_store->getData('schemasPath').'site.xml';
			$app = $this->_store->getData('schemasPath').'application.xml';
			
			$_line = __LINE__;
			if (file_exists($site)) {
				$fd = fopen($site, 'w');
				$_line = __LINE__;
				if ($fd) {
					fputs($fd, $this->_store->getData('site'));
					fclose($fd);
				}
				else {
					$err = 'Unable to open site schema';
					$notice = $err.'****1x23015****'.$_line.'****'.__FILE__;
				}
			}
			else {
				$err = 'The site schema file not exists';
				$notice = $err.'****1x23016****'.$_line.'****'.__FILE__;
			}
			
			$_line = __LINE__;
			if (file_exists($app)) {
				$fd = fopen($app, 'w');
				$_line = __LINE__;
				if ($fd) {
					fputs($fd, $this->_store->getData('application'));
					fclose($fd);
					echo 'success';
					unset($params, $data, $_line, $fd, $app, $site);
					return(true);
				}
				else {
					$err = 'Unable to open application schema';
					$notice = $err.'****1x23017****'.$_line.'****'.__FILE__;
				}
			}
			else {
				$err = 'The application schema file not exists';
				$notice = $err.'****1x23018****'.$_line.'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($params, $data, $_line, $fd, $err, $notice, $app, $site);
			return (false);
		}
		
		Function importSource($data=null)
		{
			$this->_store->setData('file_size_limit', 50);
			$this->_store->setData('framework', $data[1]);
			
			$_line = __LINE__;
			if (strcmp(($res = $this->doImport()), 'ok') == 0) {
				$_line = __LINE__;
				if (strcmp(($res = $this->installArchive()), 'ok') == 0) {
					unset($data, $res, $_line);
					rollBackFolders($this->_store->getData('pathTemp'));
					$stop = true;
					while ($stop) {
						rollBackFolders($this->_store->getData('pathTemp'));
						if (!is_dir($this->_store->getData('pathTemp'))) {
							$stop = false;
						}
					}
					unlink($this->_store->getData('temp').$this->_store->getData('fileName'));
					echo '{"success": true, "msg":{"msgInfo":"The framework source has been uploaded !"}}';
					return;
				}
			}
			
			echo '{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}';
			trigger_error('Unable to import the framework source file****1x23019****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $res, $_line);
			return;
		}
	
	Private
		Function addContentFile()
		{
			if (copy($this->_store->getData('temp').$this->_store->getData('fileName'), $this->_store->getData('contentPath').$this->_store->getData('realName'))) {
				return ('ok');
			}
			else {
				return ('Unable to add the content file');
			}
		}
		
		Function installArchive()
		{
			copyDir($this->_store->getData('pathTemp'), $this->_alias['path']['global']['frmPath'].$this->_store->getData('framework').'/source/');
			return ('ok');
		}
		
		Function installImport()
		{
			$root = get_folder_list($this->_store->getData('pathTemp'));
			$idName = $root[0];
			
			$conf = new EnsKa_INIReader($this->_store->getData('pathTemp').$idName.'/settings.ini');
			if ($conf->getError()) {
				return ('Unable to read the framework configuration. - '.$conf->getError());
			}
			
			$req = getComplexArray(dbSelect('pb.frameworks_infos', Array('id'), Array('id_name' => $conf->getLine('framework'))));
			if ($req) {
				return ('The framework is already installed');
			}
			
			if (!@rename($this->_store->getData('pathTemp').$idName.'/', $this->_alias['path']['global']['frmPath'].$conf->getLine('framework'))) {
				return ('Unable to create the framework path');
			}
			
			$req = dbInsert('pb.frameworks_infos', Array('id' => Array('create_new_id' => '%t,id'), 'name' => $conf->getLine('name'), 'version' => $conf->getLine('version'), 'revision' => $conf->getLine('revision'), 'link' => '', 'id_name' => $conf->getLine('framework')));
			if (!$req) {
				rollBackFolders($this->_alias['path']['global']['frmPath'].$conf->getLine('frmework'));
				return ('Unable to register the framework');
			}
			
			return ('ok');
		}
}

?>