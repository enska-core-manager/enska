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
 
class newUser extends EnsKa_PlateformPlugin
{
	Public
		Function create($data=null)
		{
			$params = Array
						(
							'login'		 		=>	$data[1],
							'password'	 		=>	$data[2],
							'mail'	 			=>	$data[3]
						);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'new_user')) {
				$nparams = Array('table' => get_table('pb.users_auth'), 'id' => 'uid');
				$nuid = create_new_id($nparams);
				$req = dbInsert('pb.users_auth', Array('uid' => $nuid, 'login' => $this->_store->getData('login'), 'password' => md5($this->_store->getData('password')), 'mail' => $this->_store->getData('mail'), 'uType' => 'usr'));
				$_line = __LINE__;
				if ($req) {
					$req = dbInsert('pb.users_infos', Array('uid' => $nuid));
					$_line = __LINE__;
					if ($req) {
						unset($nparams);
						$nparams = Array('table' => get_table('pb.users_groups'), 'id' => 'id');
						$nid = create_new_id($nparams);
						$req = dbInsert('pb.users_groups', Array('id' => $nid, 'uid' => $nuid, 'gid' => 10, 'status' => 'ok'));
						$_line = __LINE__;
						if ($req) {
							echo 'success';
							unset($params, $data, $req, $nparams, $nuid, $nid);
							return (true);
						}
						else {
							dbDelete('pb.users_auth', Array('uid' => $nuid));
							dbDelete('pb.users_infos', Array('uid' => $nuid));
							$err = 'UNABLE TO CREATE THE NEW USER (3)';
							$notice = $err.'****1x1401****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						dbDelete('pb.users_auth', Array('uid' => $nuid));
						$err = 'UNABLE TO CREATE THE NEW USER (2)';
						$notice = $err.'****1x1402****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'UNABLE TO CREATE THE NEW USER (1)';
					$notice = $err.'****1x1403****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters';
				$notice = $err.'****1x1404****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			
			unset($err, $params, $data, $req, $nparams, $nuid, $nid);
			return (false);
		}		
}

?>