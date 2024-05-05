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

#include "deClassLocomotion.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../math/deClassVector.h"
#include "../math/deClassQuaternion.h"
#include "../animation/deClassAnimatorInstance.h"
#include "../animation/deClassAnimatorController.h"
#include "../collider/deClassCollider.h"
#include "../collider/deClassColliderCollisionTest.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../locomotion/dedsLocomotion.h"
#include "../../locomotion/dedsLControllerMapping.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



struct sLocoNatDat{
	dedsLocomotion *locomotion;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassLocomotion::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsLoco,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassLocomotion::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sLocoNatDat &nd = *( ( sLocoNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.locomotion = NULL;
	
	// create locomotion
	nd.locomotion = new dedsLocomotion;
}

// public func destructor()
deClassLocomotion::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsLoco,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassLocomotion::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sLocoNatDat &nd = *( ( sLocoNatDat* )p_GetNativeData( myself ) );
	
	if( nd.locomotion ){
		nd.locomotion->FreeReference();
		nd.locomotion = NULL;
	}
}



// Management
///////////////

// public func bool getCanTurn()
deClassLocomotion::nfGetCanTurn::nfGetCanTurn( const sInitData &init ) : dsFunction( init.clsLoco,
"getCanTurn", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassLocomotion::nfGetCanTurn::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushBool( locomotion.GetCanTurn() );
}

// public func void setCanTurn( bool canTurn )
deClassLocomotion::nfSetCanTurn::nfSetCanTurn( const sInitData &init ) : dsFunction( init.clsLoco,
"setCanTurn", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // canTurn
}

void deClassLocomotion::nfSetCanTurn::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetCanTurn( rt->GetValue( 0 )->GetBool() );
}



// public func float getLimitLookUp()
deClassLocomotion::nfGetLimitLookUp::nfGetLimitLookUp( const sInitData &init ) : dsFunction( init.clsLoco,
"getLimitLookUp", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLimitLookUp::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLimitLookUp() );
}

// public func void setLimitLookUp( float limit )
deClassLocomotion::nfSetLimitLookUp::nfSetLimitLookUp( const sInitData &init ) : dsFunction( init.clsLoco,
"setLimitLookUp", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // limit
}

void deClassLocomotion::nfSetLimitLookUp::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetLimitLookUp( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLimitLookDown()
deClassLocomotion::nfGetLimitLookDown::nfGetLimitLookDown( const sInitData &init ) : dsFunction( init.clsLoco,
"getLimitLookDown", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLimitLookDown::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLimitLookDown() );
}

// public func void setLimitLookDown( float limit )
deClassLocomotion::nfSetLimitLookDown::nfSetLimitLookDown( const sInitData &init ) : dsFunction( init.clsLoco,
"setLimitLookDown", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // limit
}

void deClassLocomotion::nfSetLimitLookDown::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetLimitLookDown( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustTimeVertical()
deClassLocomotion::nfGetAdjustTimeVertical::nfGetAdjustTimeVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustTimeVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustTimeVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLookVertical().GetAdjustTime() );
}

// public func void setAdjustTimeVertical( float time )
deClassLocomotion::nfSetAdjustTimeVertical::nfSetAdjustTimeVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustTimeVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustTimeVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetLookVertical().SetAdjustTime( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustRangeVertical()
deClassLocomotion::nfGetAdjustRangeVertical::nfGetAdjustRangeVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustRangeVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustRangeVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLookVertical().GetAdjustRange() );
}

// public func void setAdjustRangeVertical( float time )
deClassLocomotion::nfSetAdjustRangeVertical::nfSetAdjustRangeVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustRangeVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustRangeVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetLookVertical().SetAdjustRange( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLookVerticalGoal()
deClassLocomotion::nfGetLookVerticalGoal::nfGetLookVerticalGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"getLookVerticalGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLookVerticalGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLookVertical().GetGoal() );
}

// public func void setLookVerticalGoal( float goal )
deClassLocomotion::nfSetLookVerticalGoal::nfSetLookVerticalGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"setLookVerticalGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // goal
}

void deClassLocomotion::nfSetLookVerticalGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetLookVerticalGoal( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLookVertical()
deClassLocomotion::nfGetLookVertical::nfGetLookVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"getLookVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLookVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLookVertical().GetValue() );
}

// public func void setLookVertical( float value )
deClassLocomotion::nfSetLookVertical::nfSetLookVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"setLookVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}

void deClassLocomotion::nfSetLookVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetLookVertical( rt->GetValue( 0 )->GetFloat() );
}



// public func float getLimitLookLeft()
deClassLocomotion::nfGetLimitLookLeft::nfGetLimitLookLeft( const sInitData &init ) : dsFunction( init.clsLoco,
"getLimitLookLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLimitLookLeft::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLimitLookLeft() );
}

// public func void setLimitLookLeft( float limit )
deClassLocomotion::nfSetLimitLookLeft::nfSetLimitLookLeft( const sInitData &init ) : dsFunction( init.clsLoco,
"setLimitLookLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // limit
}

void deClassLocomotion::nfSetLimitLookLeft::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetLimitLookLeft( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLimitLookRight()
deClassLocomotion::nfGetLimitLookRight::nfGetLimitLookRight( const sInitData &init ) : dsFunction( init.clsLoco,
"getLimitLookRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLimitLookRight::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLimitLookRight() );
}

// public func void setLimitLookRight( float limit )
deClassLocomotion::nfSetLimitLookRight::nfSetLimitLookRight( const sInitData &init ) : dsFunction( init.clsLoco,
"setLimitLookRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // limit
}

void deClassLocomotion::nfSetLimitLookRight::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetLimitLookRight( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustTimeHorizontal()
deClassLocomotion::nfGetAdjustTimeHorizontal::nfGetAdjustTimeHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustTimeHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustTimeHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLookHorizontal().GetAdjustTime() );
}

// public func void setAdjustTimeHorizontal( float time )
deClassLocomotion::nfSetAdjustTimeHorizontal::nfSetAdjustTimeHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustTimeHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustTimeHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetLookHorizontal().SetAdjustTime( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustRangeHorizontal()
deClassLocomotion::nfGetAdjustRangeHorizontal::nfGetAdjustRangeHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustRangeHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustRangeHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLookHorizontal().GetAdjustRange() );
}

// public func void setAdjustRangeHorizontal( float time )
deClassLocomotion::nfSetAdjustRangeHorizontal::nfSetAdjustRangeHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustRangeHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustRangeHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetLookHorizontal().SetAdjustRange( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLookHorizontalGoal()
deClassLocomotion::nfGetLookHorizontalGoal::nfGetLookHorizontalGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"getLookHorizontalGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLookHorizontalGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLookHorizontal().GetGoal() );
}

// public func void setLookHorizontalGoal( float goal )
deClassLocomotion::nfSetLookHorizontalGoal::nfSetLookHorizontalGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"setLookHorizontalGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // goal
}

void deClassLocomotion::nfSetLookHorizontalGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetLookHorizontal().SetGoal( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLookHorizontal()
deClassLocomotion::nfGetLookHorizontal::nfGetLookHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getLookHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLookHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLookHorizontal().GetValue() );
}

// public func void setLookHorizontal( float value )
deClassLocomotion::nfSetLookHorizontal::nfSetLookHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setLookHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}

void deClassLocomotion::nfSetLookHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetLookHorizontal().SetValue( rt->GetValue( 0 )->GetFloat() );
}

// public func float getTurnAdjustLookHorizontal()
deClassLocomotion::nfGetTurnAdjustLookHorizontal::nfGetTurnAdjustLookHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getTurnAdjustLookHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassLocomotion::nfGetTurnAdjustLookHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushBool( locomotion.GetTurnAdjustLookHorizontal() );
}

// public func void setTurnAdjustLookHorizontal( bool turnAdjust )
deClassLocomotion::nfSetTurnAdjustLookHorizontal::nfSetTurnAdjustLookHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setTurnAdjustLookHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // turnAdjust
}

void deClassLocomotion::nfSetTurnAdjustLookHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetTurnAdjustLookHorizontal( rt->GetValue( 0 )->GetBool() );
}



// public func float getAdjustTimeAnalogMovingHorizontal()
deClassLocomotion::nfGetAdjustTimeAnalogMovingHorizontal::nfGetAdjustTimeAnalogMovingHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustTimeAnalogMovingHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustTimeAnalogMovingHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetAnalogMovingHorizontal().GetAdjustTime() );
}

// public func void setAdjustTimeAnalogMovingHorizontal( float time )
deClassLocomotion::nfSetAdjustTimeAnalogMovingHorizontal::nfSetAdjustTimeAnalogMovingHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustTimeAnalogMovingHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustTimeAnalogMovingHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetAnalogMovingHorizontal().SetAdjustTime( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustRangeAnalogMovingHorizontal()
deClassLocomotion::nfGetAdjustRangeAnalogMovingHorizontal::nfGetAdjustRangeAnalogMovingHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustRangeAnalogMovingHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustRangeAnalogMovingHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetAnalogMovingHorizontal().GetAdjustRange() );
}

// public func void setAdjustRangeAnalogMovingHorizontal( float time )
deClassLocomotion::nfSetAdjustRangeAnalogMovingHorizontal::nfSetAdjustRangeAnalogMovingHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustRangeAnalogMovingHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustRangeAnalogMovingHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetAnalogMovingHorizontal().SetAdjustRange( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAnalogMovingHorizontalGoal()
deClassLocomotion::nfGetAnalogMovingHorizontalGoal::nfGetAnalogMovingHorizontalGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"getAnalogMovingHorizontalGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAnalogMovingHorizontalGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetAnalogMovingHorizontal().GetGoal() );
}

// public func void setAnalogMovingHorizontalGoal( float value )
deClassLocomotion::nfSetAnalogMovingHorizontalGoal::nfSetAnalogMovingHorizontalGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"setAnalogMovingHorizontalGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}

void deClassLocomotion::nfSetAnalogMovingHorizontalGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetAnalogMovingHorizontal().SetGoal( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAnalogMovingHorizontal()
deClassLocomotion::nfGetAnalogMovingHorizontal::nfGetAnalogMovingHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getAnalogMovingHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAnalogMovingHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetAnalogMovingHorizontal().GetValue() );
}

// public func void setAnalogMovingHorizontal( float value )
deClassLocomotion::nfSetAnalogMovingHorizontal::nfSetAnalogMovingHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setAnalogMovingHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}

void deClassLocomotion::nfSetAnalogMovingHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetAnalogMovingHorizontal().SetValue( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAnalogMovingVertical()
deClassLocomotion::nfGetAnalogMovingVertical::nfGetAnalogMovingVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"getAnalogMovingVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAnalogMovingVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetAnalogMovingVertical() );
}

// public func void setAnalogMovingVertical( float value )
deClassLocomotion::nfSetAnalogMovingVertical::nfSetAnalogMovingVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"setAnalogMovingVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}

void deClassLocomotion::nfSetAnalogMovingVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetAnalogMovingVertical( rt->GetValue( 0 )->GetFloat() );
}

// public func float getTurnHorizontal()
deClassLocomotion::nfGetTurnHorizontal::nfGetTurnHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getTurnHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetTurnHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTurnHorizontal() );
}

// public func void setTurnHorizontal( float value )
deClassLocomotion::nfSetTurnHorizontal::nfSetTurnHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setTurnHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}

void deClassLocomotion::nfSetTurnHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetTurnHorizontal( rt->GetValue( 0 )->GetFloat() );
}



// public func float getAnalogMovingSpeed()
deClassLocomotion::nfGetAnalogMovingSpeed::nfGetAnalogMovingSpeed( const sInitData &init ) : dsFunction( init.clsLoco,
"getAnalogMovingSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAnalogMovingSpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetAnalogMovingSpeed() );
}

// public func void setAnalogMovingSpeed( float value )
deClassLocomotion::nfSetAnalogMovingSpeed::nfSetAnalogMovingSpeed( const sInitData &init ) : dsFunction( init.clsLoco,
"setAnalogMovingSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}

void deClassLocomotion::nfSetAnalogMovingSpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetAnalogMovingSpeed( rt->GetValue( 0 )->GetFloat() );
}



// public func float getAdjustTimeOrientation()
deClassLocomotion::nfGetAdjustTimeOrientation::nfGetAdjustTimeOrientation( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustTimeOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustTimeOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetAdjustTimeOrientation() );
}

// public func void setAdjustTimeOrientation( float time )
deClassLocomotion::nfSetAdjustTimeOrientation::nfSetAdjustTimeOrientation( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustTimeOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustTimeOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetAdjustTimeOrientation( rt->GetValue( 0 )->GetFloat() );
}

// public func float getOrientation()
deClassLocomotion::nfGetOrientation::nfGetOrientation( const sInitData &init ) : dsFunction( init.clsLoco,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetOrientation() );
}

// public func void setOrientation( float orientation )
deClassLocomotion::nfSetOrientation::nfSetOrientation( const sInitData &init ) : dsFunction( init.clsLoco,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // orientation
}

void deClassLocomotion::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetOrientation( rt->GetValue( 0 )->GetFloat() );
}

// public func Quaternion getOrientationQuaternion()
deClassLocomotion::nfGetOrientationQuaternion::nfGetOrientationQuaternion( const sInitData &init ) : dsFunction( init.clsLoco,
"getOrientationQuaternion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion ){
}

void deClassLocomotion::nfGetOrientationQuaternion::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	ds.GetClassQuaternion()->PushQuaternion( rt, locomotion.GetOrientationQuaternion() );
}

// public func void setOrientationQuaternion( Quaternion orientation )
deClassLocomotion::nfSetOrientationQuaternion::nfSetOrientationQuaternion( const sInitData &init ) : dsFunction( init.clsLoco,
"setOrientationQuaternion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuaternion ); // orientation
}

void deClassLocomotion::nfSetOrientationQuaternion::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 0 )->GetRealObject() );
	
	locomotion.SetOrientationQuaternion( orientation );
}

// public func float getTurningSpeed()
deClassLocomotion::nfGetTurningSpeed::nfGetTurningSpeed( const sInitData &init ) : dsFunction( init.clsLoco,
"getTurningSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetTurningSpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTurningSpeed() );
}

// public func void setTurningSpeed( float speed )
deClassLocomotion::nfSetTurningSpeed::nfSetTurningSpeed( const sInitData &init ) : dsFunction( init.clsLoco,
"setTurningSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // speed
}

void deClassLocomotion::nfSetTurningSpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetTurningSpeed( rt->GetValue( 0 )->GetFloat() );
}

// public func Vector getAngularVelocity()
deClassLocomotion::nfGetAngularVelocity::nfGetAngularVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"getAngularVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
}

void deClassLocomotion::nfGetAngularVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	ds.GetClassVector()->PushVector( rt, locomotion.GetAngularVelocity() );
}

// public func void setAngularVelocity( Vector velocity )
deClassLocomotion::nfSetAngularVelocity::nfSetAngularVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"setAngularVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVector ); // velocity
}

void deClassLocomotion::nfSetAngularVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	const decVector &velocity = ds.GetClassVector()->GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	locomotion.SetAngularVelocity( velocity );
}



// public func bool getIsMoving()
deClassLocomotion::nfGetIsMoving::nfGetIsMoving( const sInitData &init ) : dsFunction( init.clsLoco,
"getIsMoving", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassLocomotion::nfGetIsMoving::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushBool( locomotion.GetIsMoving() );
}

// public func void setIsMoving( bool isMoving )
deClassLocomotion::nfSetIsMoving::nfSetIsMoving( const sInitData &init ) : dsFunction( init.clsLoco,
"setIsMoving", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // isMoving
}

void deClassLocomotion::nfSetIsMoving::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetIsMoving( rt->GetValue( 0 )->GetBool() );
}

// public func float getMovingSpeed()
deClassLocomotion::nfGetMovingSpeed::nfGetMovingSpeed( const sInitData &init ) : dsFunction( init.clsLoco,
"getMovingSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetMovingSpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	if( fabsf( locomotion.GetMovingDirection() ) > 90.0f ){
		rt->PushFloat( -locomotion.GetMovingSpeed() );
		
	}else{
		rt->PushFloat( locomotion.GetMovingSpeed() );
	}
}

// public func float getAbsMovingSpeed()
deClassLocomotion::nfGetAbsMovingSpeed::nfGetAbsMovingSpeed( const sInitData &init ) : dsFunction( init.clsLoco,
"getAbsMovingSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAbsMovingSpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetMovingSpeed() );
}

// public func void setMovingSpeed( float speed )
deClassLocomotion::nfSetMovingSpeed::nfSetMovingSpeed( const sInitData &init ) : dsFunction( init.clsLoco,
"setMovingSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // speed
}

void deClassLocomotion::nfSetMovingSpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetMovingSpeed( rt->GetValue( 0 )->GetFloat() );
}

// public func float getMovingOrientation()
deClassLocomotion::nfGetMovingOrientation::nfGetMovingOrientation( const sInitData &init ) : dsFunction( init.clsLoco,
"getMovingOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetMovingOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetMovingOrientation() );
}

// public func void setMovingOrientation( float orientation )
deClassLocomotion::nfSetMovingOrientation::nfSetMovingOrientation( const sInitData &init ) : dsFunction( init.clsLoco,
"setMovingOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // orientation
}

void deClassLocomotion::nfSetMovingOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetMovingOrientation( rt->GetValue( 0 )->GetFloat() );
}

// public func float getMovingDirection()
deClassLocomotion::nfGetMovingDirection::nfGetMovingDirection( const sInitData &init ) : dsFunction( init.clsLoco,
"getMovingDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetMovingDirection::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetMovingDirection() );
}

// public func Vector getMovingDirectionVector()
deClassLocomotion::nfGetMovingDirectionVector::nfGetMovingDirectionVector( const sInitData &init ) :
dsFunction( init.clsLoco, "getMovingDirectionVector", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
}

void deClassLocomotion::nfGetMovingDirectionVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	const float direction = locomotion.GetMovingDirection();
	ds.GetClassVector()->PushVector( rt, decVector( sinf( direction ), 0.0f, cosf( direction ) ) );
}

// public func void setMovingDirection( float direction )
deClassLocomotion::nfSetMovingDirection::nfSetMovingDirection( const sInitData &init ) : dsFunction( init.clsLoco,
"setMovingDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // direction
}

void deClassLocomotion::nfSetMovingDirection::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetMovingDirection( rt->GetValue( 0 )->GetFloat() );
}



// public func Vector getLinearVelocity()
deClassLocomotion::nfGetLinearVelocity::nfGetLinearVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"getLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
}

void deClassLocomotion::nfGetLinearVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	ds.GetClassVector()->PushVector( rt, locomotion.GetLinearVelocity().GetValue() );
}

// public func void setLinearVelocity( Vector velocity )
deClassLocomotion::nfSetLinearVelocity::nfSetLinearVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"setLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVector ); // velocity
}

void deClassLocomotion::nfSetLinearVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	const decVector &velocity = ds.GetClassVector()->GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	locomotion.GetLinearVelocity().SetValue( velocity );
	locomotion.GetLinearVelocity().SetGoal( velocity );
}

// public func float getAdjustTimeLinearVelocity()
deClassLocomotion::nfGetAdjustTimeLinearVelocity::nfGetAdjustTimeLinearVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustTimeLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustTimeLinearVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLinearVelocity().GetAdjustTime() );
}

// public func void setAdjustTimeLinearVelocity( float time )
deClassLocomotion::nfSetAdjustTimeLinearVelocity::nfSetAdjustTimeLinearVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustTimeLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustTimeLinearVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetLinearVelocity().SetAdjustTime( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustRangeLinearVelocity()
deClassLocomotion::nfGetAdjustRangeLinearVelocity::nfGetAdjustRangeLinearVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustRangeLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustRangeLinearVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLinearVelocity().GetAdjustRange() );
}

// public func void setAdjustRangeLinearVelocity( float range )
deClassLocomotion::nfSetAdjustRangeLinearVelocity::nfSetAdjustRangeLinearVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustRangeLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // range
}

void deClassLocomotion::nfSetAdjustRangeLinearVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetLinearVelocity().SetAdjustRange( rt->GetValue( 0 )->GetFloat() );
}



// public func float getAdjustTimeStance()
deClassLocomotion::nfGetAdjustTimeStance::nfGetAdjustTimeStance( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustTimeStance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustTimeStance::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetStance().GetAdjustTime() );
}

// public func void setAdjustTimeStance( float time )
deClassLocomotion::nfSetAdjustTimeStance::nfSetAdjustTimeStance( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustTimeStance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustTimeStance::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetStance().SetAdjustTime( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustRangeStance()
deClassLocomotion::nfGetAdjustRangeStance::nfGetAdjustRangeStance( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustRangeStance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustRangeStance::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetStance().GetAdjustRange() );
}

// public func void setAdjustRangeStance( float time )
deClassLocomotion::nfSetAdjustRangeStance::nfSetAdjustRangeStance( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustRangeStance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustRangeStance::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetStance().SetAdjustRange( rt->GetValue( 0 )->GetFloat() );
}

// public func float getStance()
deClassLocomotion::nfGetStance::nfGetStance( const sInitData &init ) : dsFunction( init.clsLoco,
"getStance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetStance::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetStance().GetValue() );
}

// public func void setStance( float stance )
deClassLocomotion::nfSetStance::nfSetStance( const sInitData &init ) : dsFunction( init.clsLoco,
"setStance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // stance
}

void deClassLocomotion::nfSetStance::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetStance().SetValue( rt->GetValue( 0 )->GetFloat() );
}

// public func float getStanceGoal()
deClassLocomotion::nfGetStanceGoal::nfGetStanceGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"getStanceGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetStanceGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetStance().GetGoal() );
}

// public func void setStanceGoal( float stance )
deClassLocomotion::nfSetStanceGoal::nfSetStanceGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"setStanceGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // stance
}

void deClassLocomotion::nfSetStanceGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetStance().SetGoal( rt->GetValue( 0 )->GetFloat() );
}



// public func bool getIsTurningIP()
deClassLocomotion::nfGetIsTurningIP::nfGetIsTurningIP( const sInitData &init ) : dsFunction( init.clsLoco,
"getIsTurningIP", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassLocomotion::nfGetIsTurningIP::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushBool( locomotion.GetIsTurningIP() );
}

// public func void setIsTurningIP( bool turnInPlace )
deClassLocomotion::nfSetIsTurningIP::nfSetIsTurningIP( const sInitData &init ) : dsFunction( init.clsLoco,
"setIsTurningIP", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // turnInPlace
}

void deClassLocomotion::nfSetIsTurningIP::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetIsTurningIP( rt->GetValue( 0 )->GetBool() );
}

// public func float getAdjustTimeTurnIP()
deClassLocomotion::nfGetAdjustTimeTurnIP::nfGetAdjustTimeTurnIP( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustTimeTurnIP", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustTimeTurnIP::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetAdjustTimeTurnIP() );
}

// public func void setAdjustTimeTurnIP( float time )
deClassLocomotion::nfSetAdjustTimeTurnIP::nfSetAdjustTimeTurnIP( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustTimeTurnIP", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}

void deClassLocomotion::nfSetAdjustTimeTurnIP::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetAdjustTimeTurnIP( rt->GetValue( 0 )->GetFloat() );
}

// public func float getTurnIP()
deClassLocomotion::nfGetTurnIP::nfGetTurnIP( const sInitData &init ) : dsFunction( init.clsLoco,
"getTurnIP", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetTurnIP::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTurnIP() );
}

// public func void setTurnIP( float value )
deClassLocomotion::nfSetTurnIP::nfSetTurnIP( const sInitData &init ) : dsFunction( init.clsLoco,
"setTurnIP", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}

void deClassLocomotion::nfSetTurnIP::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetTurnIP( rt->GetValue( 0 )->GetFloat() );
}

// public func bool getResetTimeTurnIP()
deClassLocomotion::nfGetResetTimeTurnIP::nfGetResetTimeTurnIP( const sInitData &init ) : dsFunction( init.clsLoco,
"getResetTimeTurnIP", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassLocomotion::nfGetResetTimeTurnIP::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushBool( locomotion.GetResetTimeTurnIP() );
}

// public func void setResetTimeTurnIP( bool reset )
deClassLocomotion::nfSetResetTimeTurnIP::nfSetResetTimeTurnIP( const sInitData &init ) : dsFunction( init.clsLoco,
"setResetTimeTurnIP", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // reset
}

void deClassLocomotion::nfSetResetTimeTurnIP::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetResetTimeTurnIP( rt->GetValue( 0 )->GetBool() );
}

// public func bool getReverseTimeTurnIP()
deClassLocomotion::nfGetReverseTimeTurnIP::nfGetReverseTimeTurnIP( const sInitData &init ) : dsFunction( init.clsLoco,
"getReverseTimeTurnIP", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassLocomotion::nfGetReverseTimeTurnIP::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushBool( locomotion.GetReverseTimeTurnIP() );
}

// public func void setReverseTimeTurnIP( bool reverse )
deClassLocomotion::nfSetReverseTimeTurnIP::nfSetReverseTimeTurnIP( const sInitData &init ) : dsFunction( init.clsLoco,
"setReverseTimeTurnIP", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // reverse
}

void deClassLocomotion::nfSetReverseTimeTurnIP::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetReverseTimeTurnIP( rt->GetValue( 0 )->GetBool() );
}



// public func bool getCanTurnInPlace()
deClassLocomotion::nfGetCanTurnInPlace::nfGetCanTurnInPlace( const sInitData &init ) : dsFunction( init.clsLoco,
"getCanTurnInPlace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassLocomotion::nfGetCanTurnInPlace::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	rt->PushBool( locomotion.GetCanTurnInPlace() );
}

// public func void setCanTurnInPlace( bool canTurnInPlace )
deClassLocomotion::nfSetCanTurnInPlace::nfSetCanTurnInPlace( const sInitData &init ) : dsFunction( init.clsLoco,
"setCanTurnInPlace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // canTurnInPlace
}

void deClassLocomotion::nfSetCanTurnInPlace::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	locomotion.SetCanTurnInPlace( rt->GetValue( 0 )->GetBool() );
}

// public func float getLimitTurnInPlaceLeft()
deClassLocomotion::nfGetLimitTurnInPlaceLeft::nfGetLimitTurnInPlaceLeft( const sInitData &init ) : dsFunction( init.clsLoco,
"getLimitTurnInPlaceLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLimitTurnInPlaceLeft::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	rt->PushFloat( locomotion.GetLimitTurnInPlaceLeft() );
}

// public func void setLimitTurnInPlaceLeft( float limit )
deClassLocomotion::nfSetLimitTurnInPlaceLeft::nfSetLimitTurnInPlaceLeft( const sInitData &init ) : dsFunction( init.clsLoco,
"setLimitTurnInPlaceLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // limit
}

void deClassLocomotion::nfSetLimitTurnInPlaceLeft::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	locomotion.SetLimitTurnInPlaceLeft( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLimitTurnInPlaceRight()
deClassLocomotion::nfGetLimitTurnInPlaceRight::nfGetLimitTurnInPlaceRight( const sInitData &init ) : dsFunction( init.clsLoco,
"getLimitTurnInPlaceRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLimitTurnInPlaceRight::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	rt->PushFloat( locomotion.GetLimitTurnInPlaceRight() );
}

// public func void setLimitTurnInPlaceRight( float limit )
deClassLocomotion::nfSetLimitTurnInPlaceRight::nfSetLimitTurnInPlaceRight( const sInitData &init ) : dsFunction( init.clsLoco,
"setLimitTurnInPlaceRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // limit
}

void deClassLocomotion::nfSetLimitTurnInPlaceRight::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	locomotion.SetLimitTurnInPlaceRight( rt->GetValue( 0 )->GetFloat() );
}



// public func bool getResetTimeWalk()
deClassLocomotion::nfGetResetTimeWalk::nfGetResetTimeWalk( const sInitData &init ) : dsFunction( init.clsLoco,
"getResetTimeWalk", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassLocomotion::nfGetResetTimeWalk::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushBool( locomotion.GetResetTimeWalk() );
}

// public func void setResetTimeWalk( bool reset )
deClassLocomotion::nfSetResetTimeWalk::nfSetResetTimeWalk( const sInitData &init ) : dsFunction( init.clsLoco,
"setResetTimeWalk", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // reset
}

void deClassLocomotion::nfSetResetTimeWalk::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetResetTimeWalk( rt->GetValue( 0 )->GetBool() );
}



// public func bool getCanTilt()
deClassLocomotion::nfGetCanTilt::nfGetCanTilt( const sInitData &init ) : dsFunction( init.clsLoco,
"getCanTilt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassLocomotion::nfGetCanTilt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushBool( locomotion.GetCanTilt() );
}

// public func void setCanTilt( bool canTilt )
deClassLocomotion::nfSetCanTilt::nfSetCanTilt( const sInitData &init ) : dsFunction( init.clsLoco,
"setCanTilt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // canTilt
}

void deClassLocomotion::nfSetCanTilt::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetCanTilt( rt->GetValue( 0 )->GetBool() );
}

// public func LocomotionTilt getTiltMode()
deClassLocomotion::nfGetTiltMode::nfGetTiltMode( const sInitData &init ) : dsFunction( init.clsLoco,
"getTiltMode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsLocomotionTilt ){
}

void deClassLocomotion::nfGetTiltMode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushValue( ( ( deClassLocomotion* )GetOwnerClass() )->GetClassLocomotionTilt()
		->GetVariable( locomotion.GetTiltMode() )->GetStaticValue() );
}

// public func void setTiltMode( LocomotionTilt mode )
deClassLocomotion::nfSetTiltMode::nfSetTiltMode( const sInitData &init ) : dsFunction( init.clsLoco,
"setTiltMode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLocomotionTilt ); // mode
}

void deClassLocomotion::nfSetTiltMode::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetTiltMode( ( dedsLocomotion::eTiltModes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void setCCTTiltSingle( ColliderCollisionTest collisionTest )
deClassLocomotion::nfSetCCTTiltSingle::nfSetCCTTiltSingle( const sInitData &init ) : dsFunction( init.clsLoco,
"setCCTTiltSingle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCCT ); // collisionTest
}

void deClassLocomotion::nfSetCCTTiltSingle::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	deColliderCollisionTest * const collisionTest = ds.GetClassColliderCollisionTest()->GetCollisionTest( rt->GetValue( 0 )->GetRealObject() );
	locomotion.SetCCTTiltSingle( collisionTest );
}

// public func void setCCTTiltFrontLeft( ColliderCollisionTest collisionTest )
deClassLocomotion::nfSetCCTTiltFrontLeft::nfSetCCTTiltFrontLeft( const sInitData &init ) : dsFunction( init.clsLoco,
"setCCTTiltFrontLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCCT ); // collisionTest
}

void deClassLocomotion::nfSetCCTTiltFrontLeft::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	deColliderCollisionTest * const collisionTest = ds.GetClassColliderCollisionTest()->GetCollisionTest( rt->GetValue( 0 )->GetRealObject() );
	locomotion.SetCCTTiltFrontLeft( collisionTest );
}

// public func void setCCTTiltFrontRight( ColliderCollisionTest collisionTest )
deClassLocomotion::nfSetCCTTiltFrontRight::nfSetCCTTiltFrontRight( const sInitData &init ) : dsFunction( init.clsLoco,
"setCCTTiltFrontRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCCT ); // collisionTest
}

void deClassLocomotion::nfSetCCTTiltFrontRight::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	deColliderCollisionTest * const collisionTest = ds.GetClassColliderCollisionTest()->GetCollisionTest( rt->GetValue( 0 )->GetRealObject() );
	locomotion.SetCCTTiltFrontRight( collisionTest );
}

// public func void setCCTTiltBackLeft( ColliderCollisionTest collisionTest )
deClassLocomotion::nfSetCCTTiltBackLeft::nfSetCCTTiltBackLeft( const sInitData &init ) : dsFunction( init.clsLoco,
"setCCTTiltBackLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCCT ); // collisionTest
}

void deClassLocomotion::nfSetCCTTiltBackLeft::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	deColliderCollisionTest * const collisionTest = ds.GetClassColliderCollisionTest()->GetCollisionTest( rt->GetValue( 0 )->GetRealObject() );
	locomotion.SetCCTTiltBackLeft( collisionTest );
}

// public func void setCCTTiltBackRight( ColliderCollisionTest collisionTest )
deClassLocomotion::nfSetCCTTiltBackRight::nfSetCCTTiltBackRight( const sInitData &init ) : dsFunction( init.clsLoco,
"setCCTTiltBackRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCCT ); // collisionTest
}

void deClassLocomotion::nfSetCCTTiltBackRight::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	deColliderCollisionTest * const collisionTest = ds.GetClassColliderCollisionTest()->GetCollisionTest( rt->GetValue( 0 )->GetRealObject() );
	locomotion.SetCCTTiltBackRight( collisionTest );
}



// public func float getLimitTiltUp()
deClassLocomotion::nfGetLimitTiltUp::nfGetLimitTiltUp( const sInitData &init ) : dsFunction( init.clsLoco,
"getLimitTiltUp", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLimitTiltUp::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLimitTiltUp() );
}

// public func void setLimitTiltUp( float limit )
deClassLocomotion::nfSetLimitTiltUp::nfSetLimitTiltUp( const sInitData &init ) : dsFunction( init.clsLoco,
"setLimitTiltUp", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // limit
}

void deClassLocomotion::nfSetLimitTiltUp::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetLimitTiltUp( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLimitTiltDown()
deClassLocomotion::nfGetLimitTiltDown::nfGetLimitTiltDown( const sInitData &init ) : dsFunction( init.clsLoco,
"getLimitTiltDown", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLimitTiltDown::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLimitTiltDown() );
}

// public func void setLimitTiltDown( float limit )
deClassLocomotion::nfSetLimitTiltDown::nfSetLimitTiltDown( const sInitData &init ) : dsFunction( init.clsLoco,
"setLimitTiltDown", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // limit
}

void deClassLocomotion::nfSetLimitTiltDown::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetLimitTiltDown( rt->GetValue( 0 )->GetFloat() );
}

// public func float getTiltVertical()
deClassLocomotion::nfGetTiltVertical::nfGetTiltVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"getTiltVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetTiltVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTiltVertical().GetValue() );
}

// public func void setTiltVertical( float tiltVertical )
deClassLocomotion::nfSetTiltVertical::nfSetTiltVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"setTiltVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // tiltVertical
}

void deClassLocomotion::nfSetTiltVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetTiltVertical( rt->GetValue( 0 )->GetFloat() );
}

// public func float getTiltVerticalGoal()
deClassLocomotion::nfGetTiltVerticalGoal::nfGetTiltVerticalGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"getTiltVerticalGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetTiltVerticalGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTiltVertical().GetGoal() );
}

// public func void setTiltVerticalGoal( float tiltVertical )
deClassLocomotion::nfSetTiltVerticalGoal::nfSetTiltVerticalGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"setTiltVerticalGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // tiltVertical
}

void deClassLocomotion::nfSetTiltVerticalGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetTiltVerticalGoal( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustTimeTiltVertical()
deClassLocomotion::nfGetAdjustTimeTiltVertical::nfGetAdjustTimeTiltVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustTimeTiltVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustTimeTiltVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTiltVertical().GetAdjustTime() );
}

// public func void setAdjustTimeTiltVertical( float tiltVertical )
deClassLocomotion::nfSetAdjustTimeTiltVertical::nfSetAdjustTimeTiltVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustTimeTiltVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // tiltVertical
}

void deClassLocomotion::nfSetAdjustTimeTiltVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetTiltVertical().SetAdjustTime( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustRangeTiltVertical()
deClassLocomotion::nfGetAdjustRangeTiltVertical::nfGetAdjustRangeTiltVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustRangeTiltVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustRangeTiltVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTiltVertical().GetAdjustRange() );
}

// public func void setAdjustRangeTiltVertical( float tiltVertical )
deClassLocomotion::nfSetAdjustRangeTiltVertical::nfSetAdjustRangeTiltVertical( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustRangeTiltVertical", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // tiltVertical
}

void deClassLocomotion::nfSetAdjustRangeTiltVertical::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetTiltVertical().SetAdjustRange( rt->GetValue( 0 )->GetFloat() );
}



// public func float getLimitTiltLeft()
deClassLocomotion::nfGetLimitTiltLeft::nfGetLimitTiltLeft( const sInitData &init ) : dsFunction( init.clsLoco,
"getLimitTiltLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLimitTiltLeft::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLimitTiltLeft() );
}

// public func void setLimitTiltLeft( float limit )
deClassLocomotion::nfSetLimitTiltLeft::nfSetLimitTiltLeft( const sInitData &init ) : dsFunction( init.clsLoco,
"setLimitTiltLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // limit
}

void deClassLocomotion::nfSetLimitTiltLeft::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetLimitTiltLeft( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLimitTiltRight()
deClassLocomotion::nfGetLimitTiltRight::nfGetLimitTiltRight( const sInitData &init ) : dsFunction( init.clsLoco,
"getLimitTiltRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetLimitTiltRight::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetLimitTiltRight() );
}

// public func void setLimitTiltRight( float limit )
deClassLocomotion::nfSetLimitTiltRight::nfSetLimitTiltRight( const sInitData &init ) : dsFunction( init.clsLoco,
"setLimitTiltRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // limit
}

void deClassLocomotion::nfSetLimitTiltRight::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetLimitTiltRight( rt->GetValue( 0 )->GetFloat() );
}

// public func float getTiltHorizontal()
deClassLocomotion::nfGetTiltHorizontal::nfGetTiltHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getTiltHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetTiltHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTiltHorizontal().GetValue() );
}

// public func void setTiltHorizontal( float tiltHorizontal )
deClassLocomotion::nfSetTiltHorizontal::nfSetTiltHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setTiltHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // tiltHorizontal
}

void deClassLocomotion::nfSetTiltHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetTiltHorizontal( rt->GetValue( 0 )->GetFloat() );
}

// public func float getTiltHorizontalGoal()
deClassLocomotion::nfGetTiltHorizontalGoal::nfGetTiltHorizontalGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"getTiltHorizontalGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetTiltHorizontalGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTiltHorizontal().GetGoal() );
}

// public func void setTiltHorizontalGoal( float tiltHorizontal )
deClassLocomotion::nfSetTiltHorizontalGoal::nfSetTiltHorizontalGoal( const sInitData &init ) : dsFunction( init.clsLoco,
"setTiltHorizontalGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // tiltHorizontal
}

void deClassLocomotion::nfSetTiltHorizontalGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetTiltHorizontalGoal( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustTimeTiltHorizontal()
deClassLocomotion::nfGetAdjustTimeTiltHorizontal::nfGetAdjustTimeTiltHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustTimeTiltHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustTimeTiltHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTiltHorizontal().GetAdjustTime() );
}

// public func void setAdjustTimeTiltHorizontal( float tiltHorizontal )
deClassLocomotion::nfSetAdjustTimeTiltHorizontal::nfSetAdjustTimeTiltHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustTimeTiltHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // tiltHorizontal
}

void deClassLocomotion::nfSetAdjustTimeTiltHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetTiltHorizontal().SetAdjustTime( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustRangeTiltHorizontal()
deClassLocomotion::nfGetAdjustRangeTiltHorizontal::nfGetAdjustRangeTiltHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"getAdjustRangeTiltHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetAdjustRangeTiltHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushFloat( locomotion.GetTiltHorizontal().GetAdjustRange() );
}

// public func void setAdjustRangeTiltHorizontal( float tiltHorizontal )
deClassLocomotion::nfSetAdjustRangeTiltHorizontal::nfSetAdjustRangeTiltHorizontal( const sInitData &init ) : dsFunction( init.clsLoco,
"setAdjustRangeTiltHorizontal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // tiltHorizontal
}

void deClassLocomotion::nfSetAdjustRangeTiltHorizontal::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.GetTiltHorizontal().SetAdjustRange( rt->GetValue( 0 )->GetFloat() );
}



// public func float getTiltOffset()
deClassLocomotion::nfGetTiltOffset::nfGetTiltOffset( const sInitData &init ) : dsFunction( init.clsLoco,
"getTiltOffset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassLocomotion::nfGetTiltOffset::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	rt->PushFloat( locomotion.GetTiltOffset() );
}

// public func void setTiltOffset( float tiltVertical )
deClassLocomotion::nfSetTiltOffset::nfSetTiltOffset( const sInitData &init ) : dsFunction( init.clsLoco,
"setTiltOffset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // tiltVertical
}

void deClassLocomotion::nfSetTiltOffset::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	locomotion.SetTiltOffset( rt->GetValue( 0 )->GetFloat() );
}


// public func addControllerMapping( AnimatorController controller, LocomotionAttribute attribute )
deClassLocomotion::nfAddControllerMapping::nfAddControllerMapping( const sInitData &init ) :
dsFunction( init.clsLoco, "addControllerMapping", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsAnimatorCtrl ); // controller
	p_AddParameter( init.clsLocomotionAttribute ); // attribute
}

void deClassLocomotion::nfAddControllerMapping::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	deAnimator *acAnimator = NULL;
	deAnimatorInstance *acInstance = NULL;
	int acController = 0;
	
	ds.GetClassAnimatorController()->GetController( rt->GetValue( 0 )->GetRealObject(),
		acAnimator, acInstance, acController );
	if( ! acInstance || acAnimator ){
		DSTHROW( dueInvalidParam );
	}
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	const dedsLControllerMapping::eAttributes attribute = ( dedsLControllerMapping::eAttributes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() );
	
	locomotion.AddControllerMapping( acInstance, acController, attribute );
}

// public func void removeControllerMapping( AnimatorController controller )
deClassLocomotion::nfRemoveControllerMapping::nfRemoveControllerMapping( const sInitData &init ) :
dsFunction( init.clsLoco, "removeControllerMapping", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsAnimatorCtrl ); // controller
}

void deClassLocomotion::nfRemoveControllerMapping::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	deAnimator *acAnimator = NULL;
	deAnimatorInstance *acInstance = NULL;
	int acController = 0;
	
	ds.GetClassAnimatorController()->GetController( rt->GetValue( 0 )->GetRealObject(),
		acAnimator, acInstance, acController );
	if( ! acInstance || acAnimator ){
		DSTHROW( dueInvalidParam );
	}
	
	locomotion.RemoveControllerMapping( acInstance, acController );
}

// public func void removeAllControllerMappings()
deClassLocomotion::nfRemoveAllControllerMappings::nfRemoveAllControllerMappings( const sInitData &init ) :
dsFunction( init.clsLoco, "removeAllControllerMappings", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassLocomotion::nfRemoveAllControllerMappings::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.RemoveAllControllerMappings();
}



// public func Collider getAICollider()
deClassLocomotion::nfGetAICollider::nfGetAICollider( const sInitData &init ) : dsFunction( init.clsLoco,
"getAICollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCollider ){
}

void deClassLocomotion::nfGetAICollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	ds.GetClassCollider()->PushCollider( rt, locomotion.GetAICollider() );
}

// public func void setAICollider( Collider collider )
deClassLocomotion::nfSetAICollider::nfSetAICollider( const sInitData &init ) : dsFunction( init.clsLoco,
"setAICollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCollider ); // collider
}

void deClassLocomotion::nfSetAICollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	
	locomotion.SetAICollider( collider );
}

// public func bool getUpdateAIColliderAngularVelocity()
deClassLocomotion::nfGetUpdateAIColliderAngularVelocity::nfGetUpdateAIColliderAngularVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"getUpdateAIColliderAngularVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassLocomotion::nfGetUpdateAIColliderAngularVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	rt->PushBool( locomotion.GetUpdateAIColliderAngularVelocity() );
}

// public func void setUpdateAIColliderAngularVelocity( bool canTilt )
deClassLocomotion::nfSetUpdateAIColliderAngularVelocity::nfSetUpdateAIColliderAngularVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"setUpdateAIColliderAngularVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // canTilt
}

void deClassLocomotion::nfSetUpdateAIColliderAngularVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.SetUpdateAIColliderAngularVelocity( rt->GetValue( 0 )->GetBool() );
}



// public func void cancelMovement()
deClassLocomotion::nfCancelMovement::nfCancelMovement( const sInitData &init ) : dsFunction( init.clsLoco,
"cancelMovement", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassLocomotion::nfCancelMovement::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.CancelMovement();
}

// public func void cancelInput()
deClassLocomotion::nfCancelInput::nfCancelInput( const sInitData &init ) : dsFunction( init.clsLoco,
"cancelInput", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassLocomotion::nfCancelInput::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.CancelInput();
}

// public func void cancelMotion()
deClassLocomotion::nfCancelMotion::nfCancelMotion( const sInitData &init ) : dsFunction( init.clsLoco,
"cancelMotion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassLocomotion::nfCancelMotion::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.CancelMotion();
}

// public func void cancelTurnInPlace()
deClassLocomotion::nfCancelTurnInPlace::nfCancelTurnInPlace( const sInitData &init ) : dsFunction( init.clsLoco,
"cancelTurnInPlace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassLocomotion::nfCancelTurnInPlace::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.CancelTurnInPlace();
}

// public func void forceBodyAdjustment()
deClassLocomotion::nfForceBodyAdjustment::nfForceBodyAdjustment( const sInitData &init ) : dsFunction( init.clsLoco,
"forceBodyAdjustment", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassLocomotion::nfForceBodyAdjustment::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.ForceBodyAdjustment();
}



// public func void updateLooking( float elapsed )
deClassLocomotion::nfUpdateLooking::nfUpdateLooking( const sInitData &init ) : dsFunction( init.clsLoco,
"updateLooking", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // elapsed
}

void deClassLocomotion::nfUpdateLooking::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.UpdateLooking( rt->GetValue( 0 )->GetFloat() );
}

// public func void applyStates()
deClassLocomotion::nfApplyStates::nfApplyStates( const sInitData &init ) : dsFunction( init.clsLoco,
"applyStates", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassLocomotion::nfApplyStates::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.ApplyStates();
}

// public func void updateLocomotion( float elapsed )
deClassLocomotion::nfUpdateLocomotion::nfUpdateLocomotion( const sInitData &init ) : dsFunction( init.clsLoco,
"updateLocomotion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // elapsed
}

void deClassLocomotion::nfUpdateLocomotion::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.UpdateLocomotion( rt->GetValue( 0 )->GetFloat() );
}

// public func void updateIsMoving()
deClassLocomotion::nfUpdateIsMoving::nfUpdateIsMoving( const sInitData &init ) : dsFunction( init.clsLoco,
"updateIsMoving", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassLocomotion::nfUpdateIsMoving::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.UpdateIsMoving();
}

// public func void updateOrientation( float elapsed )
deClassLocomotion::nfUpdateOrientation::nfUpdateOrientation( const sInitData &init ) : dsFunction( init.clsLoco,
"updateOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // elapsed
}

void deClassLocomotion::nfUpdateOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.UpdateOrientation( rt->GetValue( 0 )->GetFloat() );
}

// public func void updateLinearVelocity( float elapsed )
deClassLocomotion::nfUpdateLinearVelocity::nfUpdateLinearVelocity( const sInitData &init ) : dsFunction( init.clsLoco,
"updateLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // elapsed
}

void deClassLocomotion::nfUpdateLinearVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.UpdateLinearVelocity( rt->GetValue( 0 )->GetFloat() );
}

// public func void updateStance( float elapsed )
deClassLocomotion::nfUpdateStance::nfUpdateStance( const sInitData &init ) : dsFunction( init.clsLoco,
"updateStance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // elapsed
}

void deClassLocomotion::nfUpdateStance::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.UpdateStance( rt->GetValue( 0 )->GetFloat() );
}

// public func void updatePostLocomotion( float elapsed )
deClassLocomotion::nfUpdatePostLocomotion::nfUpdatePostLocomotion( const sInitData &init ) : dsFunction( init.clsLoco,
"updatePostLocomotion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // elapsed
}

void deClassLocomotion::nfUpdatePostLocomotion::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.UpdatePostLocomotion( rt->GetValue( 0 )->GetFloat() );
}

// public func void updateTilt( float elapsed )
deClassLocomotion::nfUpdateTilt::nfUpdateTilt( const sInitData &init ) : dsFunction( init.clsLoco,
"updateTilt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // elapsed
}

void deClassLocomotion::nfUpdateTilt::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.UpdateTilt( rt->GetValue( 0 )->GetFloat() );
}

// public func void updateAnimatorInstance( float elapsed )
deClassLocomotion::nfUpdateAnimatorInstance::nfUpdateAnimatorInstance( const sInitData &init ) : dsFunction( init.clsLoco,
"updateAnimatorInstance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // elapsed
}

void deClassLocomotion::nfUpdateAnimatorInstance::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.UpdateAnimatorInstance( rt->GetValue( 0 )->GetFloat() );
}

// public func void updateAICollider()
deClassLocomotion::nfUpdateAICollider::nfUpdateAICollider( const sInitData &init ) : dsFunction( init.clsLoco,
"updateAICollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassLocomotion::nfUpdateAICollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	
	locomotion.UpdateAICollider();
}

// public func void adjustOrientation(float angle)
deClassLocomotion::nfAdjustOrientation::nfAdjustOrientation( const sInitData &init ) :
dsFunction( init.clsLoco, "adjustOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // angle
}

void deClassLocomotion::nfAdjustOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	locomotion.AdjustOrientation( rt->GetValue( 0 )->GetFloat() );
}



// File Handling
//////////////////

// public func void readFromFile( FileReader reader )
deClassLocomotion::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsLoco,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassLocomotion::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	decBaseFileReader * const reader = ds.GetClassFileReader()->GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	locomotion.ReadFromFile( *reader );
}

// public func void writeToFile( FileWriter writer )
deClassLocomotion::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsLoco,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassLocomotion::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion &locomotion = *( ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion );
	const deScriptingDragonScript &ds = ( ( deClassLocomotion* )GetOwnerClass() )->GetDS();
	
	decBaseFileWriter * const writer = ds.GetClassFileWriter()->GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	locomotion.WriteToFile( *writer );
}



// public func int hashCode()
deClassLocomotion::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsLoco, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassLocomotion::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion * const locomotion = ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion;
	
	rt->PushInt( ( int )( intptr_t )locomotion );
}

// public func bool equals( Object object )
deClassLocomotion::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsLoco, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // object
}
void deClassLocomotion::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsLocomotion * const locomotion = ( ( const sLocoNatDat * )p_GetNativeData( myself ) )->locomotion;
	deClassLocomotion * const clsLoco = ( deClassLocomotion* )GetOwnerClass();
	dsValue * const object = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( object, clsLoco ) ){
		rt->PushBool( false );
		
	}else{
		const dedsLocomotion * const otherLocomotion = ( ( const sLocoNatDat * )p_GetNativeData( object ) )->locomotion;
		rt->PushBool( locomotion == otherLocomotion );
	}
}



// Class deClassLocomotion
////////////////////////////

// Constructor
////////////////

deClassLocomotion::deClassLocomotion( deScriptingDragonScript &ds ) :
dsClass( "Locomotion", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sLocoNatDat ) );
}

deClassLocomotion::~deClassLocomotion(){
}



// Management
///////////////

void deClassLocomotion::CreateClassMembers( dsEngine *engine ){
	pClsLocomotionAttribute = engine->GetClass( "Dragengine.Scenery.LocomotionAttribute" );
	pClsLocomotionTilt = engine->GetClass( "Dragengine.Scenery.LocomotionTilt" );
	
	sInitData init;
	init.clsLoco = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsVector = pDS.GetClassVector();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsAInst = pDS.GetClassAnimatorInstance();
	init.clsCollider = pDS.GetClassCollider();
	init.clsCCT = pDS.GetClassColliderCollisionTest();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	init.clsAnimatorCtrl = pDS.GetClassAnimatorController();
	init.clsLocomotionAttribute = pClsLocomotionAttribute;
	init.clsLocomotionTilt = pClsLocomotionTilt;
	
	// constants
	
	AddConstant( new dsConstant( "TILT_NONE", init.clsInteger, dedsLocomotion::etmNone ) );
	AddConstant( new dsConstant( "TILT_SINGLE", init.clsInteger, dedsLocomotion::etmSingle ) );
	AddConstant( new dsConstant( "TILT_WEIGHTED", init.clsInteger, dedsLocomotion::etmWeighted ) );
	
	// functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetCanTurn( init ) );
	AddFunction( new nfSetCanTurn( init ) );
	
	AddFunction( new nfGetLimitLookUp( init ) );
	AddFunction( new nfSetLimitLookUp( init ) );
	AddFunction( new nfGetLimitLookDown( init ) );
	AddFunction( new nfSetLimitLookDown( init ) );
	AddFunction( new nfGetAdjustTimeVertical( init ) );
	AddFunction( new nfSetAdjustTimeVertical( init ) );
	AddFunction( new nfGetAdjustRangeVertical( init ) );
	AddFunction( new nfSetAdjustRangeVertical( init ) );
	AddFunction( new nfGetLookVerticalGoal( init ) );
	AddFunction( new nfSetLookVerticalGoal( init ) );
	AddFunction( new nfGetLookVertical( init ) );
	AddFunction( new nfSetLookVertical( init ) );
	
	AddFunction( new nfGetLimitLookLeft( init ) );
	AddFunction( new nfSetLimitLookLeft( init ) );
	AddFunction( new nfGetLimitLookRight( init ) );
	AddFunction( new nfSetLimitLookRight( init ) );
	AddFunction( new nfGetAdjustTimeHorizontal( init ) );
	AddFunction( new nfSetAdjustTimeHorizontal( init ) );
	AddFunction( new nfGetAdjustRangeHorizontal( init ) );
	AddFunction( new nfSetAdjustRangeHorizontal( init ) );
	AddFunction( new nfGetLookHorizontalGoal( init ) );
	AddFunction( new nfSetLookHorizontalGoal( init ) );
	AddFunction( new nfGetLookHorizontal( init ) );
	AddFunction( new nfSetLookHorizontal( init ) );
	AddFunction( new nfGetTurnAdjustLookHorizontal( init ) );
	AddFunction( new nfSetTurnAdjustLookHorizontal( init ) );
	
	AddFunction( new nfGetAdjustTimeAnalogMovingHorizontal( init ) );
	AddFunction( new nfSetAdjustTimeAnalogMovingHorizontal( init ) );
	AddFunction( new nfGetAdjustRangeAnalogMovingHorizontal( init ) );
	AddFunction( new nfSetAdjustRangeAnalogMovingHorizontal( init ) );
	AddFunction( new nfGetAnalogMovingHorizontalGoal( init ) );
	AddFunction( new nfSetAnalogMovingHorizontalGoal( init ) );
	AddFunction( new nfGetAnalogMovingHorizontal( init ) );
	AddFunction( new nfSetAnalogMovingHorizontal( init ) );
	AddFunction( new nfGetAnalogMovingVertical( init ) );
	AddFunction( new nfSetAnalogMovingVertical( init ) );
	AddFunction( new nfGetTurnHorizontal( init ) );
	AddFunction( new nfSetTurnHorizontal( init ) );
	
	AddFunction( new nfGetAnalogMovingSpeed( init ) );
	AddFunction( new nfSetAnalogMovingSpeed( init ) );
	
	AddFunction( new nfGetAdjustTimeOrientation( init ) );
	AddFunction( new nfSetAdjustTimeOrientation( init ) );
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfSetOrientation( init ) );
	AddFunction( new nfGetOrientationQuaternion( init ) );
	AddFunction( new nfSetOrientationQuaternion( init ) );
	AddFunction( new nfGetTurningSpeed( init ) );
	AddFunction( new nfSetTurningSpeed( init ) );
	AddFunction( new nfGetAngularVelocity( init ) );
	AddFunction( new nfSetAngularVelocity( init ) );
	
	AddFunction( new nfGetIsMoving( init ) );
	AddFunction( new nfSetIsMoving( init ) );
	AddFunction( new nfGetMovingSpeed( init ) );
	AddFunction( new nfGetAbsMovingSpeed( init ) );
	AddFunction( new nfSetMovingSpeed( init ) );
	AddFunction( new nfGetMovingOrientation( init ) );
	AddFunction( new nfSetMovingOrientation( init ) );
	AddFunction( new nfGetMovingDirection( init ) );
	AddFunction( new nfGetMovingDirectionVector( init ) );
	AddFunction( new nfSetMovingDirection( init ) );
	
	AddFunction( new nfGetLinearVelocity( init ) );
	AddFunction( new nfSetLinearVelocity( init ) );
	AddFunction( new nfGetAdjustTimeLinearVelocity( init ) );
	AddFunction( new nfSetAdjustTimeLinearVelocity( init ) );
	AddFunction( new nfGetAdjustRangeLinearVelocity( init ) );
	AddFunction( new nfSetAdjustRangeLinearVelocity( init ) );
	
	AddFunction( new nfGetAdjustTimeStance( init ) );
	AddFunction( new nfSetAdjustTimeStance( init ) );
	AddFunction( new nfGetAdjustRangeStance( init ) );
	AddFunction( new nfSetAdjustRangeStance( init ) );
	AddFunction( new nfGetStance( init ) );
	AddFunction( new nfSetStance( init ) );
	AddFunction( new nfGetStanceGoal( init ) );
	AddFunction( new nfSetStanceGoal( init ) );
	
	AddFunction( new nfGetIsTurningIP( init ) );
	AddFunction( new nfSetIsTurningIP( init ) );
	AddFunction( new nfGetAdjustTimeTurnIP( init ) );
	AddFunction( new nfSetAdjustTimeTurnIP( init ) );
	AddFunction( new nfGetTurnIP( init ) );
	AddFunction( new nfSetTurnIP( init ) );
	AddFunction( new nfGetResetTimeTurnIP( init ) );
	AddFunction( new nfSetResetTimeTurnIP( init ) );
	AddFunction( new nfGetReverseTimeTurnIP( init ) );
	AddFunction( new nfSetReverseTimeTurnIP( init ) );
	
	AddFunction( new nfGetCanTurnInPlace( init ) );
	AddFunction( new nfSetCanTurnInPlace( init ) );
	AddFunction( new nfGetLimitTurnInPlaceLeft( init ) );
	AddFunction( new nfSetLimitTurnInPlaceLeft( init ) );
	AddFunction( new nfGetLimitTurnInPlaceRight( init ) );
	AddFunction( new nfSetLimitTurnInPlaceRight( init ) );
	
	AddFunction( new nfGetResetTimeWalk( init ) );
	AddFunction( new nfSetResetTimeWalk( init ) );
	
	AddFunction( new nfGetCanTilt( init ) );
	AddFunction( new nfSetCanTilt( init ) );
	AddFunction( new nfGetTiltMode( init ) );
	AddFunction( new nfSetTiltMode( init ) );
	AddFunction( new nfSetCCTTiltSingle( init ) );
	AddFunction( new nfSetCCTTiltFrontLeft( init ) );
	AddFunction( new nfSetCCTTiltFrontRight( init ) );
	AddFunction( new nfSetCCTTiltBackLeft( init ) );
	AddFunction( new nfSetCCTTiltBackRight( init ) );
	
	AddFunction( new nfGetLimitTiltUp( init ) );
	AddFunction( new nfSetLimitTiltUp( init ) );
	AddFunction( new nfGetLimitTiltDown( init ) );
	AddFunction( new nfSetLimitTiltDown( init ) );
	AddFunction( new nfGetTiltVertical( init ) );
	AddFunction( new nfSetTiltVertical( init ) );
	AddFunction( new nfGetTiltVerticalGoal( init ) );
	AddFunction( new nfSetTiltVerticalGoal( init ) );
	AddFunction( new nfGetAdjustTimeTiltVertical( init ) );
	AddFunction( new nfSetAdjustTimeTiltVertical( init ) );
	AddFunction( new nfGetAdjustRangeTiltVertical( init ) );
	AddFunction( new nfSetAdjustRangeTiltVertical( init ) );
	
	AddFunction( new nfGetLimitTiltLeft( init ) );
	AddFunction( new nfSetLimitTiltLeft( init ) );
	AddFunction( new nfGetLimitTiltRight( init ) );
	AddFunction( new nfSetLimitTiltRight( init ) );
	AddFunction( new nfGetTiltHorizontal( init ) );
	AddFunction( new nfSetTiltHorizontal( init ) );
	AddFunction( new nfGetTiltHorizontalGoal( init ) );
	AddFunction( new nfSetTiltHorizontalGoal( init ) );
	AddFunction( new nfGetAdjustTimeTiltHorizontal( init ) );
	AddFunction( new nfSetAdjustTimeTiltHorizontal( init ) );
	AddFunction( new nfGetAdjustRangeTiltHorizontal( init ) );
	AddFunction( new nfSetAdjustRangeTiltHorizontal( init ) );
	
	AddFunction( new nfGetTiltOffset( init ) );
	AddFunction( new nfSetTiltOffset( init ) );
	
	AddFunction( new nfAddControllerMapping( init ) );
	AddFunction( new nfRemoveControllerMapping( init ) );
	AddFunction( new nfRemoveAllControllerMappings( init ) );
	
	AddFunction( new nfGetAICollider( init ) );
	AddFunction( new nfSetAICollider( init ) );
	AddFunction( new nfGetUpdateAIColliderAngularVelocity( init ) );
	AddFunction( new nfSetUpdateAIColliderAngularVelocity( init ) );
	
	AddFunction( new nfCancelMovement( init ) );
	AddFunction( new nfCancelInput( init ) );
	AddFunction( new nfCancelMotion( init ) );
	AddFunction( new nfCancelTurnInPlace( init ) );
	AddFunction( new nfForceBodyAdjustment( init ) );
	
	AddFunction( new nfUpdateLooking( init ) );
	AddFunction( new nfApplyStates( init ) );
	AddFunction( new nfUpdateLocomotion( init ) );
	AddFunction( new nfUpdateIsMoving( init ) );
	AddFunction( new nfUpdateOrientation( init ) );
	AddFunction( new nfUpdateLinearVelocity( init ) );
	AddFunction( new nfUpdateStance( init ) );
	AddFunction( new nfUpdatePostLocomotion( init ) );
	AddFunction( new nfUpdateTilt( init ) );
	AddFunction( new nfUpdateAnimatorInstance( init ) );
	AddFunction( new nfUpdateAICollider( init ) );
	AddFunction( new nfAdjustOrientation( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
	
	// finish class
	CalcMemberOffsets();
}

dedsLocomotion *deClassLocomotion::GetLocomotion( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( const sLocoNatDat * )p_GetNativeData( myself->GetBuffer() ) )->locomotion;
}

void deClassLocomotion::PushLocomotion( dsRunTime *rt, dedsLocomotion *locomotion ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! locomotion ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sLocoNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->locomotion = locomotion;
	locomotion->AddReference();
}
