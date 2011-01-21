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
 
class manageLogs extends EnsKa_PlateformPlugin
{
	Public
		Function export($data=null)
		{
			$params = Array
						(
							'folder'	=>	$data[1],
							'fileName'	=>	$data[2],
						);
			$this->_store->setDatas($params);
			
			$path = $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_logs'].$this->_store->getData('folder').'/';
			if (!is_dir($path)) {
				mkdir($path, 0777, true);
			}
			
			$file = $path.$this->_store->getData('fileName').'.log';
			if (!file_exists($file)) {
				fclose(fopen($path.$this->_store->getData('fileName').'.log', 'w'));
			}
			
			$source = $file;
			$target = $this->_alias['path']['global']['point'].$this->_alias['path']['local']['path_temp'].$this->_store->getData('fileName').'.log';
			
			$_line = __LINE__;
			if (copy($source, $target)) {
				echo 'success';
				unset($data, $params, $path, $file, $tmp, $source, $target, $_line);
				return (true);
			}
			
			echo 'UNABLE TO EXPORT THE LOGS FILE';
			trigger_error('Unable to export the logs file****1x21002****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
			unset($data, $params, $path, $file, $tmp, $source, $target, $_line);
			return (false);
		}
		
		Function edit($data=null)
		{
			$params = Array
						(
							'website'			=> 	$data[1],
							'status'			=>	$data[2],
							'folder'			=>	$data[3],
							'maxSize'			=>	$data[4],
							'fileName'			=>	$data[5],
							'tError'			=>	$data[6],
							'tWarning'			=>	$data[7],
							'tNotice'			=>	$data[8],
							'tParse'			=>	$data[9],
							'tStrict'			=>	$data[10],
							'tDeprecated'		=>	$data[11],
							'tAll'				=>	$data[12]
						);
			$this->_store->setDatas($params);

			$path = $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_logs'].$this->_store->getData('folder').'/';
			if (!is_dir($path)) {
				mkdir($path, 0777, true);
			}
			
			if (!file_exists($path.$this->_store->getData('fileName').'.log')) {
				fclose(fopen($path.$this->_store->getData('fileName').'.log', 'w'));
			}

			$_line = __LINE__;
			$conf = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('website').'/conf/logs.ini');

			if (!$conf->getError()) {
				$conf->AddLine('status', $this->_store->getData('status'));
				$conf->AddLine('max_size', $this->_store->getData('maxSize'));
				$conf->AddLine('filename', $this->_store->getData('fileName'));
				$conf->AddLine('path', $this->_store->getData('folder'));
				$conf->AddLine('tError', $this->_store->getData('tError'));
				$conf->AddLine('tWarning', $this->_store->getData('tWarning'));
				$conf->AddLine('tNotice', $this->_store->getData('tNotice'));
				$conf->AddLine('tParse', $this->_store->getData('tParse'));
				$conf->AddLine('tStrict', $this->_store->getData('tStrict'));
				$conf->AddLine('tDeprecated', $this->_store->getData('tDeprecated'));
				$conf->AddLine('tAll', $this->_store->getData('tAll'));
				$conf->save();
				echo 'success';
				unset($data, $params, $_line, $path);
				return (true);
			}
			else {
				echo $conf->getError();
				trigger_error('Unable to edit the logs system****1x21001****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($data, $params, $_line, $path);
				return (false);
			}
		}
		
		Function purge($data=null)
		{
			$pathLog = $data[1];
			$path = $this->_alias['path']['global']['point'].$this->_alias['path']['global']['path_logs'].$pathLog.'/';
			
			$files = get_file_list($path);
			if (is_array($files)) {
				foreach($files as $file) {
					unlink($path.$file);
				}
			}
			
			echo 'success';
			unset($data, $pathLog, $path, $files, $file);
			return (true);
		}
}

?>