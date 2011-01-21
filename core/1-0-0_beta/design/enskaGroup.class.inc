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

class EnsKa_Group implements EnsKa_IGroup
{	
	Protected	$_gid;
	Protected	$_infos;
	Protected	$_isSet;
	
	Public
		Function __construct($gid)
		{
			try
			{
				if (is_numeric($gid)) {
					if ($gid < 1) {
						throw new EnsKa_Exception('msg');
					}
				}
				else {
					throw new EnsKa_Exception('msg');
				}
				
				$this->_gid = $gid;
				$this->setIsSet();
			}
			catch(EnsKa_Exception $e)
			{
			}
		}
		
		Function getInfos()
		{
			if (!$this->isInfosSet()) {
				$res = getComplexArray(dbSelect('pb.groups_infos', Array('gid', 'name', 'description', 'view', 'managers'), Array('gid' => $this->_gid)));
				$this->setInfos($res);
				$this->setIsSet(true);
			}
			else {
				$res = $this->_infos;
			}
			
			try
			{
				if (!is_array($res)) {
					throw new EnsKa_Exception('msg');
				}
				
				return ($res);
			}
			catch(EnsKa_Exception $e)
			{
				return (-1);
			}
		}
		
		Function setInfos($properties=NULL, $permanent=false)
		{
			if ($properties == NULL) {
				return (-1);
			}
			elseif (is_array($properties)) {
				foreach($properties as $key => $info) {
					$this->_infos[$key] = $info;
				}
			}
			else {
				return (-1);
			}
			
			if ($permanent == true) {
				return ($this->save());
			}
			
			return (0);
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
		
		Function getUsers()
		{
			$res_users = dbSelect('pb.users_groups', Array('uid','status'), Array('gid' => $this->_gid));
			
			try
			{
				if (!$res_users) {
					throw new EnsKa_Exception('msg');
				}
				
				return ($res_users);
			}
			catch(EnsKa_Exception $e)
			{
				return (-1);
			}
		}
		
		Function getManagers()
		{
			$infos = $this->getInfos();
			$managers = explode(';', $infos['managers']);
			return ($managers);
		}
		
		Function save()
		{
			$infos = $this->getInfos();
			
			if (is_array($infos)) {
				if (trim($infos['name']) == "") {
					return (-1);
				}
				
				unset($infos['gid']);
				dbUpdate('pb.groups_infos', $infos, Array('gid' => $this->_gid));
				return (0);
			}
			
			return (-1);
		}
}

?>