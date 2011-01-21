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

class EnsKa_Alias
{	
	Private	static	$_instance;
	
	public
		static Function get()
		{
			if(self::$_instance == null) {
				self::$_instance = new self();
			}
			return self::$_instance;
		}
		
		Function getSite()
		{	
			$tmp = explode('/', $_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
			$i = 0;
			$nb = count($tmp) - 1;
			$address = '';
			
			while ($i < $nb) {
				if (($i + 1) < $nb) {
					$address .= $tmp[$i].'/';
				}
				else {	
					$address .= $tmp[$i];
				}
				$i++;
			}
			
			$req = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'alias.ini');
			
			try
			{
				if ($req->getError()) {
					throw new EnsKa_Exception('Unable to get site', '0x1201', E_ERROR);
				}
			}
			catch(EnsKa_Exception $e)
			{
				return (-1);
			}
			
			$site = $req->getLine($address);
			
			if ($site) {
				return($site);
			}
			else {
				return('www');
			}
		}
		
		Function getController()
		{
			$id = explode('/', substr($_SERVER['SCRIPT_NAME'], 1, strlen($_SERVER['SCRIPT_NAME']) - 1));
			$address = explode('/', $_SERVER['REQUEST_URI']);
			$len = count($id);
			
			if (!empty($address[$len])) {
				$ctrl = explode('?', $address[$len]);
				return($ctrl[0]);
			}
			else {
				return('index');
			}
		}
		
		Function addAlias($site, $alias)
		{
			$handle = new EnsKa_INIReader(ENSKA_GLOBAL_CONF.'alias.ini');
		
			try
			{
				if ($handle->getError()) {
					throw new EnsKa_Exception('Unable to add the alias '.$alias, '0x1202', E_ERROR);
				}
			}
			catch(EnsKa_Exception $e)
			{
				return (false);
			}
			
			$handle->addLine($alias, $site, true);
			return (true);
		}
		
		Function deleteAlias($site)
		{
			GLOBAL $_alias;
			$conf = $_alias['path']['global']['point'].ENSKA_GLOBAL_CONF;
			
			try
			{
				$fd = fopen($conf.'alias.ini', 'r');
				if (!$fd) {
					throw new EnsKa_Exception('Unable to open the alias configuration file: '.$conf.'alias.ini', '0x1203', E_ERROR);
				}
				
				$res = '';
				while (!(feof($fd))) {
					$line = fgets($fd);
					if (($line != "\n") && (substr($line, 0, 1) != '#')) {
						unset($tmp);
						$tmp = explode('=', $line);
						if (isset($tmp[1])) {
							if (strcmp(trim($tmp[1]), trim($site)) != 0) {
								$res .= $line."\n";
							}
						}
					}
				}
				fclose($fd);
				
				$new = md5(create_id(5));
				$old = md5($new);
				
				$fd = @fopen($conf.$new, 'a');
				if (!$fd) {
					throw new EnsKa_Exception('Unable to open the alias TEMP configuration file: '.$conf.$new, '0x1204', E_ERROR);
				}
				
				if (!fputs($fd, $res)) {
					fclose($fd);
					throw new EnsKa_Exception('Unable to write in the alias TEMP configuration file', '0x1205', E_ERROR);
				}
				fclose($fd);
				
				if (@copy($conf.'alias.ini', $conf.$old)) {
					if (@copy($conf.$new, $conf.'alias.ini')) {
						@unlink($conf.$new);
						@unlink($conf.$old);
						return (true);
					}
					else {
						@unlink($conf.$new);
						@copy($conf.$old, $conf.'alias.ini');
						@unlink($conf.$old);
						throw new EnsKa_Exception('Unable to create the alias configuration file', '0x1206', E_ERROR);
					}
				}
				else {
					@unlink($conf.$new);
					throw new EnsKa_Exception('Unable to create the alias configuration file', '0x1206', E_ERROR);
				}
				return(true);
			}
			catch(EnsKa_Exception $e)
			{
				return (false);
			}
			return (true);
		}
		
		Function getAlias($site)
		{
			GLOBAL $_alias;
			$conf = $_alias['path']['global']['point'].ENSKA_GLOBAL_CONF;
			
			try
			{
				$fd = fopen($conf.'alias.ini', 'r');
				if (!$fd) {
					throw new EnsKa_Exception('Unable to open the alias configuration file: '.$conf.'alias.ini', '0x1207', E_ERROR);
				}
				
				$res = 'none';
				while (!(feof($fd))) {
					$line = fgets($fd);
					if (($line != "\n") && (substr($line, 0, 1) != '#')) {
						unset($tmp);
						$tmp = explode('=', $line);
						if (strcmp(trim($tmp[1]), trim($site)) == 0) {
							$res = $tmp[0];
							break;
						}
					}
				}
				fclose($fd);
				return ($res);
			}
			catch(EnsKa_Exception $e)
			{
				return (false);
			}
		}
		
	private
		Function __construct(){}
		Function __desctruct(){}
}

?>