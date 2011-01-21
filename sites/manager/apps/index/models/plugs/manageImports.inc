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
 
class manageImports extends EnsKa_PlateFormPlugin
{
	Public
		Function run($data=null)
		{
			$params = Array
						(
							'type'		=>	$data[1],
							'sOptions'	=>	$data[2],
							'login'		=>	$data[3],
							'password'	=>	trim($data[4])
						);
			$this->_store->setDatas($params);
			$this->_store->setData('wizard', true);
			
			if ($this->_store->getData('type') == 1) {
				$this->_store->setData('_class', 'manageWebsite');
			}
			else if ($this->_store->getData('type') == 2) {
				$this->_store->setData('_class', 'manageApps');
			}
			else if ($this->_store->getData('type') == 3) {
				$this->_store->setData('_class', 'manageFrameworks');
			}
			else if ($this->_store->getData('type') == 4) {
				$this->_store->setData('_class', 'manageDatabases');
			}
			else if ($this->_store->getData('type') == 5) {
				$this->_store->setData('_class', 'manageLibraries');
			}
			else if ($this->_store->getData('type') == 6) {
				$this->_store->setData('_class', 'managePlugins');
			}
			else if ($this->_store->getData('type') == 7) {
				$this->_store->setData('_class', 'manageWServices');
			}
			else if ($this->_store->getData('type') == 8) {
				$this->_store->setData('_class', 'manageAuthentification');
			}
			else if ($this->_store->getData('type') == 9) {
				$this->_store->setData('_class', 'manageCore');
			}
			
			echo $this->_import();
		}
		
	Private
		Function _import()
		{	
			imports ('plugs.'.$this->_store->getData('_class'));
			
			$_class = $this->_store->getData('_class');
			$_method = $_class.'_import';
			
			if (class_exists($_class)) {
				$obj = new $_class();
				if (method_exists($obj, 'import')) {
					if (EnsKa_ACL::get()->checkACL(Array('role' => $_class.'_import', 'type' => 'uid', 'id' => EnsKa_Sessions::get()->getSession('uid')))) {
						return ($obj->import($this->_store->getDatas()));
					}
					else {
						return ('{"success": true, "msg":{"msgInfo":"err;YOU DO NOT HAVE ENOUGHT RIGHTS TO PERFORM THIS ACTION !"}}');
					}
				}
				else {
					return ('{"success": true, "msg":{"msgInfo":"err;Unknonwn plug-in ".$_class."::import in class ".$_class"}}');
				}
			}
			else {
				if (function_exists($_class.'_import')) {
					if (EnsKa_ACL::get()->checkACL(Array('role' => $_class.'_import', 'type' => 'uid', 'id' => EnsKa_Sessions::get()->getSession('uid')))) {
						return ($_method($this->_store->getDatas()));
					}
					else {
						return ('{"success": true, "msg":{"msgInfo":"err;YOU DO NOT HAVE ENOUGHT RIGHTS TO PERFORM THIS ACTION !"}}');
					}
				}
				else {
					return ('{"success": true, "msg":{"msgInfo":"err;Unknonwn plug-in ".$_class."::import"}}');
				}
			}
		}
}

?>