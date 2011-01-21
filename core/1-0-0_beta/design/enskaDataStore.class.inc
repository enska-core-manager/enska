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
 * @category   Design
 * @package    core
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

class EnsKa_DataStore
{
	Private	$_datas;
	Private	$_storeName;
	
	Public
		Function __construct($params=NULL)
		{
			$this->_datas = Array();
			$this->setDatas($params);
		}
		
		Function getData($attr)
		{
			if (isset($this->_datas[$attr])) {
				return ($this->_datas[$attr]);
			}
			return (false);
		}
		
		Function get($attr) {
			if (isset($this->_datas[$attr])) {
				return ($this->_datas[$attr]);
			}
			return (false);
		}
		
		Function getDatas()
		{
			return ($this->_datas);
		}
		
		Function gets()
		{
			return ($this->_datas);
		}
		
		Function setData($attr=NULL, $value=NULL)
		{
			if ($attr == NULL) {
				return ('ko');
			}
			$this->_datas[$attr] = $value;
			return (true);
		}
		
		Function setDatas($datas=NULL)
		{
			if (is_array($datas)) {
				foreach ($datas as $attr => $value) {
					$this->_datas[$attr] = $value;
				}
				return (true);
			}
			return (false);
		}
		
		Function set($attr=NULL, $value=NULL)
		{
			if ($attr == NULL) {
				return ('ko');
			}
			$this->_datas[$attr] = $value;
			return (true);
		}
		
		Function sets($datas=NULL)
		{
			if (is_array($datas)) {
				foreach ($datas as $attr => $value) {
					$this->_datas[$attr] = $value;
				}
				
				return (true);
			}	
			return (false);
		}
		
		Function unsetData($attr)
		{
			unset($this->_datas[$attr]);
		}
		
		Function clear()
		{
			foreach($this->_datas as $key => $value) {
				unset($this->_datas[$key]);
			}
		}
}

?>