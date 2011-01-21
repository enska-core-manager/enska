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
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */
 
Function is_loged($token, $ua){
	
	$decode = decode_token($token, true);
	
	if (!($private_key = get_private_key($decode['uid'])))
		return (false);
	
	$ua = md5($private_key.$ua);
	$uid = md5($private_key.$decode['uid']);
	
	if ($ua == $decode['ua_key']){
		if ($uid == $decode['uid_key']){
			$res['uid'] = $decode['uid'];
			$res['app_id'] = $decode['app_id'];
			return ($res);
		}
		else
			return (false);
	}
	else
		return (false);
}

Function decode_token($token, $check=false){
	
	$len_uid = strlen($token) - 96;
	
	if ($check){
		$i = 0;
		while ($token[$i] != 'z'){
			$i++;
		}
		$res['app_id'] = substr($token, ($i + 1), 16 - ($i + 1));
	}
	
	$res['ua_key'] = substr($token, 16, 17).substr($token, (56 + $len_uid), 15);
	$res['uid_key'] = substr($token, 33, 23).substr($token, (56 + $len_uid) + 15, 9);
	$res['uid'] = substr($token, 56, $len_uid);
	
	return ($res);
}

Function get_private_key($uid){

	$req = dbquery("SELECT `key` FROM `base_ws_token` WHERE `uid`='$uid'");
	if (@mysql_num_rows($req) > 0)
		return ($private_key = @mysql_result($req, 0, 'key'));
	return (false);
}

?>