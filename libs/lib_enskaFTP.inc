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
 * EnsKa FTP
 *
 * EnsKa FTP is an extension of EnsKa_CURL for the FTP protocol
 *
 */
class EnsKa_FTP extends EnsKa_CURL
{
	Public
		Function __construct($url=null, $login=false, $passwd=false)
		{
			parent::__construct($url);
			$this->setLogin($login);
			$this->setpasswd($passwd);
			if ($login != false) {
				$this->setAuth(true);
			}
		}
		
		Function getFile($source, $target)
		{
			$this->setUrl($source);
			$fd = @fopen($target, 'w');
			if (!$fd) {
				$this->setError('Unable to open the target file');
				return (false);
			}
			
			$this->setOpt(CURLOPT_FILE, $fd);
			$res = $this->run();
			fclose($fd);
			
			if ($this->getError()) {
				return (false);
			}
			return (true);
		}
		
		Function sendFile($source, $target)
		{
			$this->setUrl($target);
		}
}

?>