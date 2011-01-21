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
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

imports ('enska.extTree');
imports ('enska.enska2ml');
imports ('trees');
imports ('plugins');

class IndexController extends EnsKa_Controller
{
	Private	$_user;
	
	Public
		Function __construct()
		{
			parent::__construct();
		}
		
		Function run()
		{
			GLOBAL $_alias;
			
			/**
			 * Check site/application rights
			 */
			$acl = Array
						(
							'type' 	=>	'uid',
							'id'	=>	EnsKa_Sessions::get()->getSession('uid')
						);
			
			/**
			 * Application check
			 */
			$acl['role'] = EnsKa_Alias::get()->getController();
			if (!EnsKa_ACL::get()->checkACL($acl, EnsKa_ACL::APPS)) {
				EnsKa_Sessions::get()->setSession('uid', 9);
				EnsKa_Sessions::get()->deleteSession('user');
				redirect('login?err=badLogin');
				exit;
			}
			
			/**
			 * Site check
			 */
			$acl['role'] = EnsKa_Alias::get()->getSite();
			if (!EnsKa_ACL::get()->checkACL($acl, EnsKa_ACL::SITE)) {
				EnsKa_Sessions::get()->setSession('uid', 9);
				EnsKa_Sessions::get()->deleteSession('user');
				redirect('login?err=badLogin');
				exit;
			}
			
			$this->_user = EnsKa_Sessions::get()->getSession('user');
			$this->view->assign('host', getHost());
			$this->view->assign('domain', $_alias['path']['global']['domain']);
			$this->view->assign('img', 'ressources/sites/manager/index/images/');
			$this->view->assign('logs_path', $_alias['path']['global']['path_logs']);
			
			$conf = EnsKa_Config::get()->getConfig();
			$this->view->assign('version', $conf['global']['version']['version']);
			$this->view->assign('revision', $conf['global']['version']['revision']);
			$this->view->assign('release', $conf['global']['version']['release']);
			
			/**
			 * Check language, TODO for next version of EnsKa Plateform
			 */
			/*$t = Array('newWebsite' => Array('minLenghtTextWebsiteAlias' => 'Application path must be at least 1 character long.'));
			$this->view->assign('lng', $t);*/
			
			$this->view->setTitle("EnsKa Core Manager (preview)");
			$this->view->addView('head');
			
			$this->view->addView('init-start');
				loadPlgs(&$this->view, 'mod');
				$this->view->addView('fnct-start');
					loadPlgs(&$this->view, 'win');
				$this->view->addView('fnct-end');
					loadPlgs(&$this->view, 'id');
			$this->view->addView('init-end');
			
			$this->view->addView('body-start');
				loadPlgs(&$this->view, 'panel');
				$this->view->addView('sht-start');
					loadPlgs(&$this->view, 'sht');
				$this->view->addView('sht-end');
			$this->view->addView('body-end');
			
			$this->view->display();
		}
		
		Function loadInterface()
		{
			$layout = $this->get['lt'];
			$loader = 'load'.$layout;
			
			if (isset($this->get['data'])) {
				$data = $this->get['data'];
			}
			else {
				$data = null;
			}
			echo $loader($data);
		}
		
		Function execPlg()
		{
			$params = $this->post['data'];
			$data = explode('::', $params);
			$fnct = $this->get['plg'];
			$plug = "$fnct";
			$method = $data[0];
			$fnct .= '_'.$method;
			imports ("plugs.$plug");
			
			try
			{
				if (class_exists($plug)) {
					$obj = new $plug();
					if (method_exists($obj, $data[0])) {
						if (EnsKa_ACL::get()->checkACL(Array('role' => $fnct, 'type' => 'uid', 'id' => EnsKa_Sessions::get()->getSession('uid')))) {
							$obj->$method($data);
						}
						else {
							echo 'YOU DO NOT HAVE ENOUGHT RIGHTS TO PERFORM THIS ACTION !';
						}
					}
					else {
						echo 'Unknonwn plug-in '.$plug.'::'.$method.' in class '.$plug;
					}
				}
				else {	
					if (function_exists($fnct)) {
						if (EnsKa_ACL::get()->checkACL(Array('role' => $fnct, 'type' => 'uid', 'id' => EnsKa_Sessions::get()->getSession('uid')))) {
							$fnct($data);
						}
						else {
							echo 'YOU DO NOT HAVE ENOUGHT RIGHTS TO PERFORM THIS ACTION !';
						}
					}
					else {
						echo 'Unknonwn plug-in '.$plug.'::'.$method;
					}
				}
			}
			catch(EnsKa_Exception $e)
			{
				echo 'EnsKa_Exception found !<br>';
				echo $e->getMessage();
			}
		}
}

?>