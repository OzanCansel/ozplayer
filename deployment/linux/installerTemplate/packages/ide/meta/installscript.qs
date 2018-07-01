function Component()
{

}

Component.prototype.createOperations = function()
{
	component.createOperations();
	
	component.addOperation("Execute" , "sh" , "@TargetDir@/kodlio-install.sh" , "workingdirectory=@TargetDir@", "UNDOEXECUTE" , "sh" , "clean.sh" )

}
