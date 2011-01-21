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
 
class manageRules extends EnsKa_PlateformPlugin
{
	Public
		Function delete($data=null)
		{
			$_line = __LINE__;
			$req = dbDelete('pb.params_rules', array('id' => $data[1]));

			if (!$req) {
				echo 'UNABLE TO DELETE THE RULE';
				trigger_error('Unable to delete the rule****1x2301****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($req, $data, $_line);
				return(false);
			}
			
			echo 'success';
			unset($req, $data, $_line);
			return (true);
		}
		
		Function deleteField($data=null)
		{
			 $params = Array
						(
							'id'	=>	$data[1],
							'field'	=>	$data[2]
						);
			$this->_store->setDatas($params);
			
			$req = getComplexArray(dbSelect('pb.params_rules', Array('rule'), Array('id' => $this->_store->getData('id'))));
			$rules = unserialize($req['rule']);
			
			$_line = __LINE__;
			if (!is_array($rules)) {
				echo 'UNABLE TO UPDATE THE RULES TABLE (1)';
				trigger_error('Unable to the rule table****1x2302****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($params, $data, $rules, $toDelete, $_line);
				return (false);
			}

			$result = Array();
			foreach($rules as $rule) {
				if ($rule['id'] != $this->_store->getData('field')) {
					$result[] = $rule;
				}
			}
			
			$_line = __LINE__;
			$req = dbUpdate('pb.params_rules', Array('rule' => serialize($result)), Array('id' => $this->_store->getData('id')));
			
			if (!$req) {
				echo 'UNABLE TO UPDATE THE RULES TABLE (2)';
				trigger_error('Unable to update the rules table****1x2303****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($params, $data, $rules, $toDelete, $rule, $rules, $res, $tmp, $req, $_line);
				return (false);
			}

			echo 'success';
			unset($params, $data, $rules, $toDelete, $rule, $rules, $res, $tmp, $req, $_line);
			return (true);
		}
		
		Function edit($data=null)
		{
			$params = Array
					(
						'name'			=>	$data[1],
						'description'	=>	$data[2],
						'id'			=>	$data[3]
					);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			$req = dbUpdate('pb.params_rules', Array('name' => $this->_store->getData('name'),'description' => $this->_store->getData('description')), Array('id' => $this->_store->getData('id')));

			if (!$req) {
				echo 'UNABLE TO UPDATE THE RULE';
				trigger_error('Unable to update the rule****1x2304****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($data, $params, $req, $_line);
				return (false);
			}

			echo 'success';
			unset($data, $params, $req, $_line);
			return (true);
		}
		
		Function addRuleParameter($data=null)
		{			
			$params = Array
						(
							'id'	=>	$data[1],
							'field'	=>	$data[2],
							'expr'	=>	$data[3],
							'fnct'	=>	$data[4]
						);
			$this->_store->setDatas($params);
		 
			$_line = __LINE__;
			if (!is_numeric($this->_store->getData('id'))) {
				echo 'YOU MUST SELECT A RULE';
				trigger_error('No rule elected****1x2305****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($data, $params, $_line);
				return (false);
			}
			
			if (trim($this->_store->getData('expr')) != '') {
				$this->_store->setData('type', 'expr');
				$this->_store->setData('rule', $this->_store->getData('expr'));
			}
			else {
				$_line = __LINE__;
				if (trim($this->_store->getData('fnct')) != '') {
					$this->_store->setData('type', 'fnct');
					$this->_store->setData('rule', $this->_store->getData('fnct'));
				}
				else {
					echo 'UNABLE TO ADD A EMPTY PARAMETER';
					trigger_error('Unable to add an empty rule****1x2306****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					unset($data, $params, $_line);
					return (false);
				}
			}
			
			$_line = __LINE__;
			$rules = getComplexArray(dbSelect('pb.params_rules', Array('rule'), Array('id' => $this->_store->getData('id'))));
			
			$rules = unserialize($rules['rule']);
			
			$id = 0;
			if (is_array($rules)) {
				foreach($rules as $rule) {
					$id = $rule['id'];
				}
				$id++;
			}
			
			$rule['id'] = $id;
			$rule['field'] = $this->_store->getData('field');
			$rule['type'] = $this->_store->getData('type');
			$rule['value'] = $this->_store->getData('rule');
			$rules[] = $rule;
			$result = serialize($rules);
			
			$_line = __LINE__;
			$req = dbUpdate('pb.params_rules', Array('rule' => $result), Array('id' => $this->_store->getData('id')));

			if (!$req) {
				echo 'UNABLE TO ADD RULES PARAMETER (2)';
				trigger_error('Unable to add the rule parameter (2)****1x2308****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($data, $params, $fields, $toUpdate, $new, $rules, $rule, $res, $req, $_line);
				return (false);
			}

			echo 'success';
			unset($data, $params, $fields, $toUpdate, $new, $rules, $rule, $res, $req, $_line);
			return (true);
		}
}

?>