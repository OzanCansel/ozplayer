function Component()
{
}

Component.prototype.createOperations = function()
{
	component.createOperations();

	
	if(systemInfo.productType === "windows"){
		component.addOperation("CreateShortcut", "@TargetDir@/configurator.exe", "@DesktopDir@/OzPlayer Configurator.lnk");
        component.addOperation("CreateShortcut", "@TargetDir@\\configurator.exe", "@StartMenuDir@\\OzPlayer Configurator.lnk",
        "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll",
        "iconId=2");
		component.addElevatedOperation("Execute" ,  "@TargetDir@\\server.exe" , "-i" , "workingdirectory=@TargetDir@", "UNDOEXECUTE" , "@TargetDir@\\server.exe" , "-u")
		component.addOperation("Execute" , "@TargetDir@\\configurator.exe" , "-d")
		component.addElevatedOperation("Execute" ,  "@TargetDir@\\server.exe" , "-s" , "workingdirectory=@TargetDir@", "UNDOEXECUTE" , "@TargetDir@\\server.exe" , "-terminate")
	}
}
