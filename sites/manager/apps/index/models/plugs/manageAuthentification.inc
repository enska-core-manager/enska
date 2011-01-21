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
 
class manageAuthentification extends EnsKa_PlateformPlugin
{
	Public
		Function export($data=null)
		{
			$engine = $data[1];
			
			$options = Array
			(
				'archiveExtension'	=>	'zip',
				'archiveName'		=>	$engine,
				'exportSchema'		=>	'authEngine',
				'replace'			=>	Array
										(
											'%idName%'	=>	$engine
										)
			);
			
			$_line = __LINE__;
			if (!$this->exportOptions($options)) {
				trigger_error('Unable to set the authentification engine export option****1x2****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($engine, $options, $data, $_line);
				return (false);
			}
			
			$this->addCPY('copyDir', $this->_alias['path']['global']['point'].$this->_alias['path']['global']['libs'].'Auth/'.$engine.'/', $this->_store->getData('tmpFolder').$engine.'/');
			
			echo $this->doExport();
			unset($engine, $options, $data, $_line);
		}
		
		Function edit($data=null)
		{
			$params = Array
					(
						'engine'	=>	$data[1],
						'default'	=>	$data[2],
						'time'		=>	$data[3],
						'options'	=>	$data[4]
					);
			$this->_store->setDatas($params);
			
			EnsKa_Config::get()->addGlobalConfig('auth', 'auth_lib', $this->_store->getData('engine'), true);
			EnsKa_Config::get()->addGlobalConfig('auth', 'auth_default_redirect', $this->_store->getData('default'), true);
			EnsKa_Config::get()->addGlobalConfig('auth', 'auth_cookie_time', $this->_store->getData('time'), true);
			
			$auth = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['global']['libs'].'Auth/'.$this->_store->getData('engine').'/auth.ini');
			
			$bkpClass = $auth->getLine('class');
			$bkpName = $auth->getLine('name');
			$bkpVersion = $auth->getLine('version');
			
			EnsKa_Config::get()->addGlobalConfig('auth', 'auth_class', $auth->getLine('class'), true);
			EnsKa_Config::get()->addGlobalConfig('auth', 'auth_name', $auth->getLine('name'), true);
			EnsKa_Config::get()->addGlobalConfig('auth', 'lib_version', $auth->getLine('version'), true);
			$auth->fileBuff($this->_store->getData('options'));
			
			$auth->reload();
			$auth->addLine('class', $bkpClass);
			$auth->addLine('name', $bkpName);
			$auth->addLine('version', $bkpVersion);
			$auth->save();
			
			echo 'success';
		}
		
		Function import($data=null)
		{
			$this->_store->setData('file_size_limit', 1);
			
			$_line = __LINE__;
			if (strcmp(($res = $this->doImport()), 'ok') == 0) {
				$_line = __LINE__;
				if (strcmp(($res = $this->installArchive()), 'ok') == 0) {
					@rollBackFolders($this->_store->getData('pathTemp'));
					@unlink($this->_store->getData('temp').$this->_store->getData('fileName'));
					if ($this->_store->getData('wizard')) {
						unset($data, $res, $_line);
						return ('{"success": true, "msg":{"msgInfo":"The authentication engine has been imported !"}}');
					}
					else {
						echo '{"success": true, "msg":{"msgInfo":"The authentication engine has been uploaded !"}}';
					}
					unset($data, $res, $_line);
					return;
				}
			}
			
			trigger_error('Unable to import the authentification engine****1x2****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $_line);
			
			if ($this->_store->getData('wizard')) {
				return ('{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}');
			}
			else {
				echo '{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}';
				unset($res);
				return;
			}
		}
		
		Function delete($data=null)
		{
			$engine = $data[1];
			$config = EnsKa_Config::get()->getConfig();
			
			if (strcmp($config['global']['auth']['auth_lib'], $engine) == 0) {
				echo 'Unable to delete the selected engine!<br>This is the current authentification mode!';
				return(false);
			}
			
			@rollBackFolders($this->_alias['path']['global']['point'].$this->_alias['path']['global']['libs'].'Auth/'.$engine);
			echo 'success';
		}
	
	Private
		Function installArchive()
		{
			copyDir($this->_store->getData('pathTemp'), $this->_alias['path']['global']['point'].$this->_alias['path']['global']['libs'].'Auth/');
			return ('ok');
		}
}

?>