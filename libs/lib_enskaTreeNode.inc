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
 * EnsKa TreeNode
 *
 * EnsKa TreeNode is a container for a leaf or branch of a XML tree
 *
 */
class EnsKa_TreeNode
{
	/**
     * The node name
     *
     * @var string
     */
	Private	$_nodeName;
	
	/**
     * The node attributes
     *
     * @var array
     */
	Private	$_attrs;
	
	/**
     * The node children
     *
     * @var EnsKa_TreeNode[]
     */
	Private	$_nodes;
	
	Public
		/**
		 * Object constructor
		 *
		 * @param string $nodeName
		 */
		Function __construct($nodeName)
		{
			$this->_nodeName = $nodeName;
		}
		
		/**
		 * Get the current node name
		 *
		 * @return string
		 */
		Function getNodeName()
		{
			return ($this->_nodeName);
		}
		
		/**
		 * Get an attribute of current node
		 *
		 * @param string $key
		 */
		Function getAttribut($key)
		{
			return ($this->_attrs[$key]);
		}
		
		/**
		 * Set an attribute of current node
		 *
		 * @param string $key, string val
		 */
		Function setAttribut($key, $val)
		{
			$this->_attrs[$key];
		}
		
		/**
		 * Delete an attribute of current node
		 *
		 * @param string $key
		 */
		Function deleteAttribut($key)
		{
			unset($this->_attrs[$key]);
		}
		
		/**
		 * Add a EnsKa_TreeNode in the current node
		 *
		 * @param EnsKa_TreeNode $node
		 */
		Function addNode($node)
		{
			$nodeName = $node->getNodeName();
			$this->_nodes[$nodeName] = $node;
		}
		
		/**
		 * Remove a EnsKa_TreeNode in the current node
		 *
		 * @param string $nodeName
		 */
		Function deleteNode($nodeName)
		{
			unset($this->_nodes[$nodeName]);
		}
		
		/**
		 * Get a EnsKa_TreeNode in the current node
		 *
		 * @param string $nodeName
		 * @return EnsKa_TreeNode
		 */
		Function getNode($nodeName)
		{
			return ($this->_nodes[$nodeName]);
		}
}

?>