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
 * Redirect to an other script
 *
 * @param string $path The page/script address
 * @param int $time The tme before the redirection in micro-seconds
 * @param string $disp Add or not the script tag
 */
Function redirect($path, $time=1, $disp='php')
{
	if (strcmp($disp, 'php') != 0) {
		echo 'setTimeout("document.location='."'".$path."'".'", '.$time.');';
	}
	else {
		echo "<script language=JavaScript>setTimeout(\"document.location = '$path'\", $time)</script>";
	}
	return;
}

/**
 * Return the domain host
 *
 * @return string
 */
Function getHost()
{
	return ($_SERVER['HTTP_HOST']);
}

/**
 * Create an random string
 *
 * @param int $size The size of the randomed string
 * @return string
 */
Function create_id($size)
{
	$chaine = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	srand((double)microtime()*1000000);
	
	$id='';
	for($i=0; $i<$size; $i++) {
		$id .= $chaine[rand()%strlen($chaine)];
	}
	return($id);
}

/**
 * Get public properties for the class EnsKa_User
 *
 * @param int $mod=null
 * @return string
 */
Function getUserProperties($mod='null')
{
	$usr = new user(EnsKa_Sessions::get()->getSession('uid'));
	$res = Array();

	$reflect = new ReflectionClass($usr);
	$properties	= $reflect->getProperties(ReflectionProperty::IS_PUBLIC);
	
	foreach ($properties as $property) {
		$property_name = $property->getName();
		if ($mod === 'all') {
			$res []= $property_name;
		}
		else {
			if ((strcmp($property_name, 'uid') != 0) && (strcmp($property_name, 'login') != 0) && (strcmp($property_name, 'mail') != 0) && (strcmp($property_name, 'first') != 0) && (strcmp($property_name, 'last') != 0)) {
				$res []= $property_name;
			}
		}
	}
	return ($res);
}

/**
 * Add/Remove public properties for the class EnsKa_User
 *
 * @param $field The property
 * @param $mod The modifer: add or del
 * @return int
 */
Function userClassFields($field, $mod)
{
	GLOBAL $_alias;
	
	$tmp = $_alias['path']['global']['point'].ENSKA_ENGINE.md5(create_id(5)).'.tmp';
	
	$_line = __LINE__;
	$fd = fopen($tmp, 'w');
	if ($fd) {
		if (strcmp($mod, 'add') == 0) {
			fputs($fd, '<?php'."\n\n");
			fputs($fd, 'class user extends EnsKa_User'."\n");
			fputs($fd, '{'."\n\t");
			
			$properties = getUserProperties();
			foreach ($properties as $property) {
				fputs($fd, 'public'."\t".'$'.$property.';'."\n\t");
			}
			
			fputs($fd, 'public'."\t".'$'.strtolower($field).';'."\n");
			
			fputs($fd, "\n\t".'public'."\n\t\t");
			fputs($fd, 'Function __construct($uid)'."\n\t\t");
			fputs($fd, '{'."\n\t\t\t");
			fputs($fd, 'parent::__construct($uid);'."\n\t\t");
			fputs($fd, '}'."\n");
			fputs($fd, '}'."\n\n");
			fputs($fd, '?>'."\n");
		}
		elseif(strcmp($mod, 'del') == 0) {
			fputs($fd, '<?php'."\n\n");
			fputs($fd, 'class user extends EnsKa_User'."\n");
			fputs($fd, '{'."\n\t");
			
			$properties = getUserProperties();
			foreach ($properties as $property) {
				if (strcmp($property, $field) != 0) {
					fputs($fd, 'public'."\t".'$'.$property.';'."\n\t");
				}
			}
			
			fputs($fd, "\n\t".'public'."\n\t\t");
			fputs($fd, 'Function __construct($uid)'."\n\t\t");
			fputs($fd, '{'."\n\t\t\t");
			fputs($fd, 'parent::__construct($uid);'."\n\t\t");
			fputs($fd, '}'."\n");
			fputs($fd, '}'."\n\n");
			fputs($fd, '?>'."\n");
		}
		
		fclose ($fd);
	}
	else {
		trigger_error('Unable to create the TEMP user class file****0x2901****'.($_line + 1).'****'.__FILE__, E_USER_ERROR);
		return (-1);
	}
	
	$_line = __LINE__;
	if (!copy($tmp, $_alias['path']['global']['point'].ENSKA_ENGINE.'user.class.inc')) {
		trigger_error('Unable to update the user class file****0x2902****'.($_line + 1).'****'.__FILE__, E_USER_NOTICE);
		return (-1);
	}
	
	@unlink($tmp);
	return (0);
}

Function get_size($size, $unit=FALSE)
{
	$size = $size / 1024 / 1024;
	
	if ($size < 0.001)
	{
		while ($size < 1)
		{
			$size *= 1000;
		}
		$size = round($size);
		if ($unit == TRUE)
			$size = $size."Oc";
	}
	elseif ($size < 1)
	{
		while ($size < 1)
		{
			$size *= 1000;
		}
		$size = round($size);
		if ($unit == TRUE)
			$size = $size."Ko";
	}
	elseif (($size) > 999 && ($size < 1000000))
	{
		while ($size > 999)
		{
			$size /= 1000;
		}
		$size = round($size);
		if ($unit == TRUE)
			$size = $size."Go";
	}
	elseif ($size > 99999)
	{
		while ($size > 999)
		{
			$size /= 1000;
		}
		$size = round($size);
		if ($unit == TRUE)
			$size = $size."To";
	}
	else
	{
		$size = round($size);
		if ($unit == TRUE)
			$size = $size."Mo";
	}
	
	return ($size);
}

/**
 * Get the application path of a website
 *
 * @param $website
 * @return string
 */
Function get_app_path($website)
{
	GLOBAL $_alias;
	
	$_line = __LINE__;
	$conf = new EnsKa_INIReader($_alias['path']['global']['sites'].trim($website).'/conf/settings.ini');
	
	if ($conf->getError()) {
		echo $conf->getError().'<br>';
		trigger_error($conf->getError().'****0x2903****'.($_line + 1).'****'.__FILE__, E_USER_ERROR);
		return (false);
	}
	
	return ($conf->getLine('path_apps'));
}

?>