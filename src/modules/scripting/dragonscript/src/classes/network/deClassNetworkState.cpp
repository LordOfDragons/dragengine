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

#include "deClassNetworkState.h"
#include "../math/deClassPoint.h"
#include "../math/deClassPoint3.h"
#include "../math/deClassVector.h"
#include "../math/deClassVector2.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../network/deClassNetworkStateListener.h"
#include "../file/deClassMemoryFile.h"
#include "../../peers/dedsNetworkState.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/resources/deResource.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/network/deNetworkState.h>
#include <dragengine/resources/network/deNetworkStateManager.h>
#include <dragengine/resources/network/value/deNetworkValue.h>
#include <dragengine/resources/network/value/deNetworkValueInteger.h>
#include <dragengine/resources/network/value/deNetworkValueFloat.h>
#include <dragengine/resources/network/value/deNetworkValueData.h>
#include <dragengine/resources/network/value/deNetworkValueString.h>
#include <dragengine/resources/network/value/deNetworkValuePoint2.h>
#include <dragengine/resources/network/value/deNetworkValuePoint3.h>
#include <dragengine/resources/network/value/deNetworkValueVector2.h>
#include <dragengine/resources/network/value/deNetworkValueVector3.h>
#include <dragengine/resources/network/value/deNetworkValueQuaternion.h>
#include <dragengine/resources/network/value/deNetworkValueVisitorIdentify.h>

#include <libdscript/packages/default/dsClassEnumeration.h>


// native structure
struct sNSNatDat{
	deNetworkState::Ref state;
};



// Native functions
/////////////////////

// Constructors, destructors
//////////////////////////////

// public func new()
deClassNetworkState::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsNS, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNetworkState::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sNSNatDat * const nd = new (p_GetNativeData(myself)) sNSNatDat;
	deClassNetworkState * const clsNS = (deClassNetworkState*)GetOwnerClass();
	
	// create state
	nd->state = clsNS->GetDS().GetGameEngine()->GetNetworkStateManager()->CreateState(false);
}

// public func destructor()
deClassNetworkState::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsNS,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNetworkState::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sNSNatDat*>(p_GetNativeData(myself))->~sNSNatDat();
}



// Management
///////////////

// public func bool getReadOnly()
deClassNetworkState::nfGetReadOnly::nfGetReadOnly(const sInitData &init) :
dsFunction(init.clsNS, "getReadOnly", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassNetworkState::nfGetReadOnly::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	
	rt->PushBool(state.GetReadOnly());
}



// Properties
///////////////

// public func int getValueCount()
deClassNetworkState::nfGetValueCount::nfGetValueCount(const sInitData &init) :
dsFunction(init.clsNS, "getValueCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassNetworkState::nfGetValueCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	
	rt->PushInt(state.GetValueCount());
}

// public func void addValue( NetworkStateValueType type, NetworkStateValueFormat format )
deClassNetworkState::nfAddValue::nfAddValue(const sInitData &init) :
dsFunction(init.clsNS, "addValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsNetworkStateValueType); // type
	p_AddParameter(init.clsNetworkStateValueFormat); // format
}
void deClassNetworkState::nfAddValue::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW_INFO(dueNullPointer, "type");
	}
	
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deNetworkValueVisitorIdentify::eNetworkValues type = (deNetworkValueVisitorIdentify::eNetworkValues)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	deNetworkValue::Ref property;
	
	switch(type){
	case deNetworkValueVisitorIdentify::envtInteger:
	case deNetworkValueVisitorIdentify::envtFloat:
	case deNetworkValueVisitorIdentify::envtPoint2:
	case deNetworkValueVisitorIdentify::envtPoint3:
	case deNetworkValueVisitorIdentify::envtVector2:
	case deNetworkValueVisitorIdentify::envtVector3:
	case deNetworkValueVisitorIdentify::envtQuaternion:{
		if(!rt->GetValue(1)->GetRealObject()){
			DSTHROW_INFO(dueNullPointer, "type");
		}
		
		const deNetworkValue::eValueFormats format = (deNetworkValue::eValueFormats)
			static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
				*rt->GetValue( 1 )->GetRealObject() );
		
		switch(type){
		case deNetworkValueVisitorIdentify::envtInteger:
			property.TakeOver(new deNetworkValueInteger(format, 0));
			break;
			
		case deNetworkValueVisitorIdentify::envtFloat:
			property.TakeOver(new deNetworkValueFloat(format, 0.0));
			break;
			
		case deNetworkValueVisitorIdentify::envtPoint2:
			property.TakeOver(new deNetworkValuePoint2(format, decPoint()));
			break;
			
		case deNetworkValueVisitorIdentify::envtPoint3:
			property.TakeOver(new deNetworkValuePoint3(format, decPoint3()));
			break;
			
		case deNetworkValueVisitorIdentify::envtVector2:
			property.TakeOver(new deNetworkValueVector2(format, decVector2()));
			break;
			
		case deNetworkValueVisitorIdentify::envtVector3:
			property.TakeOver(new deNetworkValueVector3(format, decDVector()));
			break;
			
		case deNetworkValueVisitorIdentify::envtQuaternion:
			property.TakeOver(new deNetworkValueQuaternion(format, decQuaternion()));
			break;
			
		default:
			DSTHROW(dueInvalidParam); // can never happen
		}
		}break;
		
	case deNetworkValueVisitorIdentify::envtString:
		property.TakeOver(new deNetworkValueString);
		break;
		
	case deNetworkValueVisitorIdentify::envtData:
		property.TakeOver(new deNetworkValueData(0));
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	state.AddValue(property);
}

// public func NetworkStateValueFormat getValueFormatAt( int index )
deClassNetworkState::nfGetValueFormatAt::nfGetValueFormatAt(const sInitData &init) :
dsFunction(init.clsNS, "getValueFormatAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsNetworkStateValueFormat){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValueFormatAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	deNetworkValue::eValueFormats format;
	
	state.GetValueAt(index)->Visit(identify);
	
	if(identify.IsInteger()){
		format = identify.CastToInteger()->GetFormat();
		
	}else if(identify.IsFloat()){
		format = identify.CastToFloat()->GetFormat();
		
	}else if(identify.IsPoint2()){
		format = identify.CastToPoint2()->GetFormat();
		
	}else if(identify.IsPoint3()){
		format = identify.CastToPoint3()->GetFormat();
		
	}else if(identify.IsVector2()){
		format = identify.CastToVector2()->GetFormat();
		
	}else if(identify.IsVector3()){
		format = identify.CastToVector3()->GetFormat();
		
	}else if(identify.IsQuaternion()){
		format = identify.CastToQuaternion()->GetFormat();
		
	}else if(identify.IsString()){
		format = deNetworkValue::evfUInt8;
		
	}else if(identify.IsData()){
		format = deNetworkValue::evfUInt8;
		
	}else{
		DSTHROW(dueInvalidParam);
	}
	
	rt->PushValue(((deClassNetworkState*)GetOwnerClass())->GetClassNetworkStateValueType()
		->GetVariable(format)->GetStaticValue());
}

// public func int getValueIntAt( int index )
deClassNetworkState::nfGetValueIntAt::nfGetValueIntAt(const sInitData &init) :
dsFunction(init.clsNS, "getValueIntAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValueIntAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsInteger()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->PushInt((int)identify.CastToInteger()->GetInt());
}

// public func float getValueFloatAt( int index )
deClassNetworkState::nfGetValueFloatAt::nfGetValueFloatAt(const sInitData &init) :
dsFunction(init.clsNS, "getValueFloatAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValueFloatAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsFloat()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->PushFloat((float)identify.CastToFloat()->GetFloat());
}

// public func String getValueStringAt( int index )
deClassNetworkState::nfGetValueStringAt::nfGetValueStringAt(const sInitData &init) :
dsFunction(init.clsNS, "getValueStringAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValueStringAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsString()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->PushString(identify.CastToString()->GetString());
}

// public func Point getValuePoint2At( int index )
deClassNetworkState::nfGetValuePoint2At::nfGetValuePoint2At(const sInitData &init) :
dsFunction(init.clsNS, "getValuePoint2At", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValuePoint2At::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsPoint2()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassPoint()->PushPoint(rt, identify.CastToPoint2()->GetPoint());
}

// public func Point3 getValuePoint3At( int index )
deClassNetworkState::nfGetValuePoint3At::nfGetValuePoint3At(const sInitData &init) :
dsFunction(init.clsNS, "getValuePoint3At", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValuePoint3At::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsPoint3()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassPoint3()->PushPoint(rt, identify.CastToPoint3()->GetPoint());
}

// public func Vector2 getValueVector2At( int index )
deClassNetworkState::nfGetValueVector2At::nfGetValueVector2At(const sInitData &init) :
dsFunction(init.clsNS, "getValueVector2At", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector2){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValueVector2At::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsVector2()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassVector2()->PushVector2(rt, identify.CastToVector2()->GetVector());
}

// public func Vector getValueVector3At( int index )
deClassNetworkState::nfGetValueVector3At::nfGetValueVector3At(const sInitData &init) :
dsFunction(init.clsNS, "getValueVector3At", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValueVector3At::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsVector3()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassVector()->PushVector(rt, identify.CastToVector3()->GetVector().ToVector());
}

// public func DVector getValueDVector3At( int index )
deClassNetworkState::nfGetValueDVector3At::nfGetValueDVector3At(const sInitData &init) :
dsFunction(init.clsNS, "getValueDVector3At", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValueDVector3At::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsVector3()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassDVector()->PushDVector(rt, identify.CastToVector3()->GetVector());
}

// public func Quaternion getValueQuaternionAt( int index )
deClassNetworkState::nfGetValueQuaternionAt::nfGetValueQuaternionAt(const sInitData &init) :
dsFunction(init.clsNS, "getValueQuaternionAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValueQuaternionAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsQuaternion()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassQuaternion()->PushQuaternion(rt, identify.CastToQuaternion()->GetQuaternion());
}

// public func void setValueIntAt( int index, int value )
deClassNetworkState::nfSetValueIntAt::nfSetValueIntAt(const sInitData &init) :
dsFunction(init.clsNS, "setValueIntAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsInt); // value
}
void deClassNetworkState::nfSetValueIntAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsInteger()){
		DSTHROW(dueInvalidParam);
	}
	
	identify.CastToInteger()->SetInt(rt->GetValue(1)->GetInt());
	state.NotifyValueChanged(index);
}

// public func void setValueFloatAt( int index, float value )
deClassNetworkState::nfSetValueFloatAt::nfSetValueFloatAt(const sInitData &init) :
dsFunction(init.clsNS, "setValueFloatAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsFlt); // value
}
void deClassNetworkState::nfSetValueFloatAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsFloat()){
		DSTHROW(dueInvalidParam);
	}
	
	identify.CastToFloat()->SetFloat(rt->GetValue(1)->GetFloat());
	state.NotifyValueChanged(index);
}

// public func void setValueStringAt( int index, String value )
deClassNetworkState::nfSetValueStringAt::nfSetValueStringAt(const sInitData &init) :
dsFunction(init.clsNS, "setValueStringAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsStr); // value
}
void deClassNetworkState::nfSetValueStringAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsString()){
		DSTHROW(dueInvalidParam);
	}
	
	identify.CastToString()->SetString(rt->GetValue(1)->GetString());
	state.NotifyValueChanged(index);
}

// public func void setValuePoint2At( int index, Point value )
deClassNetworkState::nfSetValuePoint2At::nfSetValuePoint2At(const sInitData &init) :
dsFunction(init.clsNS, "setValuePoint2At", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsPt); // value
}
void deClassNetworkState::nfSetValuePoint2At::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	const decPoint &value = ds.GetClassPoint()->GetPoint(rt->GetValue(1)->GetRealObject());
	
	deNetworkValueVisitorIdentify identify;
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsPoint2()){
		DSTHROW(dueInvalidParam);
	}
	
	identify.CastToPoint2()->SetPoint(value);
	state.NotifyValueChanged(index);
}

// public func void setValuePoint3At( int index, Point3 value )
deClassNetworkState::nfSetValuePoint3At::nfSetValuePoint3At(const sInitData &init) :
dsFunction(init.clsNS, "setValuePoint3At", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsPt3); // value
}
void deClassNetworkState::nfSetValuePoint3At::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	const decPoint3 &value = ds.GetClassPoint3()->GetPoint(rt->GetValue(1)->GetRealObject());
	
	deNetworkValueVisitorIdentify identify;
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsPoint3()){
		DSTHROW(dueInvalidParam);
	}
	
	identify.CastToPoint3()->SetPoint(value);
	state.NotifyValueChanged(index);
}

// public func void setValueVector2At( int index, Vector2 value )
deClassNetworkState::nfSetValueVector2At::nfSetValueVector2At(const sInitData &init) :
dsFunction(init.clsNS, "setValueVector2At", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsVector2); // value
}
void deClassNetworkState::nfSetValueVector2At::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	const decVector2 &value = ds.GetClassVector2()->GetVector2(rt->GetValue(1)->GetRealObject());
	
	deNetworkValueVisitorIdentify identify;
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsVector2()){
		DSTHROW(dueInvalidParam);
	}
	
	identify.CastToVector2()->SetVector(value);
	state.NotifyValueChanged(index);
}

// public func void setValueVector3At( int index, Vector value )
deClassNetworkState::nfSetValueVector3At::nfSetValueVector3At(const sInitData &init) :
dsFunction(init.clsNS, "setValueVector3At", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsVec); // value
}
void deClassNetworkState::nfSetValueVector3At::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	const decVector &value = ds.GetVector(rt->GetValue(1)->GetRealObject());
	
	deNetworkValueVisitorIdentify identify;
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsVector3()){
		DSTHROW(dueInvalidParam);
	}
	
	identify.CastToVector3()->SetVector(value);
	state.NotifyValueChanged(index);
}

// public func void setValueDVector3At( int index, DVector value )
deClassNetworkState::nfSetValueDVector3At::nfSetValueDVector3At(const sInitData &init) :
dsFunction(init.clsNS, "setValueDVector3At", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsDVec); // value
}
void deClassNetworkState::nfSetValueDVector3At::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	const decDVector &value = ds.GetClassDVector()->GetDVector(rt->GetValue(1)->GetRealObject());
	
	deNetworkValueVisitorIdentify identify;
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsVector3()){
		DSTHROW(dueInvalidParam);
	}
	
	identify.CastToVector3()->SetVector(value);
	state.NotifyValueChanged(index);
}

// public func void setValueQuaternionAt( int index, Quaternion value )
deClassNetworkState::nfSetValueQuaternionAt::nfSetValueQuaternionAt(const sInitData &init) :
dsFunction(init.clsNS, "setValueQuaternionAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsQuat); // value
}
void deClassNetworkState::nfSetValueQuaternionAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	const decQuaternion &value = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(1)->GetRealObject());
	
	deNetworkValueVisitorIdentify identify;
	state.GetValueAt(index)->Visit(identify);
	if(!identify.IsQuaternion()){
		DSTHROW(dueInvalidParam);
	}
	
	identify.CastToQuaternion()->SetQuaternion(value);
	state.NotifyValueChanged(index);
}

// public func float getValuePrecisionAt( int index )
deClassNetworkState::nfGetValuePrecisionAt::nfGetValuePrecisionAt(const sInitData &init) :
dsFunction(init.clsNS, "getValuePrecisionAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // index
}
void deClassNetworkState::nfGetValuePrecisionAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	
	if(identify.IsFloat()){
		rt->PushFloat((float)identify.CastToFloat()->GetPrecision());
		
	}else if(identify.IsQuaternion()){
		rt->PushFloat((float)identify.CastToQuaternion()->GetPrecision());
		
	}else if(identify.IsVector2()){
		rt->PushFloat((float)identify.CastToVector2()->GetPrecision());
		
	}else if(identify.IsVector3()){
		rt->PushFloat((float)identify.CastToVector3()->GetPrecision());
		
	}else{
		DSTHROW(dueInvalidParam);
	}
}

// public func void setValuePrecisionAt( int index, float precision )
deClassNetworkState::nfSetValuePrecisionAt::nfSetValuePrecisionAt(const sInitData &init) :
dsFunction(init.clsNS, "setValuePrecisionAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsFlt); // precision
}
void deClassNetworkState::nfSetValuePrecisionAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	const float precision = rt->GetValue(1)->GetFloat();
	deNetworkValueVisitorIdentify identify;
	
	state.GetValueAt(index)->Visit(identify);
	
	if(identify.IsFloat()){
		identify.CastToFloat()->SetPrecision(precision);
		
	}else if(identify.IsQuaternion()){
		identify.CastToQuaternion()->SetPrecision(precision);
		
	}else if(identify.IsVector2()){
		identify.CastToVector2()->SetPrecision(precision);
		
	}else if(identify.IsVector3()){
		identify.CastToVector3()->SetPrecision(precision);
		
	}else{
		DSTHROW(dueInvalidParam);
	}
	
	state.NotifyPrecisionChanged(index);
}

// public func void getValueDataAt(int index, MemoryFile data)
deClassNetworkState::nfGetValueDataAt::nfGetValueDataAt(const sInitData &init) :
dsFunction(init.clsNS, "getValueDataAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsMemoryFile); // data
}
void deClassNetworkState::nfGetValueDataAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	decMemoryFile * const file = ds.GetClassMemoryFile()->GetMemoryFile(rt->GetValue(1)->GetRealObject());
	if(!file){
		DSTHROW_INFO(dueNullPointer, "data");
	}
	
	state.GetValueAt(index)->Visit(identify);
	
	if(identify.IsData()){
		const deNetworkValueData &data = *identify.CastToData();
		if(data.GetLength() != file->GetLength()){
			file->Resize(data.GetLength());
		}
		if(data.GetLength() > 0){
			memcpy(file->GetPointer(), data.GetData(), data.GetLength());
		}
		
	}else{
		DSTHROW(dueInvalidParam);
	}
}

// public func void setValueDataAt(int index, MemoryFile data)
deClassNetworkState::nfSetValueDataAt::nfSetValueDataAt(const sInitData &init) :
dsFunction(init.clsNS, "setValueDataAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // index
	p_AddParameter(init.clsMemoryFile); // data
}
void deClassNetworkState::nfSetValueDataAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	const int index = rt->GetValue(0)->GetInt();
	deNetworkValueVisitorIdentify identify;
	
	const decMemoryFile * const file = ds.GetClassMemoryFile()->GetMemoryFile(rt->GetValue(1)->GetRealObject());
	if(!file){
		DSTHROW_INFO(dueNullPointer, "data");
	}
	
	state.GetValueAt(index)->Visit(identify);
	
	if(identify.IsData()){
		deNetworkValueData &data = *identify.CastToData();
		if(file->GetLength() != data.GetLength()){
			data.SetLength(file->GetLength());
		}
		memcpy(data.GetData(), file->GetPointer(), data.GetLength());
		
	}else{
		DSTHROW(dueInvalidParam);
	}
	
	state.NotifyPrecisionChanged(index);
}



// Notifications
//////////////////

// public func NetworkStateListener getStateListener()
deClassNetworkState::nfGetStateListener::nfGetStateListener(const sInitData &init) :
dsFunction(init.clsNS, "getStateListener", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsNSL){
}
void deClassNetworkState::nfGetStateListener::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	dedsNetworkState * const scrState = (dedsNetworkState*)state.GetPeerScripting();
	const deScriptingDragonScript &ds = ((deClassNetworkState*)GetOwnerClass())->GetDS();
	
	if(scrState){
		rt->PushObject(scrState->GetCallback(), ds.GetClassNetworkStateListener());
		
	}else{
		rt->PushObject(NULL, ds.GetClassNetworkStateListener());
	}
}

// public func void setStateListener( NetworkStateListener listener )
deClassNetworkState::nfSetStateListener::nfSetStateListener(const sInitData &init) :
dsFunction(init.clsNS, "setStateListener", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsNSL); // listener
}
void deClassNetworkState::nfSetStateListener::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkState &state = *(static_cast<sNSNatDat*>(p_GetNativeData(myself))->state);
	dedsNetworkState * const scrState = (dedsNetworkState*)state.GetPeerScripting();
	if(scrState){
		scrState->SetCallback(rt->GetValue(0)->GetRealObject());
	}
}



// Misc
/////////

// public func int hashCode()
deClassNetworkState::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsNS, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassNetworkState::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState * const state = static_cast<sNSNatDat*>(p_GetNativeData(myself))->state;
	rt->PushInt((int)(intptr_t)state);
}

// public func bool equals( Object object )
deClassNetworkState::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsNS, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // object
}
void deClassNetworkState::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkState * const state = static_cast<sNSNatDat*>(p_GetNativeData(myself))->state;
	deClassNetworkState * const clsNS = (deClassNetworkState*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsNS)){
		rt->PushBool(false);
		
	}else{
		const deNetworkState * const otherState = static_cast<sNSNatDat*>(p_GetNativeData(obj))->state;
		rt->PushBool(state == otherState);
	}
}



// Class deClassNetworkState
//////////////////////////////

// Constructor
////////////////

deClassNetworkState::deClassNetworkState(deScriptingDragonScript &ds) :
dsClass("NetworkState", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_NETWORKING);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sNSNatDat));
}

deClassNetworkState::~deClassNetworkState(){
}



// Management
///////////////

void deClassNetworkState::CreateClassMembers(dsEngine *engine){
	pClsNetworkStateValueType = engine->GetClass("Dragengine.Networking.NetworkStateValueType");
	pClsNetworkStateValueFormat = engine->GetClass("Dragengine.Networking.NetworkStateValueFormat");
	
	sInitData init;
	init.clsNS = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsPt = pDS.GetClassPoint();
	init.clsPt3 = pDS.GetClassPoint3();
	init.clsVector2 = pDS.GetClassVector2();
	init.clsVec = pDS.GetClassVector();
	init.clsDVec = pDS.GetClassDVector();
	init.clsQuat = pDS.GetClassQuaternion();
	init.clsNSL = pDS.GetClassNetworkStateListener();
	init.clsNetworkStateValueType = pClsNetworkStateValueType;
	init.clsNetworkStateValueFormat = pClsNetworkStateValueFormat;
	init.clsMemoryFile = pDS.GetClassMemoryFile();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetReadOnly(init));
	
	AddFunction(new nfGetValueCount(init));
	AddFunction(new nfAddValue(init));
	AddFunction(new nfGetValueFormatAt(init));
	AddFunction(new nfGetValueIntAt(init));
	AddFunction(new nfGetValueFloatAt(init));
	AddFunction(new nfGetValueStringAt(init));
	AddFunction(new nfGetValuePoint2At(init));
	AddFunction(new nfGetValuePoint3At(init));
	AddFunction(new nfGetValueVector2At(init));
	AddFunction(new nfGetValueVector3At(init));
	AddFunction(new nfGetValueDVector3At(init));
	AddFunction(new nfGetValueQuaternionAt(init));
	AddFunction(new nfSetValueIntAt(init));
	AddFunction(new nfSetValueFloatAt(init));
	AddFunction(new nfSetValueStringAt(init));
	AddFunction(new nfSetValuePoint2At(init));
	AddFunction(new nfSetValuePoint3At(init));
	AddFunction(new nfSetValueVector2At(init));
	AddFunction(new nfSetValueVector3At(init));
	AddFunction(new nfSetValueDVector3At(init));
	AddFunction(new nfSetValueQuaternionAt(init));
	AddFunction(new nfGetValuePrecisionAt(init));
	AddFunction(new nfSetValuePrecisionAt(init));
	AddFunction(new nfGetValueDataAt(init));
	AddFunction(new nfSetValueDataAt(init));
	
	AddFunction(new nfGetStateListener(init));
	AddFunction(new nfSetStateListener(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deNetworkState *deClassNetworkState::GetNetworkState(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sNSNatDat*>(p_GetNativeData(myself->GetBuffer()))->state;
}

void deClassNetworkState::PushNetworkState(dsRunTime *rt, deNetworkState *state){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!state){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	static_cast<sNSNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->state = state;
	state->AddReference();
}
