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

class EnsKa_Groups
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
		
		Function getGroups()
		{
			$req = dbSelect('pb.groups_infos', Array('gid'));
			return (getSimpleArray($req));
		}
		
		Function getGroupName($gid)
		{
			$res = getComplexArray(dbSelect('pb.groups_infos', Array('name'), Array('gid' => $gid)));
			return ($res['name']);
		}
		
		Function addGroup($params)
		{
			try
			{
				if(check_params($params, 'new_group')) {
					$nparams = Array('table' => get_table('pb.groups_infos'),'id' => 'gid');
					$nid = create_new_id($nparams);
					$params['gid'] = $nid;
					$gid = $nid; 
					
					$req_add = dbInsert('pb.groups_infos', $params);
					$managers = explode (';',$params['managers']);
					
					foreach($managers as $manager) {
						unset($params);
						$params = Array('table' => get_table('pb.users_groups'),'id' => 'id');
						$nid = create_new_id($params);
						unset($params);
						$params = Array('id' => $nid, 'uid' => $manager, 'gid' => $gid, 'status' => 'ok');
						dbInsert('pb.users_groups', $params);
					}
					return (true);
				}
				else {
					throw new EnsKa_Exception('Unable to create the group, wrong parameters !', '0x1701', E_NOTICE);
				}
			}
			catch(EnsKa_Exception $e)
			{
				return(false);
			}
		}

	Private
		Function __construct() {}
		Function __destruct() {}
}

?>