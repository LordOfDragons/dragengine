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

#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deClassFileWriter.h"
#include "deClassFileReader.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decZFileWriter.h>
#include <dragengine/common/string/decString.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassTimeDate.h>



struct sFileWriterNatDat{
	decBaseFileWriter::Ref fileWriter;
};



// Constructors, Destructors
//////////////////////////////

// public func new( String filename )
deClassFileWriter::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsFileWriter,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // filename
}
void deClassFileWriter::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sFileWriterNatDat * const nd = new (p_GetNativeData(myself)) sFileWriterNatDat;
	const deClassFileWriter &clsFileWriter = *(static_cast<deClassFileWriter*>(GetOwnerClass()));
	deVirtualFileSystem &vfs = *clsFileWriter.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	decPath path;
	nd.streamVersion = clsFileWriter.GetDS()->GetStreamingVersion();
	
	// create file writer
	path.SetFromUnix(rt->GetValue(0)->GetString());
	nd->fileWriter = vfs.OpenFileForWriting(path);
}

// public static func FileWriter newZCompresed(String filename)
deClassFileWriter::nfNewZCompressed::nfNewZCompressed(const sInitData &init) :
dsFunction(init.clsFileWriter, "newZCompressed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFileWriter){
	p_AddParameter(init.clsString); // filename
}
void deClassFileWriter::nfNewZCompressed::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassFileWriter &clsFileWriter = *(static_cast<deClassFileWriter*>(GetOwnerClass()));
	deVirtualFileSystem &vfs = *clsFileWriter.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	
	const char * const filename = rt->GetValue(0)->GetString();
	
	clsFileWriter.PushFileWriter(rt, decZFileWriter::Ref::New(
		new decZFileWriter(vfs.OpenFileForWriting(decPath::CreatePathUnix(filename)))));
}

// public func destructor()
deClassFileWriter::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsFileWriter,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassFileWriter::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sFileWriterNatDat*>(p_GetNativeData(myself))->~sFileWriterNatDat();
}



// Management
///////////////

// public func String getFilename()
deClassFileWriter::nfGetFilename::nfGetFilename(const sInitData &init) : dsFunction(init.clsFileWriter,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassFileWriter::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	
	rt->PushString(fileWriter.GetFilename());
}



// public func int getStreamVersion()
deClassFileWriter::nfGetStreamVersion::nfGetStreamVersion(const sInitData &init) : dsFunction(init.clsFileWriter,
"getStreamVersion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileWriter::nfGetStreamVersion::RunFunction(dsRunTime *rt, dsValue *myself){
	rt->PushInt(static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->streamVersion);
}

// public func void setStreamVersion( int version )
deClassFileWriter::nfSetStreamVersion::nfSetStreamVersion(const sInitData &init) : dsFunction(init.clsFileWriter,
"setStreamVersion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // version
}
void deClassFileWriter::nfSetStreamVersion::RunFunction(dsRunTime *rt, dsValue *myself){
	static_cast<sFileWriterNatDat*>(p_GetNativeData(myself))->streamVersion = rt->GetValue(0)->GetInt();
}



// func int getPosition()
deClassFileWriter::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsFileWriter,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassFileWriter::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	rt->PushInt(static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter->GetPosition());
}

// func void setPosition(int position)
deClassFileWriter::nfSetPosition::nfSetPosition(const sInitData &init) : dsFunction(init.clsFileWriter,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // position
}
void deClassFileWriter::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	static_cast<sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter->SetPosition(rt->GetValue(0)->GetInt());
}

// func void movePosition(int offset)
deClassFileWriter::nfMovePosition::nfMovePosition(const sInitData &init) : dsFunction(init.clsFileWriter,
"movePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // offset
}
void deClassFileWriter::nfMovePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	static_cast<sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter->MovePosition(rt->GetValue(0)->GetInt());
}

// func void setPositionEnd(int position)
deClassFileWriter::nfSetPositionEnd::nfSetPositionEnd(const sInitData &init) : dsFunction(init.clsFileWriter,
"setPositionEnd", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // position
}
void deClassFileWriter::nfSetPositionEnd::RunFunction(dsRunTime *rt, dsValue *myself){
	static_cast<sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter->SetPositionEnd(rt->GetValue(0)->GetInt());
}



// public func void writeChar( int value )
deClassFileWriter::nfWriteChar::nfWriteChar(const sInitData &init) : dsFunction(init.clsFileWriter,
"writeChar", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // value
}
void deClassFileWriter::nfWriteChar::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	
	const int value = rt->GetValue(0)->GetInt();
	if(value < -128 || value > 127){
		DSTHROW(dueInvalidParam);
	}
	
	fileWriter.WriteChar((int8_t)value);
}

// public func void writeByte( int value )
deClassFileWriter::nfWriteByte::nfWriteByte(const sInitData &init) : dsFunction(init.clsFileWriter,
"writeByte", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // value
}
void deClassFileWriter::nfWriteByte::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	
	const int value = rt->GetValue(0)->GetInt();
	if(value < 0 || value > 255){
		DSTHROW(dueInvalidParam);
	}
	
	fileWriter.WriteByte((uint8_t)value);
}

// public func void writeShort( int value )
deClassFileWriter::nfWriteShort::nfWriteShort(const sInitData &init) : dsFunction(init.clsFileWriter,
"writeShort", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // value
}
void deClassFileWriter::nfWriteShort::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	
	const int value = rt->GetValue(0)->GetInt();
	if(value < -32768 || value > 32767){
		DSTHROW(dueInvalidParam);
	}
	
	fileWriter.WriteShort((int16_t)value);
}

// public func void writeUShort( int value )
deClassFileWriter::nfWriteUShort::nfWriteUShort(const sInitData &init) : dsFunction(init.clsFileWriter,
"writeUShort", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // value
}
void deClassFileWriter::nfWriteUShort::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	
	const int value = rt->GetValue(0)->GetInt();
	if(value < 0 || value > 65535){
		DSTHROW(dueInvalidParam);
	}
	
	fileWriter.WriteUShort((uint16_t)value);
}

// public func void writeInt( int value )
deClassFileWriter::nfWriteInt::nfWriteInt(const sInitData &init) : dsFunction(init.clsFileWriter,
"writeInt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // value
}
void deClassFileWriter::nfWriteInt::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	
	fileWriter.WriteInt((int32_t)rt->GetValue(0)->GetInt());
}

// public func void writeUInt( int value )
deClassFileWriter::nfWriteUInt::nfWriteUInt(const sInitData &init) : dsFunction(init.clsFileWriter,
"writeUInt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // value
}
void deClassFileWriter::nfWriteUInt::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	
	fileWriter.WriteUInt((uint32_t)rt->GetValue(0)->GetInt());
}

// public func void writeVarUInt( int value )
deClassFileWriter::nfWriteVarUInt::nfWriteVarUInt(const sInitData &init) :
dsFunction(init.clsFileWriter, "writeVarUInt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // value
}
void deClassFileWriter::nfWriteVarUInt::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	
	fileWriter.WriteVarUInt((uint32_t)rt->GetValue(0)->GetInt());
}

// public func void writeFloat( float value )
deClassFileWriter::nfWriteFloat::nfWriteFloat(const sInitData &init) : dsFunction(init.clsFileWriter,
"writeFloat", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // value
}
void deClassFileWriter::nfWriteFloat::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	
	fileWriter.WriteFloat(rt->GetValue(0)->GetFloat());
}

// public func void writeString8( String data )
deClassFileWriter::nfWriteString8::nfWriteString8(const sInitData &init) :
dsFunction(init.clsFileWriter, "writeString8", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // data
}
void deClassFileWriter::nfWriteString8::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	const char * const data = rt->GetValue(0)->GetString();
	try{
		fileWriter.WriteString8(data);
		
	}catch(const deException &e){
		DSTHROW_INFO(dueInvalidParam, e.GetDescription());
	}
}

// public func void writeString16( String data )
deClassFileWriter::nfWriteString16::nfWriteString16(const sInitData &init) :
dsFunction(init.clsFileWriter, "writeString16", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // data
}
void deClassFileWriter::nfWriteString16::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	const char * const data = rt->GetValue(0)->GetString();
	try{
		fileWriter.WriteString16(data);
		
	}catch(const deException &e){
		DSTHROW_INFO(dueInvalidParam, e.GetDescription());
	}
}

// public func void writeString32( String data )
deClassFileWriter::nfWriteString32::nfWriteString32(const sInitData &init) :
dsFunction(init.clsFileWriter, "writeString32", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // data
}
void deClassFileWriter::nfWriteString32::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	const char * const data = rt->GetValue(0)->GetString();
	try{
		fileWriter.WriteString32(data);
		
	}catch(const deException &e){
		DSTHROW_INFO(dueInvalidParam, e.GetDescription());
	}
}

// public func void writeVarString( String data )
deClassFileWriter::nfWriteVarString::nfWriteVarString(const sInitData &init) :
dsFunction(init.clsFileWriter, "writeVarString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // data
}
void deClassFileWriter::nfWriteVarString::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	const char * const data = rt->GetValue(0)->GetString();
	try{
		fileWriter.WriteVarString(data);
		
	}catch(const deException &e){
		DSTHROW_INFO(dueInvalidParam, e.GetDescription());
	}
}

// public func void writeString( String data )
deClassFileWriter::nfWriteString::nfWriteString(const sInitData &init) : dsFunction(init.clsFileWriter,
"writeString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // data
}
void deClassFileWriter::nfWriteString::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	const char *data = rt->GetValue(0)->GetString();
	
	fileWriter.Write(data, (int)strlen(data));
}

// public func void writeData( FileReader reader )
deClassFileWriter::nfWriteData::nfWriteData(const sInitData &init) : dsFunction(init.clsFileWriter,
"writeData", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassFileWriter::nfWriteData::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	deScriptingDragonScript &ds = *(static_cast<deClassFileWriter*>(GetOwnerClass()))->GetDS();
	
	decBaseFileReader * const fileReader = ds.GetClassFileReader()->GetFileReader(rt->GetValue(0)->GetRealObject());
	if(!fileReader){
		DSTHROW(dueNullPointer);
	}
	
	int size = fileReader->GetLength();
	if(size == 0){
		return;
	}
	
	const int bufferSize = 1024;
	char *buffer = nullptr;
	
	try{
		buffer = new char[bufferSize];
		
		while(size > 0){
			const int readBytes = decMath::min(bufferSize, size);
			fileReader->Read(buffer, readBytes);
			fileWriter.Write(buffer, readBytes);
			size -= readBytes;
		}
		
		delete [] buffer;
		
	}catch(...){
		if(buffer){
			delete [] buffer;
		}
		throw;
	}
}

// public func void writeData( FileReader reader, int size )
deClassFileWriter::nfWriteData2::nfWriteData2(const sInitData &init) : dsFunction(init.clsFileWriter,
"writeData", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileReader); // reader
	p_AddParameter(init.clsInt); // size
}
void deClassFileWriter::nfWriteData2::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	deScriptingDragonScript &ds = *(static_cast<deClassFileWriter*>(GetOwnerClass()))->GetDS();
	
	int size = rt->GetValue(1)->GetInt();
	if(size < 0){
		DSTHROW(dueInvalidParam);
	}
	if(size == 0){
		return;
	}
	
	decBaseFileReader * const fileReader = ds.GetClassFileReader()->GetFileReader(rt->GetValue(0)->GetRealObject());
	if(!fileReader){
		DSTHROW(dueNullPointer);
	}
	
	const int bufferSize = 1024;
	char *buffer = nullptr;
	
	try{
		buffer = new char[bufferSize];
		
		while(size > 0){
			const int readBytes = decMath::min(bufferSize, size);
			fileReader->Read(buffer, readBytes);
			fileWriter.Write(buffer, readBytes);
			size -= readBytes;
		}
		
		delete [] buffer;
		
	}catch(...){
		if(buffer){
			delete [] buffer;
		}
		throw;
	}
}

// public func void writeTimeDate(TimeDate timeDate)
deClassFileWriter::nfWriteTimeDate::nfWriteTimeDate(const sInitData &init) :
dsFunction(init.clsFileWriter, "writeTimeDate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsTimeDate); // timeDate
}
void deClassFileWriter::nfWriteTimeDate::RunFunction(dsRunTime *rt, dsValue *myself){
	decBaseFileWriter &fileWriter = static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself))->fileWriter;
	deScriptingDragonScript &ds = *(static_cast<deClassFileWriter*>(GetOwnerClass()))->GetDS();
	dsClassTimeDate &clsTimeDate = *((dsClassTimeDate*)ds.GetScriptEngine()->GetClassTimeDate());
	
	const dsClassTimeDate::sTimeDate timeDate(clsTimeDate.GetTimeDate(rt->GetValue(0)->GetRealObject()));
	
	fileWriter.WriteByte(0); // version
	
	fileWriter.WriteUShort(timeDate.year);
	fileWriter.WriteByte(timeDate.month);
	fileWriter.WriteByte(timeDate.day);
	fileWriter.WriteByte(timeDate.hour);
	fileWriter.WriteByte(timeDate.minute);
	fileWriter.WriteByte(timeDate.second);
	
	fileWriter.WriteByte(timeDate.dayOfWeek);
	fileWriter.WriteUShort(timeDate.dayOfYear);
}



// Class deClassFileWriter
////////////////////////////

// Constructor
////////////////

deClassFileWriter::deClassFileWriter(deScriptingDragonScript *ds) :
dsClass("FileWriter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sFileWriterNatDat));
}

deClassFileWriter::~deClassFileWriter(){
}



// Management
///////////////

void deClassFileWriter::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsFileWriter = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsTimeDate = engine->GetClassTimeDate();
	
	init.clsFileReader = pDS->GetClassFileReader();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfNewZCompressed(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFilename(init));
	
	AddFunction(new nfGetStreamVersion(init));
	AddFunction(new nfSetStreamVersion(init));
	
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfMovePosition(init));
	AddFunction(new nfSetPositionEnd(init));
	
	AddFunction(new nfWriteChar(init));
	AddFunction(new nfWriteByte(init));
	AddFunction(new nfWriteShort(init));
	AddFunction(new nfWriteUShort(init));
	AddFunction(new nfWriteInt(init));
	AddFunction(new nfWriteUInt(init));
	AddFunction(new nfWriteVarUInt(init));
	AddFunction(new nfWriteFloat(init));
	AddFunction(new nfWriteString8(init));
	AddFunction(new nfWriteString16(init));
	AddFunction(new nfWriteString32(init));
	AddFunction(new nfWriteVarString(init));
	AddFunction(new nfWriteString(init));
	AddFunction(new nfWriteData(init));
	AddFunction(new nfWriteData2(init));
	AddFunction(new nfWriteTimeDate(init));
	
	CalcMemberOffsets();
}

decBaseFileWriter *deClassFileWriter::GetFileWriter(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	
	return static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself->GetBuffer()))->fileWriter;
}

int deClassFileWriter::GetStreamVersion(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	return static_cast<const sFileWriterNatDat*>(p_GetNativeData(myself->GetBuffer()))->streamVersion;
}

void deClassFileWriter::PushFileWriter(dsRunTime *rt, decBaseFileWriter *fileWriter){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!fileWriter){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	(new (rt->GetValue(0)->GetRealObject()->GetBuffer()) sFileWriterNatDat)->fileWriter = fileWriter;
}
