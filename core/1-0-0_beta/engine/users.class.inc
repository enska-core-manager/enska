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

class EnsKa_Users
{	
	Private	static	$_instance;
	Public			$_user;
	
	Public
		static Function get()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
			}
			return self::$_instance;
		}
		
		Function getUsers()
		{
			$res = dbSelect('pb.users_auth', Array('uid'));
			return (getSimpleArray($res));
		}
		
		Function getUserLogin($uid)
		{
			$res = getComplexArray(dbSelect('pb.users_auth', Array('login'), Array('uid' => $uid)));
			return ($res['login']);
		}
		
		Function addUser($params)
		{
			try
			{
				if(check_params($params, 'new_user')) {
					$nparams = Array('table' => get_table('pb.users_auth'), 'id' => 'uid');
					$nid = create_new_id($nparams);
					
					unset($params['uid']);
					$params['uid'] = $nid;
					
					$req_add = dbInsert('pb.users_auth', Array('uid' => $nid, 'login' => $params['login'], 'password' => $params['password'], 'mail' => $params['mail']));
					unset($params['login'], $params['password'], $params['mail']);
					$req_infos = dbInsert('pb.users_infos', $params);
					
					return (true);
				}
				else {
					throw new EnsKa_Exception('Unable to create the user, wrong parameters !', '0x1801', E_NOTICE);
				}
			}
			catch(EnsKa_Exception $e)
			{
				return (false);
			}
		}
		
		Function logIn($params)
		{
			GLOBAL $_alias;
			
			if(check_params($params, 'signin')) {
				$req = getComplexArray(dbSelect('pb.users_auth', Array('uid', 'login'), Array('AND' => Array('login' => $params['usrLogin'], 'password' => md5($params['usrPasswd'])))));
				
				if(isset($req['uid'])) {
					EnsKa_Sessions::get()->setSession('uid', $req['uid']);
					EnsKa_Sessions::get()->setSession('user', new user($req['uid']));
					$res = $req['uid'];
					$config = EnsKa_Config::get()->getConfig();
					setcookie('uid', $res, (time() + (int)$config['global']['auth']['auth_cookie_time']), '/', '.'.$_alias['path']['global']['domain']);
				}
				else {
					$res = BAD_LOGIN;
				}
			}
			else {
				$res = BAD_LOGIN;
			}
			return ($res);
		}
		
		Function logOut()
		{
			GLOBAL $_alias;
			
			$config = EnsKa_Config::get()->getConfig();
			EnsKa_Sessions::get()->destroySession();
			setcookie('uid', 9, (time() + (int)$config['global']['auth']['auth_cookie_time']), '/', '.'.$_alias['path']['global']['domain']);
			redirect('http://login.'.$_alias['path']['global']['domain']);
		}

	Private
		Function __construct() {}
		Function __destruct() {}
}

?>