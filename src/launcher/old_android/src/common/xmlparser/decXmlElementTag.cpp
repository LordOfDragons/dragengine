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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decXmlElementTag.h"
#include "decXmlAttValue.h"
#include "decXmlCDSect.h"
#include "decXmlCharacterData.h"
#include "decXmlVisitor.h"
#include "../exceptions.h"
#include "../string/decStringList.h"



// Class decXmlElementTag
///////////////////////////

// Constructor, destructor
////////////////////////////

decXmlElementTag::decXmlElementTag(const char *name){
	SetName(name);
}

decXmlElementTag::~decXmlElementTag(){
}



// Management
///////////////

void decXmlElementTag::SetName(const char *name){
	pName = name;
	
	const decStringList parts(pName.Split(':'));
	
	if(parts.GetCount() == 2){
		pNamespace = parts.GetAt(0);
		pLocalName = parts.GetAt(1);
		
	}else if(parts.GetCount() < 2){
		pNamespace.Empty();
		pLocalName = pName;
		
	}else{
		DETHROW(deeInvalidParam);
	}
}



decXmlCharacterData *decXmlElementTag::GetFirstData() const{
	const int count = GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		decXmlElement &element = *GetElementAt(i);
		
		if(element.CanCastToCharacterData()){
			return element.CastToCharacterData();
			
		}else if(element.CanCastToCDSect()){
			return element.CastToCDSect();
		}
	}
	
	return NULL;
}

decXmlElementTag *decXmlElementTag::GetElementIfTag(int index) const{
	decXmlElement &element = *GetElementAt(index);
	
	if(element.CanCastToElementTag()){
		return element.CastToElementTag();
		
	}else{
		return NULL;
	}
}

decXmlAttValue *decXmlElementTag::FindAttribute(const char *name) const{
	const int count = GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		decXmlElement &element = *GetElementAt(i);
		if(! element.CanCastToAttValue()){
			continue;
		}
		
		decXmlAttValue * const attribute = element.CastToAttValue();
		if(attribute->GetName() == name){
			return attribute;
		}
	}
	
	return NULL;
}



void decXmlElementTag::Visit(decXmlVisitor &visitor){
	visitor.VisitElementTag(*this);
}


bool decXmlElementTag::CanCastToElementTag() const{
	return true;
}

decXmlElementTag *decXmlElementTag::CastToElementTag(){
	return this;
}
