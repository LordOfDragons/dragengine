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

#include "deoalPLogLevel.h"
#include "../deAudioOpenAL.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/common/exceptions.h>


// Class deoalPLogLevel
/////////////////////////

// Constructor, destructor
////////////////////////////

deoalPLogLevel::deoalPLogLevel(deAudioOpenAL &oal) : deoalParameter(oal){
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
	SetDefaultValue("info");
}

deoalPLogLevel::~deoalPLogLevel(){
}


// Parameter Value
////////////////////

decString deoalPLogLevel::GetParameterValue(){
	switch(pOal.GetConfiguration().GetLogLevel()){
	case deoalConfiguration::ellError:
		return "error";
		
	case deoalConfiguration::ellWarning:
		return "warning";
		
	case deoalConfiguration::ellInfo:
		return "info";
		
	case deoalConfiguration::ellDebug:
		return "debug";
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void deoalPLogLevel::SetParameterValue(const char *value){
	const decString checkValue(decString(value).GetLower());
	if(checkValue == "error"){
		pOal.GetConfiguration().SetLogLevel(deoalConfiguration::ellError);
		
	}else if(checkValue == "warning"){
		pOal.GetConfiguration().SetLogLevel(deoalConfiguration::ellWarning);
		
	}else if(checkValue == "info"){
		pOal.GetConfiguration().SetLogLevel(deoalConfiguration::ellInfo);
		
	}else if(checkValue == "debug"){
		pOal.GetConfiguration().SetLogLevel(deoalConfiguration::ellDebug);
		
	}else{
		pOal.GetConfiguration().SetLogLevel(deoalConfiguration::ellError);
	}
}
