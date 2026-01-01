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

#include "deClassCache.h"
#include "file/deClassFileReader.h"
#include "file/deClassFileWriter.h"
#include "../deScriptingDragonScript.h"
#include "../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/string/decString.h>

#include <libdscript/exceptions.h>



struct sCacheNatDat{
	deCacheHelper *cacheHelper = nullptr;
	decString directory;
	
	~sCacheNatDat(){
		if(cacheHelper){
			delete cacheHelper;
			cacheHelper = nullptr;
		}
	}
};



// Constructors, Destructors
//////////////////////////////

// public func new( String directory )
deClassCache::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsCache, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // directory
}
void deClassCache::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCacheNatDat * const nd = new (p_GetNativeData(myself)) sCacheNatDat;
	
	// check arguments
	const char * const directory = rt->GetValue(0)->GetString();
	if(!directory){
		DSTHROW(dueNullPointer);
	}
	
	deClassCache &clsCache = *static_cast<deClassCache*>(GetOwnerClass());
	if(clsCache.GetDirectories().Has(directory)){
		DSTHROW_INFO(dueInvalidParam, "Cache directory already in use");
	}
	
	decPath path;
	path.SetFromUnix("/cache/local/game");
	path.AddUnixPath(rt->GetValue(0)->GetString());
	
	// create cache
	nd->cacheHelper = new deCacheHelper(&clsCache.GetDS().GetVFS(), path);
	nd->directory = directory;
	clsCache.GetDirectories().Add(directory);
}

// public func destructor()
deClassCache::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCache,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCache::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sCacheNatDat * const nd = static_cast<sCacheNatDat*>(p_GetNativeData(myself));
	
	static_cast<deClassCache*>(GetOwnerClass())->GetDirectories().Remove(nd->directory);
	
	nd->~sCacheNatDat();
}



// Management
///////////////

// public func FileReader read( String id )
deClassCache::nfRead::nfRead(const sInitData &init) :
dsFunction(init.clsCache, "read", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFileReader){
	p_AddParameter(init.clsString); // id
}
void deClassCache::nfRead::RunFunction(dsRunTime *rt, dsValue *myself){
	deCacheHelper &cacheHelper = *(static_cast<const sCacheNatDat*>(p_GetNativeData(myself))->cacheHelper);
	deScriptingDragonScript &ds = static_cast<deClassCache*>(GetOwnerClass())->GetDS();
	decBaseFileReader::Ref reader;
	
	reader = cacheHelper.Read(rt->GetValue(0)->GetString());
	ds.GetClassFileReader()->PushFileReader(rt, reader);
}

// public func FileWriter write( String id )
deClassCache::nfWrite::nfWrite(const sInitData &init) :
dsFunction(init.clsCache, "write", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFileWriter){
	p_AddParameter(init.clsString); // id
}
void deClassCache::nfWrite::RunFunction(dsRunTime *rt, dsValue *myself){
	deCacheHelper &cacheHelper = *(static_cast<const sCacheNatDat*>(p_GetNativeData(myself))->cacheHelper);
	deScriptingDragonScript &ds = static_cast<deClassCache*>(GetOwnerClass())->GetDS();
	decBaseFileWriter::Ref writer;
	
	writer = cacheHelper.Write(rt->GetValue(0)->GetString());
	ds.GetClassFileWriter()->PushFileWriter(rt, writer);
}

// public func void delete( String id )
deClassCache::nfDelete::nfDelete(const sInitData &init) :
dsFunction(init.clsCache, "delete", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // id
}
void deClassCache::nfDelete::RunFunction(dsRunTime *rt, dsValue *myself){
	deCacheHelper &cacheHelper = *(static_cast<const sCacheNatDat*>(p_GetNativeData(myself))->cacheHelper);
	cacheHelper.Delete(rt->GetValue(0)->GetString());
}

// public func void deleteAll()
deClassCache::nfDeleteAll::nfDeleteAll(const sInitData &init) :
dsFunction(init.clsCache, "deleteAll", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCache::nfDeleteAll::RunFunction(dsRunTime *rt, dsValue *myself){
	deCacheHelper &cacheHelper = *(static_cast<const sCacheNatDat*>(p_GetNativeData(myself))->cacheHelper);
	cacheHelper.DeleteAll();
}



// Class deClassCache
///////////////////////

// Constructor
////////////////

deClassCache::deClassCache(deScriptingDragonScript &ds) :
dsClass("Cache", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sCacheNatDat));
}

deClassCache::~deClassCache(){
}



// Management
///////////////

void deClassCache::CreateClassMembers(dsEngine *engine){
	sInitData init;
	init.clsCache = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfRead(init));
	AddFunction(new nfWrite(init));
	AddFunction(new nfDelete(init));
	AddFunction(new nfDeleteAll(init));
	
	CalcMemberOffsets();
}
