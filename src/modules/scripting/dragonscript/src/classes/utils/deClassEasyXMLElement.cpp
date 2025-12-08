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
#include <stdint.h>
#include <string.h>

#include "deClassEasyXMLElement.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"
#include "../../xml/dedsXmlDocument.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlCDSect.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlComment.h>
#include <dragengine/common/xmlparser/decXmlContainer.h>
#include <dragengine/common/xmlparser/decXmlElement.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>

#include <libdscript/libdscript.h>
#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassBlock.h>
#include <libdscript/packages/default/dsClassEnumeration.h>


// Native structure
struct sXMLElNatDat{
	decXmlElement::Ref element;
};



// Native functions
/////////////////////

// public func new( EasyXMLElementType type )
deClassEasyXMLElement::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsXmlElement, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsEasyXMLElementType); // type
}
void deClassEasyXMLElement::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sXMLElNatDat &nd = *((sXMLElNatDat*)p_GetNativeData(myself));
	
	// clear (important)
	nd.element = NULL;
	
	// create xml element
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const dedsXmlDocument::eElementTypes type = (dedsXmlDocument::eElementTypes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(type){
	case dedsXmlDocument::eetCData:
		nd.element.TakeOver(new decXmlCharacterData(""));
		break;
		
	case dedsXmlDocument::eetTag:
		nd.element.TakeOver(new decXmlElementTag(""));
		break;
		
	case dedsXmlDocument::eetComment:
		nd.element.TakeOver(new decXmlComment(""));
		break;
		
	case dedsXmlDocument::eetElement:
	default:
		DSTHROW(dueInvalidParam);
	}
}

// public func destructor()
deClassEasyXMLElement::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsXmlElement, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEasyXMLElement::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sXMLElNatDat &nd = *((sXMLElNatDat*)p_GetNativeData(myself));
	
	if(nd.element){
		nd.element->FreeReference();
		nd.element = NULL;
	}
}



// Management
///////////////

// public func int getLine()
deClassEasyXMLElement::nfGetLine::nfGetLine(const sInitData &init) :
dsFunction(init.clsXmlElement, "getLine", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassEasyXMLElement::nfGetLine::RunFunction(dsRunTime *rt, dsValue *myself){
	const decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	rt->PushInt(element.GetLineNumber());
}

// public func int getPosition()
deClassEasyXMLElement::nfGetPosition::nfGetPosition(const sInitData &init) :
dsFunction(init.clsXmlElement, "getPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassEasyXMLElement::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	rt->PushInt(element.GetPositionNumber());
}

// public func EasyXMLElementType getType()
deClassEasyXMLElement::nfGetType::nfGetType(const sInitData &init) :
dsFunction(init.clsXmlElement, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsEasyXMLElementType){
}
void deClassEasyXMLElement::nfGetType::RunFunction(dsRunTime *rt, dsValue *myself){
	const decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	dedsXmlDocument::eElementTypes type = dedsXmlDocument::eetElement;
	
	if(element.CanCastToElementTag()){
		type = dedsXmlDocument::eetTag;
		
	}else if(element.CanCastToCharacterData()){
		type = dedsXmlDocument::eetCData;
		
	}else if(element.CanCastToCDSect()){
		type = dedsXmlDocument::eetCData;
		
	}else if(element.CanCastToComment()){
		type = dedsXmlDocument::eetComment;
		
	}else{
	}
	
	rt->PushValue(((deClassEasyXMLElement*)GetOwnerClass())
		->GetClassEasyXMLElementType()->GetVariable(type)->GetStaticValue());
}



// public func String getTagName()
deClassEasyXMLElement::nfGetTagName::nfGetTagName(const sInitData &init) :
dsFunction(init.clsXmlElement, "getTagName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassEasyXMLElement::nfGetTagName::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	rt->PushString(element.CastToElementTag()->GetName());
}

// public func void setTagName( String name )
deClassEasyXMLElement::nfSetTagName::nfSetTagName(const sInitData &init) :
dsFunction(init.clsXmlElement, "setTagName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
}
void deClassEasyXMLElement::nfSetTagName::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	
	element.CastToElementTag()->SetName(name);
}



// public func String getCDataContent()
deClassEasyXMLElement::nfGetCDataContent::nfGetCDataContent(const sInitData &init) :
dsFunction(init.clsXmlElement, "getCDataContent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassEasyXMLElement::nfGetCDataContent::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	if(element.CanCastToCharacterData()){
		rt->PushString(element.CastToCharacterData()->GetData());
		
	}else if(element.CanCastToCDSect()){
		rt->PushString(element.CastToCDSect()->GetData());
		
	}else{
		DSTHROW(dueInvalidParam);
	}
}

// public func void setCDataContent( String content )
deClassEasyXMLElement::nfSetCDataContent::nfSetCDataContent(const sInitData &init) :
dsFunction(init.clsXmlElement, "setCDataContent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // content
}
void deClassEasyXMLElement::nfSetCDataContent::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const content = rt->GetValue(0)->GetString();
	
	if(element.CanCastToCharacterData()){
		element.CastToCharacterData()->SetData(content);
		
	}else if(element.CanCastToCDSect()){
		element.CastToCDSect()->SetData(content);
		
	}else{
		DSTHROW(dueInvalidParam);
	}
}



// public func String getComment()
deClassEasyXMLElement::nfGetComment::nfGetComment(const sInitData &init) :
dsFunction(init.clsXmlElement, "getComment", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassEasyXMLElement::nfGetComment::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	rt->PushString(element.CastToComment()->GetComment());
}

// public func void setComment( String content )
deClassEasyXMLElement::nfSetComment::nfSetComment(const sInitData &init) :
dsFunction(init.clsXmlElement, "setComment", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // content
}
void deClassEasyXMLElement::nfSetComment::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const content = rt->GetValue(0)->GetString();
	element.CastToComment()->SetComment(content);
}


	
// Attributes
///////////////

// public func int getAttributeCount()
deClassEasyXMLElement::nfGetAttributeCount::nfGetAttributeCount(const sInitData &init) :
dsFunction(init.clsXmlElement, "getAttributeCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassEasyXMLElement::nfGetAttributeCount::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int attributeCount = 0;
	int i;
	
	for(i=0; i<elementCount; i++){
		if(container.GetElementAt(i)->CanCastToAttValue()){
			attributeCount++;
		}
	}

	rt->PushInt(attributeCount);
}

// public func String getAttributeNamed( String name )
deClassEasyXMLElement::nfGetAttributeNamed::nfGetAttributeNamed(const sInitData &init) :
dsFunction(init.clsXmlElement, "getAttributeNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
	p_AddParameter(init.clsString); // name
}
void deClassEasyXMLElement::nfGetAttributeNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	const decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		if(!next.CanCastToAttValue()){
			continue;
		}
		
		const decXmlAttValue &attribute = *next.CastToAttValue();
		if(attribute.GetName() == name){
			rt->PushString(attribute.GetValue());
			return;
		}
	}

	DSTHROW(dueInvalidParam);
}

// public func int getAttributeIntNamed( String name )
deClassEasyXMLElement::nfGetAttributeIntNamed::nfGetAttributeIntNamed(const sInitData &init) :
dsFunction(init.clsXmlElement, "getAttributeIntNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
	p_AddParameter(init.clsString); // name
}
void deClassEasyXMLElement::nfGetAttributeIntNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	const decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		if(!next.CanCastToAttValue()){
			continue;
		}
		
		const decXmlAttValue &attribute = *next.CastToAttValue();
		if(attribute.GetName() == name){
			rt->PushInt(attribute.GetValue().ToInt());
			return;
		}
	}

	DSTHROW(dueInvalidParam);
}

// public func float getAttributeFloatNamed( String name )
deClassEasyXMLElement::nfGetAttributeFloatNamed::nfGetAttributeFloatNamed(const sInitData &init) :
dsFunction(init.clsXmlElement, "getAttributeFloatNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsString); // name
}
void deClassEasyXMLElement::nfGetAttributeFloatNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	const decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		if(!next.CanCastToAttValue()){
			continue;
		}
		
		const decXmlAttValue &attribute = *next.CastToAttValue();
		if(attribute.GetName() == name){
			rt->PushFloat(attribute.GetValue().ToFloat());
			return;
		}
	}

	DSTHROW(dueInvalidParam);
}

// public func bool getAttributeBoolNamed( String name )
deClassEasyXMLElement::nfGetAttributeBoolNamed::nfGetAttributeBoolNamed(const sInitData &init) :
dsFunction(init.clsXmlElement, "getAttributeBoolNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsString); // name
}
void deClassEasyXMLElement::nfGetAttributeBoolNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	const decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		if(!next.CanCastToAttValue()){
			continue;
		}
		
		const decXmlAttValue &attribute = *next.CastToAttValue();
		if(attribute.GetName() == name){
			rt->PushBool(attribute.GetValue() == "true"
				|| attribute.GetValue() == "yes"
				|| attribute.GetValue() == "1");
			return;
		}
	}

	DSTHROW(dueInvalidParam);
}

// public func bool hasAttributeNamed( String name )
deClassEasyXMLElement::nfHasAttributeNamed::nfHasAttributeNamed(const sInitData &init) :
dsFunction(init.clsXmlElement, "hasAttributeNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsString); // name
}
void deClassEasyXMLElement::nfHasAttributeNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	const decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		if(!next.CanCastToAttValue()){
			continue;
		}
		
		const decXmlAttValue &attribute = *next.CastToAttValue();
		if(attribute.GetName() == name){
			rt->PushBool(true);
			return;
		}
	}
	
	rt->PushBool(false);
}

// public func void setAttribute( String name, String value )
deClassEasyXMLElement::nfSetAttributeString::nfSetAttributeString(const sInitData &init) :
dsFunction(init.clsXmlElement, "setAttribute", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsString); // value
}
void deClassEasyXMLElement::nfSetAttributeString::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	const char * const value = rt->GetValue(1)->GetString();
	decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		if(!next.CanCastToAttValue()){
			continue;
		}
		
		decXmlAttValue &attribute = *next.CastToAttValue();
		if(attribute.GetName() == name){
			attribute.SetValue(value);
			return;
		}
	}
	
	decXmlAttValue::Ref attribute = NULL;
	
	try{
		attribute.TakeOver(new decXmlAttValue(name));
		attribute->SetValue(value);
		container.AddElement(attribute);
	}catch(...){
		throw;
	}
}

// public func void setAttribute( String name, int value )
deClassEasyXMLElement::nfSetAttributeInteger::nfSetAttributeInteger(const sInitData &init) :
dsFunction(init.clsXmlElement, "setAttribute", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsInteger); // value
}
void deClassEasyXMLElement::nfSetAttributeInteger::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	const int value = rt->GetValue(1)->GetInt();
	decXmlContainer &container = *element.CastToContainer();
	
	decString string;
	string.Format("%d", value);
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		if(!next.CanCastToAttValue()){
			continue;
		}
		
		decXmlAttValue &attribute = *next.CastToAttValue();
		if(attribute.GetName() == name){
			attribute.SetValue(string);
			return;
		}
	}
	
	decXmlAttValue::Ref attribute = NULL;
	
	try{
		attribute.TakeOver(new decXmlAttValue(name));
		attribute->SetValue(string);
		container.AddElement(attribute);
	}catch(...){
		throw;
	}
}

// public func void setAttribute( String name, float value )
deClassEasyXMLElement::nfSetAttributeFloat::nfSetAttributeFloat(const sInitData &init) :
dsFunction(init.clsXmlElement, "setAttribute", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsFloat); // value
}
void deClassEasyXMLElement::nfSetAttributeFloat::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	const float value = rt->GetValue(1)->GetFloat();
	decXmlContainer &container = *element.CastToContainer();
	
	decString string;
	string.Format("%.5g", value);
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		if(!next.CanCastToAttValue()){
			continue;
		}
		
		decXmlAttValue &attribute = *next.CastToAttValue();
		if(attribute.GetName() == name){
			attribute.SetValue(string);
			return;
		}
	}
	
	decXmlAttValue::Ref attribute = NULL;
	
	try{
		attribute.TakeOver(new decXmlAttValue(name));
		attribute->SetValue(string);
		container.AddElement(attribute);
	}catch(...){
		throw;
	}
}

// public func void setAttribute( String name, bool value )
deClassEasyXMLElement::nfSetAttributeBool::nfSetAttributeBool(const sInitData &init) :
dsFunction(init.clsXmlElement, "setAttribute", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsBool); // value
}
void deClassEasyXMLElement::nfSetAttributeBool::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	const bool value = rt->GetValue(1)->GetBool();
	decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		if(!next.CanCastToAttValue()){
			continue;
		}
		
		decXmlAttValue &attribute = *next.CastToAttValue();
		if(attribute.GetName() == name){
			attribute.SetValue(value ? "true" : "false");
			return;
		}
	}
	
	decXmlAttValue::Ref attribute = NULL;
	
	try{
		attribute.TakeOver(new decXmlAttValue(name));
		attribute->SetValue(value ? "true" : "false");
		container.AddElement(attribute);
	}catch(...){
		throw;
	}
}

// public func void removeAttributeNamed( String name )
deClassEasyXMLElement::nfRemoveAttributeNamed::nfRemoveAttributeNamed(const sInitData &init) :
dsFunction(init.clsXmlElement, "removeAttributeNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
}
void deClassEasyXMLElement::nfRemoveAttributeNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const name = rt->GetValue(0)->GetString();
	decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		if(!next.CanCastToAttValue()){
			continue;
		}
		
		decXmlAttValue * const attribute = next.CastToAttValue();
		if(attribute->GetName() == name){
			container.RemoveElement(attribute);
			return;
		}
	}
}

// public func void removeAllAttributes()
deClassEasyXMLElement::nfRemoveAllAttributes::nfRemoveAllAttributes(const sInitData &init) :
dsFunction(init.clsXmlElement, "removeAllAttributes", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEasyXMLElement::nfRemoveAllAttributes::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	decXmlContainer &container = *element.CastToContainer();
	
	int i;
	
	for(i=0; i<container.GetElementCount(); i++){
		decXmlElement * const next = container.GetElementAt(i);
		if(next->CanCastToAttValue()){
			container.RemoveElement(next);
			i--;
		}
	}
}



// CData
//////////

// public func String getFirstCData()
deClassEasyXMLElement::nfGetFirstCData::nfGetFirstCData(const sInitData &init) :
dsFunction(init.clsXmlElement, "getFirstCData", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassEasyXMLElement::nfGetFirstCData::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);

	decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		
		if(next.CanCastToCharacterData()){
			rt->PushString(next.CastToCharacterData()->GetData());
			return;
			
		}else if(next.CanCastToCDSect()){
			rt->PushString(next.CastToCDSect()->GetData());
			return;
		}
	}
	
	rt->PushString("");
}

// public func int getFirstCDataInt()
deClassEasyXMLElement::nfGetFirstCDataInteger::nfGetFirstCDataInteger(const sInitData &init) :
dsFunction(init.clsXmlElement, "getFirstCDataInt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassEasyXMLElement::nfGetFirstCDataInteger::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	int value = 0;
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		
		if(next.CanCastToCharacterData()){
			value = next.CastToCharacterData()->GetData().ToInt();
			break;
			
		}else if(next.CanCastToCDSect()){
			value = next.CastToCDSect()->GetData().ToInt();
			break;
		}
	}
	
	rt->PushInt(value);
}

// public func int getFirstCDataFloat()
deClassEasyXMLElement::nfGetFirstCDataFloat::nfGetFirstCDataFloat(const sInitData &init) :
dsFunction(init.clsXmlElement, "getFirstCDataFloat", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassEasyXMLElement::nfGetFirstCDataFloat::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	float value = 0.0f;
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		
		if(next.CanCastToCharacterData()){
			value = next.CastToCharacterData()->GetData().ToFloat();
			break;
			
		}else if(next.CanCastToCDSect()){
			value = next.CastToCDSect()->GetData().ToFloat();
			break;
		}
	}
	
	rt->PushFloat(value);
}

// public func bool getFirstCDataBool()
deClassEasyXMLElement::nfGetFirstCDataBool::nfGetFirstCDataBool(const sInitData &init) :
dsFunction(init.clsXmlElement, "getFirstCDataBool", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassEasyXMLElement::nfGetFirstCDataBool::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	decXmlContainer &container = *element.CastToContainer();
	
	const int elementCount = container.GetElementCount();
	bool value = false;
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &next = *container.GetElementAt(i);
		
		if(next.CanCastToCharacterData()){
			value = next.CastToCharacterData()->GetData() == "true"
				|| next.CastToCharacterData()->GetData() == "yes"
				|| next.CastToCharacterData()->GetData() == "1";
			break;
			
		}else if(next.CanCastToCDSect()){
			value = next.CastToCDSect()->GetData() == "true"
				|| next.CastToCDSect()->GetData() == "yes"
				|| next.CastToCDSect()->GetData() == "1";
			break;
		}
	}
	
	rt->PushBool(value);
}



// Elements
/////////////

// public func int getElementCount()
deClassEasyXMLElement::nfGetElementCount::nfGetElementCount(const sInitData &init) :
dsFunction(init.clsXmlElement, "getElementCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassEasyXMLElement::nfGetElementCount::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	rt->PushInt(element.CastToContainer()->GetElementCount());
}

// public func EasyXMLElement getElementAt( int index )
deClassEasyXMLElement::nfGetElementAt::nfGetElementAt(const sInitData &init) :
dsFunction(init.clsXmlElement, "getElementAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsXmlElement){
	p_AddParameter(init.clsInteger); // index
}
void deClassEasyXMLElement::nfGetElementAt::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	const int index = rt->GetValue(0)->GetInt();
	
	clsXmlElement->PushElement(rt, element.CastToContainer()->GetElementAt(index));
}

// public func bool hasElement( EasyXMLElement child )
deClassEasyXMLElement::nfHasElement::nfHasElement(const sInitData &init) :
dsFunction(init.clsXmlElement, "hasElement", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsXmlElement); // child
}
void deClassEasyXMLElement::nfHasElement::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	decXmlElement * const child = clsXmlElement->GetElement(rt->GetValue(0)->GetRealObject());
	
	rt->PushBool(element.CastToContainer()->HasElement(child));
}

// public func void addElement( EasyXMLElement element )
deClassEasyXMLElement::nfAddElement::nfAddElement(const sInitData &init) :
dsFunction(init.clsXmlElement, "addElement", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsXmlElement); // element
}
void deClassEasyXMLElement::nfAddElement::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	decXmlElement * const child = clsXmlElement->GetElement(rt->GetValue(0)->GetRealObject());
	element.CastToContainer()->AddElement(child);
}

// public func void insertElement(EasyXMLElement element, int beforeIndex)
deClassEasyXMLElement::nfInsertElement::nfInsertElement(const sInitData &init) :
dsFunction(init.clsXmlElement, "insertElement", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsXmlElement); // element
	p_AddParameter(init.clsInteger); // beforeIndex
}
void deClassEasyXMLElement::nfInsertElement::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	decXmlElement * const child = clsXmlElement->GetElement(rt->GetValue(0)->GetRealObject());
	element.CastToContainer()->InsertElement(child, rt->GetValue(1)->GetInt());
}

// public func void removeElement( EasyXMLElement element )
deClassEasyXMLElement::nfRemoveElement::nfRemoveElement(const sInitData &init) :
dsFunction(init.clsXmlElement, "removeElement", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsXmlElement); // element
}
void deClassEasyXMLElement::nfRemoveElement::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	decXmlElement * const child = clsXmlElement->GetElement(rt->GetValue(0)->GetRealObject());
	element.CastToContainer()->RemoveElement(child);
}

// public func void removeAllElements()
deClassEasyXMLElement::nfRemoveAllElements::nfRemoveAllElements(const sInitData &init) :
dsFunction(init.clsXmlElement, "removeAllElements", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEasyXMLElement::nfRemoveAllElements::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	decXmlContainer &container = *element.CastToContainer();
	
	int i;
	for(i=0; i<container.GetElementCount(); i++){
		decXmlElement * const next = container.GetElementAt(i);
		if(!next->CanCastToAttValue()){
			container.RemoveElement(next);
			i--;
		}
	}
}



// public func EasyXMLElement addTag( String name )
deClassEasyXMLElement::nfAddTag::nfAddTag(const sInitData &init) :
dsFunction(init.clsXmlElement, "addTag", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsXmlElement){
	p_AddParameter(init.clsString); // name
}
void deClassEasyXMLElement::nfAddTag::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	const char * const name = rt->GetValue(0)->GetString();
	decXmlContainer &container = *element.CastToContainer();
	decXmlElementTag::Ref tag = NULL;
	
	try{
		tag.TakeOver(new decXmlElementTag(name));
		container.AddElement(tag);
		clsXmlElement->PushElement(rt, tag);
	}catch(...){
		throw;
	}
}

// public func EasyXMLElement addDataTag( String name, String value )
deClassEasyXMLElement::nfAddDataTagString::nfAddDataTagString(const sInitData &init) :
dsFunction(init.clsXmlElement, "addDataTag", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsXmlElement){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsString); // value
}
void deClassEasyXMLElement::nfAddDataTagString::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	const char * const name = rt->GetValue(0)->GetString();
	const char * const value = rt->GetValue(1)->GetString();
	decXmlContainer &container = *element.CastToContainer();
	decXmlCharacterData::Ref cdata = NULL;
	decXmlElementTag::Ref tag = NULL;
	
	try{
		tag.TakeOver(new decXmlElementTag(name));
		
		cdata.TakeOver(new decXmlCharacterData(value));
		tag->AddElement(cdata);
		container.AddElement(tag);
		clsXmlElement->PushElement(rt, tag);
	}catch(...){
		throw;
	}
}

// public func EasyXMLElement addDataTag( String name, int value )
deClassEasyXMLElement::nfAddDataTagInteger::nfAddDataTagInteger(const sInitData &init) :
dsFunction(init.clsXmlElement, "addDataTag", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsXmlElement){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsInteger); // value
}
void deClassEasyXMLElement::nfAddDataTagInteger::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	const char * const name = rt->GetValue(0)->GetString();
	const int value = rt->GetValue(1)->GetInt();
	decXmlContainer &container = *element.CastToContainer();
	decXmlCharacterData::Ref cdata = NULL;
	decXmlElementTag::Ref tag = NULL;
	
	decString text;
	text.Format("%d", value);
	
	try{
		tag.TakeOver(new decXmlElementTag(name));
		
		cdata.TakeOver(new decXmlCharacterData(text));
		tag->AddElement(cdata);
		container.AddElement(tag);
		clsXmlElement->PushElement(rt, tag);
	}catch(...){
		throw;
	}
}

// public func EasyXMLElement addDataTag( String name, float value )
deClassEasyXMLElement::nfAddDataTagFloat::nfAddDataTagFloat(const sInitData &init) :
dsFunction(init.clsXmlElement, "addDataTag", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsXmlElement){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsFloat); // value
}
void deClassEasyXMLElement::nfAddDataTagFloat::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	const char * const name = rt->GetValue(0)->GetString();
	const float value = rt->GetValue(1)->GetFloat();
	decXmlContainer &container = *element.CastToContainer();
	decXmlCharacterData::Ref cdata = NULL;
	decXmlElementTag::Ref tag = NULL;
	
	decString text;
	text.Format("%.5g", value);
	
	try{
		tag.TakeOver(new decXmlElementTag(name));
		
		cdata.TakeOver(new decXmlCharacterData(text));
		tag->AddElement(cdata);
		container.AddElement(tag);
		clsXmlElement->PushElement(rt, tag);
	}catch(...){
		throw;
	}
}

// public func EasyXMLElement addDataTag( String name, bool value )
deClassEasyXMLElement::nfAddDataTagBool::nfAddDataTagBool(const sInitData &init) :
dsFunction(init.clsXmlElement, "addDataTag", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsXmlElement){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsBool); // value
}
void deClassEasyXMLElement::nfAddDataTagBool::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	const char * const name = rt->GetValue(0)->GetString();
	const bool value = rt->GetValue(1)->GetBool();
	decXmlContainer &container = *element.CastToContainer();
	decXmlCharacterData::Ref cdata = NULL;
	decXmlElementTag::Ref tag = NULL;
	
	try{
		tag.TakeOver(new decXmlElementTag(name));
		
		cdata.TakeOver(new decXmlCharacterData(value ? "true" : "false"));
		tag->AddElement(cdata);
		container.AddElement(tag);
		clsXmlElement->PushElement(rt, tag);
	}catch(...){
		throw;
	}
}

// public func EasyXMLElement addCData( String value )
deClassEasyXMLElement::nfAddCData::nfAddCData(const sInitData &init) :
dsFunction(init.clsXmlElement, "addCData", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsXmlElement){
	p_AddParameter(init.clsString); // value
}
void deClassEasyXMLElement::nfAddCData::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	const char * const value = rt->GetValue(0)->GetString();
	decXmlContainer &container = *element.CastToContainer();
	
	const decXmlCharacterData::Ref cdata(decXmlCharacterData::Ref::NewWith(value));
	container.AddElement(cdata);
	clsXmlElement->PushElement(rt, cdata);
}

// public func void addComment( String comment )
deClassEasyXMLElement::nfAddComment::nfAddComment(const sInitData &init) :
dsFunction(init.clsXmlElement, "addComment", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // comment
}
void deClassEasyXMLElement::nfAddComment::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const char * const text = rt->GetValue(0)->GetString();
	decXmlContainer &container = *element.CastToContainer();
	decXmlComment::Ref comment = NULL;
	
	try{
		comment.TakeOver(new decXmlComment(text));
		container.AddElement(comment);
	}catch(...){
		throw;
	}
}



// public func void forEachTag( Block ablock )
deClassEasyXMLElement::nfForEachTag::nfForEachTag(const sInitData &init) :
dsFunction(init.clsXmlElement, "forEachTag", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBlock); // block
}
void deClassEasyXMLElement::nfForEachTag::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	deClassEasyXMLElement &clsXmlElement = *((deClassEasyXMLElement*)GetOwnerClass());
	
	dsValue * const valueBlock = rt->GetValue(0);
	if(!valueBlock->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const decXmlContainer &container = *element.CastToContainer();
	const int count = container.GetElementCount();
	if(count == 0){
		return;
	}
	
	const int funcIndexRun = ((dsClassBlock*)rt->GetEngine()->GetClassBlock())->GetFuncIndexRun2();
	
	int i;
	for(i=0; i<count; i++){
		decXmlElement * const element2 = container.GetElementAt(i);
		if(!element2->CanCastToElementTag()){
			continue;
		}
		
		rt->PushString(element2->CastToElementTag()->GetName());
		clsXmlElement.PushElement(rt, element2);
		rt->RunFunctionFast(valueBlock, funcIndexRun); // Object run(Element element, String tagName)
	}
}

// public func bool hasTags()
deClassEasyXMLElement::nfHasTags::nfHasTags(const sInitData &init) :
dsFunction(init.clsXmlElement, "hasTags", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassEasyXMLElement::nfHasTags::RunFunction(dsRunTime *rt, dsValue *myself){
	decXmlElement &element = *(((sXMLElNatDat*)p_GetNativeData(myself))->element);
	
	const decXmlContainer &container = *element.CastToContainer();
	const int count = container.GetElementCount();
	if(count > 0){
		int i;
		for(i=0; i<count; i++){
			decXmlElement * const element2 = container.GetElementAt(i);
			if(element2->CanCastToElementTag()){
				rt->PushBool(true);
				return;
			}
		}
	}
	
	rt->PushBool(false);
}



// public func int hashCode()
deClassEasyXMLElement::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsXmlElement, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassEasyXMLElement::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const decXmlElement * const element = ((sXMLElNatDat*)p_GetNativeData(myself))->element;
	rt->PushInt((int)(intptr_t)element);
}

// public func bool equals( Object obj )
deClassEasyXMLElement::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsXmlElement, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassEasyXMLElement::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decXmlElement * const element = ((sXMLElNatDat*)p_GetNativeData(myself))->element;
	deClassEasyXMLElement * const clsXmlElement = (deClassEasyXMLElement*)GetOwnerClass();
	
	dsValue * const object = rt->GetValue(0);
	if(!p_IsObjOfType(object, clsXmlElement)){
		rt->PushBool(false);
		
	}else{
		decXmlElement * const other = ((sXMLElNatDat*)p_GetNativeData(object))->element;
		rt->PushBool(element == other);
	}
}



// Class deClassEasyXMLElement
////////////////////////////////

// Constructor
////////////////

deClassEasyXMLElement::deClassEasyXMLElement(deScriptingDragonScript &ds) :
dsClass("EasyXMLElement", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_XML);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sXMLElNatDat));
}

deClassEasyXMLElement::~deClassEasyXMLElement(){
}



// Management
///////////////

void deClassEasyXMLElement::CreateClassMembers(dsEngine *engine){
	pClsEasyXMLElementType = engine->GetClass("Dragengine.XML.EasyXMLElementType");
	
	sInitData init;
	init.clsXmlElement = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsBlock = engine->GetClassBlock();
	init.clsEasyXMLElementType = pClsEasyXMLElementType;
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetLine(init));
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfGetType(init));
	
	AddFunction(new nfGetTagName(init));
	AddFunction(new nfSetTagName(init));
	
	AddFunction(new nfGetCDataContent(init));
	AddFunction(new nfSetCDataContent(init));
	
	AddFunction(new nfGetComment(init));
	AddFunction(new nfSetComment(init));
	
	AddFunction(new nfGetAttributeCount(init));
	AddFunction(new nfGetAttributeNamed(init));
	AddFunction(new nfGetAttributeIntNamed(init));
	AddFunction(new nfGetAttributeFloatNamed(init));
	AddFunction(new nfGetAttributeBoolNamed(init));
	AddFunction(new nfHasAttributeNamed(init));
	AddFunction(new nfSetAttributeString(init));
	AddFunction(new nfSetAttributeInteger(init));
	AddFunction(new nfSetAttributeFloat(init));
	AddFunction(new nfSetAttributeBool(init));
	AddFunction(new nfRemoveAttributeNamed(init));
	AddFunction(new nfRemoveAllAttributes(init));
	
	AddFunction(new nfGetFirstCData(init));
	AddFunction(new nfGetFirstCDataInteger(init));
	AddFunction(new nfGetFirstCDataFloat(init));
	AddFunction(new nfGetFirstCDataBool(init));
	
	AddFunction(new nfGetElementCount(init));
	AddFunction(new nfGetElementAt(init));
	AddFunction(new nfHasElement(init));
	AddFunction(new nfAddElement(init));
	AddFunction(new nfInsertElement(init));
	AddFunction(new nfRemoveElement(init));
	AddFunction(new nfRemoveAllElements(init));
	
	AddFunction(new nfAddTag(init));
	AddFunction(new nfAddDataTagString(init));
	AddFunction(new nfAddDataTagInteger(init));
	AddFunction(new nfAddDataTagFloat(init));
	AddFunction(new nfAddDataTagBool(init));
	AddFunction(new nfAddCData(init));
	AddFunction(new nfAddComment(init));
	
	AddFunction(new nfForEachTag(init));
	AddFunction(new nfHasTags(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
}

decXmlElement *deClassEasyXMLElement::GetElement(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	return ((sXMLElNatDat*)p_GetNativeData(myself->GetBuffer()))->element;
}

void deClassEasyXMLElement::PushElement(dsRunTime *rt, decXmlElement::Ref element){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!element){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sXMLElNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->element = element;
}
