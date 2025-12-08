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

#include "dedsSoundLevelMeter.h"
#include "../deScriptingDragonScript.h"
#include "../classes/sound/deClassSpeaker.h"
#include "../classes/sound/deClassSoundLevelMeter.h"
#include "../classes/sound/deClassSoundLevelMeterSpeaker.h"
#include "../classes/sound/deClassSoundLevelMeterListener.h"

#include <libdscript/exceptions.h>
#include <libdscript/libdscript.h>

#include <dragengine/resources/sensor/deSoundLevelMeter.h>



// Class dedsSoundLevelMeter
//////////////////////////////

// Constructor, destructor
////////////////////////////

dedsSoundLevelMeter::dedsSoundLevelMeter(deScriptingDragonScript &ds,
deSoundLevelMeter *soundLevelMeter) :
pDS(ds),
pSoundLevelMeter(soundLevelMeter),
pValCB(NULL),
pHasCB(false)
{
	if(!soundLevelMeter){
		DSTHROW(dueInvalidParam);
	}
	
	pValCB.TakeOver(ds.GetScriptEngine()->GetMainRunTime()->CreateValue(ds.GetClassSoundLevelMeterListener());
}

dedsSoundLevelMeter::~dedsSoundLevelMeter(){
	if(!pValCB){
		return;
	}
	
	// check if the resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the resource
	// being deleted due to links breaking while freeing the value. if this
	// is the case delay the deletion until a safe time
	if(pSoundLevelMeter && pSoundLevelMeter->GetRefCount() > 0){
		pDS.AddValueDeleteLater(pValCB);
		
	}else{
		pDS.GetScriptEngine()->GetMainRunTime()->FreeValue(pValCB);
	}
	
	pValCB = NULL;
	pHasCB = false;
}



// Management
///////////////

dsRealObject *dedsSoundLevelMeter::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsSoundLevelMeter::SetCallback(dsRealObject *object){
	if(!pValCB){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if(object){
		rt.SetObject(pValCB, object);
		rt.CastValueTo(pValCB, pValCB, pDS.GetClassSoundLevelMeterListener());
		pHasCB = true;
		
	}else{
		rt.SetNull(pValCB, pDS.GetClassSoundLevelMeterListener());
		pHasCB = false;
	}
}



// Notifications
//////////////////

void dedsSoundLevelMeter::SpeakerAudible(const deSoundLevelMeter::cAudibleSpeaker &speaker){
	if(!pHasCB){
		return;
	}
	
	const int funcIndex = pDS.GetClassSoundLevelMeterListener()->GetFuncIndexSpeakerAudible();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	
	try{
		pDS.GetClassSoundLevelMeterSpeaker()->PushSoundLevelMeterSpeaker(rt, speaker); // speaker
		rt->RunFunctionFast(pValCB, funcIndex);
		
	}catch(const duException &e){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

void dedsSoundLevelMeter::SpeakerInaudible(deSpeaker *speaker){
	if(!speaker){
		DSTHROW(dueInvalidParam);
	}
	
	if(!pHasCB){
		return;
	}
	
	const int funcIndex = pDS.GetClassSoundLevelMeterListener()->GetFuncIndexSpeakerInaudible();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	
	try{
		pDS.GetClassSpeaker()->PushSpeaker(rt, speaker); // speaker
		rt->RunFunctionFast(pValCB, funcIndex);
		
	}catch(const duException &e){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}
