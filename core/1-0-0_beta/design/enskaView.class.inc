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

class EnsKa_View
{	
	Public	$_views;
	Private	$_vars;
	Private	$_title;
	
	Public
		Function __construct($title=NULL)
		{
			if ($title != NULL) {
				$this->setTitle($title);
			}
		}
		
		Function __destruct()
		{
			unset($this->_vars);
			unset($this->_title);
			unset($this->_views);
		}
		
		Function assign($name, $value)
		{
			$this->_vars[$name] = $value;
		}
		
		Function delete($name)
		{
			unset($this->_vars[$name]);
		}
	
		Function get($name)
		{
			return ($this->_vars[$name]);
		}
		
		Function addView($view)
		{
			$this->_views []= $view;
		}
		
		Function display($mod='usr')
		{
			GLOBAL $_alias;
			$point = $_alias['path']['global']['point'];
			
			if (strcmp($mod, 'usr') == 0) {
				$path = $_alias['path']['local']['path_apps'].$_alias['controller']."/views/";
			}
			elseif (strcmp($mod, 'sys') == 0) {
				$path = ENSKA_VIEWS;
			}
			
			foreach ($this->_views as $view) {
					$page = $path.$view.'.phtml';
					if (file_exists($point.$page)) {
						include_once($point.$page);
					}
				}
		}
		
		Function image($image, $infos=NULL, $showOut=true)
		{
			GLOBAL $_alias;
			
			$nbRoot = count(explode('/', $_SERVER['REQUEST_URI']));
			$root = '';
			while ($nbRoot-- > 0)  {
				$root .= '../';
			}
			
			if ($infos != NULL) {
				if (!is_array($infos)) {
					$infos = Array('type'=>'usr', 'width'=>0, 'height'=>0, 'alt'=>'', 'name'=>'');
				}
			}
			else {
				$infos = Array('type'=>'usr', 'width'=>0, 'height'=>0, 'alt'=>'', 'name'=>'');
			}
			
			if (!isset($infos['type'])) {
				$infos['type'] = 'usr';
			}
			
			if ($infos['type'] == 'usr') {
				if ($showOut) {
					echo '<img src="'.$root.$_alias['path']['global']['users_ressources'].$_alias['site'].'/'.$_alias['controller'].'/images/'.$image.'"';
				}
				else {
					$out = '<img src="'.$root.$_alias['path']['global']['users_ressources'].$_alias['site'].'/'.$_alias['controller'].'/images/'.$image.'"';
				}
				
			}
			elseif ($infos['type'] == 'commons') {
				if ($showOut) {
					echo '<img src="'.$root.$_alias['path']['global']['users_ressources'].$_alias['site'].'/_commons/images/'.$image.'"';
				}
				else {
					$out = '<img src="'.$root.$_alias['path']['global']['users_ressources'].$_alias['site'].'/_commons/images/'.$image.'"';
				}
				
			}
			else if ($type == 'sys') {
				if ($showOut) {
					echo '<img src="'.$root.$_alias['path']['global']['enska_ressources_img'].'/'.$image.'"';
				}
				else {
					$out = '<img src="'.$root.$_alias['path']['global']['enska_ressources_img'].'/'.$image.'"';
				}
			}
			else {
				return;
			}
			
			if (isset($infos['width'])) {
				if ($infos['width'] > 0) {
					if ($showOut) {
						echo ' width='.$infos['width'];
					}
					else {
						$out .= ' width='.$infos['width'];
					}
				}
			}
			
			if (isset($infos['height'])) {
				if ($infos['height'] > 0) {
					if ($showOut) {
						echo ' height='.$infos['height'];
					}
					else {
						$out .= ' height='.$infos['height'];
					}
				}
			}
			
			if (isset($infos['alt'])) {
				if (trim($infos['alt']) != '') {
					if ($showOut) {
						echo ' alt='.$infos['alt'];
					}
					else {
						$out .= ' alt='.$infos['alt'];
					}
				}
			}
			
			if (isset($infos['name'])) {
				if (trim($infos['name']) != '') {
					if ($showOut) {
						echo ' alt='.$infos['name'];
					}
					else {
						$out .= ' alt='.$infos['name'];
					}
				}
			}
			
			if (isset($infos['style'])) {
				if (trim($infos['style']) != '') {
					if ($showOut) {
						echo ' style='.$infos['style'];
					}
					else {
						$out .= ' style='.$infos['style'];
					}
				}
			}
			
			if ($showOut) {
				echo ' />';
			}
			else {
				return ($out. ' />');
			}
			
		}
		
		Function link($site, $link=false, $text=false, $opts=null, $showOut=true)
		{
			GLOBAL $_alias;
			$domain = $_alias['path']['global']['domain'];
			
			if ($text) {
				$res = '<a href="http://'.$site.'.'.$domain.'/'.$link.'" '.$opts.'>'.$text.'</a>';
			}
			else {
				$res = '<a href="'.$site.'" '.$opts.'>'.$link.'</a>';
			}
			
			if ($showOut) {
				echo $res;
			}
			else {
				return ($res);
			}
		}
		
		Function form($site, $link, $method, $enctype='')
		{
			GLOBAL $_alias;
			$domain = $_alias['path']['global']['domain'];
			
			$res = '<form action="http://'.$site.'.'.$domain.'/'.$link.'" method="'.$method.'" '.$enctype.'>';
			
			echo $res;
		}
		
		Function setTitle($title)
		{
			$this->_title = $title;
		}
		
		Function getTitle()
		{
			return ($this->_title);
		}
}

?>