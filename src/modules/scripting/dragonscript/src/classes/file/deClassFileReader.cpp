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

#include "deClassFileReader.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decZFileReader.h>
#include <dragengine/common/string/decString.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassTimeDate.h>



struct sFReadNatDat{
	decBaseFileReader *fileReader;
	int streamVersion;
};



// Constructors, Destructors
//////////////////////////////

// public func new( String filename )
deClassFileReader::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsFRead,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
}
void deClassFileReader::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sFReadNatDat &nd = *((sFReadNatDat*)p_GetNativeData(myself));
	const deClassFileReader &clsFRead = *((deClassFileReader*)GetOwnerClass());
	deVirtualFileSystem &vfs = *clsFRead.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	decPath path;
	
	// clear ( important )
	nd.fileReader = NULL;
	nd.streamVersion = clsFRead.GetDS()->GetStreamingVersion();
	
	// check arguments
	const char *filename = rt->GetValue(0)->GetString();
	
	// create file reader
	path.SetFromUnix(filename);
	nd.fileReader = vfs.OpenFileForReading(path);
	if(!nd.fileReader){
		DSTHROW(dueOutOfMemory);
	}
}

// public static func FileReader newZCompressed(String filename)
deClassFileReader::nfNewZCompressed::nfNewZCompressed(const sInitData &init) :
dsFunction(init.clsFRead, "newZCompressed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFRead){
	p_AddParameter(init.clsStr); // filename
}
void deClassFileReader::nfNewZCompressed::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassFileReader &clsFRead = *((deClassFileReader*)GetOwnerClass());
	deVirtualFileSystem &vfs = *clsFRead.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	
	const char * const filename = rt->GetValue(0)->GetString();
	
	clsFRead.PushFileReader(rt, decZFileReader::Ref::New(
		new decZFileReader(vfs.OpenFileForReading(decPath::CreatePathUnix(filename)))));
}

// public func destructor()
deClassFileReader::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsFRead,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassFileReader::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sFReadNatDat &nd = *((sFReadNatDat*)p_GetNativeData(myself));
	
	if(nd.fileReader){
		nd.fileReader->FreeReference();
		nd.fileReader = NULL;
	}
}



// Management
///////////////

// public func String getFilename()
deClassFileReader::nfGetFilename::nfGetFilename(const sInitData &init) : dsFunction(init.clsFRead,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassFileReader::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushString(fileReader.GetFilename());
}

// public func int getLength()
deClassFileReader::nfGetLength::nfGetLength(const sInitData &init) : dsFunction(init.clsFRead,
"getLength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfGetLength::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushInt(fileReader.GetLength());
}

// public func int getPosition()
deClassFileReader::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsFRead,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushInt(fileReader.GetPosition());
}

// public func void setPosition( int position )
deClassFileReader::nfSetPosition::nfSetPosition(const sInitData &init) : dsFunction(init.clsFRead,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // position
}
void deClassFileReader::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	fileReader.SetPosition(rt->GetValue(0)->GetInt());
}

// public func void movePosition( int offset )
deClassFileReader::nfMovePosition::nfMovePosition(const sInitData &init) : dsFunction(init.clsFRead,
"movePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // offset
}
void deClassFileReader::nfMovePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	fileReader.MovePosition(rt->GetValue(0)->GetInt());
}

// public func void setPositionEnd( int position )
deClassFileReader::nfSetPositionEnd::nfSetPositionEnd(const sInitData &init) : dsFunction(init.clsFRead,
"setPositionEnd", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // position
}
void deClassFileReader::nfSetPositionEnd::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	fileReader.SetPositionEnd(rt->GetValue(0)->GetInt());
}



// public func int getStreamVersion()
deClassFileReader::nfGetStreamVersion::nfGetStreamVersion(const sInitData &init) : dsFunction(init.clsFRead,
"getStreamVersion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfGetStreamVersion::RunFunction(dsRunTime *rt, dsValue *myself){
	rt->PushInt(((const sFReadNatDat *)p_GetNativeData(myself))->streamVersion);
}

// public func void setStreamVersion( int version )
deClassFileReader::nfSetStreamVersion::nfSetStreamVersion(const sInitData &init) : dsFunction(init.clsFRead,
"setStreamVersion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // version
}
void deClassFileReader::nfSetStreamVersion::RunFunction(dsRunTime *rt, dsValue *myself){
	((sFReadNatDat*)p_GetNativeData(myself))->streamVersion = rt->GetValue(0)->GetInt();
}



// public func bool eof()
deClassFileReader::nfEOF::nfEOF(const sInitData &init) : dsFunction(init.clsFRead,
"eof", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassFileReader::nfEOF::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushBool(fileReader.IsEOF());
}

// public func int readChar()
deClassFileReader::nfReadChar::nfReadChar(const sInitData &init) : dsFunction(init.clsFRead,
"readChar", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfReadChar::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushInt((int)fileReader.ReadChar());
}

// public func int readByte()
deClassFileReader::nfReadByte::nfReadByte(const sInitData &init) : dsFunction(init.clsFRead,
"readByte", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfReadByte::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushInt((int)fileReader.ReadByte());
}

// public func int readShort()
deClassFileReader::nfReadShort::nfReadShort(const sInitData &init) : dsFunction(init.clsFRead,
"readShort", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfReadShort::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushInt((int)fileReader.ReadShort());
}

// public func int readUShort()
deClassFileReader::nfReadUShort::nfReadUShort(const sInitData &init) : dsFunction(init.clsFRead,
"readUShort", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfReadUShort::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushInt((int)fileReader.ReadUShort());
}

// public func int readInt()
deClassFileReader::nfReadInt::nfReadInt(const sInitData &init) : dsFunction(init.clsFRead,
"readInt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfReadInt::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushInt(fileReader.ReadInt());
}

// public func int readUInt()
deClassFileReader::nfReadUInt::nfReadUInt(const sInitData &init) : dsFunction(init.clsFRead,
"readUInt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfReadUInt::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushInt((int)fileReader.ReadUInt());
}

// public func int readVarUInt()
deClassFileReader::nfReadVarUInt::nfReadVarUInt(const sInitData &init) :
dsFunction(init.clsFRead, "readVarUInt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfReadVarUInt::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushInt((int)fileReader.ReadVarUInt());
}

// public func float readFloat()
deClassFileReader::nfReadFloat::nfReadFloat(const sInitData &init) : dsFunction(init.clsFRead,
"readFloat", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileReader::nfReadFloat::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	rt->PushFloat(fileReader.ReadFloat());
}

// public func String readString8()
deClassFileReader::nfReadString8::nfReadString8(const sInitData &init) : dsFunction(init.clsFRead,
"readString8", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassFileReader::nfReadString8::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	const decString string(fileReader.ReadString8());
	rt->PushString(string);
}

// public func String readString16()
deClassFileReader::nfReadString16::nfReadString16(const sInitData &init) : dsFunction(init.clsFRead,
"readString16", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassFileReader::nfReadString16::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	const decString string(fileReader.ReadString16());
	rt->PushString(string);
}

// public func String readString32()
deClassFileReader::nfReadString32::nfReadString32(const sInitData &init) :
dsFunction(init.clsFRead, "readString32", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassFileReader::nfReadString32::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	const decString string(fileReader.ReadString32());
	rt->PushString(string);
}

// public func String readVarString()
deClassFileReader::nfReadVarString::nfReadVarString(const sInitData &init) :
dsFunction(init.clsFRead, "readVarString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassFileReader::nfReadVarString::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	const decString string(fileReader.ReadVarString());
	rt->PushString(string);
}

// public func String readString( int size )
deClassFileReader::nfReadString::nfReadString(const sInitData &init) : dsFunction(init.clsFRead,
"readString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // size
}
void deClassFileReader::nfReadString::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	const int size = rt->GetValue(0)->GetInt();
	decString string;
	string.Set(' ', size);
	fileReader.Read((char*)string.GetString(), size);
	rt->PushString(string.GetString());
}

// public func void skipString8()
deClassFileReader::nfSkipString8::nfSkipString8(const sInitData &init) :
dsFunction(init.clsFRead, "skipString8", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassFileReader::nfSkipString8::RunFunction(dsRunTime*, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	fileReader.SkipString8();
}

// public func void skipString16()
deClassFileReader::nfSkipString16::nfSkipString16(const sInitData &init) :
dsFunction(init.clsFRead, "skipString16", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassFileReader::nfSkipString16::RunFunction(dsRunTime*, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	fileReader.SkipString16();
}

// public func void skipString32()
deClassFileReader::nfSkipString32::nfSkipString32(const sInitData &init) :
dsFunction(init.clsFRead, "skipString32", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassFileReader::nfSkipString32::RunFunction(dsRunTime*, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	fileReader.SkipString32();
}

// public func void skipVarString()
deClassFileReader::nfSkipVarString::nfSkipVarString(const sInitData &init) :
dsFunction(init.clsFRead, "skipVarString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassFileReader::nfSkipVarString::RunFunction(dsRunTime*, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	
	fileReader.SkipVarString();
}

// public func TimeDate readTimeDate()
deClassFileReader::nfReadTimeDate::nfReadTimeDate(const sInitData &init) :
dsFunction(init.clsFRead, "readTimeDate", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsTimeDate){
}
void deClassFileReader::nfReadTimeDate::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileReader &fileReader = *(((const sFReadNatDat *)p_GetNativeData(myself))->fileReader);
	deScriptingDragonScript &ds = *((deClassFileReader*)GetOwnerClass())->GetDS();
	dsClassTimeDate::sTimeDate timeDate;
	
	switch(fileReader.ReadByte()){ // version
	case 0:
		timeDate.year = fileReader.ReadUShort();
		timeDate.month = fileReader.ReadByte();
		timeDate.day = fileReader.ReadByte();
		timeDate.hour = fileReader.ReadByte();
		timeDate.minute = fileReader.ReadByte();
		timeDate.second = fileReader.ReadByte();
		timeDate.dayOfWeek = fileReader.ReadByte();
		timeDate.dayOfYear = fileReader.ReadUShort();
		break;
		
	default:
		DETHROW_INFO(deeInvalidFormat, "unsupported version");
	}
	
	((dsClassTimeDate*)ds.GetScriptEngine()->GetClassTimeDate())->PushTimeDate(rt, timeDate);
}



// Class deClassFileReader
////////////////////////////

// Constructor
////////////////

deClassFileReader::deClassFileReader(deScriptingDragonScript *ds) :
dsClass("FileReader", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds) DSTHROW(dueInvalidParam);
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sFReadNatDat));
}

deClassFileReader::~deClassFileReader(){
}



// Management
///////////////

void deClassFileReader::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsFRead = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsTimeDate = engine->GetClassTimeDate();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfNewZCompressed(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFilename(init));
	AddFunction(new nfGetLength(init));
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfMovePosition(init));
	AddFunction(new nfSetPositionEnd(init));
	
	AddFunction(new nfGetStreamVersion(init));
	AddFunction(new nfSetStreamVersion(init));
	
	AddFunction(new nfEOF(init));
	AddFunction(new nfReadChar(init));
	AddFunction(new nfReadByte(init));
	AddFunction(new nfReadShort(init));
	AddFunction(new nfReadUShort(init));
	AddFunction(new nfReadInt(init));
	AddFunction(new nfReadUInt(init));
	AddFunction(new nfReadVarUInt(init));
	AddFunction(new nfReadFloat(init));
	AddFunction(new nfReadString8(init));
	AddFunction(new nfReadString16(init));
	AddFunction(new nfReadString32(init));
	AddFunction(new nfReadVarString(init));
	AddFunction(new nfReadString(init));
	AddFunction(new nfSkipString8(init));
	AddFunction(new nfSkipString16(init));
	AddFunction(new nfSkipString32(init));
	AddFunction(new nfSkipVarString(init));
	AddFunction(new nfReadTimeDate(init));
	
	CalcMemberOffsets();
}

decBaseFileReader *deClassFileReader::GetFileReader(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((const sFReadNatDat *)p_GetNativeData(myself->GetBuffer()))->fileReader;
}

int deClassFileReader::GetStreamVersion(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	return ((const sFReadNatDat *)p_GetNativeData(myself->GetBuffer()))->streamVersion;
}

void deClassFileReader::PushFileReader(dsRunTime *rt, decBaseFileReader *fileReader){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!fileReader){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sFReadNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->fileReader = fileReader;
	fileReader->AddReference();
}
