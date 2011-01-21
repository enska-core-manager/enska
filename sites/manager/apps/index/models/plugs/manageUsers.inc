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
 
class manageUsers extends EnsKa_PlateformPlugin
{
	Public
		Function setOptions($data=null)
		{
			$act = $data[1];
			
			$_line = __LINE__;
			if (strcmp($act, 'add') == 0) {
				$fieldName = strtolower($data[2]);
				$fieldType = $data[3];
				$fieldSize = $data[4];
				$fieldDefault = $data[5];
				$fieldNotNull = $data[6];
				
				$_line = __LINE__;
				if (!in_array($fieldName, getUserProperties('all'))) {
					$_line = __LINE__;
					if (userClassFields($fieldName, 'add') == 0) {
						$table = get_table('pb.users_infos');
						$query = 'ALTER TABLE `'.$table.'` ADD `'.$fieldName.'` '.$fieldType;
						
						$fieldSizeException = Array
											(
												'DATE',
												'DATETIME',
												'TIMESTAMP',
												'TEXT',
												'LONGTEXT',
												'CHAR',
												'BLOB',
												'LONGBLOB',
												'ENUM',
												'BOOLEAN',
												'BIT',
												'BINARY'
											);
						
						if(($fieldSize > 0) && (is_numeric($fieldSize)) && (!in_array($fieldType, $fieldSizeException))) {
							$query .= ' ('.$fieldSize.')';
						}
						
						if (strcmp($fieldNotNull, 'ok') == 0) {
							$query .= ' NOT NULL';
						}
						else {
							$query .= ' NULL';
						}
						
						if (trim($fieldDefault) != '') {
							$query .= ' DEFAULT \''.$fieldDefault.'\'';
						}
						
						$_line = __LINE__;
						$req = dbQuery($query);
						
						if ($req) {
							echo 'success';
							unset($err, $data, $act, $filedName, $req, $query, $table, $fieldSize, $fieldType, $fieldNotNull, $fieldDefault, $fieldSizeException, $_line);
							return (true);
						}
						else {
							userClassFields($fieldName, 'del');
							$err = 'UNABLE TO CREATE THE OPTION (2)<br>	'.$query;
							$notice = 'Unable to create the option (2) -- '.$query.'****1x2201****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						userClassFields($fieldName, 'del');
						$err = 'UNABLE TO CREATE THE OPTION (1)';
						$notice = 'Unable to create the option (1)****1x2201****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'OPTION ALREADY SET';
					$notice = 'Option alreay set****1x2203****'.($_line + 1).'****'.__FILE__;
				}
			}
			elseif (strcmp($act, 'del') == 0) {
				$fieldName = $data[2];
				$_line = __LINE__;
				if (in_array($fieldName, getUserProperties('all'))) {
					$_line = __LINE__;
					if (userClassFields($fieldName, 'del') == 0) {
						$table = get_table('pb.users_infos');
						$_line = __LINE__;
						$query = 'ALTER TABLE `'.$table.'` DROP `'.$fieldName.'`';
						
						$req = dbQuery($query);
				
						if ($req) {
							echo 'success';
							unset($data, $act, $filedName, $req, $query, $table);
							return (true);
						}
						else {
							userClassFields($fieldName, 'add');
							$err = 'UNABLE TO DELETE THE OPTION (1)';
							$notice = 'Unable to delete the option (1) -- '.$query.'****1x2204****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						$err = 'UNABLE TO DELETE THE OPTION (2)';
						$notice = 'Unable to delete the option (2)****1x2205****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'THE SELECTED OPTION NOT EXISTS';
					$notice = 'The selected option not exists****1x2206****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'NO ACTION SET';
				$notice = 'No action set****1x2207****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($err, $data, $act, $filedName, $req, $query, $table, $fieldSize, $fieldType, $fieldNotNull, $fieldDefault, $fieldSizeException, $_line, $notice);
			return (false);
		}
		
		Function editByGrid($data=null)
		{
			$params = Array
					(
						'idUser'	 	=>		$data[1],
						'login'	 		=>		$data[2],
						'mail'	 		=>		$data[3]
					);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'edit_user')) {
				$req = dbUpdate
				(
					'pb.users_auth',
					Array
					(
						'login' 		=> 		$this->_store->getData('login'),
						'mail'			=> 		$this->_store->getData('mail')
					),
					Array
					(
						'uid' 			=> 		$this->_store->getData('idUser')
					)
				);
				
				if ($req) {
					echo 'success';
					unset($data, $params, $req, $_line);
					return (true);
				}
				else {
					$err = 'Manager couldn\'t update the user';
					$notice = 'Manager couldn\'t update the user****1x2208****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters!';
				$notice = 'Wrong parameters****1x2209****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($data, $params, $req, $err, $_line, $notice);
			return (false);
		}
		
		Function editByEditor($data=null)
		{
			$properties = getUserProperties();
			$startData = 1;
			$data_auth = Array
							(
								'uid' 			=>		$data[$startData + 1],
								'login'		 	=>		$data[$startData + 2],
								'mail'	 		=>		$data[$startData + 3]
							);
					
			$data_infos = Array
							(
								'first' 		=>		$data[$startData + 4],
								'last'			=>		$data[$startData + 5]
							);
			
			$store_auth = new EnsKa_DataStore($data_auth);
			$store_infos = new EnsKa_DataStore($data_infos);
			
			if (trim($data[$startData + 6]) != "") {
				$_line = __LINE__;
				$req = dbUpdate
				(
					'pb.users_auth',
					Array
					(
						'password' 		=> 		md5($data[$startData + 6])
					),
					Array
					(
						'uid' 			=> 		$store_auth->getData('uid')
					)
				);
				
				if (!$req) {
					unset($rpoperties, $startData, $data, $data_auth, $data_infos, $req, $store_auth, $store_infos);
					echo 'Error on changing user\'s password !';
					trigger_error('Error on changing user\'s password****1x2210****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					return;
				}
			}
			
			$i = $startData + 7;
			$j = 0;
			$nb = count($properties);

			while ($j < $nb) {
				$store_infos->setData($properties[$j], $data[$i + $j]);
				$j++;
			}

			$_line = __LINE__;
			if (check_params($store_auth->getDatas(), 'edit_user')) {
				$_line = __LINE__;
				$req = dbUpdate
				(
					'pb.users_auth',
					Array
					(
						'login' 		=> 		$store_auth->getData('login'),
						'mail' 			=> 		$store_auth->getData('mail')
					),
					Array
					(
						'uid' 			=> 		$store_auth->getData('uid')
					)
				);

				if ($req) {
					$_line = __LINE__;
					$req = dbUpdate
					(
						'pb.users_infos',
						$store_infos->getDatas(),
						Array
						(
							'uid' 			=> 		$store_auth->getData('uid')
						)
					);
					
					if ($req) {
						$usr = new user($store_auth->getData('uid'));
						if (count($oldGids = $usr->getGroups()) >= 1) {
							dbDelete('pb.users_groups', Array('uid' => $store_auth->getData('uid')));
						}
						
						$tmpGroup = rtrim($data[1], '/');
						$groups = explode('/', $tmpGroup);
						
						$nbGroup = count($groups);
						
						if ($nbGroup > 1) {
							$i = 0;
							while ($i < $nbGroup) {
								unset($rights);
								$rights = explode('-', $groups[$i]);
								
								if (isset($rights[1])) {
									if($rights[1] > 0) {
										$nparams = Array('table' => get_table('pb.users_groups'), 'id' => 'id');
										$nid = create_new_id($nparams);
										$tabInsert = Array('id' => $nid, 'uid' => $store_auth->getData('uid'), 'gid' => $rights[1], 'status' => 'ok');
										dbInsert('pb.users_groups', $tabInsert);
									}
								}
								$i++;
							}
						}
						else {
							$rights = explode('-', $tmpGroup);
							
							$nparams = Array('table' => get_table('pb.users_groups'), 'id' => 'id');
							$nid = create_new_id($nparams);
							
							if (isset($rights[1])) {
								if ($rights[1] > 0) {
									$tabInsert = Array('id' => $nid, 'uid' => $store_auth->getData('uid'), 'gid' => $rights[1], 'status' => 'ok');
								}
								else {
									$tabInsert = Array('id' => $nid, 'uid' => $store_auth->getData('uid'), 'gid' => 10, 'status' => 'ok');
								}
							}
							else {
								$tabInsert = Array('id' => $nid, 'uid' => $store_auth->getData('uid'), 'gid' => 10, 'status' => 'ok');
							}
							
							dbInsert('pb.users_groups', $tabInsert);
						}
						
						echo 'success';
						unset($rpoperties, $startData, $data, $data_auth, $data_infos, $req, $i ,$j, $nb, $store_auth, $store_infos, $tmpGroup, $groups, $nbGroup, $rights, $nid, $nparams, $tabInsert, $_line);
						return (true);
					}
					else {
						$err = 'Manager couldn\'t update the user informations ! (2)';
						$notice = 'Manager couldn\'t update the user information****1x2211****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'Manager couldn\'t update the user informations ! (1)';
					$notice = 'Manager couldn\'t update the user informations****1x2212****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters!';
				$notice = 'Wrong parameters****1x2213****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($rpoperties, $startData, $data, $data_auth, $data_infos, $req, $i ,$j, $nb, $store_auth, $store_infos, $tmpGroup, $groups, $nbGroup, $rights, $nid, $nparams, $tabInsert, $err, $_line, $notice);
			return (false);
		}
		
		Function delete($data=null)
		{
			$ids = explode(',',$data[1]);
			
			foreach ($ids as $uid) {
				$user = Array
						(
							'uid' => $uid
						);
				
				if (($user['uid'] != EnsKa_Sessions::get()->getSession('uid')) && (!EnsKa_Rights::get()->is_root($user['uid']))) {
					dbDelete('pb.users_auth', $user);
					dbDelete('pb.users_infos', $user);
					dbDelete('pb.users_groups', $user);
				}
				else {
					echo 'YOU CAN NOT REMOVE YOURSELF OR A ROOT USER!';
					unset($data, $ids, $uid, $user);
					return (false);
				}
			}
			
			echo 'success';
			unset($data, $ids, $uid, $user);
			return (true);
		}
}

?>