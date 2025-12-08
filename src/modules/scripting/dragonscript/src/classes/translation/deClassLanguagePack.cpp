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

#include <libdscript/libdscript.h>

#include "deClassLanguagePack.h"
#include "../string/deClassUnicodeString.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <libdscript/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/localization/deLanguagePackManager.h>


// Native Structure
/////////////////////

struct sLPNatDat{
	deLanguagePack::Ref langPack;
};



// Native functions
/////////////////////


// Constructors, Destructor
/////////////////////////////

// public func new( String filename )
deClassLanguagePack::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsLP, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
}
void deClassLanguagePack::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sLPNatDat * const nd = (sLPNatDat*)p_GetNativeData(myself);
	const deScriptingDragonScript &ds = ((deClassLanguagePack*)GetOwnerClass())->GetDS();
	deLanguagePackManager &lpmgr = *ds.GetGameEngine()->GetLanguagePackManager();
	
	// clear ( important )
	nd->langPack = NULL;
	
	// create langPack
	nd->langPack = lpmgr.LoadLanguagePack(rt->GetValue(0)->GetString());
}

// public func destructor()
deClassLanguagePack::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsLP, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassLanguagePack::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sLPNatDat &nd = *((sLPNatDat*)p_GetNativeData(myself));
	if(!nd.langPack){
		return;
	}
	
	nd.langPack->FreeReference();
	nd.langPack = NULL;
}



// management
///////////////

// public func String getIdentifier()
deClassLanguagePack::nfGetIdentifier::nfGetIdentifier(const sInitData &init) :
dsFunction(init.clsLP, "getIdentifier", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassLanguagePack::nfGetIdentifier::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLanguagePack &langPack = *(((sLPNatDat*)p_GetNativeData(myself))->langPack);
	rt->PushString(langPack.GetIdentifier());
}

// public func UnicodeString getName()
deClassLanguagePack::nfGetName::nfGetName(const sInitData &init) :
dsFunction(init.clsLP, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsUS){
}
void deClassLanguagePack::nfGetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLanguagePack &langPack = *(((sLPNatDat*)p_GetNativeData(myself))->langPack);
	deScriptingDragonScript &ds = ((deClassLanguagePack*)GetOwnerClass())->GetDS();
	
	ds.GetClassUnicodeString()->PushUnicodeString(rt, langPack.GetName());
}

// public func UnicodeString getDescription()
deClassLanguagePack::nfGetDescription::nfGetDescription(const sInitData &init) :
dsFunction(init.clsLP, "getDescription", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsUS){
}
void deClassLanguagePack::nfGetDescription::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLanguagePack &langPack = *(((sLPNatDat*)p_GetNativeData(myself))->langPack);
	deScriptingDragonScript &ds = ((deClassLanguagePack*)GetOwnerClass())->GetDS();
	
	ds.GetClassUnicodeString()->PushUnicodeString(rt, langPack.GetDescription());
}

// public func String getFilename()
deClassLanguagePack::nfGetFilename::nfGetFilename(const sInitData &init) :
dsFunction(init.clsLP, "getFilename", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassLanguagePack::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLanguagePack &langPack = *(((sLPNatDat*)p_GetNativeData(myself))->langPack);
	rt->PushString(langPack.GetFilename());
}



// public func UnicodeString translate( String name )
deClassLanguagePack::nfTranslate::nfTranslate(const sInitData &init) :
dsFunction(init.clsLP, "translate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsUS){
	p_AddParameter(init.clsStr); // name
}
void deClassLanguagePack::nfTranslate::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLanguagePack &langPack = *(((sLPNatDat*)p_GetNativeData(myself))->langPack);
	deScriptingDragonScript &ds = ((deClassLanguagePack*)GetOwnerClass())->GetDS();
	
	const char * const name = rt->GetValue(0)->GetString();
	
	ds.GetClassUnicodeString()->PushUnicodeString(rt, langPack.Translate(name));
}

// public func UnicodeString translate2( String name, UnicodeString defaultValue )
deClassLanguagePack::nfTranslate2::nfTranslate2(const sInitData &init) :
dsFunction(init.clsLP, "translate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsUS){
	p_AddParameter(init.clsStr); // name
	p_AddParameter(init.clsUS); // defaultValue
}
void deClassLanguagePack::nfTranslate2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLanguagePack &langPack = *(((sLPNatDat*)p_GetNativeData(myself))->langPack);
	deScriptingDragonScript &ds = ((deClassLanguagePack*)GetOwnerClass())->GetDS();
	deClassUnicodeString &clsUS = *ds.GetClassUnicodeString();
	
	const char * const name = rt->GetValue(0)->GetString();
	const decUnicodeString *text = nullptr;
	
	if(langPack.Translate(name, &text)){
		clsUS.PushUnicodeString(rt, *text);
		
	}else{
		dsRealObject * const objDefaultValue = rt->GetValue(1)->GetRealObject();
		if(objDefaultValue){
			clsUS.PushUnicodeString(rt, clsUS.GetUnicodeString(objDefaultValue));
			
		}else{
			rt->PushObject(nullptr, &clsUS);
		}
	}
}

// public func UnicodeString getMissingText()
deClassLanguagePack::nfGetMissingText::nfGetMissingText(const sInitData &init) :
dsFunction(init.clsLP, "getMissingText", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS){
}

void deClassLanguagePack::nfGetMissingText::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLanguagePack &langPack = *(((sLPNatDat*)p_GetNativeData(myself))->langPack);
	deScriptingDragonScript &ds = ((deClassLanguagePack*)GetOwnerClass())->GetDS();
	
	ds.GetClassUnicodeString()->PushUnicodeString(rt, langPack.GetMissingText());
}



// public func int hashCode()
deClassLanguagePack::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsLP, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassLanguagePack::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLanguagePack * const langPack = ((sLPNatDat*)p_GetNativeData(myself))->langPack;
	
	// hash code = memory location
	rt->PushInt((int)(intptr_t)langPack);
}

// public func bool equals( Object obj )
deClassLanguagePack::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsLP, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassLanguagePack::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLanguagePack * const langPack = ((sLPNatDat*)p_GetNativeData(myself))->langPack;
	deClassLanguagePack * const clsLP = (deClassLanguagePack*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsLP)){
		rt->PushBool(false);
		
	}else{
		const deLanguagePack * const other = ((sLPNatDat*)p_GetNativeData(obj))->langPack;
		rt->PushBool(langPack->GetName() == other->GetName());
	}
}



// Class deClassLanguagePack
//////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassLanguagePack::deClassLanguagePack(deScriptingDragonScript &ds) :
dsClass("LanguagePack", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sLPNatDat));
}

deClassLanguagePack::~deClassLanguagePack(){
}



// Management
///////////////

void deClassLanguagePack::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsLP = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsUS = pDS.GetClassUnicodeString();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetIdentifier(init));
	AddFunction(new nfGetName(init));
	AddFunction(new nfGetDescription(init));
	AddFunction(new nfGetFilename(init));
	
	AddFunction(new nfTranslate(init));
	AddFunction(new nfTranslate2(init));
	AddFunction(new nfGetMissingText(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	CalcMemberOffsets();
}

deLanguagePack *deClassLanguagePack::GetLanguagePack(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sLPNatDat*)p_GetNativeData(myself->GetBuffer()))->langPack;
}

void deClassLanguagePack::PushLanguagePack(dsRunTime *rt, deLanguagePack::Ref langPack){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!langPack){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sLPNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->langPack = langPack;
}
