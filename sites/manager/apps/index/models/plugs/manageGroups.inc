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
 
class manageGroups extends EnsKa_PlateformPlugin
{
	Public
		Function editByGrid($data=null)
		{
			$params = Array
					(
						'gid'	 		=>		$data[1],
						'name'	 		=>		$data[2],
						'description'	=>		$data[3]
					);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'edit_group')) {
				$_line = __LINE__;
				$req = dbUpdate (
					'pb.groups_infos',
					Array
					(
						'name' 			=> 		$this->_store->getData('name'),
						'description'	=> 		$this->_store->getData('description')
					),
					Array
					(
						'gid' 			=> 		$this->_store->getData('gid')
					)
				);
				
				if ($req) {
					echo 'success';
					unset($data, $params, $req, $_line);
					return (true);
				}
				else {
					$err = 'Manager couldn\'t update the group';
					$notice = $err.'****1x1701****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters';
				$notice = $err.'****1x1702****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			unset($data, $params, $req, $err, $_line, $notice);
			return (false);
		}

		Function delete($data=null)
		{
			$gids = explode(',',$data[1]);
			
			foreach ($gids as $gid) {
				$_line = __LINE__;
				if (!is_sys_group($gid)) {
					dbDelete('pb.groups_infos', Array('gid' => $gid));
					dbDelete('pb.users_groups', Array('gid' => $gid));
				}
				else {
					echo 'YOU CAN NOT REMOVE A SYSTEM GROUP!';
					trigger_error('Unable to delete a system group****1x2703****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					unset($data, $gids, $gid, $_line);
					return (FALSE);
				}
			}
			
			echo 'success';
			unset($data, $gids, $gid, $_line);
			return (true);
		}
		
}

?>