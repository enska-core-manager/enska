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
 * @category   Design
 * @package    core
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

/**
 * EnsKa CURL
 *
 * EnsKa CURL is a managment class for the libcurl
 *
 */
class EnsKa_CURL
{
	Protected	$_url;
	Protected	$_curl;
	Protected	$_res;
	Protected	$_err;
	Protected	$_infos;
	Protected	$_login;
	Protected	$_passwd;
	Protected	$_auth;
	Protected	$_curlAuth;
	Protected	$_post;
	Protected	$_files;
	Protected	$_isAuth;
	Protected	$_sid;
	Protected	$_fields;
	
	Public
		Function __construct($url=null)
		{
			$this->init();
			$this->setUrl($url);
			$this->setAuth();
			$this->_isAuth = false;
		}
		
		Function run()
		{
			$this->_res = curl_exec($this->_curl);
			$this->setError(curl_error($this->_curl));
			return ($this->_res);
		}
		
		Function getUrl()
		{
			return ($this->_url);
		}
		
		Function setUrl($url)
		{
			$this->_url = $url;
			$this->setOpt(CURLOPT_URL, $this->_url);
		}
		
		Function setOpt($attr, $value)
		{
			curl_setopt($this->_curl, $attr, $value);
			$this->setError(curl_error($this->_curl));
		}
		
		Function unsetOpt($opt)
		{
			$this->setOpt($opt, '');
			$this->setError(curl_error($this->_curl));
		}
		
		Function setLogin($login=false, $flogin=false)
		{
			$this->_login = $login;
			$this->_fields['login'] = $flogin;
		}
		
		Function setPassword($passwd=false, $fpasswd=false)
		{
			$this->_passwd = $passwd;
			$this->_fields['passwd'] = $fpasswd;
		}
		
		Function setAuth($auth=false, $ident=null)
		{	
			if ($auth == true) {
				$this->_auth = true;
				
				if ($ident != null) {
					$this->setOpt(CURLOPT_USERPWD, $ident);
				}
				else {
					if (($this->_login != false) && ($this->_passwd != false)) {
						$this->setOpt(CURLOPT_USERPWD, $this->_login.':'.$this->_passwd);
					}
					elseif (($this->_login != false) && ($this->_passwd == false)) {
						$this->setOpt(CURLOPT_USERPWD, $this->_login);
					}
					else {
						$this->_auth = false;
						$this->setError('You must set an identity or a Login/Password combination');
					}
				}
			}
			else {
				$this->_auth = false;
				$this->_isAuth = false;
				$this->setOpt(CURLOPT_USERPWD, '');
			}
		}
		
		Function authenticate($url)
		{
			if (($this->_login == false) && ($this->_passwd == false)) {
				return (false);
			}
			
			if (($this->_fields['login'] == false) && ($this->_fields['passwd'] == false)) {
				return (false);
			}
			
			if (!$this->_isAuth) {
				$this->_curlAuth = new EnsKa_CURL($url);
				
				$this->_curlAuth->setOpt(CURLOPT_POST, TRUE);
				
				$this->_curlAuth->setOpt(CURLOPT_POSTFIELDS,
					array(
						$this->_fields['login'] => $this->_login,
						$this->_fields['passwd'] => $this->_passwd
					)
				);
				
				$this->_curlAuth->setOpt(CURLOPT_COOKIEJAR, realpath('cookie.txt'));
				$this->_curlAuth->setOpt(CURLOPT_FOLLOWLOCATION, TRUE);
				$this->_curlAuth->setOpt(CURLOPT_COOKIESESSION, TRUE);
				$this->_curlAuth->setOpt(CURLOPT_RETURNTRANSFER, TRUE);
				
				$res = $this->_curlAuth->run();
				$this->_curlAuth->close();
				if ($this->_curlAuth->getError()) {
					return (false);
				}
				
				if (preg_match('/(PHPSESSID=[0-9a-z,-]{32,40})/i', $res, $m)) {
					$this->setSid('?' . $m[1]);
				} 
				else if (preg_match('#<input\s+type="hidden"\s+name="([^\r\n\t <>\'"\\\]+)"\s+value="([0-9a-z,-]{32,40})"\s*/?>#i', $ret, $m)) {
					$this->setSid('?' . $m[1] . '=' . $m[2]);
				}
				return (true);
			}
			return (false);
		}
		
		Function setSid($sid)
		{
			$this->_sid = $sid;
			$this->_isAuth = true;
		}
		
		Function getSid()
		{
			return ($this->_sid);
		}
		
		Function getInfos()
		{
			$this->_infos = curl_getinfo($this->_curl);
			$this->setError(curl_error($this->_curl));
		}
		
		Function getInfo($opt)
		{
			if (isset($this->_infos[$opt])) {
				return ($this->_infos[$opt]);
			}
			else {
				$this->setError('Info not found in tab');
			}
		}
		
		Function addPost($data, $val=null)
		{
			if ((!is_arry($data)) && ($val == null)) {
				$this->_post[$data] = $val;
			}
			else {
				foreach ($data as $key => $value) {
					$this->_post[$key] = $value;
				}
			}
		}
		
		Function resetPost()
		{
			$this->_post = array();
			unset($this->_post);
		}
		
		Function addFile($file)
		{
			if (is_array($file)) {
				if (is_file($file['file'])) {
					$this->_files[$file['name']] = '@'.realpath($file['file']);
				}
			}
			else {
				$this->setError('Unable to ads the file');
			}
		}
		
		Function resetFiles()
		{
			$this->_files = array();
			unset($this->_files);
		}
		
		Function init()
		{
			if ($this->_url != null) {
				$this->_curl = curl_init($this->_url);
			}
			else {
				$this->_curl = curl_init();
			}
			
			$this->setOpt(CURLOPT_TIMEOUT, 10);
			$this->setOpt(CURLOPT_CONNECTTIMEOUT, 10);
		}
		
		Function close()
		{
			if ($this->_curl) {
				curl_close($this->_curl);
			}
		}
		
		Function getError()
		{
			if ($this->_err != '') {
				return ($this->_err);
			}
			
			return (false);
		}
		
	Private
		Function setError($error=null)
		{
			if ((trim($error) == '') || ($error == null)) {
				$this->_err = false;
			}
			else {
				$this->_err = $error;
			}
		}
}

?>