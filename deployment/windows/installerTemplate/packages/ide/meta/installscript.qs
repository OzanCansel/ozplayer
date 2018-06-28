function Component()
{
}

Component.prototype.createOperations = function()
{
	component.createOperations();
	
	if(systemInfo.productType === "windows"){
		component.addElevatedOperation("Execute" ,  "@TargetDir@\\server.exe" , "-i" , "workingdirectory=@TargetDir@", "UNDOEXECUTE" , "@TargetDir@\\server.exe" , "-u")
		component.addElevatedOperation("Execute" ,  "@TargetDir@\\server.exe" , "-s" , "workingdirectory=@TargetDir@")
	}
}
