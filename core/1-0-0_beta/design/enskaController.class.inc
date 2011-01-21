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
 * @package    core
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

 class EnsKa_Controller
{	
	Protected	$request;
	Protected	$get;
	Protected	$post;
	Protected	$files;
	Protected	$body;
	Protected	$uri;
	Protected	$link;
	Protected	$view;
	Protected	$seo;
	
	Public
		Function __construct()
		{
			$this->get = $_GET;
			$this->post = $_POST;
			$this->files = $_FILES;
			$this->uri = $_SERVER['REQUEST_URI'];
			$this->request = $_REQUEST;
			
			foreach ($this->get as $key => $value) {
				if ($value == "") {
					$this->body = $key;
				}
			}
			
			$tmp = explode ('/', $this->uri);
			$nb = count($tmp);
			$i = 0;
			
			while ($i < $nb) {
				$res[$i] = $tmp[$i];
				$i++;
			}
			
			$i = 0;
			foreach($res as $ctrl) {
				if (trim($ctrl) != '') {
					$this->link[$i] = $ctrl;
					$i++;
				}
			}
			
			$nb = count($this->link);
			$i = 2;
			while ($i < $nb)
			{
				if ($i + 1 < $nb) {
					$this->get[$this->link[$i]] = $this->link[$i + 1];
				}
				else {
					$this->seo = $this->link[$i];
					$this->get[] = $this->link[$i];
				}
				$i += 2;
			}
			
			$this->view = new EnsKa_View();
		}
		
		Function getAct()
		{
			if (isset($this->link[1])) {
				if (trim($this->link[1] != '')) {
					$this->get['act'] = $this->link[1];
					$res = $this->link[1];
				}
			}
			else {
				if (isset($this->get['act'])) {
					if (trim($this->get['act'] != '')) {
						$res = $this->get['act'];
					}
					else {
						$res = 'run';
					}
				}
				else {
					$res = 'run';
				}
			}
			
			return ($res);
		}
}

?>