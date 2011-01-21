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

/**
 * Open a handle on a MySQL database
 *
 * @param array $dbi
 * @return resource
 */
Function db_connect($dbi)
{
	$handle = mysql_connect($dbi['dbServer'], $dbi['dbUser'], $dbi['dbPassword']);
	return ($handle);
}

/**
 * Close a handle on MySQL database
 *
 */
Function db_disconnect()
{
	mysql_close(EnsKa_DBManager::get()->getHandle());
}

/**
 * Execute a MySQL query
 *
 * @param string $query
 * @throws EnsKa_Exception
 * @return mixed
 */
Function dbquery($query, $handler=null, $db=null)
{
	try 
	{
		if ($handler == null) {
			mysql_select_db(EnsKa_DBManager::get()->getDbName(), EnsKa_DBManager::get()->getHandle());
		}
		else {
			mysql_select_db($db, $handler);
		}
		$req = mysql_query($query);
		
		if (!$req) {
			throw new EnsKa_Exception('The query "'.$query.'" has failed!', '0x2201', 'E_WARNING');
		}
		
		/*
		 * Check if function have to return a boolean or a result
		 */
		if (strcasecmp(substr($query, 0, 6), 'SELECT') == 0){
			if (mysql_num_rows($req) > 0) {
				$i = -1;
				while ($line = mysql_fetch_array($req)) {
					$i++;
					
					/*
					 * Making array result
					 */
					foreach ($line as $key => $data) {
						if (!is_numeric($key)) {
							$res[$i] [$key]= $data;
						}
					}
				}
				
				/*
				 * Check if result is not null
				 */
				if ($i > -1) {
					return ($res);
				}
				
				return (false);
			}
			else {
				return (false);
			}
		}
		return ($req);
	}
	catch (EnsKa_Exception $e) {
		return (false);
	}
}

/**
 * Get max the max record ID+1 on a table
 * Use to "simulate" an auto-increment function
 *
 * @param array $params
 * @return int
 */
Function create_new_id($params)
{
	$req = dbquery('SELECT MAX('.$params['id'].') FROM '.$params['table']);
	$res = getComplexArray($req);
	
	if (is_array($res)) {
		foreach($res as $old_id) {
			$new_id = $old_id + 1;
		}
		return ($new_id);
	}
	else {
		return (-1);
	}
}

/**
 * Format a MySQL where condition from an array
 * Exeample:
 * 		- $cwhere = Array('AND' => array('id' => 1, 'foo' => 'bar'));
 *		- return: `id`='1' and `foo`='bar'
 * @param array $cwhere
 * @param array $clink
 * @return string
 */
Function whereParser($cwhere, $clink=NULL)
{
	$where = '';
	$sub = false;
	
	foreach($cwhere as $wkey => $wvalue) {
		$link = $wkey;
		
		if (is_array($wvalue)) {
			$where .= $clink.' '.whereParser($wvalue, $link);
		}
		else {
			$where .= '`'.$wkey.'`=';
			$where .= "'".mysql_real_escape_string($wvalue)."' $clink ";
			$sub = true;
		}
	}
	
	if ($sub == true) {
		$where = substr($where, 0, (strlen($where) - (strlen($clink) + 1)));
	}
	
	return ($where);
}

/**
 * Get a name table from a prefixed alias
 * Exeample:
 * 		- pb is the general prefix (Prefix Base) configured in conf/settings.ini of EnsKa Core
 *		- ps is the site prefix (Prefix Site) configured in conf/settings.ini of your site
 * @param array $table
 * @return string
 */
Function get_table($table)
{
	GLOBAL $_alias;
	
	$tmp = explode('.', $table);
	$config = EnsKa_Config::get()->getConfig();
	$dbi = $config['global']['db'];
	
	try
	{
		if (strcmp(trim($tmp[0]), 'pb') == 0) {
			$table = $dbi['dbPrefix'].$tmp[1];
		}
		elseif (strcmp(trim($tmp[0]), 'ps') == 0) {
			$table = $_alias['site']."_".$tmp[1];
		}
		/*else {
			$msg = 'Unable to get the table name';
			throw new EnsKa_Exception($msg, '0x2202', 'E_WARNING');
		}*/
		
		return ($table);
	}
	catch (EnsKa_Exception $e)
	{
		return (-1);
	}
}


/**
 * Execute a MySQL SELECT query
 * Including mysql_real_escape_string
 *
 * Exeample:
 * 		- dbSelect('ps.my_table', Array('id', 'myField'), Array('AND'=>Array('id' => 1, 'name' => 'foo')));
 *		- execute: SELECT `id`,`myField` FROM `my_table` WHERE `id`=1 AND `name`='foo'
 *		- return: Array(Array('id' => 1, 'myField' => 'fieldValue'));
 *
 * @param string $table
 * @param array $coloms='*'
 * @param array $cwhere=1
 * @param string $copts=NULL
 * @return array
 */
Function dbSelect($table, $colums='*', $cwhere=1, $copts=NULL)
{	
	$table = get_table($table);

	if (is_array($colums)) {
		$cols = '';
		foreach($colums as $colum) {
			$cols .= '`'.$colum.'`,';
		}
		$cols = substr($cols, 0, strlen($cols) - 1);
	}
	else {
		try
		{
			if (trim($colums) == '*') {
				$cols = '*';
			}
			else {
				$msg = 'Unable to retreive colums';
				throw new EnsKa_Exception($msg, '0x02203', 'E_WARNING');
			}
		}
		catch(EnsKa_Exception $e)
		{
			return (-1);
		}
	}
	
	if (is_array($cwhere)) {
		$where = whereParser($cwhere);
	}
	else {
		try
		{
			if (trim($cwhere) == 1) {
				$where = 1;
			}
			else {
				$msg = 'Unable to retreive the WHERE clause';
				throw new EnsKa_Exception($msg, '0x02204', 'E_WARNING');
			}
		}
		catch(EnsKa_Exception $e)
		{
			return (-1);
		}
	}
	
	if (trim($copts) == NULL) {
		$opts = NULL;
	}
	else {
		$opts = $copts;
	}
	
	$query = 'SELECT '.$cols.' FROM `'.$table.'` WHERE '.$where.' '.$opts;
	return (dbquery($query));
}

/**
 * Execute a MySQL INSERT query
 * Including mysql_real_escape_string
 *
 * Exeample:
 * 		- dbInsert('ps.my_table', Array('id' => 5, 'myField' => 'newValue'));
 *		- execute: INSERT INTO `my_table` (`id`, `myField`) VALUES (5, 'newValue')
 *
 * If in array coloms you put Array('create_new_id' => '%t,id'), 
 * the function going to create an auto-increment ID for the index key (must be an integer).
 * Example:
 * 		- dbInsert('ps.my_table', Array('create_new_id' => '%t,id', 'myField' => 'newValue'));
 * 		- eexecute: INSERT INTO `my_table` (`id`, `myField`) VALUES ("the_new_calculated_value", 'newValue')
 *
 * @param string $table
 * @param array $coloms
 * @return bool
 */
Function dbInsert($table, $colums)
{
	$table = get_table($table);
	
	try
	{
		if (is_array($colums)) {
			$cols = '';
			$datas = '';
			
			foreach($colums as $colum => $data) {
				$cols .= '`'.$colum.'`,';
				if (is_array($data)) {
					foreach($data as $fnct => $params) {
						$tmp = explode(',', $params);
						$fparams = '';
						
						foreach($tmp as $param) {
							$tmpp = explode('%%', $param);
							
							if (strcmp($tmpp[0], '%t') == 0) {
								if (isset($tmpp[1])) {
									$fparams['table'] = $tmpp[1];
								}
								else {
									$fparams['table'] = $table;
								}
							}
							else {
								$fparams['id'] = $param;
							}
						}
						$res = $fnct($fparams);
					}
					$datas .= "'".mysql_real_escape_string($res)."',";
				}
				else {
					$datas .= "'".mysql_real_escape_string($data)."',";
				}
			}
			$cols = substr($cols, 0, strlen($cols) - 1);
			$datas = substr($datas, 0, strlen($datas) - 1);
		}
		else {
			$msg = 'The argument [colums] is not an array';
			throw new EnsKa_Exception($msg, '0x02205', 'E_WARNING');
		}
	}
	catch(EnsKa_Exception $e)
	{
		return (-1);
	}
	
	$query = 'INSERT INTO `'.$table.'` ('.$cols.') VALUES('.$datas.')';
	return (dbquery($query));
}

/**
 * Execute a MySQL UPDATE query
 * Including mysql_real_escape_string
 *
 * Exeample:
 * 		- dbUpdate('ps.my_table', Array('myField' => 'newValue', 'foo' => 'bar'), Array('id' => 42));
 *		- execute: UPDATE FROM `my_table` SET `myField`='newValue', `foo`='bar' WHERE `id`=42
 *
 * @param string $table
 * @param array $coloms
 * @param array $cwhere
 * @return bool
 */
Function dbUpdate($table, $colums, $cwhere)
{
	$table = get_table($table);
		
	try
	{
		if (is_array($colums)) {
			$cols = '';
			
			foreach($colums as $colum => $data) {
				$cols .= '`'.$colum.'`=';
				$cols .= '"'.mysql_real_escape_string($data).'",';
			}
			
			$cols = substr($cols, 0, strlen($cols) - 1);
		}
		else {
			$msg = 'The argument [colums] is not an array';
			throw new EnsKa_Exception($msg, '0x02206', 'E_WARNING');
		}
	}
	catch(EnsKa_Exception $e)
	{
		return (false);
	}
	
	if (is_array($cwhere)) {
		$where = whereParser($cwhere);
	}
	else {
		try
		{
			if (trim($cwhere) == 1) {
				$where = 1;
			}
			else {
				$msg = 'Unable to retreive the WHERE clause';
				throw new EnsKa_Exception($msg, '0x02207', 'E_WARNING');
			}
		}
		catch(EnsKa_Exception $e) {
			return (false);
		}
	}
	
	$query = 'UPDATE `'.$table.'` SET '.$cols.' WHERE '.$where;
	return (dbquery($query));
}

/**
 * Execute a MySQL DELETE query
 * Including mysql_real_escape_string
 *
 * Exeample:
 * 		- dbDelete('ps.my_table', Array('id' => 33));
 *		- execute: DELETE FROM `my_table` WHERE `id`=33
 *
 * @param string $table
 * @param array $cwhere
 * @return bool
 */
Function dbDelete($table, $cwhere)
{	
	$table = get_table($table);
	
	if (is_array($cwhere)) {
		$where = whereParser($cwhere);
	}
	else {
		try
		{
			if (trim($cwhere) == 1) {
				$where = 1;
			}
			else {
				$msg = 'Unable to retreive the WHERE clause';
				throw new EnsKa_Exception($msg, '0x02208', 'E_WARNING');
			}
		}
		catch(EnsKa_Exception $e)
		{
			return (-1);
		}
	}
	
	$query = 'DELETE FROM `'.$table.'` WHERE '.$where;
	return (dbquery($query));
}

?>