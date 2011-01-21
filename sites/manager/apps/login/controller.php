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

class LoginController extends EnsKa_Controller
{
	Public
		Function __construct()
		{
			parent::__construct();
		}
		
		Function run()
		{
			$this->view->setTitle("EnsKa Core Manager | Login");
			if (EnsKa_Sessions::get()->getSession('lMCount') > 0) {
				if (isset($this->get['err'])) {
					if ($this->get['err'] == "badLogin") {
						$this->view->assign('error', "Bad login!");
					}
					else {
						$this->view->assign('error', "Error on login!");
					}
				}
			}
			
			$this->view->addView('hLogin');
			$this->view->addView('login');
			
			$this->view->display();
		}
		
		Function signin()
		{
			$params = Array
						(
							'usrLogin'		=>		$this->post['usrLogin'], 
							'usrPasswd'		=>		$this->post['usrPasswd']
						);
					
			$uid = EnsKa_Users::get()->logIn($params);
			
			if ($uid == BAD_LOGIN)
			{
				redirect('login?err=badLogin');
				exit;
			}
			
			EnsKa_Sessions::get()->setSession('lMCount', 1);	
			redirect('index');
		}
		
		Function logout()
		{
			GLOBAL $_alias;
			
			setcookie('uid', 9, (time() + 20), '/', '.'.$_alias['path']['global']['domain']);
			EnsKa_Sessions::get()->destroySession();
			redirect('../index');
		}
}

?>