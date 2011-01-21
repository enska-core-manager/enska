<?php

include_once('install/config.inc');

echo '
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
		<title>'.$titleBar.'</title>
		
		<link rel="stylesheet" type="text/css" href="'.$pathCSS.'extjs/311/ext-all.css" />
		<link rel="stylesheet" type="text/css" href="'.$pathCSS.'install.css" />
		<link rel="icon" type="image/png" href="'.$pathCommonImg.'favico.png" />
		
		<script src="'.$pathJS.'extjs/311/ext-base.js" type="text/javascript"></script>
		<script src="'.$pathJS.'extjs/311/adapters/ext/ext-base.js" type="text/javascript"></script>
		<script src="'.$pathJS.'extjs/311/ext-all.js" type="text/javascript"></script>
		<script src="'.$pathJS.'extjs/311/extends/statusBar.js" type="text/javascript"></script>
		
		<script src="install/wizard.js" type="text/javascript"></script>
		
	</head>

<body>

<div id="head">
	<div id="header">
		<a href="'.$editorAddress.'" style="color:white;text-decoration:none;"><h2 style="color: white;">'.$title.'</a><br>Installation assitant wizard</h2>
	</div>
</div>


<div style="display:none;">
	<div id="install-start-content">
		<div style="border:0px red solid; height: 100%">
			<div style="border: 0; border-bottom: 1px lightgray solid; height: 40px;">
				<div style="font-weight:bold; padding-top:10px; padding-left: 20px; font-size: 14px;">License</div>
			</div>
			<table height=300 width="100%">
				<tr>
					<td width="130px">
						<img src="default/ressources/enska/images/install/license_b.png" width=128 height=128>
					</td>
					<td width=100% valign=top style="padding-top: 20px; padding-left: 10px; font-size:14px">
						<span style="font-size: 20px; font-weight: bold;">Welcome on EnsKa Core Manager installation wizard.</span><br><br>
						This assistant wizard will guide you over the steps to install your new EnsKa plateform.<br>
						Read the licence,then click on \'Next\' to begin the installation.
						<br><br>
						License:<br>
						<textarea style="width: 95%; height: 100px;" readonly>'.$license.'</textarea><br><br>
						<input type="checkbox" id="license-chk">
						<i>
							By checking this case, I recognize to have taken knowledge of the constraints of the license and I accept them.
						</i>
					</td>
				</tr>
			</table>
		</div>
	</div>
	
	<div id="install-s1-content">
		<div style="border:0px red solid; height: 100%">
			<div style="border: 0; border-bottom: 1px lightgray solid; height: 40px;">
				<div style="font-weight:bold; padding-top:10px; padding-left: 20px; font-size: 14px;">Database setup</div>
			</div>
			<table height=300 width="100%">
				<tr>
					<td width="130px" style="padding-left:70px;">
						<img src="default/ressources/enska/images/install/database_b.png" width=128 height=128>
					</td>
					<td width=100% valign=top style="padding-top: 20px; padding-left: 0px; font-size:14px" align="center">
						<br>
						<table cellspacing="10">
							<tr>
								<td>Server :</td>
								<td><input type="text" size="30" id="db-server" value="localhost"></td>
								<td><img src="default/ressources/enska/images/install/server_database_s.png" width=16 height=16></td>
							</tr>
							<tr><td><br></td></tr>
							<tr>
								<td>Login :</td>
								<td><input type="text" size="30" id="db-login" value="root"></td>
								<td><img src="default/ressources/enska/images/install/user_gray_s.png" width=16 height=16></td>
							</tr>
							<tr>
								<td>Password :</td>
								<td><input type="password" size="30" id="db-password"></td>
								<td><img src="default/ressources/enska/images/install/key_s.png" width=16 height=16></td>
							</tr>
							<tr><td><br></td></tr>
							<tr>
								<td>Database :</td>
								<td><input type="text" size="30" id="db-name" value="enska_coreManager"></td>
								<td><img src="default/ressources/enska/images/install/database_s.png" width=16 height=16></td>
							</tr>
							<tr>
								<td>Table prefix :</td>
								<td><input type="text" size="30" id="db-prefix" value="core_"></td>
								<td><img src="default/ressources/enska/images/install/table_relationship_s.png" width=16 height=16></td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
		</div>
	</div>
	
	<div id="install-s2-content">
		<div style="border:0px red solid; height: 100%">
			<div style="border: 0; border-bottom: 1px lightgray solid; height: 40px;">
				<div style="font-weight:bold; padding-top:10px; padding-left: 20px; font-size: 14px;">Manager configuration</div>
			</div>
			<table height=300 width="100%">					
				<tr>
					<td width="130px" style="padding-left:70px;">
						<img src="default/ressources/enska/images/install/wizard_b.png" width=128 height=128>
					</td>
					<td width=100% valign=top style="padding-top: 20px; padding-left: 0px; font-size:14px" align="center">
						<br><br>
						<table cellspacing="10">
							<tr>
								<td>Authentification :</td>
								<td>'.$authEngine.'</td>
								<td><img src="default/ressources/enska/images/install/group_key_s.png" width=16 height=16></td>
							</tr>
							<tr><td><br></td></tr>
							<tr>
								<td>Plateform domain :</td>
								<td><input type="text" id="man-domain" size="30" value="'.$host.'"></td>
								<td><img src="default/ressources/enska/images/install/tag_blue_s.png" width=16 height=16></td>
							</tr>
							<tr><td><br></td></tr>
							<tr>
								<td>Manager alias :</td>
								<td><input type="text" id="man-manager-alias" size="30" value="manager.'.$host.'"></td>
							</tr>
							<tr>
								<td>Login alias :</td>
								<td><input type="text" id="man-login-alias" size="30"  value="login.'.$host.'"></td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
		</div>
	</div>
	
	<div id="install-s3-content">
		<div style="border:0px red solid; height: 100%">
			<div style="border: 0; border-bottom: 1px lightgray solid; height: 40px;">
				<div style="font-weight:bold; padding-top:10px; padding-left: 20px; font-size: 14px;">Root informations</div>
			</div>
			<table height=300 width="100%">					
				<tr>
					<td width="130px" style="padding-left:70px;">
						<img src="default/ressources/enska/images/install/user_gray_b.png" width=128 height=128>
					</td>
					<td width=100% valign=top style="padding-top: 20px; padding-left: 0px; font-size:14px" align="center">
						<br><br>
						<table cellspacing="10">
							<tr>
								<td>Login :</td>
								<td><input type="text" size="30" value="root" style="background-color: lightgray;" readonly></td>
								<td><img src="default/ressources/enska/images/install/user_gray_s.png" width=16 height=16></td>
							</tr>
							<tr><td><br></td></tr>
							<tr>
								<td>Password :</td>
								<td><input type="password" id="root-password" size="30" value=""></td>
								<td><img src="default/ressources/enska/images/install/key_s.png" width=16 height=16></td>
							</tr>
							<tr>
								<td>Confirm password :</td>
								<td><input type="password" id="root-password-confirm" size="30" value=""></td>
							</tr>
							<tr><td><br></td></tr>
							<tr>
								<td>Mail address :</td>
								<td><input type="text" id="root-mail" size="30"  value="root@'.$host.'"></td>
								<td><img src="default/ressources/enska/images/install/email_s.png" width=16 height=16></td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
		</div>
	</div>
	
	<div id="install-s4-content">
		<div style="border:0px red solid; height: 100%">
			<div style="border: 0; border-bottom: 1px lightgray solid; height: 40px;">
				<div style="font-weight:bold; padding-top:10px; padding-left: 20px; font-size: 14px;">Server configuration</div>
			</div>
			<table height=300 width="100%">					
				<tr>
					<td width="130px">
						<img src="default/ressources/enska/images/install/server_b.png" width=128 height=128>
					</td>
					<td width=100% valign=top style="padding-top: 20px; padding-left: 10px; font-size:14px">
						Now you need to configure your server.<br>
						Click on your environment:<br><br><br>
						<center>
							<input type="button" id="conf-local" value="Local development" style="width: 200px; height: 30px">
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							<input type="button" id="conf-production" value="Production deployment" style="width: 200px; height: 30px">
						</center>
						<br><br>
						<textarea style="width: 95%; height: 130px;" id="progress" readonly></textarea><br><br>
					</td>
				</tr>
			</table>
		</div>
	</div>
</div>

</body>
</html>
';

?>