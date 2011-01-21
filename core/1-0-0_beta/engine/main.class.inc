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

class EnsKa_MainController
{

	Protected	static	$_instance;
	
	Public				$logs;
	Public				$path;
	Public				$config;
	Public				$sessions;
	
	Public
		static Function get()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
			}
			return self::$_instance;
		}
		
		static Function load()
		{
			GLOBAL $_alias;
			
			try
			{
				self::get()->loadPath();
				self::get()->loadConfig();
				self::get()->loadLibs();
				$_alias['path'] = EnsKa_Path::get()->getPath();
				self::get()->loadSessions();
				self::get()->loadLogs();
			}
			catch (EnsKa_Exception $e){}
		}
		
		Function loadPath()
		{
			EnsKa_Path::get()->load();
		}
		
		Function loadSessions()
		{
			EnsKa_Sessions::get()->load();
		}
		
		Function loadConfig()
		{
			EnsKa_Config::get()->load(EnsKa_Path::get()->getPath());
		}
		
		Function loadLibs()
		{
			EnsKa_Libs::get()->load(EnsKa_Path::get()->getPath());
		}
		
		Function loadLogs()
		{
			if(EnsKa_Logs::getInstance()->isEnable()) {
				EnsKa_Logs::start()->addObserver
										(
											new EnsKa_LogsObserver('logs'),
											'logs'
										);
			}
		}
		
	Private
		Function __construct() {}
		Function __desctruct() {}
}

?>