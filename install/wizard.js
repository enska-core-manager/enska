
Ext.onReady
(
	function()
	{
		Ext.QuickTips.init();
		
		/* Step 1 */
		var db_server;
		var db_login;
		var db_password;
		var db_name;
		var db_prefix;
		
		/* Step 2 */
		var man_engine = 'none';
		var man_domain;
		var man_manager_alias;
		var man_login_alias;
		
		/* Step 3 */
		var root_password;
		var root_mail;
		
		/* Step 4 */
		var	conf_type;
		
		var startPanel = new Ext.FormPanel ( {
				id: 'install-start',
				border: false,
				frame: false,
				labelWidth: 50,
				defaults: {
					anchor: '80%',
					allowBlank: false,
				},
				contentEl: 'install-start-content'
			}
		);
		
		var step1Panel = new Ext.FormPanel ( {
				id: 'install-s1',
				border: false,
				frame: false,
				labelWidth: 50,
				defaults: {
					anchor: '80%',
					allowBlank: false,
				},
				contentEl: 'install-s1-content'
			}
		);
		
		var step2Panel = new Ext.FormPanel ( {
				id: 'install-s2',
				border: false,
				frame: false,
				labelWidth: 50,
				defaults: {
					anchor: '80%',
					allowBlank: false,
				},
				contentEl: 'install-s2-content'
			}
		);
		
		var step3Panel = new Ext.FormPanel ( {
				id: 'install-s3',
				border: false,
				frame: false,
				labelWidth: 50,
				defaults: {
					anchor: '80%',
					allowBlank: false,
				},
				contentEl: 'install-s3-content'
			}
		);
		
		var step4Panel = new Ext.FormPanel ( {
				id: 'install-s4',
				border: false,
				frame: false,
				labelWidth: 50,
				defaults: {
					anchor: '80%',
					allowBlank: false,
				},
				contentEl: 'install-s4-content'
			}
		);
		
		var navHandler = function(direction) {
			Ext.getCmp('win-installStatusBar').setText('');
			
			if (direction == 1) {
				if(winInstallWizard.layout.activeItem.id == 'install-start') {
					if (Ext.get('license-chk').dom.checked) {
						winInstallWizard.layout.setActiveItem('install-s1');
						updateStatus('<img src="default/ressources/enska/images/install/bullet_green.png" width=16 height=16> Step 1 / 4', 'step');
					}
				}
				else if(winInstallWizard.layout.activeItem.id == 'install-s1') {
					if (trim(Ext.get('db-name').dom.value) == '') {
						updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16> <font color=orange style="font-weight: bold">You must give a databse name !</font>', 'status');
					}
					else if (trim(Ext.get('db-login').dom.value) == '') {
						updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16> <font color=orange style="font-weight: bold">You must give a login !</font>', 'status');
					}
					else if (trim(Ext.get('db-server').dom.value) == '') {
						updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16> <font color=orange style="font-weight: bold">You must give a server name !</font>', 'status');
					}
					else {
						db_server = Ext.get('db-server').dom.value;
						db_login = Ext.get('db-login').dom.value;
						db_password = Ext.get('db-password').dom.value;
						db_name = Ext.get('db-name').dom.value;
						db_prefix = Ext.get('db-prefix').dom.value;
						if (trim(db_prefix) == '') {
							db_prefix = 'core_';
						}
						updateStatus('<img src="default/ressources/enska/images/install/loading_s.gif" width=16 height=16> <b>Checking database configuration ...</b>', 'status');
						
						var conn = new Ext.data.Connection();
						var req = conn.request ( {
								url: 'install/install.php',
								method: 'POST',
								params: "data=" + 'checkDB::' + db_server + '::' + db_name + '::' + db_prefix + '::' + db_login + '::' + db_password,
								success: function(resObj) {
									if (resObj.responseText == 'success') {
										winInstallWizard.layout.setActiveItem('install-s2');
										updateStatus('<img src="default/ressources/enska/images/install/bullet_green.png" width=16 height=16> Step 2 / 4', 'step');
									}
									else {
										updateStatus('<img src="default/ressources/enska/images/install/exclamation_s.png" width=16 height=16> <font color=red style="font-weight: bold">' + resObj.responseText + '</font>', 'status');
									}
								}
							}
						);
					}
				}
				else if(winInstallWizard.layout.activeItem.id == 'install-s2') {
					if (trim(Ext.get('man-login-alias').dom.value) == '') {
						updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16> <font color=orange style="font-weight: bold">You must give a login alias !</font>', 'status');
					}
					else if (trim(Ext.get('man-manager-alias').dom.value) == '') {
						updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16> <font color=orange style="font-weight: bold">You must give a manager alias !</font>', 'status');
					}
					else if (trim(Ext.get('man-domain').dom.value) == '') {
						updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16> <font color=orange style="font-weight: bold">You must give a plateform domain !</font>', 'status');
					}
					else if (trim(Ext.get('man-auth').dom.value) == '-') {
						updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16> <font color=orange style="font-weight: bold">You must select an authentification engine !</font>', 'status');
					}
					else {
						man_engine = Ext.get('man-auth').dom.value;
						man_domain = Ext.get('man-domain').dom.value;
						man_manager_alias = Ext.get('man-manager-alias').dom.value;
						man_login_alias = Ext.get('man-login-alias').dom.value;
						updateStatus('<img src="default/ressources/enska/images/install/loading_s.gif" width=16 height=16> <b>Checking manager configuration ...</b>', 'status');
						
						var conn = new Ext.data.Connection();
						var req = conn.request ( {
								url: 'install/install.php',
								method: 'POST',
								params: "data=" + 'checkManager::' + man_domain + '::' + man_manager_alias + '::' + man_login_alias,
								success: function(resObj) {
									if (resObj.responseText == 'success') {
										winInstallWizard.layout.setActiveItem('install-s3');
										updateStatus('<img src="default/ressources/enska/images/install/bullet_green.png" width=16 height=16> Step 3 / 4', 'step');
									}
									else {
										updateStatus('<img src="default/ressources/enska/images/install/exclamation_s.png" width=16 height=16> <font color=red style="font-weight: bold">' + resObj.responseText + '</font>', 'status');
									}
								}
							}
						);
					}
				}
				else if(winInstallWizard.layout.activeItem.id == 'install-s3') {
					if (trim(Ext.get('root-password').dom.value) == '') {
						updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16> <font color=orange style="font-weight: bold">You must give a root password !</font>', 'status');
					}
					else if (trim(Ext.get('root-password').dom.value) != trim(Ext.get('root-password-confirm').dom.value)) {
						updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16> <font color=orange style="font-weight: bold">The passwords are not the same !</font>', 'status');
					}
					else if (trim(Ext.get('root-mail').dom.value) == '') {
						updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16> <font color=orange style="font-weight: bold">You must give an e-mail address !</font>', 'status');
					}
					else {
						root_password = Ext.get('root-password').dom.value;
						root_mail = Ext.get('root-mail').dom.value;
						winInstallWizard.layout.setActiveItem('install-s4');
						updateStatus('<img src="default/ressources/enska/images/install/bullet_green.png" width=16 height=16> Step 4 / 4', 'step4');
					}
				}
				else if(winInstallWizard.layout.activeItem.id == 'install-s4') {
					if (Ext.getCmp('btn-next-install-wizard').getText() == 'Install ->') {
						updateStatus('<img src="default/ressources/enska/images/install/loading_s.gif" width=16 height=16> <b>Installing plateform ...</b>', 'install');
						Ext.get('progress').dom.value = 'Initialize ... OK\nSetup database ...';
						
						var conn = new Ext.data.Connection();
						var req = conn.request ( {
								url: 'install/install.php',
								method: 'POST',
								params: "data=" + 'setupDatabase::' + db_server + '::' + db_name + '::' + db_prefix + '::' + db_login + '::' + root_password + '::' + root_mail + '::' + man_domain + '::' + db_password,
								success: function(resObj) {
									if (resObj.responseText == 'success') {
										Ext.get('progress').dom.value += ' OK\nSetup manager ...';
										var req = conn.request ( {
												url: 'install/install.php',
												method: 'POST',
												params: "data=" + 'setupManager::' + man_engine + '::' + man_domain + '::' + man_manager_alias + '::' + man_login_alias,
												success: function(resObj) {
													if (resObj.responseText == 'success') {
														Ext.get('progress').dom.value += ' OK\nSetup core ...';
														var req = conn.request ( {
																url: 'install/install.php',
																method: 'POST',
																params: "data=" + 'setupCore',
																success: function(resObj) {
																	if (resObj.responseText == 'success') {
																		Ext.get('progress').dom.value += ' OK\nFinalize ... OK';
																		Ext.get('progress').dom.value += '\n\nYour EnsKa plateform has been correctly installed!\nManager address: http://' + man_manager_alias + '/';
																		updateStatus('<img src="default/ressources/enska/images/install/accept_s.png" width=16 height=16>', 'finish');
																	}
																	else {
																		Ext.get('progress').dom.value += ' Error!';
																		updateStatus('<img src="default/ressources/enska/images/install/exclamation_s.png" width=16 height=16> <font color=red style="font-weight: bold">' + resObj.responseText + '</font>', 'error');
																	}
																}
															}
														);
													}
													else {
														Ext.get('progress').dom.value += ' Error!';
														updateStatus('<img src="default/ressources/enska/images/install/exclamation_s.png" width=16 height=16> <font color=red style="font-weight: bold">' + resObj.responseText + '</font>', 'error');
													}
												}
											}
										);
									}
									else {
										Ext.get('progress').dom.value += ' Error!';
										updateStatus('<img src="default/ressources/enska/images/install/exclamation_s.png" width=16 height=16> <font color=red style="font-weight: bold">' + resObj.responseText + '</font>', 'error');
									}
								}
							}
						);
					}
					else {
						alert('Go to http://' + man_manager_alias + '/ to starting with your EnsKa plateform');
					}
				}
			}
			else if (direction == -1) {
				if(winInstallWizard.layout.activeItem.id == 'install-s1') {
					winInstallWizard.layout.setActiveItem('install-start');
					updateStatus('', 'backS1');
				}
				else if(winInstallWizard.layout.activeItem.id == 'install-s2') {
					winInstallWizard.layout.setActiveItem('install-s1');
					updateStatus('<img src="default/ressources/enska/images/install/bullet_green.png" width=16 height=16> Step 1 / 4', 'backS2');
				}
				else if(winInstallWizard.layout.activeItem.id == 'install-s3') {
					winInstallWizard.layout.setActiveItem('install-s2');
					updateStatus('<img src="default/ressources/enska/images/install/bullet_green.png" width=16 height=16> Step 2 / 4', 'backS2');
				}
				else if(winInstallWizard.layout.activeItem.id == 'install-s4') {
					winInstallWizard.layout.setActiveItem('install-s3');
					updateStatus('<img src="default/ressources/enska/images/install/bullet_green.png" width=16 height=16> Step 3 / 4', 'backS2');
				}
			}
		};
		
		function updateStatus(text, mod)
		{
			if (mod == 'step') {
				Ext.getCmp('win-installStatusBar').setText(text);	
				Ext.getCmp('btn-prev-install-wizard').enable();
				Ext.getCmp('btn-next-install-wizard').enable();
			}
			else if (mod == 'step4') {
				Ext.getCmp('win-installStatusBar').setText(text);	
				Ext.getCmp('btn-prev-install-wizard').enable();
				Ext.getCmp('btn-next-install-wizard').disable();
				Ext.getCmp('btn-next-install-wizard').setText('Install ->');
			}
			else if (mod == 'step4.1') {
				Ext.getCmp('win-installStatusBar').setText(text);
				Ext.getCmp('btn-next-install-wizard').enable();
			}
			else if (mod == 'install') {
				Ext.getCmp('win-installStatusBar').setText(text);
				Ext.getCmp('btn-next-install-wizard').disable();
				Ext.getCmp('btn-prev-install-wizard').disable();
			}
			else if (mod == 'finish') {
				Ext.getCmp('win-installStatusBar').setText(text);	
				Ext.getCmp('btn-prev-install-wizard').disable();
				Ext.getCmp('btn-next-install-wizard').enable();
				Ext.getCmp('btn-next-install-wizard').setText('Close');
			}
			else if (mod == 'error') {
				Ext.getCmp('win-installStatusBar').setText(text);
				Ext.getCmp('btn-next-install-wizard').enable();
				Ext.getCmp('btn-prev-install-wizard').enable();
			}
			else if(mod == 'backS1') {
				Ext.getCmp('win-installStatusBar').setText(text);	
				Ext.getCmp('btn-prev-install-wizard').disable();
				updateStatus('<img src="default/ressources/enska/images/install/accept_s.png" width=16 height=16>', 'status');
			}
			else if(mod == 'backS2') {
				Ext.getCmp('win-installStatusBar').setText(text);	
				Ext.getCmp('btn-prev-install-wizard').enable();
				Ext.getCmp('btn-next-install-wizard').enable();
				Ext.getCmp('btn-next-install-wizard').setText('Next ->');
				updateStatus('<img src="default/ressources/enska/images/install/accept_s.png" width=16 height=16>', 'status');
			}
			else if(mod == 'status') {
				Ext.getCmp('win-installStatusBar').setText(text);
			}
		}
		
		function trim(str)
		{
			return str.replace(/^\s+/g,'').replace(/\s+$/g,'')
		}
		
		var winInstallWizard = new Ext.Window ( {
				closeAction: 'close',
				id: 'win-installWizardStart',
				layout: 'card',
				width: 800,
				height: 400,
				plain: false,
				modal: false,
				resizable: false,
				closable: false,
				title: 'EnsKa Core Manager - Installation assistant wizard',
				border: false,
				bodyStyle: 'background-color: transparent',
				activeItem: 0,
				items: [
					startPanel,
					step1Panel,
					step2Panel,
					step3Panel,
					step4Panel
				],
				bbar: new Ext.ux.StatusBar ( {
						id: 'win-installStatusBar',
						items: [ {
								text: '<- Back',
								id: 'btn-prev-install-wizard',
								handler: navHandler.createDelegate(this, [-1]),
								disabled: true
							},
								'-',
							{
								text: 'Next ->',
								id: 'btn-next-install-wizard',
								handler: navHandler.createDelegate(this, [1]),
								disabled: true
							}
							
						]
					}
				)
			}
		);
		winInstallWizard.show();
		
		function showConfiguration(mod)
		{
			var configurationWizard = new Ext.form.TextArea ( {
					id:'configuration-wizard',
					wrap:'off',
					anchor:'95%'
				}
			);
			
			var winConfiguration = new Ext.Window ( {
					closeAction: 'close',
					layout: 'fit',
					modal: true,
					resizable: false,
					closable: true,
					title: 'Server configuration - ' + mod,
					width: 800,
					height: 600,
					border: false,
					items: [
						configurationWizard
					],
					buttons: [ {
							text: 'Close',
							handler: function() {
								winConfiguration.close();
							}
						}
					]
				}
			);
			winConfiguration.show();
			
			
			var conn = new Ext.data.Connection();
			var req = conn.request ( {
					url: 'install/install.php',
					method: 'POST',
					params: "data=" + 'createConfig::' + root_mail + '::' + man_domain + '::' + man_manager_alias + '::' + man_login_alias + '::' + conf_type,
					success: function(resObj) {
						var res = resObj.responseText.split(';');
						if (res[0] == 'success') {
							Ext.get('configuration-wizard').dom.value = res[1];
							updateStatus('<img src="default/ressources/enska/images/install/accept_s.png" width=16 height=16>', 'step4.1');
						}
						else {
							updateStatus('', 'step4');
							updateStatus('<img src="default/ressources/enska/images/install/exclamation_s.png" width=16 height=16> <font color=red style="font-weight: bold">' + res[1] + '</font>', 'status');
							
						}
					}
				}
			);
		}
		
		Ext.get('conf-local').on(
			'click', 
			function() {
				conf_type = 'development';
				showConfiguration('Local development');
			}
		);
		
		Ext.get('conf-production').on(
			'click', 
			function() {
				conf_type = 'production';
				showConfiguration('Production deployment');
			}
		);
		
		Ext.get('license-chk').on(
			'click', 
			function() {
				if (Ext.get('license-chk').dom.checked) {
					Ext.getCmp('btn-next-install-wizard').enable();
					updateStatus('<img src="default/ressources/enska/images/install/accept_s.png" width=16 height=16>', 'status');
				}
				else {
					Ext.getCmp('btn-next-install-wizard').disable();
					updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16>', 'status');
				}
			}
		);
		updateStatus('<img src="default/ressources/enska/images/install/error_s.png" width=16 height=16>', 'status');
		 
	}
);