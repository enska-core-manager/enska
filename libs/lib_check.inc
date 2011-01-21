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
 * @category   Library
 * @package    libs
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */
 
/**
 * Get an EnsKa rule
 *
 * @param string $rule
 * @return string
 */
Function get_rule($rule)
{
	$req = dbSelect('pb.params_rules', Array('rule'), Array('name' => $rule));
	$res = getComplexArray($req);
	return(unserialize($res['rule']));
}

/**
 * Add a new EnsKa rule
 *
 * @param string $params
 */
Function add_params_rule($params)
{
	try
	{
		if (check_params($params, 'new_params_rule')) {
			$params['id'] = Array('create_new_id' => '%t,id');
			$req = dbInsert('pb.params_rules', $params);
			if (!$req) {
				throw new EnsKa_Exception('Unable to add the new rule', '0x2301', 'E_WARNING');
			}
			return (true);
		}
		else {
			throw new EnsKa_Exception('Unable to add the new rule', '0x2302', 'E_WARNING');
		}
	}
	catch(EnsKa_Exception $e)
	{
		return (false);
	}
}

/**
 * Check a rule's parameter
 *
 * @param string $params
 * @return true if all is right or false on error
 */
Function check_params($params, $rule, $showOut=true)
{
	$exprs = get_rule($rule);
	
	if (!is_array($exprs)) {
		return (true);
	}
	
	foreach($params as $kField => $vField) {
		foreach($exprs as $expr) {
			if (strcmp($expr['field'], $kField) == 0) {
				if (strcmp($expr['type'], 'expr') == 0) {
					if (!@preg_match($expr['value'], $vField)) {
						if ($showOut) {
							echo "<i>$kField</i>: <b>$expr[value]</b> not match for <b>$kField</b><br>";
						}
						return(false);
					}
				}
				elseif (strcmp($expr['type'], 'fnct') == 0) {
					if (!$expr['value']($vField)) {
						if ($showOut) {
							echo "<i>$kField</i>: <b>$expr[value]</b> not match for <b>$kField</b><br>";
						}
						return (false);
					}
				}
				else {
					if ($showOut) {
						echo "<i>$kField</i>: <b>$expr[value]</b> not match for <b>$kField</b><br>";
					}
					return (false);
				}
			}
		}
	}
	return (true);
}

/**
 * Check if an e-mail format is good
 *
 * @param string $mail
 * @return true if right or false on error
 */
Function is_email($mail)
{
	$condition = '#^[\w.-]+@[\w.-]+\.[a-zA-Z]{2,5}$#';
	
	if(preg_match($condition, $mail)) {
		return (true);
	}
	
	return (false);
}

Function is_not_empty($str='')
{
	if (trim($str) == '') {
		return (false);
	}
	
	return (true);
}

?>