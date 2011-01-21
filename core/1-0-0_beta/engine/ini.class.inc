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
 * @category   Engine
 * @package    core
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */
 
class EnsKa_INIReader extends EnsKa_DataStore
{
	Private	$_err;
	Private	$_file;
	Private	$_flag;
	
	Public
		Function __construct($file, $flag='open')
		{
			if (!defined('POINT')) {
				define('POINT', '../');
			}
			
			try
			{
				$this->_file = $file;
				
				if ($flag == 'open') {
					$file = POINT.$this->_file;
					
					if (!file_exists($file)) {
						$file = $this->_file;
						
						if (!file_exists($file)) {
							throw new EnsKa_Exception('File not exists: '.$file, '0x1301', E_ERROR);
						}
					}
				}
				$this->_file = $file;
				$this->_flag = $flag;
				
				$this->open();
			}
			catch(EnsKa_Exception $e)
			{
				$this->setError($e->getMessage());
				return;
			}
		}
		
		Function open()
		{
			if (strcmp($this->_flag, 'open') == 0) {
				$fd = fopen($this->_file, 'r');
				if (!$fd) {
					throw new EnsKa_Exception('Unable to open file: '.$this->_file, '0x1302', E_ERROR);
				}
				
				while (!(feof($fd))) {
					$line = trim(fgets($fd));
					
					if ((strncmp(substr($line, 0, 1), '#', 1) == 0) || (strncmp(substr($line, 0, 1), ';', 1) == 0)) {
						$this->addLine($line, '#com');
					}
					elseif (substr($line, 0, 1) == '[') {
						$this->addLine($line, '#head');
					}
					elseif ($line != "\n") {
						$tmp = explode('=', $line);
						if (isset($tmp[0]) && isset($tmp[1])) {
							$this->addLine($tmp[0], $tmp[1]);
						}
					}
				}
			}
			elseif (strcmp($this->_flag, 'create') == 0) {
				$fd = fopen($this->_file, 'w');
				if (!$fd) {
					throw new EnsKa_Exception('Unable to create file: '.$this->_file, '0x1303', E_ERROR);
				}
			}
			else {
				throw new EnsKa_Exception('Incorrect flag!', '0x1304', E_ERROR);
			}
			fclose($fd);
		}
		
		Function getLine($attr)
		{
			return ($this->getData($attr));
		}
		
		Function getLines()
		{
			return ($this->getDatas());
		}
		
		Function addLine($attr, $val, $save=false)
		{
			$this->setData(trim($attr), trim($val));
			if ($save == true) {
				$this->save();
			}
			
		}
		
		Function deleteLine($attr, $save=false)
		{
			$this->unsetData($attr);
			if ($save == true) {
				$this->save();
			}
		}
		
		Function fileBuff($buff)
		{
			try 
			{
				$fd = fopen($this->_file, 'w');
				if (!$fd) {
					throw new EnsKa_Exception('Unable to open file: '.$this->_file, '0x1305', E_ERROR);
				}
			}
			catch(EnsKa_Exception $e)
			{
				$this->setError($e->getMessage());
				return;
			}
			
			fwrite($fd, $buff);
			fclose($fd);
		}
		
		Function save()
		{
			try
			{
				$fd = fopen($this->_file, 'w');			
				if (!$fd) {
					throw new EnsKa_Exception('Unable to open file: '.$this->_file, '0x1306', E_ERROR);
				}
			}
			catch(EnsKa_Exception $e)
			{
				$this->setError($e->getMessage());
				return;
			}
			
			$datas = $this->getDatas();
			foreach($datas as $key => $val) {
				if ((strcmp($val, '#com') == 0) || (strcmp($val, '#head') == 0)) {
					$buff = $key."\n";
				}
				else {
					$buff = $key.'='.$val."\n";
				}
				fwrite($fd, $buff);
			}
			
			fclose($fd);
		}
		
		Function reload()
		{
			$this->open();
		}
		
		Function getError()
		{
			if ($this->_err != '') {
				return ($this->_err);
			}
			return (false);
		}
		
	Private
		Function setError($err)
		{
			$this->_err = $err;
		}
}

?>