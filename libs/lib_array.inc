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
 * Remove a tab level without the index keys kept.
 * Ex: 
 *		$tab = array(array('f1','f2','f3'));
 *		return (array('f1','f2','f3'));
 *
 * @param array $tab
 * @throws EnsKa_Exception
 * @return array
 */
Function getSimpleArray($tab=NULL)
{
	$msg = 'The argument is not an array';
	try {
		if ($tab) {
			if (!is_array($tab)) {
				throw new EnsKa_Exception($msg, '0x2001', 'E_ERROR');
			}
			
			foreach($tab as $datas) {
				if (is_array($datas)) {
					foreach ($datas as $data) {
						$res []= $data;
					}
				}
				else {
					$res []= $datas;
				}
			}
			
			return ($res);
		}
		else {
			throw new EnsKa_Exception($msg, '0x2001', 'E_ERROR');
		}
	}
	catch(EnsKa_Exception $e) {
		return (false);
	}
}

/**
 * Remove a tab level and keep index keys.
 * Ex: 
 *		$tab = array(array('f1','f2','f3',array('foo' => 'bar', 'hello' => 'world')));
 *		return (array('f1','f2','f3', array('foo' => 'bar', 'hello' => 'world')));
 *
 * @param array $tab
 * @throws EnsKa_Exception
 * @return array
 */
Function getComplexArray($tab=NULL)
{
	$msg = 'The argument is not an array';
	try {
		if ($tab) {
			if (!is_array($tab)) {
				throw new Exception($msg, '0x2002', 'E_ERROR');
			}
			foreach($tab as $datas) {
				if (is_array($datas)) {
					foreach ($datas as $key => $data) {
						$res [$key]= $data;
					}
				}
				else {
					$res []= $datas;
				}
			}
			
			return ($res);
		}
		else {
			throw new EnsKa_Exception($msg, '0x2002', 'E_ERROR');
		}
	}
	catch(Exception $e) {
		return (false);
	}
}

/**
 * Add a level to an array.
 *
 * @param array $tabs The container array
 * @param string $toNode The container node
 * @param string $levelName The new level name
 * @return mixed
 */
Function addArrayLevel($tabs, $toNode, $levelName)
{
	if ($toNode == RN) {
		$tab[$levelName] = NULL;
		return($tab);
	}
	else {
		foreach($tabs as $tabName=>$tabValue) {
			if (strcmp($tabName, $toNode) == 0) {
				$tabs[$tabName][$levelName] = NULL;
				return($tabs);
			}
			else {
				$res = addArrayLevel($tabs[$tabName], $toNode, $levelName);
				if ($res) {
					$tabs[$tabName] = $res;
					return($tabs);
				}
			}
		}
	}
	return (false);
}

/**
 * Add a value to an array node.
 *
 * @param array $tab The container array
 * @param string $toNode The container node
 * @param mixed $value The value to dump
 * @return mixed
 */
Function addValueInArray($tab, $toNode, $value)
{
	foreach($tab as $tabName=>$tabValue) {
		if (strcmp($tabName, $toNode) == 0) {
			$tab[$tabName] []= $value;
			return($tab);
		}
		else {
			$res = addValueInArray($tab[$tabName], $toNode, $value);
			if ($res) {
				$tab[$tabName] = $res;
				return($tab);
			}
		}
	}
	return (false);
}

/**
 * Make a string for extJs array format.
 * Ex: 
 *		$tab = array(array('f1','f2','f3',array('foo' => 'bar', 'hello' => 'world')));
 *		return (array('f1','f2','f3', array('foo' => 'bar', 'hello' => 'world')));
 *
 * @param EnsKa_DataStore $store
 * @throws EnsKa_Exception
 * @return string
 */
Function getExtArray($store=null)
{
	$msg = 'The argiment is not an array';
	try {
		if ($store) {
			$datas = $store->getDatas();
			if (is_array($datas)) {
				$res = '[';
				foreach ($datas as $data) {
					$res .= '[';
				
					if (is_array($data)) {
						foreach($data as $key => $val) {
							if (is_numeric($val)) {
								$res .= $val.',';
							}
							else {
								$res .= "'".$val."',";
							}
						}
					}
					else {
						if (is_numeric($data)) {
							$res .= $data.',';
						}
						else {
							$res .= "'".$data."',";
						}
					}
					
					$res = substr($res, 0, strlen($res) - 1);
					$res .= '],';
				}	
				$res = substr($res, 0, strlen($res) - 1);
				$res .= ']';
				return ($res);
			}
			else {
				throw new EnsKa_Exception($msg, '0x2003', 'E_ERROR');
			}
		}
		else {
			throw new EnsKa_Exception($msg, '0x2003', 'E_ERROR');
		}
	}
	catch(EnsKa_Exception $e) {
		return (-1);
	}
}

?>