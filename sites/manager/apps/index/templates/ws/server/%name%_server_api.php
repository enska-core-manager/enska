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
 
DEFINE (NOT_LOGED, -1);

include ('lib/includes.php');

class %name%ApiServer{

		Function execute($token, $ua, $param){
			if (!($decode = is_loged($token, $ua))){
				return (NOT_LOGED);
				exit;
			}
			
			if (!is_authorized_fnct($param['fnct']))
				return (-1);
			
			$param['uid'] = $decode['uid'];
			$param['applicationId'] = $decode['app_id'];
			$res = $param['fnct']($param);
			
			return ($res);
		}
}

//Pour le dev ! Ne pas oublier lors de la release !
ini_set('soap.wsdl_cache_enabled', 0);

$%name%_api_server = new SoapServer("http://%host%/%website%/%name%/ws/%name%_wsdl_api.wsdl");
$%name%_api_server->setClass("%name%ApiServer");

$%name%_api_server->handle();

?>