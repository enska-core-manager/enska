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

DEFINE (NO_SID, 0);
DEFINE (SQL_ERROR, -2);
DEFINE (NO_RESULT, -1);

Function is_authorized_fnct($fnct){

	$fncts = Array(
				%functions%
			);
	$i = 0;
	$nb = count($fncts);
	while ($i < $nb){
		if (strcmp($fnct, $fncts[$i]) == 0)
			return (true);
	}
	return (false);
}

Function get_prefix(){

	GLOBAL $_PREFIX;
	return ($_PREFIX);
}

?>