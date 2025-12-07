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
#include <stdint.h>

#include "deClassRuntimeMeter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/utils/decTimer.h>



struct sRuntimeMeterNatDat{
	struct sCounter{
		int time;
		int updates;
	};
	
	decTimer *meters;
	int meterCount;
	
	sCounter *counters;
	int counterCount;
	
	decTimer &GetMeterAt(int index){
		if(index < 0){
			DSTHROW(dueInvalidParam);
		}
		
		if(index >= meterCount){
			decTimer * const newArray = new decTimer[index + 1];
			if(meters){
				memcpy(newArray, meters, sizeof(decTimer) * meterCount);
				delete [] meters;
			}
			meters = newArray;
			meterCount = index + 1;
		}
		
		return meters[index];
	}
	
	sCounter &GetCounterAt(int index){
		if(index < 0){
			DSTHROW(dueInvalidParam);
		}
		
		if(index >= counterCount){
			sCounter * const newArray = new sCounter[index + 1];
			if(counters){
				memcpy(newArray, counters, sizeof(sCounter) * counterCount);
				delete [] counters;
			}
			counters = newArray;
			counterCount = index + 1;
		}
		
		return counters[index];
	}
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassRuntimeMeter::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsRTM,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassRuntimeMeter::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat * const nd = (sRuntimeMeterNatDat*)p_GetNativeData(myself);
	
	nd->meters = NULL;
	nd->meterCount = 0;
	nd->counters = NULL;
	nd->counterCount = 0;
}

// public func destructor()
deClassRuntimeMeter::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsRTM,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassRuntimeMeter::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	
	if(nd.meters){
		delete [] nd.meters;
		nd.meters = NULL;
		nd.meterCount = 0;
	}
	
	if(nd.counters){
		delete [] nd.counters;
		nd.counters = NULL;
		nd.counterCount = 0;
	}
}



// Management
///////////////

// public func void reset( int meter )
deClassRuntimeMeter::nfReset::nfReset(const sInitData &init) : dsFunction(init.clsRTM,
"reset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // meter
}
void deClassRuntimeMeter::nfReset::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const int meter = rt->GetValue(0)->GetInt();
	nd.GetMeterAt(meter).Reset();
}

// public func void reset( int meterFirst, int meterCount )
deClassRuntimeMeter::nfReset2::nfReset2(const sInitData &init) : dsFunction(init.clsRTM,
"reset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // meterFirst
	p_AddParameter(init.clsInt); // meterCount
}
void deClassRuntimeMeter::nfReset2::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const int meterFirst = rt->GetValue(0)->GetInt();
	const int meterCount = rt->GetValue(1)->GetInt();
	int i;
	
	for(i=0; i<meterCount; i++){
		nd.GetMeterAt(meterFirst + i).Reset();
	}
}

// public func float elapsed( int meter )
deClassRuntimeMeter::nfElapsed::nfElapsed(const sInitData &init) : dsFunction(init.clsRTM,
"elapsed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // meter
}
void deClassRuntimeMeter::nfElapsed::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const int meter = rt->GetValue(0)->GetInt();
	const float elapsed = nd.GetMeterAt(meter).GetElapsedTime();
	rt->PushFloat(elapsed);
}

// public func int elapsedMicroSec( int meter )
deClassRuntimeMeter::nfElapsedMicroSec::nfElapsedMicroSec(const sInitData &init) : dsFunction(init.clsRTM,
"elapsedMicroSec", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // meter
}
void deClassRuntimeMeter::nfElapsedMicroSec::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const int meter = rt->GetValue(0)->GetInt();
	const int elapsed = (int)(nd.GetMeterAt(meter).GetElapsedTime() * 1e6f);
	rt->PushInt(elapsed);
}



// public func void counterReset( int counter )
deClassRuntimeMeter::nfCounterReset::nfCounterReset(const sInitData &init) : dsFunction(init.clsRTM,
"counterReset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // counter
}
void deClassRuntimeMeter::nfCounterReset::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	sRuntimeMeterNatDat::sCounter &counter = nd.GetCounterAt(rt->GetValue(0)->GetInt());
	counter.time = 0;
	counter.updates = 0;
}

// public func void counterReset( int counterFirst, int counterCount )
deClassRuntimeMeter::nfCounterReset2::nfCounterReset2(const sInitData &init) : dsFunction(init.clsRTM,
"counterReset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // counterFirst
	p_AddParameter(init.clsInt); // counterCount
}
void deClassRuntimeMeter::nfCounterReset2::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const int counterFirst = rt->GetValue(0)->GetInt();
	const int counterCount = rt->GetValue(1)->GetInt();
	int i;
	
	for(i=0; i<counterCount; i++){
		sRuntimeMeterNatDat::sCounter &counter = nd.GetCounterAt(counterFirst + i);
		counter.time = 0;
		counter.updates = 0;
	}
}

// public func void counterAdd( int meter, int counter )
deClassRuntimeMeter::nfCounterAdd::nfCounterAdd(const sInitData &init) : dsFunction(init.clsRTM,
"counterAdd", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // meter
	p_AddParameter(init.clsInt); // counter
}
void deClassRuntimeMeter::nfCounterAdd::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const int meter = rt->GetValue(0)->GetInt();
	const int elapsed = (int)(nd.GetMeterAt(meter).GetElapsedTime() * 1e6f);
	sRuntimeMeterNatDat::sCounter &counter = nd.GetCounterAt(rt->GetValue(1)->GetInt());
	counter.time += elapsed;
	counter.updates++;
}

// public func int counterTime( int counter )
deClassRuntimeMeter::nfCounterTime::nfCounterTime(const sInitData &init) : dsFunction(init.clsRTM,
"counterTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // counter
}
void deClassRuntimeMeter::nfCounterTime::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const sRuntimeMeterNatDat::sCounter &counter = nd.GetCounterAt(rt->GetValue(0)->GetInt());
	rt->PushInt(counter.time);
}

// public func float counterTimeSeconds( int counter )
deClassRuntimeMeter::nfCounterTimeSeconds::nfCounterTimeSeconds(const sInitData &init) :
dsFunction(init.clsRTM, "counterTimeSeconds", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // counter
}
void deClassRuntimeMeter::nfCounterTimeSeconds::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const sRuntimeMeterNatDat::sCounter &counter = nd.GetCounterAt(rt->GetValue(0)->GetInt());
	rt->PushFloat((float)(1e-6 * (double)counter.time));
}

// public func int counterUpdates( int counter )
deClassRuntimeMeter::nfCounterUpdates::nfCounterUpdates(const sInitData &init) : dsFunction(init.clsRTM,
"counterUpdates", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // counter
}
void deClassRuntimeMeter::nfCounterUpdates::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const sRuntimeMeterNatDat::sCounter &counter = nd.GetCounterAt(rt->GetValue(0)->GetInt());
	rt->PushInt(counter.updates);
}

// public func int counterAverageTime( int counter )
deClassRuntimeMeter::nfCounterAverageTime::nfCounterAverageTime(const sInitData &init) : dsFunction(init.clsRTM,
"counterAverageTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // counter
}
void deClassRuntimeMeter::nfCounterAverageTime::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const sRuntimeMeterNatDat::sCounter &counter = nd.GetCounterAt(rt->GetValue(0)->GetInt());
	
	if(counter.updates > 0){
		rt->PushInt(counter.time / counter.updates);
		
	}else{
		rt->PushInt(0);
	}
}

// public func float counterAverageTimeSeconds( int counter )
deClassRuntimeMeter::nfCounterAverageTimeSeconds::nfCounterAverageTimeSeconds(const sInitData &init) :
dsFunction(init.clsRTM, "counterAverageTimeSeconds", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // counter
}
void deClassRuntimeMeter::nfCounterAverageTimeSeconds::RunFunction(dsRunTime *rt, dsValue *myself){
	sRuntimeMeterNatDat &nd = *((sRuntimeMeterNatDat*)p_GetNativeData(myself));
	const sRuntimeMeterNatDat::sCounter &counter = nd.GetCounterAt(rt->GetValue(0)->GetInt());
	
	if(counter.updates > 0){
		rt->PushFloat((float)((1e-6 * (double)counter.time) / (double)counter.updates));
		
	}else{
		rt->PushFloat(0.0f);
	}
}



// Class deClassRuntimeMeter
//////////////////////////////

// Constructor
////////////////

deClassRuntimeMeter::deClassRuntimeMeter(deScriptingDragonScript *ds) :
dsClass("RuntimeMeter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sRuntimeMeterNatDat));
}

deClassRuntimeMeter::~deClassRuntimeMeter(){
}



// Management
///////////////

void deClassRuntimeMeter::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsRTM = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfReset(init));
	AddFunction(new nfReset2(init));
	AddFunction(new nfElapsed(init));
	AddFunction(new nfElapsedMicroSec(init));
	
	AddFunction(new nfCounterReset(init));
	AddFunction(new nfCounterReset2(init));
	AddFunction(new nfCounterAdd(init));
	AddFunction(new nfCounterTime(init));
	AddFunction(new nfCounterTimeSeconds(init));
	AddFunction(new nfCounterUpdates(init));
	AddFunction(new nfCounterAverageTime(init));
	AddFunction(new nfCounterAverageTimeSeconds(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}
