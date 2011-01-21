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

/******************************************
 * File:			./api/lib.php
 * Version:			1.0.0
 * Last modified:	05/09/2010
 *****************************************/

/**
 ** Put your functions here !
 **/

 
 Function my_function($params){
	return;
 }

/****************************************************************************************************/
 
/**
 ** API system functions, no change !!
 **/

Function get_api_connect_options(){

	$opts_to_load = Array('get_api_keys', 'get_proxy_opts');
	$nb = count($opts_to_load);
	$i = 0;
	
	while ($i < $nb){
		$fnct = $opts_to_load[$i];
		$to_load = $fnct();
		foreach ($to_load as $to_load_key => $to_load_opt)
			$opts[$to_load_key] = $to_load_opt;
		$i++;
	}
	return ($opts);
}

Function get_proxy_opts(){

	GLOBAL $_PROXY_OPTS;
	return ($_PROXY_OPTS);
}

Function get_api_keys(){

	GLOBAL $public_key;
	GLOBAL $private_key;
	
	$keys['login'] = $public_key;
	$keys['password'] = $private_key;
	
	return ($keys);
}

Function get_wsdl_path(){

	GLOBAL $wsdl;
	return ($wsdl);
}

Function get_ua(){
	return ($_SERVER['HTTP_USER_AGENT']);
}

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
			return (FALSE);
		$req = mysql_db_query($_MYSQL_DATABASE, $query, $connection);
	} 
	else {
		$req = mysql_db_query($_MYSQL_DATABASE, $query, $connect);
	}
	
	if ($connect == 'ok') {
		mysql_close($connection);
	}
	
	if (!$req) {
		return (FALSE);
	}
	
	return ($req);
}

Function redirect($path, $time=1){

	echo "<script language=JavaScript>setTimeout(\"document.location = '$path'\", $time)</script>";
	return;
}

?>