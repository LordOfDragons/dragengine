/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "deoxrPLogLevel.h"
#include "../deVROpenXR.h"

#include <dragengine/common/exceptions.h>


// Class deoxrPLogLevel
/////////////////////////

// Constructor, destructor
////////////////////////////

deoxrPLogLevel::deoxrPLogLevel(deVROpenXR &oxr) : deoxrParameter(oxr){
	pParameter.SetName("logLevel");
	pParameter.SetDescription("Log level.");
	pParameter.SetType(deModuleParameter::eptSelection);
	pParameter.SetCategory(deModuleParameter::ecAdvanced);
	pParameter.SetDisplayName("Log Level");
	
	const deModuleParameter::SelectionEntry entries[4] = {
		{"error", "Error", "Log only errors."},
		{"warning", "Warning", "Log errors and warnings."},
		{"info", "Info", "Log errors, warnings and information."},
		{"debug", "Debug", "Log everything"}};
	pParameter.AddSelectionEntries(entries, 4);
	pParameter.SetDefaultValue("info");
}


// Parameter Value
////////////////////

decString deoxrPLogLevel::GetParameterValue(){
	switch(pOxr.GetLogLevel()){
	case deVROpenXR::LogLevel::error:
		return "error";
		
	case deVROpenXR::LogLevel::warning:
		return "warning";
		
	case deVROpenXR::LogLevel::info:
		return "info";
		
	case deVROpenXR::LogLevel::debug:
		return "debug";
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void deoxrPLogLevel::SetParameterValue(const char *value){
	const decString checkValue(decString(value).GetLower());
	if(checkValue == "error"){
		pOxr.SetLogLevel(deVROpenXR::LogLevel::error);
		
	}else if(checkValue == "warning"){
		pOxr.SetLogLevel(deVROpenXR::LogLevel::warning);
		
	}else if(checkValue == "info"){
		pOxr.SetLogLevel(deVROpenXR::LogLevel::info);
		
	}else if(checkValue == "debug"){
		pOxr.SetLogLevel(deVROpenXR::LogLevel::debug);
		
	}else{
		pOxr.SetLogLevel(deVROpenXR::LogLevel::error);
	}
}
