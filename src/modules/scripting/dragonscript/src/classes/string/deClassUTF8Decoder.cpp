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

#include "deClassUTF8Decoder.h"
#include "../../deClassPathes.h"

#include <dragengine/common/string/unicode/decUTF8Decoder.h>
#include <libdscript/exceptions.h>



// native structure
struct sU8DNatDat{
	decUTF8Decoder *decoder;
};



// native functions
/////////////////////



// Constructors, destructors
//////////////////////////////

// public func new( String string, int position )
deClassUTF8Decoder::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsU8D,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // string
	p_AddParameter(init.clsInt); // position
}
void deClassUTF8Decoder::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sU8DNatDat *nd = (sU8DNatDat*)p_GetNativeData(myself);
	// clear ( important )
	nd->decoder = NULL;
	nd->decoder = new decUTF8Decoder;
	if(! nd->decoder) DSTHROW(dueOutOfMemory);
	nd->decoder->SetString(rt->GetValue(0)->GetString());
	nd->decoder->SetPosition(rt->GetValue(1)->GetInt());
}

// public func destructor()
deClassUTF8Decoder::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsU8D,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassUTF8Decoder::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sU8DNatDat *nd = (sU8DNatDat*)p_GetNativeData(myself);
	
	if(nd->decoder){
		delete nd->decoder;
		nd->decoder = NULL;
	}
}



// Accessors
//////////////

// public func String getString()
deClassUTF8Decoder::nfGetString::nfGetString(const sInitData &init) : dsFunction(init.clsU8D,
"getString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassUTF8Decoder::nfGetString::RunFunction(dsRunTime *rt, dsValue *myself){
	decUTF8Decoder *decoder = ((sU8DNatDat*)p_GetNativeData(myself))->decoder;
	rt->PushString(decoder->GetString());
}

// public func int getLength()
deClassUTF8Decoder::nfGetLength::nfGetLength(const sInitData &init) : dsFunction(init.clsU8D,
"getLength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassUTF8Decoder::nfGetLength::RunFunction(dsRunTime *rt, dsValue *myself){
	decUTF8Decoder *decoder = ((sU8DNatDat*)p_GetNativeData(myself))->decoder;
	rt->PushInt(decoder->GetLength());
}

// public func int getPosition()
deClassUTF8Decoder::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsU8D,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassUTF8Decoder::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	decUTF8Decoder *decoder = ((sU8DNatDat*)p_GetNativeData(myself))->decoder;
	rt->PushInt(decoder->GetPosition());
}

// public func bool hasReachedEnd()
deClassUTF8Decoder::nfHasReachedEnd::nfHasReachedEnd(const sInitData &init) : dsFunction(init.clsU8D,
"hasReachedEnd", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassUTF8Decoder::nfHasReachedEnd::RunFunction(dsRunTime *rt, dsValue *myself){
	decUTF8Decoder *decoder = ((sU8DNatDat*)p_GetNativeData(myself))->decoder;
	rt->PushBool(decoder->HasReachedEnd());
}

// public func void setString( String string )
deClassUTF8Decoder::nfSetString::nfSetString(const sInitData &init) : dsFunction(init.clsU8D,
"setString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // string
}
void deClassUTF8Decoder::nfSetString::RunFunction(dsRunTime *rt, dsValue *myself){
	decUTF8Decoder *decoder = ((sU8DNatDat*)p_GetNativeData(myself))->decoder;
	decoder->SetString(rt->GetValue(0)->GetString());
}

// public func void setPosition( int position )
deClassUTF8Decoder::nfSetPosition::nfSetPosition(const sInitData &init) : dsFunction(init.clsU8D,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // position
}
void deClassUTF8Decoder::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	decUTF8Decoder *decoder = ((sU8DNatDat*)p_GetNativeData(myself))->decoder;
	decoder->SetPosition(rt->GetValue(0)->GetInt());
}

// public func int decodeNextCharacter()
deClassUTF8Decoder::nfDecodeNextCharacter::nfDecodeNextCharacter(const sInitData &init) : dsFunction(init.clsU8D,
"decodeNextCharacter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassUTF8Decoder::nfDecodeNextCharacter::RunFunction(dsRunTime *rt, dsValue *myself){
	decUTF8Decoder *decoder = ((sU8DNatDat*)p_GetNativeData(myself))->decoder;
	rt->PushInt(decoder->DecodeNextCharacter());
}



// public func bool equals( Object object )
deClassUTF8Decoder::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsU8D, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // object
}
void deClassUTF8Decoder::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	decUTF8Decoder *decoder = ((sU8DNatDat*)p_GetNativeData(myself))->decoder;
	deClassUTF8Decoder *clsU8D = (deClassUTF8Decoder*)GetOwnerClass();
	dsValue *object = rt->GetValue(0);
	
	if(! p_IsObjOfType(object, clsU8D)){
		rt->PushBool(false);
	}else{
		decUTF8Decoder *otherString = ((sU8DNatDat*)p_GetNativeData(object))->decoder;
		rt->PushBool(decoder == otherString);
	}
}

// public func int hashCode()
deClassUTF8Decoder::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsU8D, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassUTF8Decoder::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	decUTF8Decoder *decoder = ((sU8DNatDat*)p_GetNativeData(myself))->decoder;
	rt->PushInt((int)(intptr_t)decoder);
}



// class deClassUTF8Decoder
/////////////////////////////

// constructor
////////////////

deClassUTF8Decoder::deClassUTF8Decoder() : dsClass("UTF8Decoder", DSCT_CLASS,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	p_SetNativeDataSize(sizeof(sU8DNatDat));
}

deClassUTF8Decoder::~deClassUTF8Decoder(){
}



// management
///////////////

void deClassUTF8Decoder::CreateClassMembers(dsEngine *engine){
	sInitData init;
	// store classes
	init.clsU8D = this;
	init.clsObj = engine->GetClassObject();
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsInt = engine->GetClassInt();
	init.clsStr = engine->GetClassString();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetString(init));
	AddFunction(new nfGetLength(init));
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfHasReachedEnd(init));
	AddFunction(new nfSetString(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfDecodeNextCharacter(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

decUTF8Decoder &deClassUTF8Decoder::GetUTF8Decoder(dsRealObject *myself) const{
	sU8DNatDat *nd = (sU8DNatDat*)p_GetNativeData(myself->GetBuffer());
	return *nd->decoder;
}
