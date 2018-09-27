# generates a code template for a specific SWC
def generateCodeFile(swc):
	code = "#include <Rte_" + swc.ShortName + ".h>\n\n"
		
	# get all internal behaviors
	internalBehaviors = swc.InternalBehaviors.Elements
	# check if there are some SWCs
	if swc.InternalBehaviors.Count == 0:
		print "Error: No internal behaviors for software component '" + swc.ShortName + "' found! Code generation aborted!"
		return

	# for every internal behaviour determine the runnables
	for internalBehavior in internalBehaviors:
		#print internalBehavior.ShortName
		runnables = internalBehavior.Runnables.Elements
		# check if there are runnables
		if internalBehavior.Runnables.Count == 0:
			print "Error: No runnables for internal behavior '" + internalBehavior.ShortName + "' of software component '" + swc.Name + "' found! Code generation aborted!"
			return           
		
		# for every runnable generate code
		for runnable in runnables:
			print "Start of code generation for runnable '" + runnable.ShortName + "'"
			
			code = code + "void " + swc.ShortName + "_" + runnable.ShortName + "()\n{\n\t// These functions can be used to access the data elements\n"
			
			# implicit receive
			dataReadAccesses = runnable.DataReadAccesss.Elements
			# implicit write
			dataWriteAccesses = runnable.DataWriteAccesss.Elements
			dataSendPoints = runnable.DataSendPoints.Elements
			dataReceivePointByArguments = runnable.DataReceivePointByArguments.Elements
			dataReceivePointByValues = runnable.DataReceivePointByValues.Elements
			# read IRV access
			readLocalVariables = runnable.ReadLocalVariables.Elements
			# written IRV access
			writtenLocalVariables = runnable.WrittenLocalVariables.Elements
			serverCallPoints = runnable.ServerCallPoints.Elements
			
			i = 0
			for dataReadAccess in dataReadAccesses:
				# get port name
				portName = dataReadAccess.AccessedVariable.AutosarVariableIref.PortPrototypeRef.ShortName
				dataElement = dataReadAccess.AccessedVariable.AutosarVariableIref.TargetDataPrototypeRef
				dataElementName = dataElement.ShortName
				dataElementType = dataElement.TypeTref.ShortName
				# add to code
				code = code + "\t//" + dataElementType + " parameter" + str(i) + " = Rte_IRead_" + runnable.ShortName + "_" + portName + "_" + dataElementName + "();\n"
				i = i + 1
			
			for dataWriteAccess in dataWriteAccesses:
				# get port name
				portName = dataWriteAccess.AccessedVariable.AutosarVariableIref.PortPrototypeRef.ShortName
				dataElement = dataWriteAccess.AccessedVariable.AutosarVariableIref.TargetDataPrototypeRef
				dataElementName = dataElement.ShortName
				dataElementType = dataElement.TypeTref.ShortName
				# add to code
				code = code + "\t//Rte_IWrite_" + runnable.ShortName + "_" + portName + "_" + dataElementName + "(" + dataElementType + " parameter" + str(i) + ");\n"
				i = i + 1
			
			# not tested yet
			for dataSendPoint in dataSendPoints:
				# get port name
				portName = dataSendPoint.AccessedVariable.AutosarVariableIref.PortPrototypeRef.ShortName
				dataElement = dataSendPoint.AccessedVariable.AutosarVariableIref.TargetDataPrototypeRef
				dataElementName = dataElement.ShortName
				dataElementType = dataElement.TypeTref.ShortName
				# add to code
				code = code + "\t//Rte_Send_" + runnable.ShortName + "_" + portName + "_" + dataElementName + "(" + dataElementType + " parameter" + str(i) + ");\n"
				i = i + 1
			
			for readLocalVariable in readLocalVariables:			
				dataElement = readLocalVariable.AccessedVariable.LocalVariableRef
				dataElementName = dataElement.ShortName
				dataElementType = dataElement.TypeTref.ShortName
				# add to code
				code = code + "\t//" + dataElementType + " parameter" + str(i) + " = Rte_IrvRead" + "_" + runnable.ShortName + "_" + dataElementName + "();\n"
				i = i + 1			
			
			for writtenLocalVariable in writtenLocalVariables:			
				dataElement = writtenLocalVariable.AccessedVariable.LocalVariableRef
				dataElementName = dataElement.ShortName
				dataElementType = dataElement.TypeTref.ShortName
				# add to code
				code = code + "\t//Rte_IrvWrite" + "_" + runnable.ShortName + "_" + dataElementName + "(" + dataElementType + " parameter" + str(i) + ");\n"
				i = i + 1
			
			for serverCallPoint in serverCallPoints:
				# get port name
				portName = serverCallPoint.OperationIref.ContextRPortRef.ShortName
				# get operation
				operation = serverCallPoint.OperationIref.TargetRequiredOperationRef
				# get operation name
				operationName =  operation.ShortName
				# get all operation arguments
				operationArguments = operation.Arguments.Elements
				# add to code
				code = code + "\t//Rte_Call" + "_" + portName + "_" + operationName + "("
				
				for argument in operationArguments:
					code = code + argument.TypeTref.ShortName + "* parameter" + str(i) + ","		
					i = i + 1
				# delete last comma
				code = code[:-1]
				code = code + ");\n"
			
			code = code + "}\n"

	#print code			
	
	if not os.path.exists(os.path.expanduser("~\\CodeTemplates")):
		os.makedirs(os.path.expanduser("~\\CodeTemplates"))
		print "Folder 'CodeTemplates' created."

	path = os.path.expanduser("~\\CodeTemplates\\" + swc.ShortName + ".c")
	print "File '" + path + "' created."

	# open file
	file = open(path, 'w+')
	# write code
	file.write(code)
	# close file
	file.close()	
	
	
# function to search all AR-PACKAGES
def searchArPackages(parentPackage):
	# get all child elements of the package
	for element in parentPackage.Elements.Elements:
		# check for Application Software Components
		if element.ElementType == "IApplicationSwComponentType":
			generateCodeFile(element)
		# check for Sensor Actuator Software Components
		elif element.ElementType == "ISensorActuatorSwComponentType":
			generateCodeFile(element)
		# check for ECU Abstraction Software Components
		elif element.ElementType == "IEcuAbstractionSwComponentType":
			generateCodeFile(element)
			
	# get all packages
	arPackages = parentPackage.ArPackages
	# check if there are any packages within this package
	if arPackages.Count != 0:
		for arPackage in arPackages.Elements:
			#print arPackage.ShortName
			searchArPackages(arPackage)

import sys
import os
# import win32com
from win32com.client import Dispatch

#------------------------------------------------------------------------------
# start of initialization
#------------------------------------------------------------------------------
# Open COM connection
SystemDeskApplication = Dispatch("SystemDesk.Application.4.4")
ApplicationRootDir = SystemDeskApplication.ApplicationRootDir
ApplicationScriptingDir = os.path.join(ApplicationRootDir + r"\Tools\Scripting")
sys.path.insert(0, ApplicationScriptingDir)
BswAutomationDir = os.path.join(ApplicationRootDir + r"\Bin\Automation")
sys.path.insert(0, BswAutomationDir)

import SystemDeskEnums
reload(SystemDeskEnums)

#------------------------------------------------------------------------------
# end of initialization 
#------------------------------------------------------------------------------

project = SystemDeskApplication.ActiveProject
projectName = project.Name
print projectName
print "Generation of code templates started ..."

autosarRoot = project.RootAutosar
arPackages = autosarRoot.ArPackages
if arPackages.Count != 0:
	for arPackage in arPackages.Elements:
		searchArPackages(arPackage)

print "Generation finished!"