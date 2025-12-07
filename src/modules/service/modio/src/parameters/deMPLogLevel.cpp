/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deMPLogLevel.h"
#include "../deModio.h"
#include "../deModioService.h"

#include <dragengine/common/exceptions.h>


// Class deMPLogLevel
///////////////////////

// Constructor, destructor
////////////////////////////

deMPLogLevel::deMPLogLevel(deModio &module) :
deModioParameter(module),
pLogLevel(Modio::LogLevel::Warning)
{
	SetName("logLevel");
	SetDescription("Log level.");
	SetType(deModuleParameter::eptSelection);
	SetCategory(ecAdvanced);
	SetDisplayName("Log Level");
	
	const deModuleParameter::SelectionEntry entries[4] = {
		{"error", "Error", "Log only errors."},
		{"warning", "Warning", "Log errors and warnings."},
		{"info", "Info", "Log errors, warnings and information."},
		{"debug", "Debug", "Log everything"}
	};
	AddSelectionEntries(entries, 4);
	SetDefaultValue("warning");
}

deMPLogLevel::~deMPLogLevel(){
}



// Parameter Value
////////////////////

decString deMPLogLevel::GetParameterValue(){
	switch(pLogLevel){
	case Modio::LogLevel::Error:
		return "error";
		
	case Modio::LogLevel::Warning:
		return "warning";
		
	case Modio::LogLevel::Info:
		return "info";
		
	case Modio::LogLevel::Trace:
		return "debug";
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void deMPLogLevel::SetParameterValue(const char *value){
	const decString checkValue(decString(value).GetLower());
	if(checkValue == "error"){
		pLogLevel = Modio::LogLevel::Error;
		
	}else if(checkValue == "warning"){
		pLogLevel = Modio::LogLevel::Warning;
		
	}else if(checkValue == "info"){
		pLogLevel = Modio::LogLevel::Info;
		
	}else if(checkValue == "debug"){
		pLogLevel = Modio::LogLevel::Trace;
		
	}else{
		pLogLevel = Modio::LogLevel::Warning;
	}
	
	if(deModioService::Global()){
		Modio::SetLogLevel(pLogLevel);
	}
}
