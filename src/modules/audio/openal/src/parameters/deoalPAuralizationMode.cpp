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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "deoalPAuralizationMode.h"
#include "../deAudioOpenAL.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoalPAuralizationMode
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalPAuralizationMode::deoalPAuralizationMode(deAudioOpenAL &oal) : deoalParameter(oal)
{
	SetName("auralizationMode");
	SetDescription("Auralization mode influences how sound is calculate for the listener.");
	SetType(deModuleParameter::eptSelection);
	
	const deModuleParameter::SelectionEntry entries[3] = {
		{"disabled", "Disabled", 
			"Auralization disabled. Direct sound without any material based effects. "
			"This mode is enforced if APU has no EFX support or EFX support has been disabled." },
		{"directSound", "Direct Sound",
			"Direct sound auralization only. Direct sound effect based on materials are used. "
			"This includes muffling of sound over different frquency bands. Uses single collision "
			"test along sound direction from source to listener to calculate the result."},
		{"full", "Full auralization",
			"Full auralization. Enables all auralization effects. Uses ray-tracing to calculate "
			"various parameters. This is expensive depending on the hardware used."}
	};
	
	AddSelectionEntries(entries, 3);
	
	SetCategory(ecBasic);
	SetDisplayName("Auralization Mode");
	SetDefaultValue("full");
}

deoalPAuralizationMode::~deoalPAuralizationMode(){
}



// Management
///////////////

decString deoalPAuralizationMode::GetParameterValue(){
	switch(pOal.GetConfiguration().GetAuralizationMode()){
	case deoalConfiguration::eamDisabled:
		return "disabled";
		
	case deoalConfiguration::eamDirectSound:
		return "directSound";
		
	case deoalConfiguration::eamFull:
		return "full";
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void deoalPAuralizationMode::SetParameterValue(const char *value){
	const decString svalue(decString(value).GetLower());
	if(svalue.EqualsInsensitive("disabled")){
		pOal.GetConfiguration().SetAuralizationMode(deoalConfiguration::eamDisabled);
		
	}else if(svalue.EqualsInsensitive("directsound")){
		pOal.GetConfiguration().SetAuralizationMode(deoalConfiguration::eamDirectSound);
		
	}else if(svalue.EqualsInsensitive("full")){
		pOal.GetConfiguration().SetAuralizationMode(deoalConfiguration::eamFull);
		
	}else{
		pOal.GetConfiguration().SetAuralizationMode(deoalConfiguration::eamDisabled);
	}
}
