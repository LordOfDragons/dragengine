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

#include "deClassMemoryFile.h"
#include "deClassFileReader.h"
#include "deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/file/decZFileReader.h>
#include <dragengine/common/file/decZFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <libdscript/exceptions.h>



struct sMemFileNatDat{
	decMemoryFile::Ref memoryFile;
};



// Constructors, Destructors
//////////////////////////////

// public func new( String filename )
deClassMemoryFile::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsMemFile,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
}
void deClassMemoryFile::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sMemFileNatDat * const nd = new (p_GetNativeData(myself)) sMemFileNatDat;
	
	// check arguments
	const char * const filename = rt->GetValue(0)->GetString();
	
	// create memory file
	nd->memoryFile = new decMemoryFile(filename);
}

// public func new( MemoryFile memoryFile )
deClassMemoryFile::nfNewCopy::nfNewCopy(const sInitData &init) : dsFunction(init.clsMemFile,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsMemFile); // memoryFile
}
void deClassMemoryFile::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sMemFileNatDat * const nd = new (p_GetNativeData(myself)) sMemFileNatDat;
	const deClassMemoryFile &clsMemFile = *(static_cast<deClassMemoryFile*>(GetOwnerClass()));
	
	// check arguments
	const decMemoryFile * const copyMemoryFile = clsMemFile.GetMemoryFile(rt->GetValue(0)->GetRealObject());
	if(!copyMemoryFile){
		DSTHROW(dueNullPointer);
	}
	
	// create memory file
	nd->memoryFile = new decMemoryFile(copyMemoryFile->GetFilename());
	
	const int size = copyMemoryFile->GetLength();
	if(size > 0){
		nd->memoryFile->Resize(size, true);
		memcpy(nd->memoryFile->GetPointer(), copyMemoryFile->GetPointer(), size);
	}
}

// public func destructor()
deClassMemoryFile::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsMemFile,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassMemoryFile::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sMemFileNatDat*>(p_GetNativeData(myself))->~sMemFileNatDat();
}



// Management
///////////////

// public func String getFilename()
deClassMemoryFile::nfGetFilename::nfGetFilename(const sInitData &init) : dsFunction(init.clsMemFile,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassMemoryFile::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMemoryFile &memoryFile = static_cast<const sMemFileNatDat*>(p_GetNativeData(myself))->memoryFile;
	rt->PushString(memoryFile.GetFilename());
}

// public func int getSize()
deClassMemoryFile::nfGetSize::nfGetSize(const sInitData &init) : dsFunction(init.clsMemFile,
"getSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassMemoryFile::nfGetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMemoryFile &memoryFile = static_cast<const sMemFileNatDat*>(p_GetNativeData(myself))->memoryFile;
	rt->PushInt(memoryFile.GetLength());
}

// public func void setSize( int size )
deClassMemoryFile::nfSetSize::nfSetSize(const sInitData &init) : dsFunction(init.clsMemFile,
"setSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // size
}
void deClassMemoryFile::nfSetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	decMemoryFile &memoryFile = static_cast<const sMemFileNatDat*>(p_GetNativeData(myself))->memoryFile;
	memoryFile.Resize(rt->GetValue(0)->GetInt(), false);
}



// public func FileReader getReader()
deClassMemoryFile::nfGetReader::nfGetReader(const sInitData &init) : dsFunction(init.clsMemFile,
"getReader", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFRead){
}
void deClassMemoryFile::nfGetReader::RunFunction(dsRunTime *rt, dsValue *myself){
	decMemoryFile * const memoryFile = static_cast<const sMemFileNatDat*>(p_GetNativeData(myself))->memoryFile;
	const deScriptingDragonScript &ds = (static_cast<deClassMemoryFile*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassFileReader()->PushFileReader(rt, decMemoryFileReader::Ref::NewWith(memoryFile));
}

// public func FileWriter getWriter( bool append )
deClassMemoryFile::nfGetWriter::nfGetWriter(const sInitData &init) : dsFunction(init.clsMemFile,
"getWriter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFWrite){
	p_AddParameter(init.clsBool); // append
}
void deClassMemoryFile::nfGetWriter::RunFunction(dsRunTime *rt, dsValue *myself){
	decMemoryFile * const memoryFile = static_cast<const sMemFileNatDat*>(p_GetNativeData(myself))->memoryFile;
	const deScriptingDragonScript &ds = (static_cast<deClassMemoryFile*>(GetOwnerClass()))->GetDS();
	
	const bool append = rt->GetValue(0)->GetBool();
	
	ds.GetClassFileWriter()->PushFileWriter(rt, decMemoryFileWriter::Ref::NewWith(memoryFile, append));
}

// public func FileReader getReaderZCompressed()
deClassMemoryFile::nfGetReaderZCompressed::nfGetReaderZCompressed(const sInitData &init) :
dsFunction(init.clsMemFile, "getReaderZCompressed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFRead){
}
void deClassMemoryFile::nfGetReaderZCompressed::RunFunction(dsRunTime *rt, dsValue *myself){
	decMemoryFile * const memoryFile = static_cast<const sMemFileNatDat*>(p_GetNativeData(myself))->memoryFile;
	const deScriptingDragonScript &ds = (static_cast<deClassMemoryFile*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassFileReader()->PushFileReader(rt, decZFileReader::Ref::New(new decZFileReader(
		decMemoryFileReader::Ref::NewWith(memoryFile))));
}

// public func FileWriter getWriterZCompressed()
deClassMemoryFile::nfGetWriterZCompressed::nfGetWriterZCompressed(const sInitData &init) :
dsFunction(init.clsMemFile, "getWriterZCompressed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFWrite){
}
void deClassMemoryFile::nfGetWriterZCompressed::RunFunction(dsRunTime *rt, dsValue *myself){
	decMemoryFile * const memoryFile = static_cast<const sMemFileNatDat*>(p_GetNativeData(myself))->memoryFile;
	const deScriptingDragonScript &ds = (static_cast<deClassMemoryFile*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassFileWriter()->PushFileWriter(rt, decZFileWriter::Ref::New(new decZFileWriter(
		decMemoryFileWriter::Ref::NewWith(memoryFile, false))));
}



// Class deClassMemoryFile
////////////////////////////

// Constructor
////////////////

deClassMemoryFile::deClassMemoryFile(deScriptingDragonScript &ds) :
dsClass("MemoryFile", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sMemFileNatDat));
}

deClassMemoryFile::~deClassMemoryFile(){
}



// Management
///////////////

void deClassMemoryFile::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsMemFile = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	
	init.clsFRead = pDS.GetClassFileReader();
	init.clsFWrite = pDS.GetClassFileWriter();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfNewCopy(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFilename(init));
	AddFunction(new nfGetSize(init));
	AddFunction(new nfSetSize(init));
	
	AddFunction(new nfGetReader(init));
	AddFunction(new nfGetWriter(init));
	AddFunction(new nfGetReaderZCompressed(init));
	AddFunction(new nfGetWriterZCompressed(init));
	
	CalcMemberOffsets();
}

decMemoryFile *deClassMemoryFile::GetMemoryFile(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	return static_cast<const sMemFileNatDat*>(p_GetNativeData(myself->GetBuffer()))->memoryFile;
}

void deClassMemoryFile::PushMemoryFile(dsRunTime *rt, decMemoryFile *memoryFile){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!memoryFile){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	(new (rt->GetValue(0)->GetRealObject()->GetBuffer()) sMemFileNatDat)->memoryFile = memoryFile;
}
