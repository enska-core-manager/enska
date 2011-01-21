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

class EnsKa_MML
{
	Public	$_exportFile;
	
	Private	$_db;
	Private	$_server;
	Private	$_handler;
	Private	$_login;
	Private	$_passwd;
	
	Public
		Function __construct($db, $srv, $login, $passwd='')
		{
			$this->_db = $db;
			$this->_server = $srv;
			$this->_login = $login;
			$this->_passwd = $passwd;
		}
		
		Function connect()
		{
			if (!$this->_handler) {
				if ((trim($this->_server) != '') && (trim($this->_db) != '')) {
					$this->_handler = mysql_connect($this->_server, $this->_login, $this->_passwd);
					if ($this->_handler) {
						return (true);
					}
				}
				else {
					return (false);
				}
			}
			return (true);
		}
		
		Function disconnect()
		{
			mysql_close($this->_handler);
		}
		
		Function databaseExists()
		{
			if (mysql_select_db($this->_db, $this->_handler)) {
				return (true);
			}
			return (false);
		}
		
		Function listTables()
		{
			$res = dbquery('SHOW TABLES FROM '.$this->_db, $this->_handler, $this->_db);
			if ($res) {
				$i = 0;
				while ($row = mysql_fetch_row($res)) {
				   $table[$i] = $row[0];
				   $i++;
				}
				mysql_free_result($res);
				return ($table);
			}
			else {
				return (false);
			}
		}
		
		Function getNbRecords($table)
		{
			$res = dbquery('SELECT COUNT(*) FROM '.$table, $this->_handler, $this->_db);
			if($res) {
				return ($res);
			}
			else{
				return (false);
			}
		}
		
		Function getTableFields($table)
		{
			$res = dbquery('SHOW COLUMNS FROM '.$table, $this->_handler, $this->_db);
			if($res) {
			   while ($row = mysql_fetch_assoc($res)) {
					$result[] = $row;
			   }
			   return ($result);
			}
			else{
				return (false);
			}
		}
		
		Function request($query)
		{
			$res = dbquery($query, $this->_handler, $this->_db);
			if($res) {
			   return ($res);
			}
			else{
				return (false);
			}
		}
		
		Function getPrimaryKey($table)
		{
			$fields = $this->getTableFields($table);
			foreach($fields as $field) {
				if (strcmp($field['Key'], 'PRI') == 0) {
					return ($field);
				}
			}
		}
		
		Function getStats()
		{
			return (mysql_stat($this->_handler));
		}
		
		Function createField($table, $options)
		{
			$_line = __LINE__;
			$query = 'ALTER TABLE `'.$table.'` ADD `'.$options->getData('name').'` '.$options->getData('fType');

			$fieldSizeException = Array
								(
									'DATE',
									'DATETIME',
									'TIMESTAMP',
									'TEXT',
									'LONGTEXT',
									'CHAR',
									'BLOB',
									'LONGBLOB',
									'ENUM',
									'BOOLEAN',
									'BIT',
									'BINARY'
								);
			
			if(($options->getData('size') > 0) && (is_numeric($options->getData('size'))) && (!in_array($options->getData('fType'), $fieldSizeException))) {
				$query .= ' ('.$options->getData('size').')';
			}

			if ($options->getData('unsigned') == 1) {
				$query .= ' UNSIGNED';
			}
			
			if ($options->getData('notnull') == 1) {
				$query .= ' NOT NULL';
			}
			else {
				$query .= ' NULL';
			}
			
			if ($options->getData('auto') == 1) {
				$query .= ' AUTO_INCREMENT';
			}
			
			if ($options->getData('primary') == 1) {
				$query .= ' PRIMARY KEY';
			}
			
			if ($options->getData('unique') == 1) {
				$query .= ' UNIQUE';
			}
			
			if (trim($options->getData('default')) != '') {
				$query .= ' DEFAULT \''.$options->getData('default').'\'';
			}
			
			$_line = __LINE__;
			$req = $this->request($query);
			
			if ($req) {
				unset($fieldSizeException, $_line, $req, $table, $query);
				return (0);
			}
			else {
				$err = 'UNABLE TO CREATE THE FIELD (2)<br>	'.$query.'<br><br>'.mysql_error();
				$notice = 'Unable to create the option (2) -- '.$query.'****1x2201****'.($_line + 1).'****'.__FILE__;
			}
			
			trigger_error($notice, E_USER_NOTICE);
			unset($fieldSizeException, $_line, $req, $table, $query);
			return ($err);
		}
		
		Function deleteField($table, $field)
		{
			$query = 'ALTER TABLE `'.$table.'` DROP `'.$field.'`';
			$req = $this->request($query);			
			if ($req) {
				return (0);
			}
			else {
				return (-1);
			}
		}
		
		Function createTable($table, $field)
		{
			$_line = __LINE__;
			$query = 'CREATE TABLE `'.$table.'`(`'.$field->getData('name').'` '.$field->getData('fType');

			$fieldSizeException = Array
								(
									'DATE',
									'DATETIME',
									'TIMESTAMP',
									'TEXT',
									'LONGTEXT',
									'CHAR',
									'BLOB',
									'LONGBLOB',
									'ENUM',
									'BOOLEAN',
									'BIT',
									'BINARY'
								);
			
			if(($field->getData('size') > 0) && (is_numeric($field->getData('size'))) && (!in_array($field->getData('fType'), $fieldSizeException))) {
				$query .= ' ('.$field->getData('size').')';
			}
			
			if ($field->getData('unsigned') == 1) {
				$query .= ' UNSIGNED';
			}
			
			if ($field->getData('notnull') == 1) {
				$query .= ' NOT NULL';
			}
			else {
				$query .= ' NULL';
			}
			
			if ($field->getData('auto') == 1) {
				$query .= ' AUTO_INCREMENT';
			}
			
			if ($field->getData('primary') == 1) {
				$query .= ' PRIMARY KEY';
			}
			
			if ($field->getData('unique') == 1) {
				$query .= ' UNIQUE (`'.$field->getData('name').'`)';
			}
			
			if (trim($field->getData('default')) != '') {
				$query .= ' DEFAULT \''.$field->getData('default').'\'';
			}
			
			$query .=') ENGINE = MYISAM';
			
			$_line = __LINE__;
			$req = $this->request($query);
			
			if ($req) {
				unset($fieldSizeException, $_line, $req, $table, $query);
				return (0);
			}
			else {
				$err = 'UNABLE TO CREATE THE TABLE <br>	'.$query.'<br><br>'.mysql_error();
				$notice = 'Unable to create the table -- '.$query.' -- '.mysql_error().'****1x2201****'.($_line + 1).'****'.__FILE__;
			}
			
			trigger_error($notice, E_USER_NOTICE);
			unset($fieldSizeException, $_line, $req, $table, $query);
			return ($err);
		}
		
		Function deleteTable($table)
		{
			$query = 'DROP TABLE `'.$table.'`';
			$req = $this->request($query);
			if ($req) {
				return (true);
			}
			else {
				return (false);
			}
		}
		
		Function deleteDatabase($database)
		{
			$query = 'DROP DATABASE `'.$database.'`';
			$req = $this->request($query);
			if ($req) {
				return (true);
			}
			else {
				return (false);
			}
		}
		
		Function scriptRequest($script, $showOut=true, $sFile=false, $showErr=false)
		{
			if (!$sFile) {
				$file = md5(create_id(5)).'.tmp';
				$fd = fopen($file, 'w');
				fputs($fd, $script);
				fclose($fd);
			}
			else {
				$file = $script;
			}
			
			$err = false;
			$requetes = '';
			$lines = file($file);
			foreach($lines as $line) {
				if (substr(trim($line), 0, 2) != "--") {
					$requetes .= $line;
				}
			}
			
			if (!$sFile) {
				unlink($file);
			}
				
			$out = '';
			$reqs = explode(";\n",$requetes);
			foreach($reqs as $req) {
				$req = strReplace(strReplace($req, "\\'", "'"), '\"', '"').';';
				if (trim($req) != ";") {
					if (!$this->request($req)){
						$err = true;
						$out .= 'Unable to execute the request: '.$req."\n".mysql_error()."\n\n\n";
					}
					else {
						$out .= $req."\n".'# Successfull'."\n\n";
					}
				}
			}
			
			if ($showOut) {
				echo $out;
			}
			else {
				if ($showErr) {
					if ($err) {
						return(false);
					}
				}
				return (true);
			}
		}
		
		Function export($tables, $drop=false)
		{
			GLOBAL $_alias;
			
			$out = "-- EnsKa Core Manager SQL Dump"."\n";
			$out .= "-- Version 1.0.0 preview"."\n";
			$out .= "-- http://www.enska-system.com"."\n";
			$out .= "--"."\n";
			$out .= "-- Host:\t\t".$this->_server."\n";
			$out .= "-- Database:\t".$this->_db."\n";
			$out .= "-- Date:\t\t".date('Y-m-d').' at '.date('H:m:i')."\n\n";
			$out .= "-- -----------------------------------\n\n\n\n";
			
			if (!is_array($tables)) {
				$tmp[] = $tables;
				unset($tables);
				$tables = $tmp;
				unset($tmp);
			}
			
			if ($drop) {
				/*
					$out .= 'DROP DATABASE `'.$this->_db.'`;'."\n";
					$out .= 'CREATE DATABASE `'.$this->_db.'`;'."\n";
				*/
				$out .= 'USE `'.$this->_db.'`;'."\n";
				$out .=  "\n\n";
			}
			
			$listTables = $this->listTables();
			
			foreach($tables as $table)
			{
				if (in_array($table, $listTables)) {
					$out .= '--'."\n";
					$out .= '-- Table structure for table '.'`'.$table.'`'."\n";
					$out .= '--'."\n\n";
					if ($drop) {
						$out .= 'DROP TABLE IF EXISTS `'.$table.'`;'."\n";
					}
					$listeCreationsTables = mysql_query("show create table ".$table, $this->_handler);
					while($creationTable = mysql_fetch_array($listeCreationsTables)) {
						$out .= $creationTable[1].";\n\n";
					}
					
					$donnees = mysql_query('SELECT * FROM '.$table, $this->_handler);
					$out .= "--\n";
					$out .= '-- Dumping data for table `'.$table.'`'."\n";
					$out .= "--\n\n";
					
					while($nuplet = mysql_fetch_array($donnees)) {
						$out .= 'INSERT INTO `'.$table.'` VALUES(';
						for($i=0; $i < mysql_num_fields($donnees); $i++) {
							if($i != 0) {
								$out .=  ', ';
							}
							if(mysql_field_type($donnees, $i) == "string" || mysql_field_type($donnees, $i) == "blob") {
								$out .=  "'";
							}
							$out .= strReplace(addslashes($nuplet[$i]), "'", "''");
							if(mysql_field_type($donnees, $i) == "string" || mysql_field_type($donnees, $i) == "blob") {
								$out .=  "'";
							}
						}
						$out .=  ');'."\n";
					}
					$out .= "\n";
				}
			}
		 
			$file = $_alias['path']['global']['point'].$_alias['path']['local']['path_temp'].'sql.export.'.$this->_db.'.'.date('Y-m-d').'_'.date('H-m-i').'.sql';
			$this->_exportFile = $file;
			
			$fd = fopen($file, "wb");
			if ($fd) {
				fwrite($fd, $out);
				fclose($fd);
				return (true);
			}
			else {
				return (false);
			}
		}
}

?>