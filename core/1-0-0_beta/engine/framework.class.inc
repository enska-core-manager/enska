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
 * @category   Engine
 * @package    core
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

class EnsKa_FrameworkLoader
{	
	private	$_id = 0;
	private	$_idName = null;
	private	$_siteName = null;
	
	public
		Function __construct($siteName=null, $autoload=true)
		{
			if ($siteName != null) {
				$this->_siteName = $siteName;
				$this->getFrmId($siteName);
				$this->getFrmName($siteName);
			}
		}
		
		Function run()
		{
			GLOBAL $_alias;
			
			$_alias['framework'] = $this->_idName;
			include_once($_alias['path']['global']['frmPath'].'loader.php');
		}
		
		Function getFrmName()
		{
			if ($this->_idName != null) {
				return ($this->_idName);
			}
			else {
				$req = getComplexArray(dbSelect('pb.frameworks_infos', Array('id_name'), Array('id' => $this->_id)));
				
				try
				{
					if (is_array($req)) {
						$this->_idName = $req['id_name'];
						return ($this->_idName);
					}
					else {
						throw new EnsKa_Exception('Unable to get the framework name', '0x1601', E_ERROR);
					}
				}
				catch(EnsKa_Exception $e)
				{
					return (false);
				}
			}
		}
		
		Function getFrmId()
		{
			if ($this->_id != 0) {
				return ($this->_id);
			}
			else {
				$req = getComplexArray(dbSelect('pb.site_infos', Array('framework'), Array('id_name' => $this->_siteName)));
				try
				{
					if (is_array($req)) {
						$this->_id = $req['framework'];
						return ($this->_id);
					}
					else {
						throw new EnsKa_Exception('Unable to get the framework id', '0x1602', E_ERROR);
					}
				}
				catch(EnsKa_Exception $e)
				{
					return (false);
				}
			}
		}
}

?>