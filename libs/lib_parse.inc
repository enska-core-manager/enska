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
 * Format a string in PHP code source
 *
 * @param string $str
 * @param bool $color
 * @return string
 */
Function check_php_code($str, $color=FALSE)
{
	
	$i = 0;
	$str_len = strlen($str);
	$res = "";
	$flag = TRUE;
	
	while ($i < $str_len) {
		if ($flag == TRUE) {
			if (substr($str, $i, 5) == '<php>') {
				$i += 4;
				$flag = FALSE;
				$res .= "<div id=\"bcode\">";
			}
			else
				$res .= $str[$i];
		}
		elseif ($flag == FALSE) {
			if (substr($str, $i, 6) == '</php>') {
				$i += 6;
				$flag = TRUE;
				$res .= "</div>";
			}
			else {
				if ($color == TRUE) {
					if (substr($str, $i, 5) == '<?php') {
						$res .= "<font color=red>".htmlentities('<?php')."<br></font>";
						$i += 5;
					}
					if (substr($str, $i, 2) == '?>') {
						$res .= "<font color=red>?></font>";
						$i += 2;
					}
					if (substr($str, $i, 6) == 'class ') {
						$res .= "<font color=purple>class </font>";
						$i += 5;
					}
					if (substr($str, $i, 8) == 'function') {
						$res .= "<font color=purple>function</font>";
						$i += 8;
					}
					if (substr($str, $i, 8) == 'Function') {
						$res .= "<font color=purple>Function</font>";
						$i += 7;
					}
					elseif (substr($str, $i, 6) == 'public') {
						$res .= "<font color=red>public</font>";
						$i += 5;
					}
					elseif (substr($str, $i, 7) == 'private') {
						$res .= "<font color=red>private</font>";
						$i += 6;
					}
					elseif (substr($str, $i, 9) == 'protected') {
						$res .= "<font color=red>protected</font>";
						$i += 8;
					}
					elseif (substr($str, $i, 4) == 'trim') {
						$res .= "<font color=green>trim</font>";
						$i += 3;
					}
					elseif (substr($str, $i, 4) == 'strlen') {
						$res .= "<font color=green>trim</font>";
						$i += 3;
					}
					elseif (substr($str, $i, 2) == 'if') {
						$res .= "<font color=purple>if</font>";
						$i += 1;
					}
					elseif (substr($str, $i, 6) == 'elseif') {
						$res .= "<font color=purple>elseif</font>";
						$i += 5;
					}
					elseif (substr($str, $i, 4) == 'else') {
						$res .= "<font color=purple>else</font>";
						$i += 3;
					}
					elseif (substr($str, $i, 5) == 'while') {
						$res .= "<font color=purple>while</font>";
						$i += 4;
					}
					elseif (substr($str, $i, 3) == 'for') {
						$res .= "<font color=purple>for</font>";
						$i += 2;
					}
					elseif (substr($str, $i, 1) == '=') {
						$res .= "<font color=darkgreen>=</font>";
					}
					elseif (substr($str, $i, 2) == '++') {
						$res .= "<font color=darkgreen>++</font>";
						$i += 1;
					}
					elseif (substr($str, $i, 2) == '--') {
						$res .= "<font color=darkgreen>--</font>";
						$i += 1;
					}
					elseif (substr($str, $i, 2) == '+=') {
						$res .= "<font color=darkgreen>+=</font>";
						$i += 1;
					}
					elseif (substr($str, $i, 2) == '||') {
						$res .= "<font color=darkgreen>||</font>";
						$i += 1;
					}
					elseif (substr($str, $i, 2) == '&&') {
						$res .= "<font color=darkgreen>&&</font>";
						$i += 1;
					}
					elseif (substr($str, $i, 2) == '->') {
						$res .= "<font color=green>-></font>";
						$i += 1;
					}
					elseif (substr($str, $i, 5) == 'print') {
						$res .= "<font color=green>print</font>";
						$i += 4;
					}
					elseif (substr($str, $i, 5) == 'Print') {
						$res .= "<font color=green>Print</font>";
						$i += 4;
					}
					elseif (substr($str, $i, 4) == 'exit') {
						$res .= "<font color=red>exit</font>";
						$i += 3;
					}
					elseif (substr($str, $i, 5) == 'array') {
						$res .= "<font color=green>array</font>";
						$i += 4;
					}
					elseif (substr($str, $i, 1) == '{') {
						$res .= "<font color=darkgreen>{</font>";
					}
					elseif (substr($str, $i, 1) == '}') {
						$res .= "<font color=darkgreen>}</font>";
					}
					elseif (substr($str, $i, 1) == '(') {
						$res .= "<font color=darkgreen>(</font>";
					}
					elseif (substr($str, $i, 1) == ')') {
						$res .= "<font color=darkgreen>)</font>";
					}
					elseif ($str[$i] == '"') {
					
						$i++;
						$tmp = $i;
						$len = 0;
						
						while (substr($str, $tmp, 1) != '"') {
							$len++;
							$tmp++;
						}
						
						$save = "";
						$j = 0;
						$tmp = $i;
						
						while ($j < $len) {
							$save .= $str[$tmp];
							$j++;
							$tmp++;
						}
						
						$res .= "<font color=red>".'"'.$save.'"'."</font>";
						$i = $tmp;
					}
					elseif ($str[$i] == "'") {
					
						$i++;
						$tmp = $i;
						$len = 0;
						
						while (substr($str, $tmp, 1) != "'") {
							$len++;
							$tmp++;
						}
						
						$save = "";
						$j = 0;
						$tmp = $i;
						
						while ($j < $len) {
							$save .= $str[$tmp];
							$j++;
							$tmp++;
						}
						$res .= "<font color=red>'$save'</font>";
						$i = $tmp;
					}
					elseif (substr($str, $i, 2) == '/*') {
						$i++;
						$tmp = $i;
						$len = 0;
						
						while (substr($str, $tmp, 2) != '*/') {
							$len++;
							$tmp++;
						}
						
						$save = "";
						$j = 0;
						$tmp = $i;
						
						while ($j < $len) {
							$save .= $str[$tmp];
							$j++;
							$tmp++;
						}
						
						$res .= "<font color=orange>/$save*/</font>";
						$i = $tmp + 1;
					}
					elseif (substr($str, $i, 1) == '#') {
						$i++;
						$tmp = $i;
						$len = 0;
						
						while ((substr($str, $tmp, 1) != "\n") && (substr($str, $tmp, 2) != "\r")) {
							if ($tmp == $str_len) {
								break;
							}
							
							$len++;
							$tmp++;
						}
						
						$save = "";
						$j = 0;
						$tmp = $i;
						
						
						while ($j < $len) {
							$save .= $str[$tmp];
							$j++;
							$tmp++;
						}
						
						$res .= "<font color=orange>#$save</font>";
						$i = $tmp;
					}
					else {
						$res .= $str[$i];
					}
				}
				else {
					$res .= $str[$i];
				}
			}
		}
		$i++;
	}
	return ($res);
}

?>