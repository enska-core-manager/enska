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
 * @category   Model
 * @package    models/plugs
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */
 
class newACL extends EnsKa_PlateformPlugin
{
	Public
		Function create($data=null)
		{
			$params = Array
						(
							'storeName' 		=>	$data[0],
							'authUsers' 		=>	$data[1],
							'UnAuthUsers' 		=>	$data[2],
							'roleName' 			=>	$data[3],
							'roleDescription'	=>	$data[4],
							'rType'				=>	'sys'
						);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'new_acl_role')) {
				$this->_store->setData('authorizations', $this->getRights($this->_store));
				
				$nparams = Array('table' => get_table('pb.acl_infos'), 'id' => 'id');
				$nid = create_new_id($nparams);
				$_line = __LINE__;
				$req = dbInsert('pb.acl_infos', Array('id' => $nid, 'rType' => $this->_store->getData('rType'), 'role' => $this->_store->getData('roleName'), 'rights' => $this->_store->getData('authorizations'), 'description' => $this->_store->getData('roleDescription')));
				
				switch ($req) {
					case 1:
						echo 'success';
						unset($nparams, $data, $params, $req, $nid);
						return (true);
						break;
						
					default: 
						echo 'UNABLE TO CREATE ROLE';
						trigger_error('UNABLE TO CREATE ROLE****1x1202****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
						unset($nparams, $data, $params, $req, $nid);
						return (false);
						break;
				}
			}
			else {
				$err = 'WRONG PARAMETERS';
				trigger_error($err.'****1x1201****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($data, $params);
				return (false);
			}			
		}
}

?>