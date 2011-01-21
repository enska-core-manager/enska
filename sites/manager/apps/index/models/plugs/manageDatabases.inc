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
 
class manageDatabases extends EnsKa_PlateformPlugin
{
	Public
		Function import($data=null)
		{
			$this->_store->setDatas($data);
			return($this->importScript());
		}
		
		Function updateRecord($data=null)
		{	
			$params = Array
						(
							'datas'		=>	json_decode(strReplace($data[1], ':-:', '::')),
							'key'		=>	$data[2],
							'value'		=>	$data[3],
							'table'		=>	$data[4],
							'login'		=>	$data[5],
							'password'	=>	$data[6]
						);
			$this->_store->setDatas($params);
			
			$db = EnsKa_Config::get()->getConfig();
			$db = $db['global']['db'];
			$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $this->_store->getData('login'), $this->_store->getData('password'));
			
			if ($sql->connect()) {
				$primary = $sql->getPrimaryKey($this->_store->getData('table'));
				$primary = $primary['Field'];
				if($sql->request('UPDATE `'.$this->_store->getData('table').'` SET `'.$this->_store->getData('key').'`='."'".$this->_store->getData('value')."'".' WHERE `'.$primary.'`='."'".$where = $this->_store->getData('datas')->$primary."'")) {
					echo 'success';
					return(true);
				}
				else {
					$err = 'Unable to update table';
				}
			}
			else {
				$err = 'Unable to connect to database';
			}
			
			echo strtoupper($err).'<br><br>'.mysql_error();
			return (false);
		}
		
		Function deleteRecord($data=null)
		{	
			$params = Array
						(
							'datas'		=>	json_decode(strReplace($data[1], ':-:', '::')),
							'key'		=>	$data[2],
							'value'		=>	$data[3],
							'table'		=>	$data[4],
							'login'		=>	$data[5],
							'password'	=>	$data[6]
						);
			$this->_store->setDatas($params);
			
			$db = EnsKa_Config::get()->getConfig();
			$db = $db['global']['db'];
			$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $this->_store->getData('login'), $this->_store->getData('password'));
			
			if ($sql->connect()) {
				$primary = $sql->getPrimaryKey($this->_store->getData('table'));
				$primary = $primary['Field'];
				if($sql->request('DELETE FROM `'.$this->_store->getData('table').'` WHERE `'.$primary.'`='."'".$where = $this->_store->getData('datas')->$primary."'")) {
					echo 'success';
					return(true);
				}
				else {
					$err = 'Unable to delete the selected record';
				}
			}
			else {
				$err = 'Unable to connect to database';
			}
			
			echo strtoupper($err).'<br><br>'.mysql_error();
			return (false);
		}
		
		Function createField($data=null)
		{
			$params = Array
						(
							'table'		=>	$data[1],
							'name'		=>	$data[2],
							'fType'		=>	$data[3],
							'notnull'	=>	$data[4],
							'unsigned'	=>	$data[5],
							'auto'		=>	$data[6],
							'unique'	=>	$data[7],
							'primary'	=>	$data[8],
							'size'		=>	$data[9],
							'default'	=>	$data[10],
							'login'		=>	$data[11],
							'password'	=>	$data[12]
						);
			$this->_store->setDatas($params);
			
			$db = EnsKa_Config::get()->getConfig();
			$db = $db['global']['db'];
			$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $this->_store->getData('login'), $this->_store->getData('password'));
			
			if ($sql->connect()) {
				if ($sql->createField($this->_store->getData('table'), $this->_store) == 0) {
					echo 'success';
					return(true);
				}
				else {
					$err = 'Unable create the new field';
				}
			}
			else {
				$err = 'Unable to connect to database';
			}
			
			echo strtoupper($err).'<br><br>'.mysql_error();
			return (false);
		}
		
		Function deleteField($data=null)
		{
			$params = Array
						(
							'table'		=>	$data[1],
							'name'		=>	$data[2],
							'login'		=>	$data[3],
							'password'	=>	$data[4]
						);
			$this->_store->setDatas($params);
			
			$db = EnsKa_Config::get()->getConfig();
			$db = $db['global']['db'];
			$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $this->_store->getData('login'), $this->_store->getData('password'));
			
			if ($sql->connect()) {
				if ($sql->deleteField($this->_store->getData('table'), $this->_store->getData('name')) == 0) {
					echo 'success';
					return(true);
				}
				else {
					$err = 'Unable to delete the selected field';
				}
			}
			else {
				$err = 'Unable to connect to database';
			}
			
			echo strtoupper($err).'<br><br>'.mysql_error();
			return (false);
		}
		
		Function createTable($data=null)
		{
			$params = Array
						(
							'table'		=>	$data[1],
							'name'		=>	$data[2],
							'fType'		=>	$data[3],
							'notnull'	=>	$data[4],
							'unsigned'	=>	$data[5],
							'auto'		=>	$data[6],
							'unique'	=>	$data[7],
							'primary'	=>	$data[8],
							'size'		=>	$data[9],
							'default'	=>	$data[10],
							'login'		=>	$data[11],
							'password'	=>	$data[12]
						);
			$this->_store->setDatas($params);
			
			$db = EnsKa_Config::get()->getConfig();
			$db = $db['global']['db'];
			$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $this->_store->getData('login'), $this->_store->getData('password'));
			
			if ($sql->connect()) {
				if ($sql->createTable($this->_store->getData('table'), $this->_store) == 0) {
					echo 'success';
					return(true);
				}
				else {
					$err = 'Unable create the new table';
				}
			}
			else {
				$err = 'Unable to connect to database';
			}
			
			echo strtoupper($err).'<br><br>'.mysql_error();
			return (false);
		}
		
		Function deleteTable($data=null)
		{
			$params = Array
						(
							'table'		=>	$data[1],
							'login'		=>	$data[2],
							'password'	=>	$data[3]
						);
			$this->_store->setDatas($params);
			
			$db = EnsKa_Config::get()->getConfig();
			$db = $db['global']['db'];
			$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $this->_store->getData('login'), $this->_store->getData('password'));
			
			if ($sql->connect()) {
				if ($sql->deleteTable($this->_store->getData('table')) == 0) {
					echo 'success';
					return(true);
				}
				else {
					$err = 'Unable to delete the table';
				}
			}
			else {
				$err = 'Unable to connect to database';
			}
			
			echo strtoupper($err).'<br><br>'.mysql_error();
			return (false);
		}
		
		Function scriptRequest($data=null)
		{
			$params = Array
						(
							'script'	=>	$_POST['script'],
							'login'		=>	$data[1],
							'password'	=>	$data[2]
						);
			$this->_store->setDatas($params);
			
			$db = EnsKa_Config::get()->getConfig();
			$db = $db['global']['db'];
			$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $this->_store->getData('login'), $this->_store->getData('password'));
			
			
			if ($sql->connect()) {
				$sql->scriptRequest($this->_store->getData('script'));
				return;
			}
			else {
				$err = 'Unable to connect to database';
			}
			
			echo strtoupper($err).'<br><br>'.mysql_error();
			return (false);
		}
		
		Function export($data=null)
		{
			$params = Array
						(
							'all'		=>	$data[1],
							'sTable'	=>	$data[2],
							'sTables'	=>	$data[3],
							'drop'		=>	$data[4],
							'login'		=>	$data[5],
							'password'	=>	$data[6]
						);
			$this->_store->setDatas($params);
			
			if ($this->_store->getData('drop') == 1) {
				$this->_store->setData('drop', true);
			}
			else {
				$this->_store->setData('drop', false);
			}
			
			$db = EnsKa_Config::get()->getConfig();
			$db = $db['global']['db'];
			$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $this->_store->getData('login'), $this->_store->getData('password'));
			
			if ($sql->connect()) {
				if ($this->_store->getData('all') == 1) {
					if ($sql->export($sql->listTables(), $this->_store->getData('drop'))) {
						echo 'success';
						return;
					}
					else {
						$err = 'Unable to export the database';
					}
				}
				elseif (strcmp($this->_store->getData('sTable'), 'none') != 0) {
					if ($sql->export($this->_store->getData('sTable'), $this->_store->getData('drop'))) {
						echo 'success';
						return;
					}
					else {
						$err = 'Unable to export the table '.$this->_store->getData('sTable');
					}
				}
				elseif (strcmp($this->_store->getData('sTables'), 'none') != 0) {
					if ($sql->export(explode(',',$this->_store->getData('sTables')), $this->_store->getData('drop'))) {
						echo 'success';
						return;
					}
					else {
						$err = 'Unable to export the tables '.$this->_store->getData('sTables');
					}
				}
			}
			else {
				$err = 'Unable to connect to database';
			}
			
			echo strtoupper($err).'<br><br>'.mysql_error();
			return (false);
		}
		
		Function importScript($data=null)
		{
			if (!$this->_store->getData('wizard')) {
				$params = Array
							(
								'login'		=>	$data[1],
								'password'	=>	$data[2]
							);
				$this->_store->setDatas($params);
			}
			
			$this->_store->setData('file_size_limit', 100);
			
			$_line = __LINE__;
			if (strcmp(($res = $this->doImport('file')), 'ok') == 0) {
				$_line = __LINE__;
				if (strcmp(($res = $this->runScript()), 'ok') == 0) {
					unset($data, $res, $_line);
					rollBackFolders($this->_store->getData('pathTemp'));
					unlink($this->_store->getData('temp').$this->_store->getData('fileName'));
					if ($this->_store->getData('wizard')) {
						return ('{"success": true, "msg":{"msgInfo":"The SQL Script has been successfully imported !"}}');
					}
					else {
						echo '{"success": true, "msg":{"msgInfo":"The SQL Script has been successfully executed !"}}';
						return;
					}
				}
			}
			
			trigger_error('Unable import the content file****1x23010****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $_line);
			
			if ($this->_store->getData('wizard')) {
				return ('{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}');
			}
			else {
				echo '{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}';
			}
			
			return;
		}
		
	Private
		Function runScript()
		{
			$db = EnsKa_Config::get()->getConfig();
			$db = $db['global']['db'];
			$sql = new EnsKa_MML($db['dbName'], $db['dbServer'], $this->_store->getData('login'), $this->_store->getData('password'));
			
			if ($sql->connect()) {
				$sql->scriptRequest($this->_store->getData('temp').$this->_store->getData('fileName'), false, true);
				return('ok');
			}
			else {
				return ('Unable to connect to database');
			}
		}
}

?> 