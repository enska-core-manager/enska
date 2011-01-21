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
 
class manageRoles extends EnsKa_PlateformPlugin
{
	Public
		Function delete($data=null)
		{
			$idRole = $data[1];
			$_line = __LINE__;
			$req = getComplexArray(dbSelect('pb.acl_infos', Array('rType'), Array('id' => $idRole)));
			
			if($req) {
				$_line = __LINE__;
				if (strcmp($req['rType'], 'sys') != 0) {
					$_line = __LINE__;
					$req = dbDelete('pb.acl_infos', Array('id' => $idRole));
					if ($req) {
						echo 'success';
						unset($req, $data, $idRole);
						return (true);
					}
					else {
						$err = 'UNABLE TO DELETE THE SELECTED ROLE';
						$notice = $err.'****1x2401****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'YOU CAN NOT DELETE A SYSTEM ROLE';
					$notice = $err.'****1x2402****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'UNABLE TO READ THE SELECTED ROLE';
				$notice = $err.'****1x2403****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($req, $data, $idRole, $err, $notice, $_line);
			return (false);
		}

		Function edit($data=null)
		{
			$params = Array
						(
							'authUsers' 	=>		$data[1],
							'UnAuthUsers' 	=>		$data[2],
							'roleName' 		=>		$data[3],
							'description' 	=>		$data[4],
							'idRole'		=>		$data[5]
						);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'edit_acl_role')) {
				$_line = __LINE__;
				$req = getComplexArray(dbSelect('pb.acl_infos', Array('rType'), Array('id' => $this->_store->getData('idRole'))));
				if($req) {
					$_line = __LINE__;
					$req = dbUpdate
					(
						'pb.acl_infos',
						Array
						(
							'role' 			=> 		$this->_store->getData('roleName'),
							'description' 	=> 		$this->_store->getData('description')
						),
						Array
						(
							'id' 			=> 		$this->_store->getData('idRole')
						)
					);
					
					if ($req) {
						$authorizations = $this->getRights($this->_store);
						$_line = __LINE__;
						$req = dbUpdate('pb.acl_infos', Array('rights' => $authorizations), Array('id' => $this->_store->getData('idRole')));
						if ($req) {
							echo 'success';
							unset($data, $params, $authorizations, $_line);
							return (true);
						}
						else {
							$err = 'MANAGER COULD NOT UPDATE THE ROLE RIGHTS';
							$notice = $err.'****1x2404****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						$err = 'MANAGER COULD NOT UPDATE THE ROLE INFORMATIONS';
						$notice = $err.'****1x2405****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'UNABLE TO READ THE SELECTED ROLE';
					$notice = $err.'****1x2406****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters';
				$notice = $err.'****1x2407****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($data, $params, $authorizations, $err, $notice, $_line);
			return (false);	
		}
}

?>