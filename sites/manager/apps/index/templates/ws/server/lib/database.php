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
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

Function dbconnect(){

	GLOBAL $_AUTH_MYSQL;
	
	$connection = mysql_connect($_AUTH_MYSQL['server'], $_AUTH_MYSQL['user'], $_AUTH_MYSQL['pass']);
	if (!$connection)
		return (-1);
	return($connection);
}

Function dbquery($query, $connect='ok'){

	GLOBAL $_MYSQL_DATABASE;
	
	if($connect == "ok") {
		$connection = dbconnect();
		if ($connection == -1)
			return (false);
		$req = mysql_db_query($_MYSQL_DATABASE, $query, $connection);
	} 
	else {
		$req = mysql_db_query($_MYSQL_DATABASE, $query, $connect);
	}
	
	if ($connect == 'ok') {
		mysql_close($connection);
	}
	
	if (!$req) {
		return (false);
	}
	return ($req);
}

?>