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
 * EnsKa XMLReader
 *
 * EnsKa XMLReader translate a XML file to an parsed stdClass object tree
 *
 */
class EnsKa_XMLReader
{
	/**
     * File top open
     *
     * @var string
     */
	Private	$_file;
	
	/**
     * Main object tree
     *
     * @var stdClass
     */
	Private $_object;
	
	/**
     * The DOM document instance
     *
     * @var DomDocument
     */
	Private	$_dom;
	
	/**
     * The root object node
     *
     * @var documentElement
     */
	Private	$_root;
	
	/**
     * Error trace
     *
     * @var string
     */
	Private	$_err;
	
	Public
		/**
		 * Object constructor
		 *
		 * @param string $file
		 */
		Function __construct($file)
		{
			$this->_file = $file;
			$file = POINT.$this->_file;
			
			if (!file_exists($file)) {
				$file = $this->_file;
				
				if (!file_exists($file)) {
					$this->setError(NO_EXIST);
					return;
				}
			}
			
			/*
			 * Init variables
			 */
			$this->_file = $file;
			$this->_object = new stdClass();
			$this->_dom = new DomDocument();
			$this->open();
			
			/*
			 * If any error detected, lunch the parser
			 */
			if (!$this->getError()) {
				$this->parse($this->_root, $this->_object->root);
			}
			else {
				trigger_error($this->getError().'****0x2600', E_USER_WARNING);
			}
		}
		
		/**
		 * Get the final tree object
		 *
		 * @return stdClass
		 */
		Function getTree()
		{
			return ($this->_object);
		}
		
		/**
		 * Get error trace
		 *
		 * @return string
		 */
		Function getError()
		{
			if ($this->_err != '') {
				return ($this->_err);
			}
			return (false);
		}
		
	Private
		/**
		 * Open and load the XML file
		 *
		 */
		Function open()
		{
			if (!$this->_dom->load($this->_file)) {
				$this->setError(NO_OPEN);
				return;
			}
			$this->setRoot();
		}
		
		/**
		 * Parse the XML file
		 * (recursive function)
		 *
		 * @param DomDocument $domElement
		 * @param stdClass $objectElement
		 */
		Function parse($domElement, $objectElement)
		{
			/*
			 * Get the element name
			 */
			$objectElement->name = $domElement->nodeName;
			
			/*
			 * Get CDATA value with a good format
			 */
			@$objectElement->textValue = trim($domElement->firstChild->nodeValue);
			
			/*
			 * Getting attributes
			 */
			if ($domElement->hasAttributes()) {
				$objectElement->attributes = array();
				foreach($domElement->attributes as $attName=>$dom_attribute) {
					$objectElement->attributes[$attName] = $dom_attribute->value;
				}
			}
			
			/*
			 * Getting the son element then course XML tree
			 * The first son is always the text node, so length > 1
			 */
			if ($domElement->childNodes->length > 1) {
				$objectElement->children = array();
				foreach($domElement->childNodes as $dom_child) {
					if ($dom_child->nodeType == XML_ELEMENT_NODE) {
						$childObject = new stdClass();
						$this->parse($dom_child, $childObject);
						array_push($objectElement->children, $childObject);
					}
				}
			}
		}
		
		/**
		 * Set the tree object root
		 *
		 */
		Function setRoot()
		{
			$this->_root = $this->_dom->documentElement;
			$this->_object->root = new stdClass();
		}
		
		/**
		 * Set the error trace
		 *
		 * @param string $err
		 */
		Function setError($err)
		{
			$this->_err = $err;
		}
}

?>