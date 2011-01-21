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

class EnsKa_Config
{	
	Private	static	$_globalConfig;
	Private	static	$_localConfig;
	Private	static	$_path;	
	Private	static	$_instance;
	
	Public
		static Function get()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
			}
			return self::$_instance;
		}
		
		static Function load($path)
		{
			self::get()->setPath($path);
			self::get()->loadGlobalConfig();
			self::get()->loadLocalConfig();
		}
		
		Function getConfig()
		{
			$conf = Array(

				'global'	=>		self::$_globalConfig,
				'local'		=>		self::$_localConfig

			);
			return ($conf);
		}
		
		Function addGlobalConfig($conf, $name, $option, $reload=false)
		{
			self::$_globalConfig[$conf][$name] = $option;
			if ($reload == true){
				if (!file_exists(self::$_globalConfig[$conf]['load'])) {
					@trigger_error('File configuration file not exists: '.self::$_globalConfig[$conf]['load'].' -- 0x1501', E_USER_NOTICE);
				}
				$this->removeConfig($conf, $name);
				$this->writeConfig($conf, $name, $option);
			}
		}
		
		Function addLocalConfig($conf, $name, $option, $reload=false)
		{
			self::$_localConfig[$conf][$name] = $option;
			if ($reload == true) {
				if (!file_exists(self::$_localConfig[$conf]['load'])) {
					@trigger_error('File configuration file not exists: '.self::$_localConfig[$conf]['load'].' -- 0x1502', E_USER_NOTICE);
				}
				$this->removeConfig($conf, $name);
				$this->writeConfig($conf, $name, $option);
			}
		}
		
		Function deleteGlobalConfig($conf, $name, $permanent=true)
		{
			unset(self::$_globalConfig[$conf][$name]);
			if ($permanent == true) {
				if (!file_exists(self::$_globalConfig[$conf]['load'])) {
					@trigger_error('File configuration file not exists: '.self::$_globalConfig[$conf]['load'].' -- 0x1503', E_USER_NOTICE);
				}
				$this->removeConfig($conf, $name);
			}
		}
		
		Function deleteLocalConfig($conf, $name, $permanent=true)
		{
			unset(self::$_localConfig[$conf][$name]);
			if ($permanent == true) {
				if (!file_exists(self::$_localConfig[$conf]['load'])) {
					@trigger_error('File configuration file not exists: '.self::$_globalConfig[$conf]['load'].' -- 0x1504', E_USER_NOTICE);
				}
				$this->removeConfig($conf, $name);
			}
		}
		
	Private
		Function __construct() {}
		Function __destruct() {}
		
		Function setPath($path)
		{
			self::$_path = $path;
		}
		
		Function get_conf_name($file)
		{
			$tmp = explode('/', $file);
			$nb = count($tmp) - 1;
			$conf_name = $tmp[$nb];
			unset($tmp);
			$tmp = explode('.', $conf_name);
			$conf_name = trim($tmp[0]);
			return($conf_name);
		}
	
		Function loadConfigFile($file, $mod)
		{
			if (strncmp($file, '../', 3) != 0) {
				$file = self::$_path['global']['point'].self::$_path['global']['path_conf'].$file;
			}
			
			if (!file_exists($file)) {
				@trigger_error('File configuration file not exists: '.$file.' -- 0x1505', E_USER_NOTICE);
			}
			
			$fd = @fopen($file, 'r');
			if (!$fd) {
				@trigger_error('Unable to open the configuration file: '.$file.' -- 0x1506', E_USER_NOTICE);
				return;
			}
			
			while (!feof($fd)) {
				$buff = fgets($fd, 4096);
				$line = explode ('=', $buff);
				if (($nb = count($line)) == 2) {
					$conf = $this->get_conf_name($file);
					
					if ($mod == GLOBALE) {
						self::$_globalConfig[$conf]['load'] = $file;
						$this->addGlobalConfig($conf, trim($line[0]), trim($line[1]));
					}
					elseif ($mod == LOCAL) {
						self::$_localConfig[$conf]['load'] = $file;
						$this->addLocalConfig($conf, trim($line[0]), trim($line[1]));
					}
					else {
						fclose($fd);
						@trigger_error('Unknown configuration mod: '.$mod.' -- 0x1510', E_USER_NOTICE);
						return;
					}
				}
			}
			fclose($fd);
			return (true);
		}
	
		Function loadGlobalConfig()
		{
			foreach (self::$_path['global'] as $name => $path) {
				$tmp = explode('.', $name);
				if (($nb = count($tmp)) == 2) {
					if (strcmp(trim($tmp[0]), 'load') == 0) {
						$this->loadConfigFile($path, GLOBALE);
					}
				}
			}
		}
		
		Function loadLocalConfig()
		{
			foreach (self::$_path['local'] as $name => $path) {
				$tmp = explode('.', $name);
				if (($nb = count($tmp)) == 2) {
					if (strcmp(trim($tmp[0]), 'load') == 0) {
						$this->loadConfigFile($path, LOCAL);
					}
				}
			}
		}
		
		Function writeConfig($file, $name, $option)
		{
			$conf = new EnsKa_INIReader(self::$_path['global']['path_conf'].$file.'.ini');
			
			if (!$conf->getError()) {
				$conf->addLine($name, $option, true);
				return (true);
			}
			else {
				@trigger_error('Unable to open the configuration file: '.$file.' -- 0x1507', E_USER_NOTICE);
				return (false);
			}
		}
		
		Function removeConfig($file, $name)
		{
			$conf = new EnsKa_INIReader(self::$_path['global']['path_conf'].$file.'.ini');
			
			if (!$conf->getError()) {
				$conf->deleteLine($name, true);
				return (true);
			}
			else {
				@trigger_error('Unable to open the configuration file: '.$file.' -- 0x1508', E_USER_NOTICE);
				return (false);
			}
		}
}

?>