<?php

include_once('core/1-0-0_beta/design/enskaDataStore.class.inc');
include_once('core/1-0-0_beta/engine/ini.class.inc');
include_once('libs/lib_files.inc');

$titleBar = 'EnsKa Core Manager (preview)';
$title = 'EnsKa Core Manager <small>(preview)</small>';
$editorAddress = 'http://www.enska-system.com';


$pathCSS = 'default/ressources/enska/css/';
$pathJS = 'default/ressources/enska/js/';
$pathCommonImg = 'default/ressources/sites/manager/_commons/images/';

$license = getLicense();
$authEngine = '<select id="man-auth" style="width:100%">'.getAuthEngine().'</select>';
$host = $_SERVER['HTTP_HOST'];

function getLicense()
{
	$fd = fopen('LICENSE.txt', 'r');
	$license = '';
	while (!(feof($fd))) {
		$license .= str_replace('\"', '"', str_replace("\'", "'", fgets($fd)));
	}
	fclose($fd);
	return ($license);
}

function getAuthEngine()
{
	$path = 'libs/Auth/';
	$folders = get_folder_list($path);
	$out = '<option value="-"></option>';
	
	foreach($folders as $folder) {
		$auth = new EnsKa_INIReader($path.$folder.'/auth.ini');
		$out .= '<option value="'.$folder.'">'.$auth->getLine('name').'</option>';
	}
	
	return($out);
}

?>