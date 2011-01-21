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

class EnsKa_Path
{
	Private	static	$_globalPath;
	Private	static	$_localPath;

	Private	static	$_instance;
	
	Public
		static Function get()
		{
			if(self::$_instance == null)
			{
				self::$_instance = new self();
			}
			
			return self::$_instance;
		}
		
		Function load()
		{
			self::loadGlobalPath();
			self::loadLocalPath();
		}
		
		Function reload()
		{
			self::loadGlobalPath();
			self::loadLocalPath();
		}
		
		Function getPath($type=ALL)
		{
			if ($type==ALL)
			{
				$path = Array(
				
					'global'	=>		self::$_globalPath,
					'local'		=>		self::$_localPath
					
				);
			}
			elseif($type==GLOBALE)
			{
				$path = getComplexArray(Array(
				
					'global'	=>		self::$_globalPath
					
				));
			}
			elseif($type==LOCAL)
			{
				$path = getComplecArray(Array(
				
					'local'		=>		self::$_localPath
					
				));
			}
			else
			{
				$path = NULL;
			}
			
			return ($path);
		}
		
		Function addGlobalPath($name, $path, $reload=true)
		{
			self::$_globalPath[$name] = $path;
			
			if ($reload == true)
			{
				$fd = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'settings.ini');
				$fd->addLine($name, $path, true);
			}
		}
		
		Function addLocalPath($name, $path, $reload=true)
		{
			self::$_localPath[$name] = $path;
			
			if ($reload == true)
			{
				$fd = new EnsKa_INIReader(ENSKA_LOCAL_CONF.'settings.ini');
				$fd->addLine($name, $path, true);
			}
		}
		
		Function deleteGlobalPath($name, $permanent=true)
		{
			unset(self::$_globalPath[$name]);
			
			if ($permanent == true)
			{
				$fd = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'settings.ini');
				$fd->deleteLine($name, true);
			}
		}
		
		Function deleteLocalPath($name, $permanent=true)
		{
			unset(self::$_localPath[$name]);
			
			if ($permanent == true)
			{
				$fd = new EnsKa_INIReader(ENSKA_LOCAL_CONF.'settings.ini');
				$fd->deleteLine($name, true);
			}
		}
		
		Function getCurrentPath()
		{
			GLOBAL $_alias;
			return (self::$_localPath['path_apps'].$_alias['controller'].'/');
		}
		
	Private
		Function __construct(){}
		Function __destruct(){}
		
		Function loadGlobalPath()
		{
			$fd = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'settings.ini');
			$lines = $fd->getLines();
			
			foreach($lines as $attr => $value)
			{
				$this->addGlobalPath($attr, $value, false);
			}
			
			return (0);
		}
		
		Function loadLocalPath()
		{
			GLOBAL $_alias;
			
			$fd = new EnsKa_INIReader(self::$_globalPath['sites'].$_alias['site'].'/'.ENSKA_LOCAL_CONF.'settings.ini');
			$lines = $fd->getLines();
			
			foreach($lines as $attr => $value)
			{
				$this->addLocalPath($attr, self::$_globalPath['sites'].$_alias['site'].'/'.$value, false);
			}
			
			return (0);
			
		}
}

?>