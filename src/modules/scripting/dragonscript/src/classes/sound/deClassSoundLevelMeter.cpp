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

#include "deClassSoundLevelMeter.h"
#include "deClassSoundLevelMeterSpeaker.h"
#include "deClassSoundLevelMeterListener.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../physics/deClassLayerMask.h"
#include "../../peers/dedsSoundLevelMeter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/sensor/deSoundLevelMeter.h>
#include <dragengine/resources/sensor/deSoundLevelMeterManager.h>

#include <libdscript/libdscript.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



struct sSLMNatDat{
	deSoundLevelMeter::Ref soundLevelMeter;
};



// Constructors, Destructors
//////////////////////////////

// public func new( int type )
deClassSoundLevelMeter::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSoundLevelMeter::nfNew::RunFunction(dsRunTime*, dsValue *myself){
	sSLMNatDat * const nd = new (p_GetNativeData(myself)) sSLMNatDat;
	deSoundLevelMeterManager *tsMgr = ((deClassSoundLevelMeter*)GetOwnerClass())
		->GetDS().GetGameEngine()->GetSoundLevelMeterManager();
	
	// create soundLevelMeter
	nd->soundLevelMeter = tsMgr->CreateSoundLevelMeter();
}

// public func destructor()
deClassSoundLevelMeter::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSoundLevelMeter::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSLMNatDat*>(p_GetNativeData(myself))->~sSLMNatDat();
}



// Management
///////////////

// public func SoundLevelMeterType getType()
deClassSoundLevelMeter::nfGetType::nfGetType(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSoundLevelMeterType){
}
void deClassSoundLevelMeter::nfGetType::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	rt->PushValue(((deClassSoundLevelMeter*)GetOwnerClass())->GetClassSoundLevelMeterType()
		->GetVariable(soundLevelMeter.GetType())->GetStaticValue());
}

// public func setType( SoundLevelMeterType type )
deClassSoundLevelMeter::nfSetType::nfSetType(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "setType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSoundLevelMeterType); // type
}
void deClassSoundLevelMeter::nfSetType::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	soundLevelMeter.SetType((deSoundLevelMeter::eMeterTypes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func DVector getPosition()
deClassSoundLevelMeter::nfGetPosition::nfGetPosition(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "getPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDVector){
}
void deClassSoundLevelMeter::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	const deScriptingDragonScript &ds = ((deClassSoundLevelMeter*)GetOwnerClass())->GetDS();
	
	ds.GetClassDVector()->PushDVector(rt, soundLevelMeter.GetPosition());
}

// public func setPosition( DVector position )
deClassSoundLevelMeter::nfSetPosition::nfSetPosition(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "setPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVector); // position
}
void deClassSoundLevelMeter::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	const deScriptingDragonScript &ds = ((deClassSoundLevelMeter*)GetOwnerClass())->GetDS();
	
	soundLevelMeter.SetPosition(ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject()));
}

// public func Quaternion getOrientation()
deClassSoundLevelMeter::nfGetOrientation::nfGetOrientation(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "getOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion){
}
void deClassSoundLevelMeter::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	const deScriptingDragonScript &ds = ((deClassSoundLevelMeter*)GetOwnerClass())->GetDS();
	
	ds.GetClassQuaternion()->PushQuaternion(rt, soundLevelMeter.GetOrientation());
}

// public func setOrientation( Quaternion orientation )
deClassSoundLevelMeter::nfSetOrientation::nfSetOrientation(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "setOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuaternion); // orientation
}
void deClassSoundLevelMeter::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	const deScriptingDragonScript &ds = ((deClassSoundLevelMeter*)GetOwnerClass())->GetDS();
	
	soundLevelMeter.SetOrientation(ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(0)->GetRealObject()));
}

// public func float getConeAngle()
deClassSoundLevelMeter::nfGetConeAngle::nfGetConeAngle(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "getConeAngle", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSoundLevelMeter::nfGetConeAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	rt->PushFloat(soundLevelMeter.GetConeAngle() * RAD2DEG);
}

// public func setConeAngle( float angle )
deClassSoundLevelMeter::nfSetConeAngle::nfSetConeAngle(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "setConeAngle", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // angle
}
void deClassSoundLevelMeter::nfSetConeAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	soundLevelMeter.SetConeAngle(rt->GetValue(0)->GetFloat() * DEG2RAD);
}

// public func float getAudibleDistance()
deClassSoundLevelMeter::nfGetAudibleDistance::nfGetAudibleDistance(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "getAudibleDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSoundLevelMeter::nfGetAudibleDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	rt->PushFloat(soundLevelMeter.GetAudibleDistance());
}

// public func setAudibleDistance( float audibleDistance )
deClassSoundLevelMeter::nfSetAudibleDistance::nfSetAudibleDistance(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "setAudibleDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // audibleDistance
}
void deClassSoundLevelMeter::nfSetAudibleDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	soundLevelMeter.SetAudibleDistance(rt->GetValue(0)->GetFloat());
}

// public func LayerMask getLayerMask()
deClassSoundLevelMeter::nfGetLayerMask::nfGetLayerMask(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "getLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask){
}
void deClassSoundLevelMeter::nfGetLayerMask::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	const deScriptingDragonScript &ds = ((deClassSoundLevelMeter*)GetOwnerClass())->GetDS();
	
	ds.GetClassLayerMask()->PushLayerMask(rt, soundLevelMeter.GetLayerMask());
}

// public func void setLayerMask( LayerMask layerMask )
deClassSoundLevelMeter::nfSetLayerMask::nfSetLayerMask(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "setLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLayerMask); // layerMask
}
void deClassSoundLevelMeter::nfSetLayerMask::RunFunction(dsRunTime *rt, dsValue *myself){
	deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	const deScriptingDragonScript &ds = ((deClassSoundLevelMeter*)GetOwnerClass())->GetDS();
	
	soundLevelMeter.SetLayerMask(ds.GetClassLayerMask()->GetLayerMask(rt->GetValue(0)->GetRealObject()));
}

// public func bool getEnabled()
deClassSoundLevelMeter::nfGetEnabled::nfGetEnabled(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "getEnabled", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSoundLevelMeter::nfGetEnabled::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	
	rt->PushBool(soundLevelMeter.GetEnabled());
}

// public func void setEnabled( bool enabled )
deClassSoundLevelMeter::nfSetEnabled::nfSetEnabled(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "setEnabled", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassSoundLevelMeter::nfSetEnabled::RunFunction(dsRunTime *rt, dsValue *myself){
	deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	
	soundLevelMeter.SetEnabled(rt->GetValue(0)->GetBool());
}



// public func int getAudibleSpeakerCount()
deClassSoundLevelMeter::nfGetAudibleSpeakerCount::nfGetAudibleSpeakerCount(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "getAudibleSpeakerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassSoundLevelMeter::nfGetAudibleSpeakerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	
	rt->PushInt(soundLevelMeter.GetAudibleSpeakerCount());
}

// public func SoundLevelMeterSpeaker getAudibleSpeakerAt( int index )
deClassSoundLevelMeter::nfGetAudibleSpeakerAt::nfGetAudibleSpeakerAt(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "getAudibleSpeakerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSoundLevelMeterSpeaker){
	p_AddParameter(init.clsInteger); // index
}
void deClassSoundLevelMeter::nfGetAudibleSpeakerAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	const deScriptingDragonScript &ds = ((deClassSoundLevelMeter*)GetOwnerClass())->GetDS();
	
	ds.GetClassSoundLevelMeterSpeaker()->PushSoundLevelMeterSpeaker(rt,
		soundLevelMeter.GetAudibleSpeakerAt(rt->GetValue(0)->GetInt()));
}



// public func SoundLevelMeterListener getListener()
deClassSoundLevelMeter::nfGetListener::nfGetListener(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "getListener", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSoundLevelMeterListener){
}
void deClassSoundLevelMeter::nfGetListener::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	const deScriptingDragonScript &ds = ((deClassSoundLevelMeter*)GetOwnerClass())->GetDS();
	const dedsSoundLevelMeter * const peer = (dedsSoundLevelMeter*)soundLevelMeter.GetPeerScripting();
	
	if(peer){
		rt->PushObject(peer->GetCallback(), ds.GetClassSoundLevelMeterListener());
		
	}else{
		rt->PushObject(NULL, ds.GetClassSoundLevelMeterListener());
	}
}

// public func void setListener( SoundLevelMeterListener listener )
deClassSoundLevelMeter::nfSetListener::nfSetListener(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "setListener", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSoundLevelMeterListener); // listener
}
void deClassSoundLevelMeter::nfSetListener::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter &soundLevelMeter = *(static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter);
	dedsSoundLevelMeter * const peer = (dedsSoundLevelMeter*)soundLevelMeter.GetPeerScripting();
	if(peer){
		peer->SetCallback(rt->GetValue(0)->GetRealObject());
	}
}



// public func int hashCode()
deClassSoundLevelMeter::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassSoundLevelMeter::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter * const soundLevelMeter = static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter;
	
	// hash code = memory location
	rt->PushInt((int)(intptr_t)soundLevelMeter);
}

// public func bool equals( Object obj )
deClassSoundLevelMeter::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSoundLevelMeter, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassSoundLevelMeter::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSoundLevelMeter * const soundLevelMeter = static_cast<sSLMNatDat*>(p_GetNativeData(myself))->soundLevelMeter;
	deClassSoundLevelMeter * const clsSoundLevelMeter = (deClassSoundLevelMeter*)GetOwnerClass();
	
	dsValue * const object = rt->GetValue(0);
	if(!p_IsObjOfType(object, clsSoundLevelMeter)){
		rt->PushBool(false);
		
	}else{
		rt->PushBool(soundLevelMeter == static_cast<sSLMNatDat*>(p_GetNativeData(object))->soundLevelMeter);
	}
}



// Class deClassSoundLevelMeter
/////////////////////////////////

// Constructor
////////////////

deClassSoundLevelMeter::deClassSoundLevelMeter(deScriptingDragonScript &ds) :
dsClass("SoundLevelMeter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSLMNatDat));
}

deClassSoundLevelMeter::~deClassSoundLevelMeter(){
}



// Management
///////////////

void deClassSoundLevelMeter::CreateClassMembers(dsEngine *engine){
	pClsSoundLevelMeterType = engine->GetClass("Dragengine.Scenery.SoundLevelMeterType");
	
	sInitData init;
	init.clsSoundLevelMeter = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsDVector = pDS.GetClassDVector();
	init.clsLayerMask = pDS.GetClassLayerMask();
	init.clsSoundLevelMeterSpeaker = pDS.GetClassSoundLevelMeterSpeaker();
	init.clsSoundLevelMeterListener = pDS.GetClassSoundLevelMeterListener();
	init.clsSoundLevelMeterType = pClsSoundLevelMeterType;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetType(init));
	AddFunction(new nfSetType(init));
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfGetConeAngle(init));
	AddFunction(new nfSetConeAngle(init));
	AddFunction(new nfGetAudibleDistance(init));
	AddFunction(new nfSetAudibleDistance(init));
	AddFunction(new nfGetLayerMask(init));
	AddFunction(new nfSetLayerMask(init));
	AddFunction(new nfGetEnabled(init));
	AddFunction(new nfSetEnabled(init));
	
	AddFunction(new nfGetAudibleSpeakerCount(init));
	AddFunction(new nfGetAudibleSpeakerAt(init));
	
	AddFunction(new nfGetListener(init));
	AddFunction(new nfSetListener(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSoundLevelMeter *deClassSoundLevelMeter::GetSoundLevelMeter(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	return static_cast<sSLMNatDat*>(p_GetNativeData(myself->GetBuffer()))->soundLevelMeter;
}

void deClassSoundLevelMeter::PushSoundLevelMeter(dsRunTime *rt, deSoundLevelMeter *soundLevelMeter){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!soundLevelMeter){
		rt->PushObject(NULL, this);
		return;
	}
	
	// create new value
	rt->CreateObjectNakedOnStack(this);
	static_cast<sSLMNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))
		->soundLevelMeter = soundLevelMeter;
	soundLevelMeter->AddReference();
}
