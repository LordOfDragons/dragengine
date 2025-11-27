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

#include "dexsiPLogLevel.h"
#include "../deXSystemInput.h"

#include <dragengine/common/exceptions.h>


// Class dexsiPLogLevel
/////////////////////////

// Constructor, destructor
////////////////////////////

dexsiPLogLevel::dexsiPLogLevel(deXSystemInput &xsi) : dexsiParameter(xsi){
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

decString dexsiPLogLevel::GetParameterValue(){
	switch(pModule.GetLogLevel()){
	case deXSystemInput::LogLevel::error:
		return "error";
		
	case deXSystemInput::LogLevel::warning:
		return "warning";
		
	case deXSystemInput::LogLevel::info:
		return "info";
		
	case deXSystemInput::LogLevel::debug:
		return "debug";
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void dexsiPLogLevel::SetParameterValue(const char *value){
	const decString checkValue(decString(value).GetLower());
	if(checkValue == "error"){
		pModule.SetLogLevel(deXSystemInput::LogLevel::error);
		
	}else if(checkValue == "warning"){
		pModule.SetLogLevel(deXSystemInput::LogLevel::warning);
		
	}else if(checkValue == "info"){
		pModule.SetLogLevel(deXSystemInput::LogLevel::info);
		
	}else if(checkValue == "debug"){
		pModule.SetLogLevel(deXSystemInput::LogLevel::debug);
		
	}else{
		pModule.SetLogLevel(deXSystemInput::LogLevel::error);
	}
}
