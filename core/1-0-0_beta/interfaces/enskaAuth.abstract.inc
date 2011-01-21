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
 * @category   Interfaces
 * @package    core
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

abstract class EnsKa_IAuth
{
	const	SUCCESS = 1;
	const	FAILED = 2;
	
	Private	$_options;
	
	Abstract Protected Function Authenticate($login, $password);
	Abstract Protected Function Disconnect();
	Abstract Protected Function getInfos();
	
	Public 
		Function setOptions($options=null)
		{
			$config = EnsKa_Config::get()->getConfig();
			$_options = new EnsKa_DataStore($config['global']['auth']);
		}
		
		Function redirect($mod)
		{
			GLOBAL $_alias;
			
			if ($mod == self::SUCCESS) {
				redirect('http://'.$_COOKIE['url']);
			}
			elseif ($mod == self::FAILED) {
				redirect('http://login.'.$_alias['path']['global']['domain']);
			}
		}
}

?>