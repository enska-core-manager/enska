<?php

include_once('../core/1-0-0_beta/design/enskaDataStore.class.inc');
include_once('../core/1-0-0_beta/engine/ini.class.inc');
include_once('../core/1-0-0_beta/interfaces/enskaObserver.interface.inc');
include_once('../core/1-0-0_beta/design/enskaObserver.class.inc');
include_once('../core/1-0-0_beta/engine/logs.class.inc');

include_once('../libs/lib_files.inc');
include_once('../libs/lib_string.inc');
include_once('../libs/lib_utils.inc');
include_once('../libs/lib_mysql.inc');
include_once('../libs/lib_enska2ml.inc');

define('DEV', 'development');
define('PRO', 'production');

$params = '';
if (isset($_POST['data'])) {
	$params = explode('::', $_POST['data']);
}
else {
	$params = explode('::', $_GET['data']);
}

if (function_exists($params[0])) {
	echo $params[0](&$params);
}
else {
	echo 'Unable to complet this step! Please contact the support!';
}

Function checkDB(&$params)
{
	$db_server = trim($params[1]);
	$db_name = trim($params[2]);
	$db_prefix = trim($params[3]);
	$db_login = trim($params[4]);
	$db_password = trim($params[5]);
	
	$sql = new EnsKa_MML($db_name, $db_server, $db_login, $db_password);
	
	if ($sql->connect()) {
		return ('success');
	}
	else {
		return ('Unable to connect to the database');
	}
}

Function checkManager(&$params)
{
	$man_domain = explode('.', ltrim(ltrim(trim(trim(trim($params[1]), '/'), '.'), 'http://'), 'https://'));
	$man_manager_alias = explode('.', ltrim(ltrim(trim(trim(trim($params[2]), '/'), '.'), 'http://'), 'https://'));
	$man_login_alias = explode('.', ltrim(ltrim(trim(trim(trim($params[3]), '/'), '.'), 'http://'), 'https://'));
	
	if (count($man_domain) < 1) {
		return ('Unable to register the domain');
	}
	
	if (count($man_manager_alias) < 2) {
		return ('Unable to register the manager alias<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Ex: manager.'.ltrim(ltrim(trim(trim(trim($params[1]), '/'), '.'), 'http://'), 'https://').')');
	}
	
	if (count($man_login_alias) < 2) {
		return ('Unable to register the login alias<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Ex: login.'.ltrim(ltrim(trim(trim(trim($params[1]), '/'), '.'), 'http://'), 'https://').')');
	}
	
	return ('success');
}

Function createConfig(&$params)
{
	$root_mail = trim($params[1]);
	$man_domain = ltrim(ltrim(trim(trim(trim($params[2]), '/'), '.'), 'http://'), 'https://');
	$man_manager_alias = ltrim(ltrim(trim(trim(trim($params[3]), '/'), '.'), 'http://'), 'https://');
	$man_login_alias = ltrim(ltrim(trim(trim(trim($params[4]), '/'), '.'), 'http://'), 'https://');
	$conf_type = $params[5];
	$replace = Array
				(
					'%rootMail%'	=>	$root_mail,
					'%domain%'		=>	$man_domain,
					'%realPath%'	=>	rtrim($_SERVER['SCRIPT_FILENAME'], 'install/install.php'),
					'%port%'		=>	$_SERVER['SERVER_PORT'],
					'\"'			=>	'"'
				);
	
	$res = 'success;';
	
	$res .= 'Copy the following lines in your virtual-host file configuration:'."\n";
	$res .= '------------------------------------------------------------------------------------'."\n\n";
	$fd = fopen('vhost.conf', 'r');
	if (!$fd) {
		return ('Unable to open the V-Host configuration file!');
	}
	while (!(feof($fd))) {
		$res .= fgets($fd);
	}
	fclose($fd);
	
	if (strcmp($conf_type, DEV) == 0) {
		$res .= "\n\n".'------------------------------------------------------------------------------------'."\n";
		$res .= 'Copy the following lines in your hosts file configuration:'."\n";
		$res .= '------------------------------------------------------------------------------------'."\n\n";
		$fd = fopen('hosts.conf', 'r');
		if (!$fd) {
			return ('Unable to open the Hosts configuration file!');
		}
		while (!(feof($fd))) {
			$res .= fgets($fd);
		}
		fclose($fd);
	}
	
	$res = strReplace($res, $replace);
	
	return ($res);
}

Function setupDatabase(&$params)
{
	sleep(1);
	
	$user = substr(md5(create_id(10)), 10, 5);
	$password = substr(md5(create_id(10)), 10, 5);
	
	$replace = Array
				(
					'database'		=>	$params[2],
					'prefix'		=>	$params[3],
					'user'			=>	$user,
					'host'			=>	$params[1],
					'password'		=>	$password,
					'rootMail'		=>	$params[6],
					'rootPassword'	=>	md5($params[5]),
					'domain'		=>	$params[7]
				);
	
	strReplaceInFile('sql.conf', 'sql.sql', $replace);
	
	$sql = new EnsKa_MML($params[2], $params[1], $params[4], $params[8]);
	
	if ($sql->connect()) {
		if ($sql->scriptRequest('sql.sql', false, true, true)) {
			$sql->disconnect();
			
			$conf = new EnsKa_INIReader('../conf/db.ini');
			$conf->addLine('dbName', $params[2]);
			$conf->addLine('dbUser', $user);
			$conf->addLine('dbPassword', $password);
			$conf->addLine('dbServer', $params[1]);
			$conf->addLine('dbPrefix', $params[3]);
			$conf->save();
			
			unset($user, $password, $replace, $sql, $conf);
			return ('success');
		}
		else {
			$sql->deleteDatabase($params[2]);
			$sql->disconnect();
			unset($user, $password, $replace, $sql);
			return ('Unable to connect to the database  (1)');
		}
	}
	else {
		unset($user, $password, $replace, $sql);
		return ('Unable to connect to the database (2)');
	}
}

Function setupManager(&$params)
{
	sleep(1);
	
	$engine = $params[1];
	$domain = ltrim(ltrim(trim(trim(trim($params[2]), '/'), '.'), 'http://'), 'https://');
	$manager_alias = ltrim(ltrim(trim(trim(trim($params[3]), '/'), '.'), 'http://'), 'https://');
	$login_alias = ltrim(ltrim(trim(trim(trim($params[4]), '/'), '.'), 'http://'), 'https://');
	
	$conf = new EnsKa_INIReader('../conf/settings.ini');
	$conf->addLine('domain', $domain, true);
	unset($conf);
	
	$auth = new EnsKa_INIReader('../conf/auth.ini');
	$conf = new EnsKa_INIReader('../libs/Auth/'.$engine.'/auth.ini');
	$auth->addLine('auth_lib', $engine);
	$auth->addLine('auth_class', $conf->getLine('class'));
	$auth->addLine('auth_name', $conf->getLine('name'));
	$auth->addLine('lib_version', $conf->getLine('version'));
	$auth->save();
	unset($conf, $auth);
	
	$alias = new EnsKa_INIReader('../conf/alias.ini');
	$alias->addLine($manager_alias, 'manager');
	$alias->addLine($login_alias, 'login');
	$alias->save();
	unset($alias);
	
	return ('success');
}

Function setupCore(&$params)
{
	sleep(2);
	
	@unlink('../index.php');
	@unlink('../default/ressources/enska/css/install.css');
	rollBackFolders('../install/');
	rollBackFolders('../default/ressources/enska/images/install/');
	
	$fd = fopen('../.htaccess', 'w');
	fputs($fd, 'Options -Indexes');
	fclose($fd);
	
	return ('success');
}

?>