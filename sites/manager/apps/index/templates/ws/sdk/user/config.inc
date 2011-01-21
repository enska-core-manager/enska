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
 * File:			./user/config.php
 * Version:			1.0.0
 * Last modified:	05/09/2010
 *****************************************/

/**
 * Your API keys
 */

$public_key = '';
$private_key = '';


/**
 * Proxy configuration
 */

$_PROXY_OPTS = Array(
	'proxy_host'     => '',
	'proxy_port'     => '',
	'proxy_login'    => '',
	'proxy_password' => '');

	
/**
 * WSDL path
 */

$wsdl = 'http://%host%/%website%/%name%/ws/%name%_wsdl_api.wsdl';


/*
 * Database configuration
 */

//Connection information
$_AUTH_MYSQL = Array(
	'server' => 'localhost',
	'user' => 'root',
	'pass' => '');

//Database name
$_MYSQL_DATABASE = '';

?>