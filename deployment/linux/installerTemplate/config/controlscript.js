function Controller()
{

}

Controller.prototype.IntroductionPageCallback = function()
{
    var widget = gui.currentPageWidget(); // get the current wizard page
    if (widget != null) {
        widget.title = "Kodlio Kurulum"; // set the page title
        widget.MessageLabel.setText("Kodlio kurulum sihirbazına hoşgeldiniz."); // set the welcome text	
    }
}

Controller.prototype.TargetDirectoryPageCallback = function(){
    var widget = gui.currentPageWidget(); // get the current wizard page
    if (widget != null) {
        widget.title = "Yüklenecek Dizin"; // set the page title
	     widget.MessageLabel.setText("Lütfen Kodlio'nun yükleneceği klasörü seçiniz.");
    }
}

Controller.prototype.ComponentSelectionPageCallback = function(){
    var widget = gui.currentPageWidget(); // get the current wizard page
    if (widget != null) {
        widget.title = "Yüklenecek Parçalar"; // set the page title
    }
}

Controller.prototype.LicenseAgreementPageCallback = function(){
    var widget = gui.currentPageWidget(); // get the current wizard page
    if (widget != null) {
		  widget.title = "Lisans Sözleşmesi";
        widget.AcceptLicenseLabel.setText("Evet, sözleşmeyi okudum ve kabul ediyorum."); // set the page title
	     widget.RejectLicenseLabel.setText("Hayır, sözleşmeyi okudum ve kabul etmiyorum.");
    }
}

 Controller.prototype.ReadyForInstallationPageCallback = function(){
    var widget = gui.currentPageWidget(); // get the current wizard page
    if (widget != null) {
		  widget.title = "Kurulum nerdeyse bitmek üzere...";
		  widget.MessageLabel.setText("Kurulum dosyaları bilgisayarınıza yüklenmeye hazır. Yükleme işlemine devam etmek için 'Install' butonuna basınız.");
    }
}

Controller.prototype.FinishedPageCallback = function(){
    var widget = gui.currentPageWidget(); // get the current wizard page
    if (widget != null) {
		  widget.title = "Kodlio başarıyla yüklendi.";
		  widget.MessageLabel.setText("Kurulum sihirbazından çıkmak için 'Finish'e basınız.");
    }
}
