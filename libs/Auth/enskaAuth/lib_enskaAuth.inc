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
 * @category   Library
 * @package    libs
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */
 
/**
 * EnsKa Auth
 *
 * EnsKa Auth authenticate user through EnsKa Core Plateform
 *
 */
 class EnsKa_Auth extends EnsKa_IAuth
 {
	Private	$_uid;
	Private	$_user = false;
	
	Public
		Function Authenticate($login, $password)
		{
			$params = Array
						(
							'usrLogin'		=>		$login, 
							'usrPasswd'		=>		$password
						);
			$this->_uid = EnsKa_Users::get()->logIn($params);
			
			return ($this->_uid);
		}
		
		Function Disconnect()
		{
			EnsKa_Users::get()->logOut();
		}
		
		Function getInfos()
		{
			if ($_user) {
				return($this->_user->getInfos());
			}
			else {
				$this->_user = new user($this->_uid);
				return($this->_user->getInfos());
			}
		}
 }
?>