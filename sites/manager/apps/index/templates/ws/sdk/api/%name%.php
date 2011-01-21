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
 * File:			./api/%name%.php
 * Version:			1.0.0
 * Last modified:	05/09/2010
 *****************************************/

//Pour la version bêta, ne pas oublier de l'enlever lors de la release !!
ini_set('soap.wsdl_cache_enabled', 0);

session_register('SESSION');

require_once ('../user/config.inc');
include_once ('lib.inc');

class %name%{

	Private	$api;
	
	Public
		Function __construct(){
			$this->setToken();
			$this->apiConnect();
		}
		
		Function __destruct(){}
		
		/*Function sampleFunction($myParam1, $myParam2){
			$params = Array(
						'fnct' => 'sampleRemoteFunction', //The remote function
						'myParam1' => $myParam1, //Parameter 1
						'myParam2' => $myParam2, //Parameter 2
					);
			return ($this->api->execute($this->getToken(), get_ua(), $params)); //The luncher
		}*/
		
	Private
		Function apiConnect(){
			$this->api = new SoapClient(get_wsdl_path(), get_api_connect_options());
		}
		
		Function getToken(){
			if (!isset($_SESSION['token']))
				setToken();
			return ($_SESSION['token']);
		}
		
		Function setToken(){
			if (!isset($_SESSION['token'])){
				if (!isset($_GET['token'])){
					redirect('http://www.domain.tld/login'); //TODO: Your login page
					exit;
				}
				else
					$_SESSION['token'] = $_GET['token'];
			}
		}
}




?>