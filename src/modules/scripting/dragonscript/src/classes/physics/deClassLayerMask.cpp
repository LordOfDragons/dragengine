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

#include "deClassLayerMask.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <libdscript/exceptions.h>



struct sCFNatDat{
	decLayerMask *layerMask;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassLayerMask::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsLyM,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassLayerMask::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCFNatDat *nd = static_cast<sCFNatDat*>(p_GetNativeData(myself);
	
	// create layer mask
	nd->layerMask = new decLayerMask;
}

// public func new( LayerMask layerMask )
deClassLayerMask::nfNew2::nfNew2(const sInitData &init) : dsFunction(init.clsLyM,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLyM); // layerMask
}
void deClassLayerMask::nfNew2::RunFunction(dsRunTime *rt, dsValue *myself){
	sCFNatDat * const nd = new (p_GetNativeData(myself)) sCFNatDat;
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	dsRealObject *obj = rt->GetValue(0)->GetRealObject();
	
	// create layer mask
	nd->layerMask = new decLayerMask(clsLyM.GetLayerMask(obj));
}

// static public func LayerMask newWith( int bit )
deClassLayerMask::nfNewWith::nfNewWith(const sInitData &init) : dsFunction(init.clsLyM,
"newWith", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsLyM){
	p_AddParameter(init.clsInt); // bit
}
void deClassLayerMask::nfNewWith::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const int bit = rt->GetValue(0)->GetInt();
	
	decLayerMask layerMask;
	layerMask.SetBit(bit);
	
	clsLyM.PushLayerMask(rt, layerMask);
}

// static public func LayerMask newWith( int bit1, int bit2 )
deClassLayerMask::nfNewWith2::nfNewWith2(const sInitData &init) : dsFunction(init.clsLyM,
"newWith", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsLyM){
	p_AddParameter(init.clsInt); // bit1
	p_AddParameter(init.clsInt); // bit2
}
void deClassLayerMask::nfNewWith2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const int bit1 = rt->GetValue(0)->GetInt();
	const int bit2 = rt->GetValue(1)->GetInt();
	
	decLayerMask layerMask;
	layerMask.SetBit(bit1);
	layerMask.SetBit(bit2);
	
	clsLyM.PushLayerMask(rt, layerMask);
}

// static public func LayerMask newWith( int bit1, int bit2, int bit3 )
deClassLayerMask::nfNewWith3::nfNewWith3(const sInitData &init) : dsFunction(init.clsLyM,
"newWith", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsLyM){
	p_AddParameter(init.clsInt); // bit1
	p_AddParameter(init.clsInt); // bit2
	p_AddParameter(init.clsInt); // bit3
}
void deClassLayerMask::nfNewWith3::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const int bit1 = rt->GetValue(0)->GetInt();
	const int bit2 = rt->GetValue(1)->GetInt();
	const int bit3 = rt->GetValue(2)->GetInt();
	
	decLayerMask layerMask;
	layerMask.SetBit(bit1);
	layerMask.SetBit(bit2);
	layerMask.SetBit(bit3);
	
	clsLyM.PushLayerMask(rt, layerMask);
}

// static public func LayerMask newWith( int bit1, int bit2, int bit3, int bit4 )
deClassLayerMask::nfNewWith4::nfNewWith4(const sInitData &init) : dsFunction(init.clsLyM,
"newWith", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsLyM){
	p_AddParameter(init.clsInt); // bit1
	p_AddParameter(init.clsInt); // bit2
	p_AddParameter(init.clsInt); // bit3
	p_AddParameter(init.clsInt); // bit4
}
void deClassLayerMask::nfNewWith4::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const int bit1 = rt->GetValue(0)->GetInt();
	const int bit2 = rt->GetValue(1)->GetInt();
	const int bit3 = rt->GetValue(2)->GetInt();
	const int bit4 = rt->GetValue(3)->GetInt();
	
	decLayerMask layerMask;
	layerMask.SetBit(bit1);
	layerMask.SetBit(bit2);
	layerMask.SetBit(bit3);
	layerMask.SetBit(bit4);
	
	clsLyM.PushLayerMask(rt, layerMask);
}

// static public func LayerMask newAll()
deClassLayerMask::nfNewAll::nfNewAll(const sInitData &init) : dsFunction(init.clsLyM,
"newAll", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsLyM){
}
void deClassLayerMask::nfNewAll::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	clsLyM.PushLayerMask(rt, ~decLayerMask());
}

// public func destructor()
deClassLayerMask::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsLyM,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassLayerMask::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sCFNatDat*>(p_GetNativeData(myself))->~sCFNatDat();
}



// Management
///////////////

// public func void clearMask()
deClassLayerMask::nfClearMask::nfClearMask(const sInitData &init) : dsFunction(init.clsLyM,
"clearMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassLayerMask::nfClearMask::RunFunction(dsRunTime *rt, dsValue *myself){
	decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	layermask.ClearMask();
}

// public func void setBit( int bit )
deClassLayerMask::nfSetBit::nfSetBit(const sInitData &init) : dsFunction(init.clsLyM,
"setBit", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // bit
}
void deClassLayerMask::nfSetBit::RunFunction(dsRunTime *rt, dsValue *myself){
	decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	layermask.SetBit(rt->GetValue(0)->GetInt());
}

// public func void clearBit( int bit )
deClassLayerMask::nfClearBit::nfClearBit(const sInitData &init) : dsFunction(init.clsLyM,
"clearBit", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // bit
}
void deClassLayerMask::nfClearBit::RunFunction(dsRunTime *rt, dsValue *myself){
	decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	layermask.ClearBit(rt->GetValue(0)->GetInt());
}

// public func bool isBitSet( int bit )
deClassLayerMask::nfIsBitSet::nfIsBitSet(const sInitData &init) : dsFunction(init.clsLyM,
"isBitSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsInt); // bit
}
void deClassLayerMask::nfIsBitSet::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	rt->PushBool(layermask.IsBitSet(rt->GetValue(0)->GetInt()));
}

// public func bool isBitCleared( int bit )
deClassLayerMask::nfIsBitCleared::nfIsBitCleared(const sInitData &init) : dsFunction(init.clsLyM,
"isBitCleared", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsInt); // bit
}
void deClassLayerMask::nfIsBitCleared::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	rt->PushBool(layermask.IsBitCleared(rt->GetValue(0)->GetInt()));
}

// public func bool isEmpty()
deClassLayerMask::nfIsEmpty::nfIsEmpty(const sInitData &init) : dsFunction(init.clsLyM,
"isEmpty", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassLayerMask::nfIsEmpty::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	rt->PushBool(layermask.IsEmpty());
}



// public func bool matches( LayerMask layerMask )
deClassLayerMask::nfMatches::nfMatches(const sInitData &init) : dsFunction(init.clsLyM,
"matches", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsLyM); // layerMask
}
void deClassLayerMask::nfMatches::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const decLayerMask &layermask2 = clsLyM.GetLayerMask(rt->GetValue(0)->GetRealObject());
	
	rt->PushBool(layermask.Matches(layermask2));
}

// public func bool matchesNot( LayerMask layerMask )
deClassLayerMask::nfMatchesNot::nfMatchesNot(const sInitData &init) : dsFunction(init.clsLyM,
"matchesNot", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsLyM); // layerMask
}
void deClassLayerMask::nfMatchesNot::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const decLayerMask &layermask2 = clsLyM.GetLayerMask(rt->GetValue(0)->GetRealObject());
	
	rt->PushBool(layermask.MatchesNot(layermask2));
}



// public func String toBitString()
deClassLayerMask::nfToBitString::nfToBitString(const sInitData &init) : dsFunction(init.clsLyM,
"toBitString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassLayerMask::nfToBitString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	rt->PushString(layermask.ToBitString());
}

// public func void setFromBitString( String string )
deClassLayerMask::nfSetFromBitString::nfSetFromBitString(const sInitData &init) : dsFunction(init.clsLyM,
"setFromBitString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // string
}
void deClassLayerMask::nfSetFromBitString::RunFunction(dsRunTime *rt, dsValue *myself){
	decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	layermask.SetFromBitString(rt->GetValue(0)->GetString());
}

// public func String toHexString()
deClassLayerMask::nfToHexString::nfToHexString(const sInitData &init) : dsFunction(init.clsLyM,
"toHexString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassLayerMask::nfToHexString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	rt->PushString(layermask.ToHexString());
}

// public func void setFromHexString( String string )
deClassLayerMask::nfSetFromHexString::nfSetFromHexString(const sInitData &init) : dsFunction(init.clsLyM,
"setFromHexString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // string
}
void deClassLayerMask::nfSetFromHexString::RunFunction(dsRunTime *rt, dsValue *myself){
	decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	layermask.SetFromHexString(rt->GetValue(0)->GetString());
}



// Operators
//////////////

// public func LayerMask ~()
deClassLayerMask::nfOpInverse::nfOpInverse(const sInitData &init) : dsFunction(init.clsLyM,
"~", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsLyM){
}
void deClassLayerMask::nfOpInverse::RunFunction(dsRunTime *rt, dsValue *myself){
	decLayerMask layermask(*(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask));
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	
	clsLyM.PushLayerMask(rt, ~layermask);
}

// public func LayerMask &( LayerMask layerMask )
deClassLayerMask::nfOpAnd::nfOpAnd(const sInitData &init) : dsFunction(init.clsLyM,
"&", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsLyM){
	p_AddParameter(init.clsLyM); // layerMask
}
void deClassLayerMask::nfOpAnd::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const decLayerMask &layermask2 = clsLyM.GetLayerMask(rt->GetValue(0)->GetRealObject());
	
	clsLyM.PushLayerMask(rt, layermask & layermask2);
}

// public func LayerMask |( LayerMask layerMask )
deClassLayerMask::nfOpOr::nfOpOr(const sInitData &init) : dsFunction(init.clsLyM,
"|", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsLyM){
	p_AddParameter(init.clsLyM); // layerMask
}
void deClassLayerMask::nfOpOr::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const decLayerMask &layermask2 = clsLyM.GetLayerMask(rt->GetValue(0)->GetRealObject());
	
	clsLyM.PushLayerMask(rt, layermask | layermask2);
}

// public func LayerMask ^( LayerMask layerMask )
deClassLayerMask::nfOpXor::nfOpXor(const sInitData &init) : dsFunction(init.clsLyM,
"^", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsLyM){
	p_AddParameter(init.clsLyM); // layerMask
}
void deClassLayerMask::nfOpXor::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const decLayerMask &layermask2 = clsLyM.GetLayerMask(rt->GetValue(0)->GetRealObject());
	
	clsLyM.PushLayerMask(rt, layermask ^ layermask2);
}

// public func LayerMask &=( LayerMask layerMask )
deClassLayerMask::nfOpAndAssign::nfOpAndAssign(const sInitData &init) : dsFunction(init.clsLyM,
"&=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsLyM){
	p_AddParameter(init.clsLyM); // layerMask
}
void deClassLayerMask::nfOpAndAssign::RunFunction(dsRunTime *rt, dsValue *myself){
	decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const decLayerMask &layermask2 = clsLyM.GetLayerMask(rt->GetValue(0)->GetRealObject());
	
	clsLyM.PushLayerMask(rt, layermask &= layermask2);
}

// public func LayerMask |=( LayerMask layerMask )
deClassLayerMask::nfOpOrAssign::nfOpOrAssign(const sInitData &init) : dsFunction(init.clsLyM,
"|=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsLyM){
	p_AddParameter(init.clsLyM); // layerMask
}
void deClassLayerMask::nfOpOrAssign::RunFunction(dsRunTime *rt, dsValue *myself){
	decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const decLayerMask &layermask2 = clsLyM.GetLayerMask(rt->GetValue(0)->GetRealObject());
	
	clsLyM.PushLayerMask(rt, layermask |= layermask2);
}

// public func LayerMask ^=( LayerMask layerMask )
deClassLayerMask::nfOpXorAssign::nfOpXorAssign(const sInitData &init) : dsFunction(init.clsLyM,
"^=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsLyM){
	p_AddParameter(init.clsLyM); // layerMask
}
void deClassLayerMask::nfOpXorAssign::RunFunction(dsRunTime *rt, dsValue *myself){
	decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	deClassLayerMask &clsLyM = *((deClassLayerMask*)GetOwnerClass());
	const decLayerMask &layermask2 = clsLyM.GetLayerMask(rt->GetValue(0)->GetRealObject());
	
	clsLyM.PushLayerMask(rt, layermask ^= layermask2);
}



// Special
////////////

// public func int hashCode()
deClassLayerMask::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsLyM, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassLayerMask::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	decLayerMask *layermask = static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask;
	rt->PushInt((int)(intptr_t)layermask);
}

// public func bool equals( Object object )
deClassLayerMask::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsLyM, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // object
}
void deClassLayerMask::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassLayerMask *clsLyM = (deClassLayerMask*)GetOwnerClass();
	dsValue *object = rt->GetValue(0);
	
	if(!p_IsObjOfType(object, clsLyM)){
		rt->PushBool(false);
		
	}else{
		const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
		const decLayerMask &otherMask = *(static_cast<sCFNatDat*>(p_GetNativeData(object))->layerMask);
		rt->PushBool(layermask == otherMask);
	}
}

// public func String toString()
deClassLayerMask::nfToString::nfToString(const sInitData &init) : dsFunction(init.clsLyM,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassLayerMask::nfToString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decLayerMask &layermask = *(static_cast<sCFNatDat*>(p_GetNativeData(myself))->layerMask);
	rt->PushString(layermask.ToHexString());
}



// Class deClassLayerMask
///////////////////////////

// Constructor
////////////////

deClassLayerMask::deClassLayerMask(deScriptingDragonScript *ds) :
dsClass("LayerMask", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sCFNatDat));
}

deClassLayerMask::~deClassLayerMask(){
}



// Management
///////////////

void deClassLayerMask::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsLyM = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNew2(init));
	AddFunction(new nfNewWith(init));
	AddFunction(new nfNewWith2(init));
	AddFunction(new nfNewWith3(init));
	AddFunction(new nfNewWith4(init));
	AddFunction(new nfNewAll(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfClearMask(init));
	AddFunction(new nfSetBit(init));
	AddFunction(new nfClearBit(init));
	AddFunction(new nfIsBitSet(init));
	AddFunction(new nfIsBitCleared(init));
	AddFunction(new nfIsEmpty(init));
	
	AddFunction(new nfToBitString(init));
	AddFunction(new nfSetFromBitString(init));
	AddFunction(new nfToHexString(init));
	AddFunction(new nfSetFromHexString(init));
	
	AddFunction(new nfMatches(init));
	AddFunction(new nfMatchesNot(init));
	
	AddFunction(new nfOpInverse(init));
	AddFunction(new nfOpAnd(init));
	AddFunction(new nfOpOr(init));
	AddFunction(new nfOpXor(init));
	AddFunction(new nfOpAndAssign(init));
	AddFunction(new nfOpOrAssign(init));
	AddFunction(new nfOpXorAssign(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	AddFunction(new nfToString(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

const decLayerMask &deClassLayerMask::GetLayerMask(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	return *(static_cast<sCFNatDat*>(p_GetNativeData(myself->GetBuffer()))->layerMask);
}

void deClassLayerMask::PushLayerMask(dsRunTime *rt, const decLayerMask &layermask){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sCFNatDat &nd = *static_cast<sCFNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.layerMask = NULL;
	
	try{
		nd.layerMask = new decLayerMask(layermask);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
