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
 * Get a string between two delimiter
 *
 * Example:
 *		- get_str_between("my name is {John Travolta} I'm an actor!", '{', '}');
 *		- return: John Travolta
 *
 * @param string $str
 * @param string $bet_start
 * @param string $bet_end
 * @return string
 */
Function get_str_between($str, $bet_start, $bet_end)
{
	$i = 0;
	$len = strlen($str);
	$res = '';
	
	while ($i < $len) {
		if (substr($str, $i, strlen($bet_start)) == $bet_start) {	
			$i = $i + strlen($bet_start);	
			while (substr($str, $i, strlen($bet_end)) != $bet_end) {
				$res .= $str[$i];
				$i++;
			}
		}
		else {
			$i++;
		}
	}
	return ($res);
}

/**
 * Epitech POWAAAAA ;)
 * Don't care about it! 42 that's all!
 *
 * @param string $str
 * @param string $sep
 * @return array
 */ 
Function str_to_word_tab($str, $sep)
{
	$i = 0;
  	$j = 0;
  	$start = 0;
  	$len = strlen($str);
	
  	while ($i <= $len) {
    	if ((substr($str, $i, 1) == $sep) || ($i == $len)) {
			$size = $i - $start;
			$res[$j] = substr($str, $start, $size);
			$start = $i;
			$start++;
			$j++;
    	}
		$i++;
	}
	return ($res);
}

/**
 * Trunc a string from right and add '...' at the end if the trunc is much longer as the trunc size
 *
 * Example:
 *		- trunc_str_right('Hello world!', '4');
 *		- return: Hell...
 *
 * @param string $str
 * @param int $nb
 * @return string
 */
Function trunc_str_right($str, $nb)
{
	if(strlen($str) <= $nb) {
		return ($str);
	}
	
	$res = substr($str, 0, strlen($str) - $nb);
	$res .= '...';
  
	return ($res);
}

/**
 * Trunc a string from left and add '...' at the begining if the trunc is much longer as the trunc size
 *
 * Example:
 *		- trunc_str_left('Hello world!', '4');
 *		- return: ...o world!
 *
 * @param string $str
 * @param int $nb
 * @return string
 */
Function trunc_str_left($str, $nb)
{
  if(strlen($str) <= $nb) {
	return ($str);
  }
  
  $res = '...';
  $res .= substr($str, $nb, strlen($str) - $nb);
  
  return ($res);
}

/**
 * Replace a word by an other one in a sentence
 *
 * Example:
 *		- strReplace('Hello my world!', 'my', 'the');
 *		- return: Hello the wolrd!
 *
 * @param string $str
 * @param mixed $that
 * @param string $by
 * @return string
 */
Function strReplace($str, $that, $by=null)
{
	if ((!(is_array($that))) && ($by != null)) {
		$i = 0;
		$len = strlen($str);
		$res = '';
		
		while ($i < $len) {
			if (strcmp(substr($str, $i, strlen($that)), $that) == 0) {
				$res .= $by;
				$i += strlen($that);
			}
			else {
				$res .= $str[$i];
				$i++;
			}
		}
	}
	else {
		$res = $str;
		if (is_array($that)) {
			foreach($that as $key => $val) {
				$res = strReplace($res, trim($key), trim($val));
			}
		}
	}
	return ($res);
}

Function strReplaceInFile($source, $target, $replace)
{
	if (!($fs = @fopen($source, 'r'))) {
		trigger_error('No source file: '.$source.'****0x', E_USER_NOTICE);
		return (false);
	}
	
	if (!($fd = fopen($target, 'w'))) {
		fclose($fs);
		trigger_error('No destination file: '.$target.'****0x', E_USER_NOTICE);
		return (false);
	}
	
	$buff = '';
	while (!(feof($fs))) {
		$buff .= fgets($fs);
	}
	fclose($fs);
	
	foreach($replace as $key => $val) {
		$buff = strReplace($buff, '%'.$key.'%', $val);
	}
	
	fputs($fd, $buff);
	fclose($fd);
	
	unset($buff, $fd, $fs, $source, $target, $key, $val, $replace);
	return (true);
}

?>