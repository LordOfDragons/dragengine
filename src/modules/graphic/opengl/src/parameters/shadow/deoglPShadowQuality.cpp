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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deoglPShadowQuality.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include <dragengine/common/exceptions.h>



// class deoglPShadowQuality
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPShadowQuality::deoglPShadowQuality(deGraphicOpenGl &ogl) : deoglParameter(ogl){
	SetName("shadowQuality");
	SetDescription("Quality of shadow maps. Higher quality uses higher shadow map size "
		"to improve shadow quality. Lower quality uses lower shadow map size improving performance.");
	SetType(deModuleParameter::eptSelection);
	SetCategory(ecBasic);
	SetDisplayName("Shadow Quality");
	
	const deModuleParameter::SelectionEntry entries[6] = {
		{"off", "Off", "Disable shadow mapping. This can break games so use this option "
			"only if nothing else helps." },
		{"veryLow", "Very Low", "Very low quality. For weak GPU delivering best performance."},
		{"low", "Low", "Low quality. Prefer performance over quality."},
		{"medium", "Medium", "Medium quality. Balance between quality and performance. Recommended choice."},
		{"high", "High", "High quality. Prefer quality over performance."},
		{"veryHigh", "Very High", "Very high quality. For high-end GPU delivering best quality."}
	};
	AddSelectionEntries(entries, 6);
	SetDefaultValue("high");
}

deoglPShadowQuality::~deoglPShadowQuality(){
}



// Parameter Value
////////////////////

decString deoglPShadowQuality::GetParameterValue(){
	switch(pOgl.GetConfiguration().GetShadowQuality()){
	case deoglConfiguration::esqOff:
		return "off";
		
	case deoglConfiguration::esqVeryLow:
		return "veryLow";
		
	case deoglConfiguration::esqLow:
		return "low";
		
	case deoglConfiguration::esqMedium:
		return "medium";
		
	case deoglConfiguration::esqHigh:
	default:
		return "high";
		
	case deoglConfiguration::esqVeryHigh:
		return "veryHigh";
	}
}

void deoglPShadowQuality::SetParameterValue(const char *value){
	const decString checkValue(decString(value).GetLower());
	
	if(checkValue == "off"){
		pOgl.GetConfiguration().SetShadowQuality(deoglConfiguration::esqOff);
		
	}else if(checkValue == "verylow"){
		pOgl.GetConfiguration().SetShadowQuality(deoglConfiguration::esqVeryLow);
		
	}else if(checkValue == "low"){
		pOgl.GetConfiguration().SetShadowQuality(deoglConfiguration::esqLow);
		
	}else if(checkValue == "medium"){
		pOgl.GetConfiguration().SetShadowQuality(deoglConfiguration::esqMedium);
		
	}else if(checkValue == "high"){
		pOgl.GetConfiguration().SetShadowQuality(deoglConfiguration::esqHigh);
		
	}else if(checkValue == "veryhigh"){
		pOgl.GetConfiguration().SetShadowQuality(deoglConfiguration::esqVeryHigh);
		
	}else{
		pOgl.GetConfiguration().SetShadowQuality(deoglConfiguration::esqHigh);
	}
}
