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

class EnsKa_Sessions
{	
	Private	static	$_instance;
	
	Public
		static Function get()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
			}
			return self::$_instance;
		}
		
		Function load()
		{
			GLOBAL $_alias;
			
			$conf = new EnsKa_INIReader($_alias['path']['global']['point']. $_alias['path']['global']['sites'].$_alias['site'].'/conf/cache.ini');
			if (!$conf->getError()) {
				self::setSessionCacheExpire((int)$conf->getLine('session_cache_expire'));
				self::setSessionCacheLimiter((int)$conf->getLine('session_cache_limiter'));
			}
			
			session_start();
			
			if (!isset($_SESSION['uid'])) {
				self::setSession('uid', 9);
				self::setSession('lMCount', 0);
				self::setSession('test', 0);
				if (strcmp(trim('manager.'.$_alias['path']['global']['domain']), trim($_SERVER['SERVER_NAME'])) != 0) {
					if (!isset($_COOKIE['uid'])) {
						setcookie('uid', 9, (time() + 20), '/', '.'.$_alias['path']['global']['domain']);
					}
					else {
						self::setSession('uid', $_COOKIE['uid']);
					}
				}
			}
			else {
				if (strcmp(trim('manager.'.$_alias['path']['global']['domain']), trim($_SERVER['SERVER_NAME'])) != 0) {					
					if (!isset($_COOKIE['uid'])) {
						setcookie('uid', self::getSession('uid'), (time() + 20), '/', '.'.$_alias['path']['global']['domain']);
					}
					else {
						self::setSession('uid', $_COOKIE['uid']);
					}
				}
			}
		}
		
		Function setSession($name, $value)
		{
			$_SESSION[$name] = $value;
		}
		
		Function getSession($name)
		{
			return (@$_SESSION[$name]);
		}
		
		Function deleteSession($name)
		{
			unset($_SESSION[$name]);
		}
		
		Function destroySession()
		{
			session_destroy();
		}
		
		Function setSessionCacheExpire($time=0)
		{
			if (is_numeric($time)) {
				session_cache_expire($time);
			}
			else {
				return (false);
			}
			return (true);
		}
		
		Function setSessionCacheLimiter($mod='public')
		{
			session_cache_limiter($mod);
		}
		
	Private
		Function __construct()
		{
			/*Constructor*/
		}
		
		Function __desctruct()
		{
			/*Destructor*/
		}
}

?>