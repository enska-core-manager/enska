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
 * @category   Design
 * @package    core
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

class EnsKa_User implements EnsKa_IUser
{	
	Protected	$_uid;
	Protected	$_infos;
	Protected	$_isSet;
	
	Public		$uid;
	Public		$login;
	Public		$mail;
	public		$first;
	public		$last;
	
	Public
		Function __construct($uid)
		{
			try
			{
				if (is_numeric($uid)) {
					if ($uid < 1) {
						$msg = 'UID does not exists';
						throw new EnsKa_Exception($msg, '0x3001', 'E_ERROR');
					}
				}
				else {
					$msg = 'UID must be a numeric';
					throw new EnsKa_Exception($msg, '0x3002', 'E_ERROR');
				}
				
				$this->_uid = $uid;
				$this->setIsSet();
				$this->getInfos();
			}
			catch(EnsKa_Exception $e){}
		}
		
		Function getUid()
		{
			return ($this->_uid);
		}
		
		Function getNames()
		{
			return ($this->first.' '.$this->last);
		}
		
		Function getInfos()
		{
			if (!$this->isInfosSet()) {
				$res = array_merge (
					getComplexArray(dbSelect('pb.users_auth', Array('login','mail'), Array('uid' => $this->_uid))),
					getComplexArray(dbSelect('pb.users_infos', '*', Array('uid' => $this->_uid)))
				);
				
				$this->setInfos($res);
				
				foreach($res as $key => $value) {
					$prop = new ReflectionProperty($this, $key);
					$prop->setValue($this, $value);
				}
				
				$this->setIsSet(true);
			}
			else {
				$res = $this->_infos;
			}
			
			try
			{
				if (!is_array($res)) {
					$msg = 'Error on user infos';
					throw new EnsKa_Exception($msg, '0x3003', 'E_ERROR');
				}
				return ($res);
			}
			catch(EnsKa_Exception $e)
			{
				return (-1);
			}
		}
		
		Function setInfos($properties)
		{
			if (is_array($properties)) {
				foreach($properties as $key => $info) {
					$this->_infos[$key] = $info;
				}
			}
			else {
				return (-1);
			}
			return (0);
		}
		
		Function getGroups()
		{
			$res_groups = dbSelect('pb.users_groups', Array('gid','status'), Array('uid' => $this->_uid));
			
			try
			{
				if (!$res_groups) {
					$msg = 'User has no group';
					throw new EnsKa_Exception($msg, '0x3004', 'E_NOTICE');
				}
				return ($res_groups);
			}
			catch(EnsKa_Exception $e)
			{
				return (-1);
			}
		}
		
		Function save()
		{
			$infos = $this->getInfos();
			
			unset($infos['login']);
			unset($infos['mail']);
			unset($infos['uid']);
			
			try
			{
				if (is_array($infos)) {
					if ((trim($this->login) == "") || (trim($this->mail) == "") || (!is_email($this->mail))) {
						$msg = 'User has no group';
						throw new EnsKa_Exception($msg, '0x3005', 'E_NOTICE');
					}
					
					foreach ($infos as $key => $value) {
						$prop = new ReflectionProperty($this, $key);
						$res[$key] = $prop->getValue($this);
					}
					
					dbUpdate('pb.users_auth', Array('login' => $this->login, 'mail' => $this->mail), Array('uid' => $this->_uid));
					dbUpdate('pb.users_infos', $res, Array('uid' => $this->_uid));
					
					return (0);
				}
				else
				{
					$msg = 'User has no group';
					throw new EnsKa_Exception($msg, '0x3006', 'E_NOTICE');
				}
			}
			catch(EnsKa_Exception $e)
			{
				return (-1);
			}
			
			return (-1);
		}
		
		Function setIsSet($value=false)
		{
			$this->_isSet = false;
			
			if ($value) {
				$this->_isSet = true;
			}
		}
		
		Function isInfosSet()
		{
			return ($this->_isSet);
		}
}

?>