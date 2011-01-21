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
 * @category   Engine
 * @package    core
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

class EnsKa_Logs
{	
	Protected	static	$_instance;
	Private				$_errs;
	Private				$_opts;
	
	Public
		static Function start()
		{
			error_reporting(0);
			ini_set('display_errors', 0);
			
			set_error_handler(array(self::getErrorsInstance(),'errorsHandler'));
			set_exception_handler(array(self::getExceptionsInstance(),'exceptionsHandler'));
			return (self::getInstance());
		}

		static Function stop()
		{
			restore_error_handler();
			restore_exception_handler();
		}
		
		static Function getErrorsInstance()
		{
			return EnsKa_Errors::get();
		}
		
		static Function getExceptionsInstance()
		{
			return EnsKa_Exceptions::get();
		}
		
		static Function getInstances()
		{
			$instances = Array(
				'exceptions' => self::getExceptionsInstance(),
				'errors' => self::getErrorsInstance()
			);
			return($instances);
		}
		
		static Function getInstance()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
			}
			return self::$_instance;
		}
		
		Function addObserver($observer, $name, $mod=ALL)
		{
			$instances = self::getInstances();
			
			if ($mod == ALL) {
				$instances['errors']->addObserver($observer, $name);
				$instances['exceptions']->addObserver($observer, $name);
			}
			elseif ($mod == ERRORS) {
				$instances['errors']->addObserver($observer, $name);
			}
			elseif ($mod == EXCEPTIONS) {
				$instances['exceptions']->addObserver($observer, $name);
			}
		}
		
		Function deleteObserver($name, $mod=ALL)
		{
			$instances = self::getInstances();
			
			if ($mod == ALL) {
				$instances['errors']->deleteObserver($name);
				$instances['exceptions']->deleteObserver($name);
			}
			elseif ($mod == ERRORS) {
				$instances['errors']->deleteObserver($name);
			}
			elseif ($mod == EXCEPTIONS) {
				$instances['exceptions']->deleteObserver($name);
			}
		}
		
		Function isEnable()
		{
			GLOBAL $_alias;
			
			$logs = new EnsKa_INIReader($_alias['path']['global']['point']. $_alias['path']['global']['sites'].$_alias['site'].'/conf/logs.ini');
			if (!$logs->getError()) {
				if (strcmp($logs->getLine('status'), 'enable') == 0) {
					$this->_opts['max_size'] = (int)trim($logs->getLine('max_size')) * (1024 * 1024);
					$this->_opts['logs_path'] = $_alias['path']['global']['point']. $_alias['path']['global']['path_logs'].trim($logs->getLine('path')).'/';
					$this->_opts['logs_name'] = trim($logs->getLine('filename')).'.log';
					
					$this->_opts['type']['error'] = trim($logs->getLine('tError'));
					if(strcmp(trim($logs->getLine('tError')), 'none') != 0) {
						$this->_opts['type']['u_error'] = 'E_USER_ERROR';
					}
					
					$this->_opts['type']['warning'] = trim($logs->getLine('tWarning'));
					if(strcmp(trim($logs->getLine('tWarning')), 'none') != 0) {
						$this->_opts['type']['u_warning'] = 'E_USER_WARNING';
					}
					
					$this->_opts['type']['notice'] = trim($logs->getLine('tNotice'));
					if(strcmp(trim($logs->getLine('tNotice')), 'none') != 0) {
						$this->_opts['type']['u_notice'] = 'E_USER_NOTICE';
					}
					
					$this->_opts['type']['parse'] = trim($logs->getLine('tParse'));
					$this->_opts['type']['strict'] = trim($logs->getLine('tStrict'));
					$this->_opts['type']['deprecated'] = trim($logs->getLine('tDeprecated'));
					$this->_opts['type']['all'] = trim($logs->getLine('tAll'));
					
					@mkdir($this->_opts['logs_path'], 0777, true);
					if (!file_exists($this->_opts['logs_path'].$this->_opts['logs_name'])) {
						fclose(fopen($this->_opts['logs_path'].$this->_opts['logs_name'], 'w'));
					}
					return (true);
				}
			}
			return (false);
		}
		
		Function getOpts()
		{
			return($this->_opts);
		}
		
		Function getErrType($eCode)
		{
			foreach($this->_opts['type'] as $code) {
				if (strcmp($code, 'none') == 0) {
					continue;
				}
				if(($eCode == @constant($code)) || ($eCode == 30719))
					return (true);
					
			}
			return (false);
		}
		
		Function getErrors()
		{
			return ($this->_errs);
		}
		
		Function setErrors($params)
		{
			$this->_errs = $params;
		}
		
		Function getSource($file, $line)
		{
			if (file_exists($file)) {
				$fd = fopen($file, 'r');
				if ($fd) {
					if ($line <= 5) {
						$i = 0;
					}
					else {
						$i = $line - 5;
					}
					
					$j = 1;
					$buff = '';
					while (!feof($fd)) {
						$tmp = fgets($fd);
						if ($j == $i) {
							if ($i == $line) {
								$buff .= '->'.$i.'&nbsp;&nbsp;&nbsp;'.$tmp;
							}
							else {
								$buff .= $i.'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$tmp;
							}
							
							if ($i < $line + 5) {
								$i++;
							}
							else {
								break;
							}
						}
						$j++;
					}
					fclose ($fd);
					return ('<php>'.$buff.'</php>');
				}
			}
		}
		
	Private
		Function __construct() {}
		Function __destruct() {}
}

class EnsKa_Exception extends Exception
{    
    Public
		Function __construct($message='unknown error', $code='0x0000', $level=E_ERROR)
		{
			
			$this->message = $message;
			$this->code = $level;
			
			$params = Array(
									'message' 		=> 		$message,
									'code' 			=> 		$level,
									'eCode'			=>		$code,
									'trace' 		=> 		$this->getTrace(),
									'logType'		=>		'exception'
				);
			EnsKa_Logs::getInstance()->setErrors($params);
			EnsKa_Logs::getErrorsInstance()->sendEvent($params);
		}
}

class Enska_Exceptions implements EnsKa_ISubject
{	
	Protected	static	$_instance;
	Protected	static	$_pos;
	Private				$_observers;

	Public
		static Function get()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
				self::$_pos = -1;
			}
			return self::$_instance;
		}
		
		
		Function exceptionsHandler($e)
		{
			$params = Array(
								'message' 		=> 		$e->getMessage(),
								'code' 			=> 		$e->getCode(),
								'line' 			=> 		$e->getLine(),
								'file' 			=> 		$e->getFile(),
								'trace' 		=> 		$e->getTrace(),
								'fnct' 			=> 		'writeLogs',
								'logType'		=>		'error'
			);
			EnsKa_Logs::getInstance()->setErrors($params);
			self::sendEvent($params);
		}
		
		Function addObserver($observer, $name)
		{
			self::$_pos++;
			$this->_observers[self::$_pos]['name'] = $name;
			$this->_observers[self::$_pos]['observer'] = $observer;
		}
		
		Function deleteObserver($name)
		{
			$i=0;
			$nb = count($this->_observers);
			while ($i < $nb) {
				$ob = $this->_observers[$i];
				if (strcmp($ob['name'], $name) == 0) {
					$ob['observer']->getEvent('deleteObserver');
					unset($this->_observers[$i]);
				}
				$i++;
			}
		}
		
		Function sendEvent($params)
		{
			$i = 0;
			$nb = count($this->_observers);
			while ($i < $nb) {
				if ($this->_observers[$i]['observer'] != NULL) {
					$observer = $this->_observers [$i]['observer'];
					$observer->getEvent($params);
				}
				$i++;
			}
		}
		
	Private
		Function __construct() {}
		Function __destruct() {}
}

class Enska_Errors implements EnsKa_ISubject
{
	Protected	static	$_instance;
	Protected	static	$_pos;
	Private				$_observers;
	
	Public
		static Function get()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
				self::$_pos = -1;
			}
			return self::$_instance;
		}
		
		Function errorsHandler($code='0x0000', $message='unknown error', $file, $line, $context)
		{
			$params = Array
						(
							'code'		=>	$code,
							'message'	=>	$message,
							'file'		=>	$file,
							'line'		=>	$line,
							'context'	=>	$context,
							'logType'	=>	'php_error'
						);
			$this->sendEvent($params);
		}
		
		Function addObserver($observer, $name)
		{
			self::$_pos++;
			$this->_observers [self::$_pos]['name'] = $name;
			$this->_observers [self::$_pos]['observer'] = $observer;
		}
		
		Function deleteObserver($name)
		{
			$i=0;
			$nb = count($this->_observers);
			while ($i < $nb) {
				$ob = $this->_observers[$i];
				if (strcmp($ob['name'], $name) == 0) {
					$ob['observer']->getEvent('delete');
					unset($this->_observers [$i]);
				}
				$i++;
			}
		}
		
		Function sendEvent($params)
		{
			$i = 0;
			$nb = count($this->_observers);
			while ($i < $nb) {
				if ($this->_observers[$i]['observer'] != NULL) {
					$observer = $this->_observers [$i]['observer'];
					$observer->getEvent($params);
				}
				$i++;
			}
		}
		
	Private
		Function __construct() {}
		Function __destruct() {}
}

class EnsKa_LogsObserver extends EnsKa_Observer
{
	Private	$_opts;
	Private	$_file;
	
	Public
		Function __construct($name)
		{
			parent::__construct($name);
		}
		
		Function doEvent($params)
		{
			$this->_opts = EnsKa_Logs::getInstance()->getOpts();
			
			if (strcmp($params['logType'], 'php_error') == 0) {
				$eMsg = $params['message'];
				$eFile = $params['file'];
				$eLine = $params['line'];
				$eCode = $params['code'];
				if (($eCode == E_USER_ERROR) || ($eCode == E_USER_WARNING) || ($eCode == E_USER_NOTICE)) {
					$tmp = explode('****', $eMsg);
					$eMsg = $tmp[0];
					$code = $tmp[1];
					if (isset($tmp[2])) {
						$eLine = $tmp[2];
					}
					if (isset($tmp[3])) {
						$eFile = $tmp[3];
					}
				}
				else {
					$code = '0x4000';
				}
			}
			else {
				$trace = $params['trace'][0];
				$eCode = '0';
				if (isset($params['code'])) {
					$eCode = $params['code'];
				}
				
				$code = '0x0000';
				if (isset($params['eCode'])) {
					$code = $params['eCode'];
				}
				
				$eMsg = 'unknown error';
				if (isset($params['message'])) {
					$eMsg = $params['message'];
				}
				
				$eFile = 'unknown file';
				if (isset($trace['file'])) {
					$eFile = $trace['file'];
				}
				
				$eLine = 'unknown line';
				if (isset($trace['line'])) {
					$eLine = $trace['line'];
				}
				
				$eFunction = 'unknown function';
				if (isset($trace['function'])) {
					$eFunction = $trace['function'];
				}
			}
			
			if (EnsKa_Logs::GetInstance()->getErrType($eCode)) {
				$id = $this->getErrFileId();
				if ($id > 0) {
					$log = $id.'*'.$this->getErrType($eCode).'*'.$code.'*'.$eMsg.'*'.strReplace($eFile, '\\', '/').' line '.$eLine.'*'.date('Y-m-d H:i:s').'*'.'9';
					if ($id == 1) {
						$fd = @fopen($this->_file, 'w');
						if ($fd) {
							fputs($fd, $log);
							fclose($fd);
						}
						else {
							echo 'UNABLE TO OPEN THE LOG FILE';
						}
					}
					else {
						$fd = @fopen($this->_file, 'a+');
						if ($fd) {
							@fputs($fd, "\n".$log);
							@fclose($fd);
						}
						else {
							echo 'UNABLE TO OPEN THE LOG FILE';
						}
					}
				}
			}
			
			/*echo '<pre>';
				var_dump($params);
			echo '</pre>';*/
			
			
		}
	
	Private
		Function getErrFileId()
		{
			$this->_file = $this->_opts['logs_path'].$this->_opts['logs_name'];
			if (filesize($this->_file) >= $this->_opts['max_size']) {
				fclose(fopen($this->_file, 'w'));
				return (1);
			}
			else {
				$fd = @fopen($this->_file, 'r');
				if ($fd) {
					$maxId = 0;
					while (!feof($fd)) {
						$line = explode('*', trim(fgets($fd)));
						$maxId = (int)trim($line[0]);
					}
					fclose($fd);
					$maxId++;
					return ($maxId);
				}
				return (0);
			}
		}
		
		Function getErrType($eCode)
		{
			switch ($eCode) {
				case 1:
					return('E_ERROR');
				case 2:
					return('E_WARNING');
				case 4:
					return('E_PARSE');
				case 8:
					return('E_NOTICE');
				case 256:
					return('E_ERROR');
				case 512:
					return('E_WARNING');
				case 1024:
					return('E_NOTICE');
				case 2048:
					return('E_STRICT');
				case 8192:
					return('E_DEPRECATED');
			}
			retunr('UNKNOWN');
		}
}

?>