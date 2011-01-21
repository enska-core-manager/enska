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
 
class manageCache extends EnsKa_PlateformPlugin
{
	Public
		Function edit($data=null)
		{
			$params = Array
						(
							'website'			=> 	$data[1],
							'status'			=>	$data[2],
							'folder'			=>	$data[3],
							'timeC'				=>	$data[4],
							'timeS'				=>	$data[5],
							'sessionsLimiter'	=>	$data[6]
						);
			$this->_store->setDatas($params);

			$_line = __LINE__;
			$conf = new EnsKa_INIReader($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$this->_store->getData('website').'/conf/cache.ini');

			if ((strcmp($this->_store->getData('website'), 'manager') == 0) || (strcmp($this->_store->getData('website'), 'login') == 0) || (strcmp($this->_store->getData('website'), 'esandobx') == 0)) {
				$status = 'disable';
				if (!$conf->getError()) {
					$conf->AddLine('status', $status);
					$conf->AddLine('cache_path', '');
					$conf->AddLine('time_to_cache', 0);
					$conf->AddLine('session_cache_expire', 30);
					$conf->AddLine('session_cache_limiter', 'private');
					$conf->save();
					echo 'success';
					unset($data, $params, $_line, $conf, $status);
					return (true);
				}
				else {
					echo $conf->getError();
					trigger_error('Unable to edit the cache settings -- '.$conf->getError().'****1x2801****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
					unset($data, $params, $_line, $conf, $status);
					return (false);
				}
			}
			
			$status = 'disable';
			if ($this->_store->getData('status') == 1) {
				$status = 'enable';
			}
			if (!$conf->getError()) {
				$conf->AddLine('status', $status);
				$conf->AddLine('cache_path', $this->_store->getData('folder'));
				$conf->AddLine('time_to_cache', $this->_store->getData('timeC'));
				$conf->AddLine('session_cache_expire', $this->_store->getData('timeS'));
				$conf->AddLine('session_cache_limiter', $this->_store->getData('sessionsLimiter'));
				$conf->save();
				echo 'success';
				return (true);
			}
			else {
				echo $conf->getError();
				trigger_error('Unable to edit the cache settings -- '.$conf->getError().'****1x2802****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
				unset($data, $params, $_line, $conf, $status);
				return (false);
			}
		}
		
		Function purge($data=null)
		{
			$cache_path = $data[1];
			$path = $this->_alias['path']['global']['point']. $this->_alias['path']['global']['path_cache'].trim($cache_path).'/';
			
			$files = get_file_list($path);
			foreach($files as $file) {
				@unlink($path.$file);
			}
			
			echo 'success';
			return (true);
		}
		
		Function removeExclusion($data=nulll)
		{
			$website = $data[1];
			$id = $data[2];
			
			$_line = __LINE__;
			$fd = fopen($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$website.'/conf/cache_exclude.ini', 'r');
			if ($fd) {
				$maxId = 0;
				$save = '';
				while (!(feof($fd))) {
					$line = trim(fgets($fd));
					$tmp = explode('*', $line);
					if ((int)trim($tmp[0]) != $id) {
						$save .= $line."\n";
					}
				}
				fclose($fd);
				$save = trim($save);
				$_line = __LINE__;
				$fd = fopen($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$website.'/conf/cache_exclude.ini', 'w');
				if ($fd) {
					$_line = __LINE__;
					if (trim($save) != '') {
						$_line = __LINE__;
						if(fputs($fd, $save)) {
							fclose($fd);
							echo 'success';
							unset($data, $website, $id, $fd, $save, $line, $tmp);
							return (true);
						}
						else {
							$err = 'UNABLE TO WRITE IN THE EXCLUDES CACHE FILE -- '.$this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$website.'/conf/cache_exclude.ini';
							$notice = $err.'****1x2803****'.($_line + 1).'****'.__FILE__;
						}
					}
					else {
						fclose($fd);
						echo 'success';
						unset($data, $website, $id, $fd, $save, $line, $tmp);
						return (true);
					}
				}
				else {
					$err = 'UNABLE TO OPEN THE EXCLUDE CACHE FILE CONFIGURATION (2)';
					$notice = $err.'****1x2804****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'UNABLE TO OPEN THE EXCLUDE CACHE FILE CONFIGURATION (1)';
				$notice = $err.'****1x2805****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_ERROR);
			unset($data, $website, $id, $fd, $save, $line, $tmp, $_line, $notice);
			return (false);
		}
		
		Function addExclusion($data=null)
		{
			$website = $data[1];
			$url = $data[2];
			
			$_line = __LINE__;
			$fd = fopen($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$website.'/conf/cache_exclude.ini', 'r');
			if ($fd) {
				$maxId = 0;
				while (!(feof($fd))) {
					$line = trim(fgets($fd));
					$tmp = explode('*', $line);
					$maxId = (int)trim($tmp[0]);
				}
				fclose($fd);
				$_line = __LINE__;
				$fd = fopen($this->_alias['path']['global']['point'].$this->_alias['path']['global']['sites'].$website.'/conf/cache_exclude.ini', 'a+');
				if ($fd) {
					$maxId++;
					$url = ltrim($url, 'http://');
					$url = rtrim($url, '/');
					$toPut = "\n".$maxId.'*'.$url;
					if ($maxId == 1) {
						$toPut = $maxId.'*'.$url;
					}
					$_line = __LINE__;
					if(fputs($fd, $toPut)) {
						fclose($fd);
						echo 'success';
						unset($data, $website, $url, $fd, $maxId, $line, $tmp, $_line);
						return (true);
					}
					else {
						$err = 'UNABLE TO WRITE IN THE EXCLUDES CACHE FILE';
						$notice = $err.'****1x2806****'.($_line + 1).'****'.__FILE__;
					}
					fclose($fd);
				}
				else {
					$err = 'UNABLE TO OPEN THE EXCLUDE CACHE FILE CONFIGURATION (2)';
					$notice = $err.'****1x2807****'.($_line + 1).'****'.__FILE__;
				}
			}
			else {
				$err = 'UNABLE TO OPEN THE EXCLUDE CACHE FILE CONFIGURATION (1)';
				$notice = $err.'****1x2808****'.($_line + 1).'****'.__FILE__;
			}
			
			echo $err;
			trigger_error($notice, E_USER_ERROR);
			unset($data, $website, $url, $fd, $maxId, $line, $tmp, $_line,$notice);
			return (false);
		}
}

?>