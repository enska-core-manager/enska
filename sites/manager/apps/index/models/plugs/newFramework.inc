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
 
class newFramework extends EnsKa_PlateformPlugin
{
	Public
		Function create($data=nulll)
		{	
			$datas = Array(
						'name'		=>  $data[1],
						'idName'	=>	$data[2],
						'version'	=>	$data[3]
					);
			$this->_store->setDatas($datas);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'new_framework')) {
				$_line = __LINE__;
				if ($this->createStructure()) {
					$_line = __LINE__;
					$req = dbInsert('pb.frameworks_infos', Array('id' => Array('create_new_id' => '%t,id'), 'name' => $this->_store->getData('name'), 'version' => $this->_store->getData('version'), 'revision' => '1', 'link' => '', 'id_name' => $this->_store->getData('idName')));
					if ($req) {
						echo 'success';
						unset($data, $datas, $_line);
						return (true);
					}
					else {
						rollBackFolders($this->_alias['path']['global']['frmPath'].'/'.$this->_store->getData('idName'));
						$err = 'Unable to create the framework information';
						$notice = $err.'****1x1903****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'Unable to create the framework structure';
					$notice = $err.'****1x1902****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters!';
				$notice = $err.'****1x1901****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_errro($notice, E_USER_NOTICE);
			unset($data, $datas, $_line, $notice, $err);
			return (false);
		}
	
	Private
		Function createStructure()
		{
			mkdir($this->_alias['path']['global']['frmPath'].'/'.$this->_store->getData('idName'), 0777, true);
			mkdir($this->_alias['path']['global']['frmPath'].'/'.$this->_store->getData('idName').'/content/', 0777, true);
			mkdir($this->_alias['path']['global']['frmPath'].'/'.$this->_store->getData('idName').'/schemas/', 0777, true);
			mkdir($this->_alias['path']['global']['frmPath'].'/'.$this->_store->getData('idName').'/source/', 0777, true);
			$source = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_apps'].$this->_alias['controller'].'/templates/default/frameworks/loader.php';
			$target = $this->_alias['path']['global']['frmPath'].'/'.$this->_store->getData('idName').'/loader.php';
			copy($source, $target);
			unset($source, $target);
			return (true);
		}
}

?>