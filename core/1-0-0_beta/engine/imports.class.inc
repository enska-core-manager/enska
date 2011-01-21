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

Function imports_css($css, $browser="FF")
{
	GLOBAL $_alias;
	
	$path = Enska_path::get()->getPath();
	$path_global = $path['global'];
	
	$msg = 'Impossible d\'importer la librairie CSS "'.$css.'"';
	$tab_css = explode('.', $css);
	$nb = count($tab_css);
	
	$nbRoot = count(explode('/', $_SERVER['REQUEST_URI']));
	$root = '';
	while ($nbRoot-- > 0)  {
		$root .= '../';
	}
	
  	if ($tab_css[0] == 'enska') {
		$path = $path_global['enska_ressources_css'];
		$i = 1;
		while ($i < $nb) {
			if (($i + 1) == $nb) {
				$path .= $tab_css[$i].'.css';
			}
			else {
				$path .= $tab_css[$i].'/';
			}
			$i++;
		}
		
		try
		{
			if (!file_exists($path)) {
				throw new EnsKa_Exception($msg, '0x1001', 'E_WARNING');
			}
			
			if ($browser == 'IE') {
				echo '<!--[if IE]>';
			}
			
			echo '<link rel="stylesheet" type="text/css" href="'.$root.$path.'" />';
			
			if ($browser == 'IE') {
				echo '<![endif]-->';
			}
			
		}
		catch(EnsKa_Exception $e)
		{
			return (false);
		}
	}
	elseif ($tab_css[0] == '_commons') {
		$path = $path_global['users_ressources'].$_alias['site'].'/_commons/css/';
		$i = 1;
		while ($i < $nb) {
			if (($i + 1) == $nb) {
				$path .= $tab_css[$i].'.css';
			}
			else {
				$path .= $tab_css[$i].'/';
			}
			$i++;
		}
		
		try
		{
			if (!file_exists($path)) {
				throw new EnsKa_Exception($msg, '0x1001', 'E_WARNING');
			}
			
			if ($browser == 'IE') {
				echo '<!--[if IE]>';
			}
			
			echo '<link rel="stylesheet" type="text/css" href="'.$root.$path.'" />';
			
			if ($browser == 'IE') {
				echo '<![endif]-->';
			}
			
		}
		catch(EnsKa_Exception $e)
		{
			return (false);
		}
	}
	else {
		if ($nb == 2) {
			$path = $path_global['users_ressources'].$tab_css[0].'/css/'.$tab_css[1].'.css';
		}
		elseif ($nb == 1) {
			$path = $path_global['users_ressources'].$_alias['site'].'/'.$_alias['controller'].'/css/'.$tab_css[0].'.css';
		}
		else {
			return (false);
		}
		try
		{	
			if (!file_exists($path)) {
				throw new EnsKa_Exception($msg, '0x1002', 'E_WARNING');
			}
			
			if ($browser == 'IE') {
				echo '<!--[if IE]>';
			}
			
			echo '<link rel="stylesheet" type="text/css" href="'.$root.$path.'" />';
			
			if ($browser == 'IE') {
				echo '<![endif]-->';
			}
		}
		catch(EnsKa_Exception $e)
		{
			return (false);
		}
	}
	return (0);
}

Function imports_js($js)
{	
	GLOBAL $_alias;
	
	$path = Enska_path::get()->getPath();
	$path_global = $path['global'];
	
	$msg = 'Impossible d\'importer la librairie JavaScript "'.$js.'"';
	$tab_js = explode('.', $js);
	$nb = count($tab_js);
	
	$nbRoot = count(explode('/', $_SERVER['REQUEST_URI']));
	$root = '';
	while ($nbRoot-- > 0)  {
		$root .= '../';
	}
	
  	if ($tab_js[0] == 'enska') {
		$path = $path_global['enska_ressources_js'];
		$i = 1;
		while ($i < $nb) {
			if ($tab_js[$i] != '*') {
				if (($i + 1) == $nb) {
					$path .= $tab_js[$i].'.js';
					try
					{
						if (!file_exists($path)) {
							throw new EnsKa_Exception($msg, '0x1003', 'E_WARNING');
						}
						echo '<script src="'.$root.$path.'" type="text/javascript"></script>';	
					}
					catch(EnsKa_Exception $e)
					{
						return (false);
					}
				}
				else {
					$path .= $tab_js[$i].'/';
				}
			}
			else {
				$files = get_file_list($path);
				try
				{
					if (is_array($files)) {
						$nb = count($files);
						$i = 0;
						while ($i < $nb) {
							$file = $path.$files[$i];
							echo '<script src="'.$root.$file.'" type="text/javascript"></script>';
							$i++;
						}
					}
					else {
						throw new EnsKa_Exception($msg, '0x1004', 'E_WARNING');
					}
				}
				catch(EnsKa_Exception $e)
				{
					return (false);
				}
			}
			$i++;
		}
	}
	elseif ($tab_js[0] == '_commons') {
		$path = $path_local['users_ressources'].$_alias['site'].'/_commons/js/';
		$i = 1;
		while ($i < $nb) {
			if ($tab_js[$i] != '*') {
				if (($i + 1) == $nb) {
					$path .= $tab_js[$i].'.js';
					try
					{
						if (!file_exists($path)) {
							throw new EnsKa_Exception($msg, '0x1003', 'E_WARNING');
						}
						echo '<script src="'.$root.$path.'" type="text/javascript"></script>';	
					}
					catch(EnsKa_Exception $e)
					{
						return (false);
					}
				}
				else {
					$path .= $tab_js[$i].'/';
				}
			}
			else {
				$files = get_file_list($path);
				try
				{
					if (is_array($files)) {
						$nb = count($files);
						$i = 0;
						while ($i < $nb) {
							$file = $path.$files[$i];
							echo '<script src="'.$root.$file.'" type="text/javascript"></script>';
							$i++;
						}
					}
					else {
						throw new EnsKa_Exception($msg, '0x1004', 'E_WARNING');
					}
				}
				catch(EnsKa_Exception $e)
				{
					return (false);
				}
			}
			$i++;
		}
	}
	else
	{
		if ($nb == 1) {
			$path = $path_local['users_ressources'].$_alias['site'].'/'.$_alias['controller'].'/js/'.$js.'.js';
			try
			{
				if (file_exists($path)) {
					echo '<script src="'.$root.$path.'" type="text/javascript" />';
				}
				else {
					throw new EnsKa_Exception($msg, '0x1005', 'E_WARNING');
				}
			}
			catch(EnsKa_Exception $e)
			{
				return (false);
			}
		}
		elseif ($nb >= 2) {
			$path = $path_local['users_ressources'].$_alias['site'].'/'.$_alias['controller'].'/js/'.$tab_js[0].'/';
			$i = 1;
			while ($i < $nb) {
				if ($tab_js[$i] != '*') {
					if (($i + 1) == $nb) {
						$path .= $tab_js[$i].'.js';
						try
						{
							if (!file_exists($path)) {
								throw new EnsKa_Exception($msg, '0x1006', 'E_WARNING');
							}
							echo '<script src="'.$root.$path.'" type="text/javascript" />';	
						}
						catch(EnsKa_Exception $e)
						{
							return (false);
						}
					}
					else {
						$path .= $tab_js[$i].'/';
					}
				}
				else {
					$files = get_file_list($path);
					try
					{
						if (is_array($files)) {
							$nb = count($files);
							$i = 0;
							while ($i < $nb) {
								$file = $path.$files[$i];
								echo '<script src="'.$root.$file.'" type="text/javascript" />';
								$i++;
							}
						}
						else {
							throw new EnsKa_Exception($msg, '0x1007', 'E_WARNING');
						}
					}
					catch(EnsKa_Exception $e)
					{
						return (false);
					}
				}
				$i++;
			}
		}
	}
	return (0);
}

Function imports($app, $type=NULL)
{
	GLOBAL $_alias;
	
	$path = Enska_path::get()->getPath();
	$path_global = $path['global'];
	$path_local = $path['local'];
	
	$msg = 'Impossible d\'importer la librairie PHP "'.$app.'"';
	$tab_app = explode('.', $app);
	$nb = count($tab_app);
	
	if ($tab_app[0] == 'enska') {
		$path = $path_global['point'].$path_global['libs'];
		$i = 1;
		while ($i < $nb) {
			if ($tab_app[$i] != '*') {
				if (($i + 1) == $nb) {
					$tmp = $path.$tab_app[$i].'.php';
					try
					{
						if (!file_exists($tmp)) {
							$tmp = $path.'lib_'.$tab_app[$i].'.inc';
							if (!file_exists($tmp)) {
								$tmp = $path.'lib_'.$tab_app[$i].'.class.php'; 
								if (!file_exists($tmp)) {
									$tmp = $path.'lib_'.$tab_app[$i].'.class.inc';	
									if (!file_exists($tmp)) {
										throw new EnsKa_Exception($msg, '0x1010', 'E_ERROR');
									}
								}
							}
						}
					}
					catch(EnsKa_Exception $e)
					{
						return (false);
					}
					include_once($tmp);
					return (true);
				}
				else {
					$path .= $tab_app[$i].'/';
				}
			}
			$i++;
		}
		return (true);
	}
	elseif ($tab_app[0] == '_commons') {
		$path = $path_global['point'].$path_local['path_apps'].'_commons/';
		$i = 1;
		while ($i < $nb) {
			if ($tab_app[$i] != '*') {
				if (($i + 1) == $nb) {
					$tmp = $path.$tab_app[$i].'.php';
					try
					{
						if (!file_exists($tmp)) {
							$tmp = $path.$tab_app[$i].'.inc';
							if (!file_exists($tmp)) {
								$tmp = $path.$tab_app[$i].'.class.php'; 
								if (!file_exists($tmp)) {
									$tmp = $path.$tab_app[$i].'.class.inc';	
									if (!file_exists($tmp)) {
										throw new EnsKa_Exception($msg, '0x1010', 'E_ERROR');
									}
								}
							}
						}
					}
					catch(EnsKa_Exception $e)
					{
						return (false);
					}
					
					include_once($tmp);
					return (true);
				}
				else {
					$path .= $tab_app[$i].'/';
				}
			}
			else {
				$files = get_file_list($path);
				try
				{
					if (is_array($files)) {
						$nb = count($files);
						$j = 0;
						while ($j < $nb) {
							$file = $path.$files[$j];
							include_once($file);
							$j++;
						}
					}
					else {
						throw new EnsKa_Exception($msg, '0x1007', 'E_WARNING');
					}
				}
				catch(EnsKa_Exception $e)
				{
					return (false);
				}
				return (true);
			}
			$i++;
		}
		return (true);
	}
	else
	{
		if ($nb == 1) {
			$path = $path_global['point'].$path_local['path_apps'].$_alias['controller'].'/models/'.$app.'.php';
			try
			{
				if (!file_exists($path)) {
					$path = $path_global['point'].$path_local['path_apps'].$_alias['controller'].'/models/'.$app.'.inc';
					if (!file_exists($path)) {
						$path = $path_global['point'].$path_local['path_apps'].$_alias['controller'].'/models/'.$app.'.class.php';
						if (!file_exists($path)) {
							$path = $path_global['point'].$path_local['path_apps'].$_alias['controller'].'/models/'.$app.'.class.inc';
							if (!file_exists($path)) {
								throw new EnsKa_Exception($msg, '0x1009', 'E_ERROR');
							}
						}
					}
				}
			}
			catch(EnsKa_Exception $e)
			{
				return (false);
			}
			include_once ($path);
			return(0);
		}
		elseif ($nb >= 2) {
			$path = $path_global['point'].$path_local['path_apps'].$_alias['controller'].'/models/'.$tab_app[0].'/';
			$i = 1;
			while ($i < $nb) {
				if ($tab_app[$i] != '*') {
					if (($i + 1) == $nb) {
						$tmp = $path.$tab_app[$i].'.php';
						try
						{
							if (!file_exists($tmp)) {
								$tmp = $path.$tab_app[$i].'.inc';
								if (!file_exists($tmp)) {
									$tmp = $path.$tab_app[$i].'.class.php'; 
									if (!file_exists($tmp)) {
										$tmp = $path.$tab_app[$i].'.class.inc';	
										if (!file_exists($tmp)) {
											throw new EnsKa_Exception($msg, '0x1010', 'E_ERROR');
										}
									}
								}
							}
						}
						catch(EnsKa_Exception $e)
						{
							return (false);
						}
						
						include_once($tmp);
						return (true);
					}
					else {
						$path .= $tab_app[$i].'/';
					}
				}
				else {
					$files = get_file_list($path);
					try
					{
						if (is_array($files)) {
							$nb = count($files);
							$j = 0;
							while ($j < $nb) {
								$file = $path.$files[$j];
								include_once($file);
								$j++;
							}
						}
						else {
							throw new EnsKa_Exception($msg, '0x1007', 'E_WARNING');
						}
					}
					catch(EnsKa_Exception $e)
					{
						return (false);
					}
					return (true);
				}
				$i++;
			}
		}
	}
}

?>