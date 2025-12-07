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

#include "deoglPVSyncMode.h"
#include "../deGraphicOpenGl.h"
#include "../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>


// class deoglPVSyncMode
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglPVSyncMode::deoglPVSyncMode(deGraphicOpenGl &ogl) : deoglParameter(ogl){
	SetName("vsyncMode");
	SetDescription("Sets V-Sync mode. This can be overriden by GPU drivers.");
	SetType(deModuleParameter::eptSelection);
	SetCategory(ecAdvanced);
	SetDisplayName("V-Sync Mode");
	
	const deModuleParameter::SelectionEntry entries[3] = {
		{"adaptive", "Adaptive",
			"Enables V-Sync if frame rate is higher than monitor frame rate. "
			"Disables V-Sync if frame rate is lower than monitor frame rate. "
			"This can prevent stuttering due to frame rate dips. "
			"If system does not support V-Sync control behaves the same as 'On'."},
		
		{"on", "On", "Enables V-Sync."},
		
		{"off", "Off",
			"Disables V-Sync. "
			"If system does not support V-Sync control behaves the same as 'On'."}
	};
	AddSelectionEntries(entries, 3);
	SetDefaultValue("adaptive");
}

deoglPVSyncMode::~deoglPVSyncMode(){
}



// Parameter Value
////////////////////

decString deoglPVSyncMode::GetParameterValue(){
	switch(pOgl.GetConfiguration().GetVSyncMode()){
	case deoglConfiguration::evsmAdaptive:
		return "adaptive";
		
	case deoglConfiguration::evsmOn:
		return "on";
		
	case deoglConfiguration::evsmOff:
		return "off";
		
	default:
		return "adaptive";
	}
}

void deoglPVSyncMode::SetParameterValue(const char *value){
	const decString checkValue(decString(value).GetLower());
	
	if(checkValue == "adaptive"){
		pOgl.GetConfiguration().SetVSyncMode(deoglConfiguration::evsmAdaptive);
		
	}else if(checkValue == "on"){
		pOgl.GetConfiguration().SetVSyncMode(deoglConfiguration::evsmOn);
		
	}else if(checkValue == "off"){
		pOgl.GetConfiguration().SetVSyncMode(deoglConfiguration::evsmOff);
		
	}else{
		pOgl.GetConfiguration().SetVSyncMode(deoglConfiguration::evsmAdaptive);
	}
}
