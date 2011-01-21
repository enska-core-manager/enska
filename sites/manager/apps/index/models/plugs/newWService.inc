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
 
class newWService extends EnsKa_PlateformPlugin
{
	Public
		Function create($data=null)
		{
			$params = Array
							(
								'authUsers' 	=>		$data[1],
								'UnAuthUsers' 	=>		$data[2],
								'idName'	 	=>		$data[3],
								'description'	=>		$data[4],
								'website'	 	=>		$data[5],
								'host'	 		=>		$data[6],
								'database'	 	=>		$data[7],
								'prefix'		=>		$data[8],
								'user'	 		=>		$data[9],
								'passwd'		=>		$data[10],
								'generate'	 	=>		$data[11]
							);
			$this->_store->setDatas($params);
			
			$_line = __LINE__;
			if (check_params($this->_store->getDatas(), 'new_ws')) {
				$_line = __LINE__;
				if (!is_dir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/'.$this->_store->getData('idName').'/')) {
					if (!is_dir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/')) {
						@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/', 0755);
					}
					
					/**
					 * Making WS folders
					 */
					@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/'.$this->_store->getData('idName').'/', 0755);
					@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/'.$this->_store->getData('idName').'/sdk/', 0755);
					@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/'.$this->_store->getData('idName').'/sdk/api/', 0755);
					@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/'.$this->_store->getData('idName').'/sdk/app/', 0755);
					@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/'.$this->_store->getData('idName').'/sdk/user/', 0755);
					@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/'.$this->_store->getData('idName').'/ws/', 0755);
					@mkdir($this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/'.$this->_store->getData('idName').'/ws/lib/', 0755);
					
					$this->_store->setData('pathAPI', $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_webservices'].$this->_store->getData('website').'/'.$this->_store->getData('idName').'/');
					
					/**
					 * Creating the api file system
					 */
					 
					$_line = __LINE__;
					if ($this->createServerAPI()) {
						$_line = __LINE__;
						if ($this->createServerLIB()) {
							$_line = __LINE__;
							if ($this->createSDK()) {
								$_line = __LINE__;
								if ($this->createAPI()) {
									echo 'success';
									return (true);
								}
								else {
									$err = 'UNABLE TO CREATE THE API';
									$notice = $err.'****1x1301****'.($_line + 1).'****'.__FILE__;
								}
							}
							else {
								$err = 'UNABLE TO CREATE THE SDK';
								$notice = $err.'****1x1302****'.($_line + 1).'****'.__FILE__;
							}
						}
						else {
							$err = 'UNABLE TO CREATE THE SERVER LIB';
							$notice = $err.'****1x1303****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						$err = 'UNABLE TO CREATE THE SERVER API';
						$notice = $err.'****1x1304****'.($_line + 1).'****'.__FILE__;
					}
				}
				else {
					$err = 'WEBService ALREADY EXISTS!';
					$notice = $err.'****1x1305****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'Wrong parameters!';
				$notice = $err.'****1x1306****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_NOTICE);
			rollBackFolders($this->_store->getData('pathAPI'));
			return (false);
		}
	
	Private
		Function createServerAPI()
		{
			$pathTPL = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_templates'].'/ws/server/';
			$pathServer = $this->_store->getData('pathAPI').'ws/';
			
			$_line = __LINE__;
			if (!copy($pathTPL.'ws.update.ini', $pathServer.'ws.update.ini')) {
				echo 'UNABLE TO CREATE THE UPDATE.INI<br>';
				trigger_error('UNABLE TO CREATE THE UPDATE.INI****1x1307****'.($_line + 1).'****'.__FILE__, E_USER_ERROR);
				return (false);
			}
			
			$replace = Array
						(
							'%name%'		=>	$this->_store->getData('idName'),
							'%website%'		=>	$this->_store->getData('website'),
							'%host%'		=>	$this->_store->getData('host'),
							'%pathAuth%'	=>	$this->_alias['path']['global']['path_webservices_auth']
						);
			
			/**
			 * Creating the API file
			 */
				$this->strReplaceInFile($pathTPL.'%name%_server_api.php', $pathServer.$this->_store->getData('idName').'_server_api.php', $replace);
			
			/**
			 * Creating the WSDL file
			 */
				$this->strReplaceInFile($pathTPL.'%name%_wsdl_api.wsdl', $pathServer.$this->_store->getData('idName').'_wsdl_api.wsdl', $replace);
			
			/**
			 * Creating the .HTACCESS file
			 */
				$this->strReplaceInFile($pathTPL.'.htaccess', $pathServer.'.htaccess', $replace);
			
			unset($pathTPL, $pathServer, $replace);
			return (true);
		}

		Function createServerLIB()
		{
			$pathTPL = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_templates'].'/ws/server/lib/';
			$pathServer = $this->_store->getData('pathAPI').'ws/lib/';
			
			$files = get_file_list($pathTPL);
			
			if (!is_array($files)) {
				return (false);
			}
			
			$replace = Array
							(
								'%name%'		=>	$this->_store->getData('idName'),
								'%website%'		=>	$this->_store->getData('website'),
								'%host%'		=>	$this->_store->getData('host'),
								'%user%'		=>	'',
								'%passwd%'		=>	'',
								'%database%'	=>	'',
								'%prefix%'		=>	''
							);
				
			if ($this->_store->getData('user') != 1) {
				$replace['%user%'] = $this->_store->getData('user');
			}
			
			if ($this->_store->getData('passwd') != 1) {
				$replace['%passwd%'] = $this->_store->getData('passwd');
			}
			
			if ($this->_store->getData('database') != 1) {
				$replace['%database%'] = $this->_store->getData('database');
			}
			
			if ($this->_store->getData('prefix') != 1) {
				$replace['%prefix%'] = $this->_store->getData('prefix');
			}
			
			foreach($files as $file) {
				$this->strReplaceInFile($pathTPL.$file, $pathServer.$file, $replace);
			}
			
			unset($$pathTPL, $pathServer, $replace, $files, $file);
			return (true);
		}

		Function createSDK()
		{
			$pathTPL = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_templates'].'/ws/sdk/';
			$pathServer = $this->_store->getData('pathAPI').'sdk/';
			
			$replace = Array
							(
								'%name%'		=>	$this->_store->getData('idName'),
								'%website%'		=>	$this->_store->getData('website'),
								'%host%'		=>	$this->_store->getData('host'),
								'%pathAuth%'	=>	$this->_alias['path']['global']['path_webservices_auth']
							);
			
			/**
			 * Creatting API files
			 */
				if (!@copy($pathTPL.'api/lib.inc', $pathServer.'api/lib.inc')) {
					return (false);
				}
				
				$this->strReplaceInFile($pathTPL.'api/%name%.php', $pathServer.'api/'.$this->_store->getData('idName').'.php', $replace);
				$this->strReplaceInFile($pathTPL.'app/index.php', $pathServer.'app/index.php', $replace);
				$this->strReplaceInFile($pathTPL.'user/config.inc', $pathServer.'user/config.inc', $replace);
				
			unset($pathTPL, $pathServer, $replace);
			return (true);
		}

		Function createAPI()
		{
			$nparams = Array('table' => get_table('pb.ws_infos'), 'id' => 'id');
			$nid = create_new_id($nparams);
			
			$req = 	dbInsert('pb.ws_infos', 
					Array
						(
							'id' => $nid, 
							'name' => $this->_store->getData('idName'), 
							'website' => $this->_store->getData('website'), 
							'description' => $this->_store->getData('description'), 
							'domain' => $this->_store->getData('host'), 
							'version' => '1.0.0', 
							'revision' => '1',
							'maj_link' => 'none', 
							'maj_auto' => 0, 
							'maj_check' => 'D', 
							'maj_date' => '0000-00-00'
						)
			);
			
			switch ($req)
			{
				case 1:
					$req2 = dbInsert('pb.ws_auth', Array('id' => $nid, 'rights' => $this->getRights($this->_store)));
					switch ($req2)
					{
						case 1:
							return (true);
							break;
							
						default:
							echo mysql_error().'<br>';
							dbDelete('pb.ws_infos', Array('id' => $nid));
							return (false);
							break;
					}
					break;
				default:
					echo mysql_error().'<br>';
					return (false);
					break;
			}
		}
}
?>