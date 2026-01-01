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

#include "gdeProperty.h"
#include "../filepattern/gdeFilePattern.h"

#include <dragengine/common/exceptions.h>



// Class gdeProperty
//////////////////////

// Constructor, destructor
////////////////////////////

gdeProperty::gdeProperty() :
pType(eptString),
pMinimumValue(0.0f),
pMaximumValue(1.0f),
pPathPatternType(epptAll),
pIdentifierUsage(false){
}

gdeProperty::gdeProperty(const char *name) :
pName(name),
pType(eptString),
pMinimumValue(0.0f),
pMaximumValue(1.0f),
pPathPatternType(epptAll),
pIdentifierUsage(false){
}

gdeProperty::gdeProperty(const gdeProperty &property) :
pName(property.pName),
pDescription(property.pDescription),
pType(property.pType),
pMinimumValue(property.pMinimumValue),
pMaximumValue(property.pMaximumValue),
pDefaultValue(property.pDefaultValue),
pOptions(property.pOptions),
pPathPatternType(property.pPathPatternType),
pIdentifierGroup(property.pIdentifierGroup),
pIdentifierUsage(property.pIdentifierUsage)
{
	property.pCustomPathPattern.Visit([&](const gdeFilePattern &p){
		pCustomPathPattern.Add(gdeFilePattern::Ref::New(p));
	});
}

gdeProperty::~gdeProperty(){
}



// Management
///////////////

void gdeProperty::SetName(const char *name){
	pName = name;
}

void gdeProperty::SetDescription(const char *description){
	pDescription = description;
}

void gdeProperty::SetType(ePropertyTypes type){
	if(type < eptString || type > eptIdentifier){
		DETHROW(deeInvalidParam);
	}
	pType = type;
}

void gdeProperty::SetMinimumValue(float value){
	pMinimumValue = value;
}

void gdeProperty::SetMaximumValue(float value){
	pMaximumValue = value;
}

void gdeProperty::SetPathPatternType(ePathPatternTypes type){
	if(type < epptAll || type > epptCustom){
		DETHROW(deeInvalidParam);
	}
	pPathPatternType = type;
}

void gdeProperty::SetIdentifierGroup(const char *group){
	pIdentifierGroup = group;
}

void gdeProperty::SetIdentifierUsage(bool usage){
	pIdentifierUsage = usage;
}

void gdeProperty::SetDefaultValue(const char *value){
	pDefaultValue = value;
}



gdeProperty &gdeProperty::operator=(const gdeProperty &property){
	pName = property.pName;
	pDescription = property.pDescription;
	pType = property.pType;
	pMinimumValue = property.pMinimumValue;
	pMaximumValue = property.pMaximumValue;
	pDefaultValue = property.pDefaultValue;
	pOptions = property.pOptions;
	pPathPatternType = property.pPathPatternType;
	pIdentifierGroup = property.pIdentifierGroup;
	pIdentifierUsage = property.pIdentifierUsage;
	
	pCustomPathPattern.RemoveAll();
	property.pCustomPathPattern.Visit([&](const gdeFilePattern &p){
		pCustomPathPattern.Add(gdeFilePattern::Ref::New(p));
	});
	
	return *this;
}
