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
 * EnsKa ZIP
 *
 * EnsKa ZIP is an extension of stdClass ZipArchive
 * Added features:
 *					- public function addDir($source, $target)
 *						- Add a "full" directory in a zip archive.
 *
 */
class EnsKa_ZIP extends ZipArchive
{
	Public
		/**
		 * Add a "full" directory in a zip archive
		 *
		 * @param string $source
		 * @param string $target
		 */
		Function addDir($source, $target=null) {
			/*
			 * Check if the directory source is exists
			 */
			if (!is_dir($source)) {
				trigger_error('Directory '.$source.' does not exist****0x2701', E_USER_ERROR); 
				return (false);
			}
			
			/*
			 * Format the string path
			 */
			if (substr($source, -1) != '/') { 
				$source.= '/';
			} 
			
			$dirStack = array($source);
			$cutFrom = strrpos(substr($source, 0, -1), '/')+1;

			/*
			 * While stack not empty loop
			 */
			while (!empty($dirStack)) {
				$currentDir = array_pop($dirStack);
				$filesToAdd = array();
				$dir = dir($currentDir);
				
				/*
				 * Loop on current directory
				 */
				while (false !== ($node = $dir->read())) {
					if (($node == '..') || ($node == '.')) {
						continue;
					}
					
					/*
					 * If current fd is a directory, add to $dirStack
					 */
					if (is_dir($currentDir . $node)) {
						array_push($dirStack, $currentDir . $node . '/');
					}
					
					/*
					 * If current fd is a file, add to $fileStack
					 */
					if (is_file($currentDir . $node)) {
						$filesToAdd []= $node;
					}
				}

				
				$localDir = substr($currentDir, $cutFrom);
				
				/*
				 * Create dir of the current stack position in the zip archive
				 */
				$this->addEmptyDir($target.$localDir);
				
				/*
				 * Adding file of the current stack position in zip archive
				 */
				foreach ($filesToAdd as $file) {
					$this->addFile($currentDir.$file, $target.$localDir.$file);
				}
			}
			return (true);
		}
}

?>