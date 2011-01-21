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

class EnsKa_Libs
{	
	Private	static	$_instance;
	Private	static	$_path;
	Private			$_config;
	
	Public
		static Function get()
		{
			if(self::$_instance == null)
			{
				self::$_instance = new self();
			}
			return self::$_instance;
		}
		
		Function load($path)
		{
			self::setPath($path);
			$this->_config = EnsKa_Config::get()->getConfig();
			self::loadDefault();
			self::loadLocal();
		}
		
	Private
		Function __construct()
		{
			/*Constructor*/
		}
		
		Function __destruct()
		{
			/*Desctructor*/
		}
		
		Function setPath($path)
		{
			self::$_path = $path;
		}
		
		Function loadDefault()
		{
			if (isset($this->_config['global']['libs'])) {
				foreach($this->_config['global']['libs'] as $libName => $stat) {
					if (strncmp('lib_', trim($libName), 4) == 0) {
						if (trim($stat) == 'enable') {
							$lib = substr(trim($libName), -(strlen(trim($libName)) - 4));
							imports('enska.'.$lib);
						}
					}
				}
			}
		}
		
		Function loadLocal()
		{
			if (isset($this->_config['local']['libs'])) {
				foreach($this->_config['local']['libs'] as $libName => $stat) {
					if (strncmp('lib_', trim($libName), 4) == 0) {
						if (trim($stat) == 'enable') {
							$lib = substr(trim($libName), -(strlen(trim($libName)) - 4));
							imports('enska.'.$lib);
						}
					}
				}
			}
		}
}

?>