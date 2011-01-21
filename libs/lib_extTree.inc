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
 * EnsKa EXTTreeNodes
 *
 * EnsKa EXTTreeNodes makes tree in PHP for ExtJs Tree panel control
 *
 */
class EnsKa_EXTTreeNodes
{
	/**
     * The node container
     *
     * @var array
     */
	Private	$_nodes;
	
	public
		/*
		 * Object constructor
		 *
		 */
		Function  __construct()
		{
			$this->_nodes = Array();
		}
		
		/**
		 * Create a new branch/leaf
		 *
		 * @param array $params
		 */
		Function add($params=null)
		{
			if ($params != null) {
				$node = new EnsKa_EXTTreeNode($params);
				$this->addNode($node);
			}
		}
     
		/**
		 * Add a node in current tree
		 *
		 * @param EnsKa_EXTTreeNode $node
		 */
		Function addNode($node)
		{
			$this->_nodes[] = $node;
		}
	 
		/**
		 * Translate the current in jSon format
		 *
		 */
		Function toJson()
		{
			return json_encode($this->_nodes);
		}		
}

/**
 * EnsKa EXTTreeNode
 *
 * EnsKa EXTTreeNode makes leaf and branch tree in PHP for ExtJs Tree panel control
 *
 */
class EnsKa_EXTTreeNode
{
	/**
     * The node id
     *
     * @var mixed
     */
	public $id = "";
	
	/**
     * The node text
     *
     * @var string
     */
	public $text = "";
	
	/**
     * The node icon
     *
     * @var string
     */
    public $iconCls = "";
	
	/**
     * Is leaf
     *
     * @var bool
     */
    public $leaf = false;
	
	/**
     *Is draggable
     *
     * @var bool
     */
    public $draggable = false;
	
	/**
     * The node link
     *
     * @var string
     */
    public $href = "";
	
	/**
     * The node target link
     *
     * @var string
     */
    public $hrefTarget = "";
	
	public
		/*
		 * Object construtor
		 *
		 * @param array $params
		 */
		Function  __construct($params)
		{
			foreach($params as $key => $value) {
				$this->$key = $value;
			}
		}
		
		/**
		 * Setting the node id
		 *
		 * @param mixed $id
		 */
		Function setId($id)
		{
			$this->id = $id;
		}
		
		/**
		 * Setting the node text
		 *
		 * @param string $text
		 */
		Function setText($text)
		{
			$this->text = $text;
		}
		
		/**
		 * Setting the node icon
		 *
		 * @param string $icionCls
		 */
		Function setIconCls($iconCls)
		{
			$this->iconCls = $iconCls;
		}
		
		/**
		 * Setting if the node is a leaf
		 *
		 * @param bool $leaf
		 */
		Function setLeaf($leaf)
		{
			$this->leaf = $leaf;
		}
		
		/**
		 * Setting if the node is draggable
		 *
		 * @param bool $draggable
		 */
		Function setDraggable($draggable)
		{
			$this->draggable = $draggable;
		}
		
		/**
		 * Setting the node link
		 *
		 * @param string $href
		 */
		Function setHref($href)
		{
			$this->href = $href;
		}
		
		/**
		 * Setting the node target link
		 *
		 * @param string $hrefTarget
		 */
		Function setHrefTarget($hrefTarget)
		{
			$this->hrefTarget = $hrefTarget;
		}
}

?>