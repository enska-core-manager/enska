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
 
class newGroup extends EnsKa_PlateformPlugin
{
	Public
		Function create($data=null)
		{
			$params = Array
					(
						'name'		 		=>	$data[1],
						'description'	 	=>	$data[2]
					);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'new_group')) {
				$nparams = Array('table' => get_table('pb.groups_infos'), 'id' => 'gid');
				$ngid = create_new_id($nparams);
				$_line = __LINE__;
				$req = dbInsert('pb.groups_infos', Array('gid' => $ngid, 'name' => $this->_store->getData('name'), 'description' => $this->_store->getData('description'), 'view' => 'private', 'managers' => '1', 'gType' => 'usr'));
				if ($req) {
					echo 'success';
					unset($data, $params, $err, $nparams, $ngid, $req, $_line);
					return (true);
				}
				else {
					$err = 'UNABLE TO CREATE THE NEW GROUP';
					$notice = $err.'****1x1701****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters';
				$notice = $err.'****1x1702****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			unset($data, $params, $err, $nparams, $ngid, $req, $notice, $_line);
			return (false);
		}
}

?>