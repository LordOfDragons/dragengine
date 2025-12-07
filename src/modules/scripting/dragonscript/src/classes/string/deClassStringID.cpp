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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "deClassStringID.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>

#include <libdscript/exceptions.h>



// Native data
////////////////

struct sStrIDNatDat{
	int index;
};



// Native functions
/////////////////////

// Constructors, destructors
//////////////////////////////

// public func new()
deClassStringID::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsStringID, DSFUNC_CONSTRUCTOR,
DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassStringID::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sStrIDNatDat &nd = *((sStrIDNatDat*)p_GetNativeData(myself));
	
	nd.index = 0; // 0 is the empty string inserted first in the table
}

// public func new( String string )
deClassStringID::nfNew2::nfNew2(const sInitData &init) :
dsFunction(init.clsStringID, DSFUNC_CONSTRUCTOR,
DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // string
}
void deClassStringID::nfNew2::RunFunction(dsRunTime *rt, dsValue *myself){
	sStrIDNatDat &nd = *((sStrIDNatDat*)p_GetNativeData(myself));
	deClassStringID &sclass = *((deClassStringID*)GetOwnerClass());
	
	nd.index = sclass.InsertString(rt->GetValue(0)->GetString());
}

// public func destructor()
deClassStringID::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsStringID, DSFUNC_DESTRUCTOR,
DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassStringID::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
}



// Management
///////////////

// public func String getString()
deClassStringID::nfGetString::nfGetString(const sInitData &init) :
dsFunction(init.clsStringID, "getString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassStringID::nfGetString::RunFunction(dsRunTime *rt, dsValue *myself){
	const int index = ((sStrIDNatDat*)p_GetNativeData(myself))->index;
	const deClassStringID &clsStringID = *((deClassStringID*)GetOwnerClass());
	
	rt->PushString(clsStringID.GetStringAt(index));
}



// File Handling
//////////////////

// static public func StringID readFromFile( FileReader reader )
deClassStringID::nfReadFromFile::nfReadFromFile(const sInitData &init) :
dsFunction(init.clsStringID, "readFromFile", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsStringID){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassStringID::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassStringID &sclass = *((deClassStringID*)GetOwnerClass());
	const deClassFileReader &clsFileReader = *sclass.GetDS().GetClassFileReader();
	
	decBaseFileReader * const reader = clsFileReader.GetFileReader(
		rt->GetValue(0)->GetRealObject());
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	sclass.PushStringID(rt, sclass.InsertString(reader->ReadString8()));
}

// public func void writeToFile( FileWriter writer )
deClassStringID::nfWriteToFile::nfWriteToFile(const sInitData &init) :
dsFunction(init.clsStringID, "writeToFile", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassStringID::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const int index = ((sStrIDNatDat*)p_GetNativeData(myself))->index;
	const deClassStringID &sclass = *((deClassStringID*)GetOwnerClass());
	const deClassFileWriter &clsFileWriter = *sclass.GetDS().GetClassFileWriter();
	
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(
		rt->GetValue(0)->GetRealObject());
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	writer->WriteString8(sclass.GetStringAt(index));
}



// public func int hashCode()
deClassStringID::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsStringID, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassStringID::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	// we simply return the index as hash code which is the perfect and fastest hash code.
	// by supporting strings in equals call we run into a potential problem. if a dictionary
	// is used the hash code of StringID and String are different causing the dictionary to
	// not find a StringID key using a String. we want though to keep the benefits of using
	// the index as hash code so we simply require the user to not fall into this trap
	rt->PushInt(((sStrIDNatDat*)p_GetNativeData(myself))->index);
}

// public func bool equals( Object object )
deClassStringID::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsStringID, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // object
}
void deClassStringID::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const int index = ((sStrIDNatDat*)p_GetNativeData(myself))->index;
	deClassStringID * const sclass = (deClassStringID*)GetOwnerClass();
	dsClass * const scString = rt->GetEngine()->GetClassString();
	dsValue * const object = rt->GetValue(0);
	
	if(object){
		if(p_IsObjOfType(object, sclass)){
			const int otherIndex = ((sStrIDNatDat*)p_GetNativeData(object))->index;
			rt->PushBool(index == otherIndex);
			
		}else if(p_IsObjOfType(object, scString)){
			rt->PushBool(sclass->GetStringAt(index) == object->GetString());
			
		}else{
			rt->PushBool(false);
		}
		
	}else{
		rt->PushBool(false);
	}
}

// public func String toString()
deClassStringID::nfToString::nfToString(const sInitData &init) :
dsFunction(init.clsStringID, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassStringID::nfToString::RunFunction(dsRunTime *rt, dsValue *myself){
	const int index = ((sStrIDNatDat*)p_GetNativeData(myself))->index;
	const deClassStringID &sclass = *((deClassStringID*)GetOwnerClass());
	
	rt->PushString(sclass.GetStringAt(index));
}



// Class deClassStringID
//////////////////////////

// Constructor
////////////////

deClassStringID::deClassStringID(deScriptingDragonScript &ds) :
dsClass("StringID", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds)
{
	// empty string is the first entry in the table
	pStrings.Add("");
	pLookUpTable.SetAt("", (void*)(intptr_t)0);
	
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sStrIDNatDat));
}

deClassStringID::~deClassStringID(){
}



// Management
///////////////

void deClassStringID::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsStringID = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNew2(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetString(init));
	
	AddFunction(new nfReadFromFile(init));
	AddFunction(new nfWriteToFile(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	AddFunction(new nfToString(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

int deClassStringID::GetStringCount() const{
	return pStrings.GetCount();
}

int deClassStringID::GetStringID(dsRealObject *myself){
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	return ((sStrIDNatDat*)p_GetNativeData(myself->GetBuffer()))->index;
}

void deClassStringID::PushStringID(dsRunTime *rt, int index){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sStrIDNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->index = index;
}

int deClassStringID::InsertString(const char *string){
	if(!string){
		DSTHROW(dueInvalidParam);
	}
	
	void *entry;
	if(pLookUpTable.GetAt(string, &entry)){
		return (int)(intptr_t)entry;
	}
	
	const int index = pStrings.GetCount();
	pStrings.Add(string);
	pLookUpTable.SetAt(string, (void*)(intptr_t)index);
	return index;
}

const decString &deClassStringID::GetStringAt(int index) const{
	return pStrings.GetAt(index);
}
