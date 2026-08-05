/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deoglPReflectionQuality.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>


// class deoglPReflectionQuality
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPReflectionQuality::deoglPReflectionQuality(deGraphicOpenGl &ogl) : deoglParameter(ogl){
	SetName("reflectionQuality");
	SetDescription("Quality of reflections.");
	SetType(deModuleParameter::eptSelection);
	SetCategory(ecBasic);
	SetDisplayName("Reflection Quality");
	
	AddSelectionEntry({"veryLow", "Very Low",
		"Very low quality. For weak GPU delivering best performance."});
	
	AddSelectionEntry({"low", "Low", "Low quality. Prefer performance over quality."});
	
	AddSelectionEntry({"medium", "Medium",
		"Medium quality. Balance between quality and performance. Recommended choice."});
	
	AddSelectionEntry({"high", "High", "High quality. Prefer quality over performance."});
	
	AddSelectionEntry({"veryHigh", "Very High",
		"Very high quality. For high-end GPU delivering best quality."});
	
	SetDefaultValue("high");
}

deoglPReflectionQuality::~deoglPReflectionQuality() = default;


// Parameter Value
////////////////////

decString deoglPReflectionQuality::GetParameterValue(){
	switch(pOgl.GetConfiguration().GetReflectionQuality()){
	case deoglConfiguration::erqVeryLow:
		return "veryLow";
		
	case deoglConfiguration::erqLow:
		return "low";
		
	case deoglConfiguration::erqMedium:
		return "medium";
		
	case deoglConfiguration::erqHigh:
	default:
		return "high";
		
	case deoglConfiguration::erqVeryHigh:
		return "veryHigh";
	}
}

void deoglPReflectionQuality::SetParameterValue(const char *value){
	const decString checkValue(decString(value).GetLower());
	
	if(checkValue == "verylow"){
		pOgl.GetConfiguration().SetReflectionQuality(deoglConfiguration::erqVeryLow);
		
	}else if(checkValue == "low"){
		pOgl.GetConfiguration().SetReflectionQuality(deoglConfiguration::erqLow);
		
	}else if(checkValue == "medium"){
		pOgl.GetConfiguration().SetReflectionQuality(deoglConfiguration::erqMedium);
		
	}else if(checkValue == "high"){
		pOgl.GetConfiguration().SetReflectionQuality(deoglConfiguration::erqHigh);
		
	}else if(checkValue == "veryhigh"){
		pOgl.GetConfiguration().SetReflectionQuality(deoglConfiguration::erqVeryHigh);
	}
}