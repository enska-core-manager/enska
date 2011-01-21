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

class EnsKa_DBManager
{
	Private	static	$_instance;
	Private static	$_handle;
	Private static	$_dbName;
	
	Public
		static Function get()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
			}
			return self::$_instance;
		}
		
		static Function start()
		{
			self::get()->setHandle();
		}
		
		Function getHandle()
		{
			return (self::$_handle);
		}
		
		Function getDbName()
		{
			return (self::$_dbName);
		}
		
	Private
		Function __construct()
		{
			/* Object constructor */
		}
		
		Function __destruct()
		{
			self::killHandle();
		}
		
		Function setHandle()
		{
			$config = EnsKa_Config::get()->getConfig();
			$dbi = $config['global']['db'];
			
			if (!isset($dbi['config'])) {
				if (trim($dbi['dbName']) != '') {
					try
					{
						imports('enska.'.$dbi['dbType']);
						self::$_handle = db_connect($dbi);
						self::$_dbName = $dbi['dbName'];
						EnsKa_Config::get()->addGlobalConfig('db', 'config', 'ok');
					}
					catch(EnsKa_Exception $e)
					{
						return (false);
					}
				}
			}
		}
		
		Function killHandle()
		{
			db_disconnect();
		}
}

?>