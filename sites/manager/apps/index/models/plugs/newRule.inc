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
 
class newRule extends EnsKa_PlateformPlugin
{
	Public
		Function create($data=null)
		{
			$params = Array
						(
							'name'		 		=>	$data[1],
							'description'		=>	$data[2],
							'type'				=>	'usr'
						);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'new_params_rule')) {
				$nparams = Array('table' => get_table('pb.params_rules'), 'id' => 'id');
				$nid = create_new_id($nparams);
				
				$_line = __LINE__;
				$req = dbInsert('pb.params_rules', Array('id' => $nid, 'type' => $this->_store->getData('type'), 'name' => $this->_store->getData('name'), 'description' => $this->_store->getData('description')));
				
				if ($req) {
					echo 'success';
					unset($params, $nid, $req, $data);
					return (true);
				}
				else {
					echo 'UNABLE TO CREATE THE NEW RULE';
					trigger_error('Unable to create the new rule****1x1501****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					unset($params, $nid, $req, $data);
					return (false);
				}
			}
			else {
				echo 'Wrong parameters';
				trigger_error('Wrong parameters****1x1502****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($params, $nid, $req, $data);
				return (false);
			}
		}
}

?>