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

class EnsKa_Rights
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
		
		Function is_in_group($uid, $gid)
		{
			$req = dbSelect('pb.users_groups', Array('id'), Array('AND' => Array('uid' => $uid, 'gid' => $gid, 'status' => 'ok')));
			
			if (!is_array($req)) {
				return(false);
			}
			return (true);
		}
		
		Function is_root($uid)
		{
			$req = dbSelect('pb.users_groups', Array('id'), Array('AND' => Array('uid' => $uid, 'gid' => 1)));
			
			if (!is_array($req)) {
				return(false);
			}
			return (true);
		}
		
	Private
		Function __construct() {}
		Function __destruct() {}
}

?>