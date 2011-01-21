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
 
class manageLibraries extends EnsKa_PlateFormPlugin
{
	Public
		Function export($data)
		{
			$lib = 'lib_'.$data[1];
			
			$options = Array
			(
				'archiveExtension'	=>	'zip',
				'archiveName'		=>	$lib,
				'exportSchema'		=>	'library',
				'replace'			=>	Array
										(
											'%libName%'	=>	$lib
										)
			);
			
			$_line = __LINE__;
			if (!$this->exportOptions($options)) {
				trigger_error('Unable to set the library export option****1x2601****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($lib, $data, $_line);
				return (false);
			}
			
			$this->addCPY('copy', $this->_alias['path']['global']['point'].$this->_alias['path']['global']['libs'].$lib.'.inc', $this->_store->getData('tmpFolder').$lib.'/'.$lib.'.inc');
			
			echo $this->doExport();
			unset($lib, $data, $_line);
		}
		
		Function import($data=null)
		{
			$this->_store->setData('file_size_limit', 1);
			
			$_line = __LINE__;
			if (strcmp(($res = $this->doImport()), 'ok') == 0) {
				$_line = __LINE__;
				if (strcmp(($res = $this->installArchive()), 'ok') == 0) {
					unset($data, $res, $_line);
					rollBackFolders($this->_store->getData('pathTemp'));
					if ($this->_store->getData('wizard')) {
						return('{"success": true, "msg":{"msgInfo":"The library(ies) has been successfully imported !"}}');
					}
					else {
						echo '{"success": true, "msg":{"msgInfo":"The library(ies) has been uploaded !"}}';
						return;
					}
				}
			}
			
			trigger_error('Unable import the library****1x2602****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $_line);
			
			if ($this->_store->getData('wizard')) {
				return('{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}');
			}
			else {
				echo '{"success": true, "msg":{"msgInfo":"err;'.$res.'"}}';
			}
			unset($res);
			return;
		}
		
		Function edit($data=null)
		{
			$params = Array
						(
							'lib'	 		=>		'lib_'.$data[1],
							'loaded'		=>		$data[2],
							'website'		=>		$data[3]
						);
			$this->_store->setDatas($params);
			
			if (strcmp($this->_store->getData('website'), 'default') == 0) {
				$_line = __LINE__;
				$libsFile = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'libs.ini');
			}
			else {
				$_line = __LINE__;
				$libsFile = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('website').'/conf/libs.ini');
			}
			
			if (!$libsFile->getError()) {
				$libsFile->addLine($this->_store->getData('lib'), $this->_store->getData('loaded'), true);
				echo 'success';
				unset($libsFile, $params, $data, $_line);
				return (true);
			}
			else {
				$err = 'UNABLE TO SAVE THE LIBS CONFIGURATION FILE<br>'.$libsFile->getError();
				trigger_error('Unable to save the libs configuaration file -- '.$libsFile->getError().'****1x2603****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			}
			
			echo $err;
			unset($err, $libsFile, $params, $data);
			return (false);
		}
		
		Function delete($data=null)
		{
			$lib = 'lib_'.$data[1];
			$sites = dbSelect('pb.site_infos', Array('id_name'));
			
			foreach($sites as $site) {
				$libsFile = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$site['id_name'].'/conf/libs.ini');
				$libsFile->deleteLine($lib, true);
				unset($libsFile);
			}
			
			$libsFile = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'libs.ini');
			$libsFile->deleteLine($lib, true);
			
			@unlink($this->_alias['path']['global']['point'].$this->_alias['path']['global']['libs'].$lib.'.inc');
			
			echo 'success';
			unset($data, $lib, $sites, $site, $libsFile);
			return (true);
		}
	
	Private
		Function installArchive()
		{
			$files = get_folder_list($this->_store->getData('pathTemp'));
			$sites = dbSelect('pb.site_infos', Array('id_name'));
			
			foreach($files as $file) {
				if (!file_exists($this->_alias['path']['global']['point'].$this->_alias['path']['global']['libs'].$file.'.inc')) {
					$_line = __LINE__;
					if(@copy($this->_store->getData('pathTemp').$file.'/'.$file.'.inc', $this->_alias['path']['global']['point'].$this->_alias['path']['global']['libs'].$file.'.inc')) {
						foreach($sites as $site) {
							$libsFile = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$site['id_name'].'/conf/libs.ini');
							$libsFile->addLine(substr($file, 0, strlen($file)), 'disable', true);
							unset($libsFile);
						}
						$libsFile = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'libs.ini');
						$libsFile->addLine(substr($file, 0, strlen($file)), 'disable', true);
						unset($libsFile);
						if (isset($err)) {
							if (strcmp($err, 'ok') == 0) {
								$err = 'ok';
							}
						}
					}
					else {
						$err .= 'UNABLE TO COPY THE LIBRARY '.$file.'<br>'.$this->_store->getData('pathTemp').$file;
						trigger_error('Unable to copy the library: '.$file.'****1x2604****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					}
				}
				else {
					$err .= 'THE LIBRARY '.$file.' ALREADY EXISTS !<br>';
					trigger_error('The library '.$file.' already exists****1x2605****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				}
			}
			
			if ((strcmp(trim($err), 'ok') == 0) || (trim($err) == '')) {
				return ('ok');
			}
			unset($files, $file, $sites, $site, $libsFile, $_line);
			return ($err);
		}
}

?>