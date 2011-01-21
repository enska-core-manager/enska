/**
  * Ext.ux.IconCombo Tutorial
  * by Jozef Sakalos, aka Saki
  * http://extjs.com/learn/Tutorial:Extending_Ext_Class
  */
 
// reference local blank image
//Ext.BLANK_IMAGE_URL = '../extjs/resources/images/default/s.gif';
 
// create application
iconcombo = function() {
 
    // public space
    return {
        // public properties, e.g. strings to translate
 
        // public methods
        init: function() {
            var icnCombo = new Ext.ux.IconCombo({
                store: new Ext.data.SimpleStore({
                    fields: ['countryCode', 'countryName', 'countryFlag'],
                    data: [
                        ['US', 'United States', 'x-flag-us'],
                        ['DE', 'Germany', 'x-flag-de'],
                        ['FR', 'France', 'x-flag-fr']
                    ]
                }),
                valueField: 'countryCode',
                displayField: 'countryName',
                iconClsField: 'countryFlag',
                triggerAction: 'all',
                mode: 'local',
                width: 160
            });
            icnCombo.render('combo-ct');
            icnCombo.setValue('DE');
        }
    };
}(); // end of app
 
// end of file