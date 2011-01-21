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
 * Get a specific path folder list
 * If the parameter $complete is set the array 
 * returned the complete path of folders else it
 * return just their names
 *
 * @param string $path
 * @param string $complete
 * @return array
 */
Function get_folder_list($path, $complete=FALSE)
{
	if (($path == "") || (!is_dir($path))) {
		trigger_error($path.' is not a directory****0x2801', E_USER_ERROR);
		return (-1);
	}
	
	if (substr($path, -1) != '/') { 
		$path .= '/'; 
	}
  
	$handle = @opendir($path);
  
	$exit = false;
	$i = 0;
	$j = -1;
	
	while ($exit == false) {
	
		 @$file[$i] = readdir($handle);
		 
		if ($file[$i] == "") {
			 $exit = true;
		}
		else {
			if (($file[$i]!=".")&&($file[$i]!="..")) {
				if (!is_dir($file[$i]."/")) {
					$j++;
					if ($complete == TRUE) {
						$res[$j] = $path.$file[$i];
					}
					else {
						$res[$j] = $file[$i];
					}
				}
			}
		}
		$i++;
	}
	return ($res);
}

/**
 * Get a specific path file list
 * If the parameter $complete is set the array 
 * returned the complete path of files else it
 * return just their names
 *
 * @param string $path
 * @param string $complete
 * @return array
 */
Function get_file_list($path, $complete=FALSE)
{
	if (($path == "") || (!is_dir($path))) {
		trigger_error($path.' is not a directory****0x2802', E_USER_ERROR);
		return (-1);
	}

	if (substr($path, -1) != '/') {
		$path .= '/'; 
	}

	$handle = @opendir($path);
	$exit = false;
	$i = 0;
	$j = -1;
	$res = null;

	while ($exit == false) {
		@$file[$i] = readdir($handle);

		if ($file[$i] == ""){
			$exit = true;
		}
		else {
			if (($file[$i]!=".")&&($file[$i]!="..")) {
				if (!is_dir($file[$i]."/")) {
					$j++;
					if ($complete == TRUE) {
						$res[$j] = $path.$file[$i];
					}
					else {
						$res[$j] = $file[$i];
					}
				}
			}
		}     
		$i++;
	}
	return ($res);
}

/**
 * Get a specific path folder list
 * If the parameter $complete is set the array 
 * returned the complete path of folders else it
 * return just their names
 *
 * @param string $path
 * @param string $complete
 * @return array
 */
Function get_folder_content($path, $complete=FALSE)
{
	if (($path == "") || (!is_dir($path))) {
		trigger_error($path.' is not a directory****0x2803', E_USER_ERROR);
		return (-1);
	}
	
	if (substr($path, -1) != '/') {
		$path .= '/'; 
	}

	$handle = @opendir($path);
	$exit = false;
	$i = 0;
	$j = -1;
	
	while ($exit == false) {
		@$file[$i] = readdir($handle);
		
		if ($file[$i] == "") {
			$exit = true;
		}
		else {
			if (($file[$i] != '.') && ($file[$i] != '..')) {
				$j++;
				if ($complete == TRUE) {
					$res[$j] = $path.$file[$i];
				}
				else {
					$res[$j] = $file[$i];
				}
			}
		}     
		$i++;
	}
	return ($res);
}

/**
 * Delete a folder like the unix command "rm -rf"
 *
 * @param string $dir
 * @return bool
 */
Function rollBackFolders($dir)
{ 
	if (!file_exists($dir)) {
		return true;
	}

	$handle = @opendir($dir);
	$exit = false;
	
	while ($exit == false) {
		$file = @readdir($handle);
		
		if ($file == "") {
			$exit = true;
		}
		else {
			if ((strcmp($file, '.') != 0) && (strcmp($file, '..') != 0)) {
				chmod($dir.'/'.$file, 0777);
				if (is_dir($dir.'/'.$file)) {
					rollBackFolders($dir.'/'.$file);
				}
				else {
					@unlink($dir.'/'.$file);
				}
			}
		}
	}
	
	if (@rmdir($dir.'/')) {
		return (true);
	}
	else{
		return (rollBackFolders($dir));
	}
}

/**
 * Copy a folder like the unix command "cp -r"
 *
 * @param string $source
 * @param string $target
 * @return bool
 */
Function copyDir($source, $target)
{ 
	$dir = opendir($source); 
	@mkdir($target);
	
    while (false !== ($file = readdir($dir))) { 
        if (($file != '.') && ($file != '..')) { 
            if (is_dir($source.'/'.$file)) { 
                copyDir($source.'/'.$file, $target.'/'.$file);
            } 
            else { 
                @copy($source.'/'.$file, $target.'/'.$file); 
            } 
        } 
    } 	
    closedir($dir);
	return (true);
}

/**
 * Open a file and put his content on a variable
 *
 * @param string $fileName
 * @param int $size
 * @return mixed
 */
Function readFileContent($fileName, $size=1024)
{
	if (file_exists($fileName)) {
		$fd = fopen($fileName, 'r');
		if ($fd) {	
			$line = '';
			while (!(feof($fd))) {
				$line .= fgets($fd, $size);
			} 
			return ($line);
		}
	}
	trigger_error($fileName.' is not exists****0x2804', E_USER_ERROR);
	return (false);
}
 
?>