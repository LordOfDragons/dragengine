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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debnPLogLevel.h"
#include "../../deNetworkBasic.h"
#include "../../configuration/debnConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class debnPLogLevel
////////////////////////

// Constructor, destructor
////////////////////////////

debnPLogLevel::debnPLogLevel(deNetworkBasic &network) :
debnParameter(network)
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

debnPLogLevel::~debnPLogLevel(){
}



// Parameter Value
////////////////////

decString debnPLogLevel::GetParameterValue(){
	switch(pNetwork.GetConfiguration().GetLogLevel()){
	case debnConfiguration::ellError:
		return "error";
		
	case debnConfiguration::ellWarning:
		return "warning";
		
	case debnConfiguration::ellInfo:
		return "info";
		
	case debnConfiguration::ellDebug:
		return "debug";
		
	default:
		return "warning";
	}
}

void debnPLogLevel::SetParameterValue(const char *value){
	const decString checkValue(decString(value).GetLower());
	if(checkValue == "error"){
		pNetwork.GetConfiguration().SetLogLevel(debnConfiguration::ellError);
		
	}else if(checkValue == "warning"){
		pNetwork.GetConfiguration().SetLogLevel(debnConfiguration::ellWarning);
		
	}else if(checkValue == "info"){
		pNetwork.GetConfiguration().SetLogLevel(debnConfiguration::ellInfo);
		
	}else if(checkValue == "debug"){
		pNetwork.GetConfiguration().SetLogLevel(debnConfiguration::ellDebug);
		
	}else{
		pNetwork.GetConfiguration().SetLogLevel(debnConfiguration::ellWarning);
	}
}
