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
 * EnsKa Request
 *
 * EnsKa Request is an extension of EnsKa_CURL for the HTTP protocol
 *
 */
class EnsKa_Request extends EnsKa_CURL
{
	Public
		Function __construct($url=null)
		{
			parent::__construct($url);
		}
		
		Function isUrl($url=null)
		{
			if ($url != null) {
				$this->setUrl($url);
			}
			
			$this->setOpt(CURLOPT_NOBODY, TRUE);
			$this->run();
			
			if ($this->getError()) {
				return (false);
			}
			
			$this->getInfos();
			if (in_array($this->getInfo('http_code'), array(200,300,301,302,307))) {
				return (true);
			}
			else {
				return (false);
			}
		}
		
		Function getPage($auth=false)
		{	
			if ($auth && ($this->_isAuth == false)) {
				if (!$this->authenticate($auth)); {
					return (false);
				}
				$this->setUrl($this->getUrl().$this->getSid());
				$this->setOpt(CURLOPT_COOKIEFILE, realpath('cookie.txt'));
			}
			
			$this->setOpt(CURLOPT_NOBODY, FALSE);
			$this->setOpt(CURLOPT_RETURNTRANSFER, TRUE);
			$this->run();
			
			if ($this->getError()) {
				return (false);
			}
			
			return ($this->_res);
		}
		
		Function getFile($target, $auth=false)
		{
			GLOBAL $_alias;
			
			$fd = @fopen($target, 'w');
			if (!$fd) {
				$this->setError('Unable to open the temp file');
				return (false);
			}
			
			$this->setOpt(CURLOPT_NOBODY, FALSE);
			$this->setOpt(CURLOPT_RETURNTRANSFER, TRUE);
			$this->setOpt(CURLOPT_FILE, $fd);
			$res = $this->run();
			fclose($fd);
			
			if ($this->getError()) {
				return (false);
			}
			return (true);
		}
}

?>