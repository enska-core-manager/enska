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

/**
 * General EnsKa start includes
 */
include_once(ENSKA_DESIGN.'enskaDataStore.class.inc');
include_once(ENSKA_ENGINE.'ini.class.inc');
include_once (ENSKA_CORE.'core.env.inc');

/**
 * Get current site and controller
 */
$_alias['site'] = EnsKa_Alias::get()->getSite();
$_alias['controller'] = EnsKa_Alias::get()->getController();

try {

	/**
	 * load the main controller
	 */
	EnsKa_MainController::load();
	EnsKa_Path::get()->addGlobalPath('current_path', EnsKa_Path::get()->getCurrentPath(), false);
	EnsKa_Path::get()->addGlobalPath('frmPath', INCLUDE_PATH . FRM_PATH, false);
	
	/**
	 * Start the core
	 */
	EnsKa_Engine::start();
	$_alias['path'] = EnsKa_Path::get()->getPath();
	$_alias['env'] = $_alias['path']['global']['env'];
	$config = EnsKa_Config::get()->getConfig();
	
	if (strcmp(trim('login.'.$_alias['path']['global']['domain']), trim($_SERVER['SERVER_NAME'])) != 0) {
		setcookie('url', $_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'], (time() + (int)$config['global']['auth']['auth_cookie_time']), '/', '.'.$_alias['path']['global']['domain']);
	}
	else {
		if (!isset($_COOKIE['url'])) {
			setcookie('url', $config['global']['auth']['auth_default_redirect'], (time() + (int)$config['global']['auth']['auth_cookie_time']), '/', '.'.$_alias['path']['global']['domain']);
		}
	}
	
	/**
	 * Check site/application exists
	 */
	if (!file_exists(POINT.$_alias['path']['global']['current_path'].'controller.php')) {
			$view = new EnsKa_View();
			$view->addView('404');
			$view->display('sys');
			exit;
	}
	
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
		if (strcmp(EnsKa_Alias::get()->getSite(), 'manager') != 0) {
			redirect('http://login.'.$_alias['path']['global']['domain'].'/');
			exit;
		}
	}
	
	/**
	 * Site check
	 */
	$acl['role'] = EnsKa_Alias::get()->getSite();
	if (!EnsKa_ACL::get()->checkACL($acl, EnsKa_ACL::SITE)) {
		if (strcmp(EnsKa_Alias::get()->getSite(), 'manager') != 0) {
			redirect('http://login.'.$_alias['path']['global']['domain'].'/');
			exit;
		}
	}
	
	/**
	 * Load cache
	 */
	if (EnsKa_Cache::get()->isEnable()) {
		if (EnsKa_Cache::get()->checkCache()) {
			EnsKa_Cache::get()->release();
			exit;
		}
		else {
			EnsKa_Cache::get()->capture();
		}
	}
	/**
	 * Load the site framework
	 */
	$frm = new EnsKa_FrameworkLoader($_alias['site']);
	if ($frm->getFrmId() == 1) {
		@include_once($_alias['path']['global']['current_path'].'controller.php');
		
		/**
		 * Load the application
		 * Only for EnsKa Core Framework
		 */
		$ctrl = ucfirst($_alias['controller']).'Controller';
		$controller = new $ctrl();
		$act = $controller->getAct();
		if (method_exists($controller, $act)) {
			$controller->$act();
		}
		else {
			$view = new EnsKa_View();
			$view->addView('404');
			$view->display('sys');
			exit;
		}
	}
	else {
	
		/**
		 * Load the application
		 */	
		$_alias['path']['global']['frmPath'] = $_alias['path']['global']['frmPath'].$frm->getFrmName().'/';
		set_include_path($_alias['path']['global']['frmPath']. PATH_SEPARATOR . get_include_path());
		$frm->run();
	}
	
	if (EnsKa_Cache::get()->isEnable()) {
		EnsKa_Cache::get()->show();
	}
}
/**
 * Catch the site/application error
 */
catch(Exception $e) {
	if (EnsKa_Cache::get()->isEnable()) {
		EnsKa_Cache::get()->show();
	}
	echo $e->getMessage();
	echo '<br>ERR FRONT - <b>PHP Exception</b>';
}
catch(EnsKa_Exception $e) {
	if (EnsKa_Cache::get()->isEnable()) {
		EnsKa_Cache::get()->show();
	}
	echo $e->getMessage();
	echo '<br>ERR FRONT - <b>EnsKa Exception</b>';
}

?>