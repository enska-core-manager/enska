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

class EnsKa_Cache
{
	Private	static	$_instance;
	Private	$_timeToCache;
	Private	$_cachePath;
	Private	$_expire;
	Private	$_urlEncoded;
	Private $_url;
	
	Public
		static Function get()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
			}	
			return self::$_instance;
		}
		
		Function isEnable()
		{
			GLOBAL $_alias;
			
			try
			{
				$cache = new EnsKa_INIReader($_alias['path']['global']['point']. $_alias['path']['global']['sites'].$_alias['site'].'/conf/cache.ini');
				if (!$cache->getError()) {
					if (strcmp($cache->getLine('status'), 'enable') == 0) {
						$this->_timeToCache = (int)trim($cache->getLine('time_to_cache'));
						$this->_cachePath = $_alias['path']['global']['point']. $_alias['path']['global']['path_cache'].trim($cache->getLine('cache_path')).'/';
						$this->_expire = time() - ($this->_timeToCache * 60);
						$this->_urlEncoded = strReplace(strReplace($_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'], '/', '%'), '?', '$').'.cache';
						$this->_url = rtrim($_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'], '/');
						@mkdir($this->_cachePath, 0777, true);
						if ($this->isExclude()) {
							return (false);
						}
						return (true);
					}
				}
				else {
					throw new EnsKa_Exception($cache->getError(), '0x1401', E_NOTICE);
				}
			}
			catch(EnsKa_Exception $e)
			{
				return (false);
			}
		}
		
		Function checkCache()
		{
			GLOBAL $_alias;
			
			if(file_exists($this->_cachePath.$this->_urlEncoded) && @filemtime($this->_cachePath.$this->_urlEncoded) > $this->_expire) {
				return (true);
			}
			return (false);
		}
		
		Function capture()
		{
			ob_start();
		}
		
		Function show()
		{
			file_put_contents($this->_cachePath.$this->_urlEncoded, ob_get_contents());
			ob_end_clean();
			readfile($this->_cachePath.$this->_urlEncoded);
		}
		
		Function release()
		{
			readfile($this->_cachePath.$this->_urlEncoded);
		}
		
	Private
		Function isExclude()
		{
			GLOBAL $_alias;
			
			try
			{
				$file = $_alias['path']['global']['point']. $_alias['path']['global']['sites'].$_alias['site'].'/conf/cache_exclude.ini';
				$fd = fopen($file, 'r');
				if ($fd) {
					while (!(feof($fd))) {
						$line = trim(fgets($fd));
						$tmp = explode('*', $line);
						if (isset($tmp[1])) {
							if (strcmp(trim($tmp[1]), trim($this->_url)) == 0) {
								fclose($fd);
								return (true);
							}
						}
					}
					fclose($fd);
				}
				else {
					throw new EnsKa_Exception('Unable to open the cache excludes configuration file: '.$file, '0x1402', E_NOTICE);
				}
			}
			catch(EnsKa_Exception $e)
			{
				return (false);
			}
		}
		
		Function __construct() {}
		Function __destruct() {}
}

?>