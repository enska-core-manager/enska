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

class EnsKa_Subject implements EnsKa_ISubject
{
	Protected			$_observers;
	
	Protected	static	$_pos;
	
	Public
		Function __construct($observer=NULL)
		{
			$this->_observers = Array();
			self::$_pos = -1;
			if ($observer != NULL) {
				$this->addObserver($observer, $observer->getObserverName());
			}
		}
		
		Function __destruct()
		{
			/*Destructor*/
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
					$observer = $this->_observers[$i]['observer'];
					$observer->getEvent($params);
				}
				$i++;
			}
		}
}

class EnsKa_Observer implements EnsKa_IObserver
{
	Protected	$_observerName;
	
	Protected
		Function __construct($name) 
		{
			if (trim($name) != '') {
				$this->setObserverName($name);
			}
		}
		
		Function __destruct()
		{
			/*Destrcutor*/
		}
		
		Function setObserverName($name)
		{
			$this->_observerName = $name;
		}
		
		Function getObserverName()
		{
			return ($this->_observerName);
		}
		
		Function getEvent($params)
		{
			$this->doEvent($params);
		}
}

class EnsKa_SwitchObserver implements EnsKa_IObserver, EnsKa_ISubject
{
	Protected	static	$_pos;
	Private				$_observers;
	
	Public
		Function __construct($observer=NULL)
		{
			$this->_observers = Array();
			self::$_pos = -1;
			
			if ($observer != NULL) {
				$this->addObserver($observer, $observer->getObserverName());
			}
		}
		
		Function __destruct()
		{
			/*Destructor*/
		}
		
		Function addObserver($observer, $name)
		{
			self::$_pos++;
			$this->_observers[self::$_pos]['name'] = $name;
			$this->_observers[self::$_pos]['observer'] = $observer;
		}
		
		Function deleteObserver($name)
		{
			$i = 0;
			$nb = count($this->_observers);
			
			while ($i < $nb) {
				$observer = $this->_observers [$i];
				
				if (strcmp($observer['name'], $name) == 0) {
					unset($this->_observers[$i]);
				}
				
				$i++;
			}
		}
		
		Function getEvent($params)
		{
			$this->sendEvent($params);
		}
		
		Function sendEvent($params)
		{
			$i = 0;
			$nb = count($this->_observers);
			while ($i < $nb) {
				if ($this->_observers [$i]['observer'] != NULL) {
					$observer = $this->_observers [$i]['observer'];
					$observer->getEvent($params);
				}
				$i++;
			}
		}
}

?>