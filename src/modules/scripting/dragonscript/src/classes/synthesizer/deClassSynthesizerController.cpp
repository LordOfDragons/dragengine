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
#include <string.h>

#include "deClassSynthesizer.h"
#include "deClassSynthesizerController.h"
#include "deClassSynthesizerInstance.h"
#include "../curve/deClassCurveBezier.h"
#include "../math/deClassVector.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>



// Native Structure
/////////////////////

struct sSynthesizerCtrlNatDat{
	deSynthesizer *synthesizer;
	deSynthesizerInstance *instance;
	int index;
};



// Native functions
/////////////////////

// private func new()
deClassSynthesizerController::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid ){
}
void deClassSynthesizerController::nfNew::RunFunction( dsRunTime*, dsValue* ){
	DSTHROW( dueInvalidParam );
}

// public func destructor()
deClassSynthesizerController::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSynthesizerController::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSynthesizerCtrlNatDat &nd = *( ( sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	if( nd.synthesizer ){
		nd.synthesizer->FreeReference();
		nd.synthesizer = NULL;
	}
	if( nd.instance ){
		nd.instance->FreeReference();
		nd.instance = NULL;
	}
	nd.index = -1;
}



// Management
///////////////

// public func Synthesizer getSynthesizer()
deClassSynthesizerController::nfGetSynthesizer::nfGetSynthesizer( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "getSynthesizer", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSynthesizer ){
}
void deClassSynthesizerController::nfGetSynthesizer::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassSynthesizerController* )GetOwnerClass() )->GetDS();
	
	ds.GetClassSynthesizer()->PushSynthesizer( rt, nd.synthesizer );
}

// public func SynthesizerInstance getSynthesizerInstance()
deClassSynthesizerController::nfGetSynthesizerInstance::nfGetSynthesizerInstance( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "getSynthesizerInstance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSynthesizerInst ){
}
void deClassSynthesizerController::nfGetSynthesizerInstance::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassSynthesizerController* )GetOwnerClass() )->GetDS();
	
	ds.GetClassSynthesizerInstance()->PushSynthesizerInstance( rt, nd.instance );
}

// public func int getControllerIndex()
deClassSynthesizerController::nfGetControllerIndex::nfGetControllerIndex( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "getControllerIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSynthesizerController::nfGetControllerIndex::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	
	rt->PushInt( nd.index );
}



// public func String getName()
deClassSynthesizerController::nfGetName::nfGetName( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassSynthesizerController::nfGetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	const deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	
	rt->PushString( controller.GetName() );
}

// public func void setName( String name )
deClassSynthesizerController::nfSetName::nfSetName( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "setName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // name
}
void deClassSynthesizerController::nfSetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	
	controller.SetName( rt->GetValue( 0 )->GetString() );
	
	if( nd.instance ){
		nd.instance->NotifyControllerChangedAt( nd.index );
	}
}

// public func float getLower()
deClassSynthesizerController::nfGetLower::nfGetLower( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "getLower", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassSynthesizerController::nfGetLower::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	const deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	
	rt->PushFloat( controller.GetMinimumValue() );
}

// public func float getUpper()
deClassSynthesizerController::nfGetUpper::nfGetUpper( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "getUpper", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassSynthesizerController::nfGetUpper::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	const deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	
	rt->PushFloat( controller.GetMaximumValue() );
}

// public func void setRange( float lower, float upper )
deClassSynthesizerController::nfSetRange::nfSetRange( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "setRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // lower
	p_AddParameter( init.clsFloat ); // upper
}
void deClassSynthesizerController::nfSetRange::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	const float lower = rt->GetValue( 0 )->GetFloat();
	const float upper = rt->GetValue( 1 )->GetFloat();
	
	controller.SetValueRange( lower, upper );
	
	if( nd.instance ){
		nd.instance->NotifyControllerChangedAt( nd.index );
	}
}

// public func CurveBezier getCurve()
deClassSynthesizerController::nfGetCurve::nfGetCurve( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "getCurve", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsCurveBezier ){
}
void deClassSynthesizerController::nfGetCurve::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	const deClassSynthesizerController &clsAC = *( ( deClassSynthesizerController* )GetOwnerClass() );
	const deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	
	clsAC.GetDS().GetClassCurveBezier()->PushCurve( rt, controller.GetCurve() );
}

// public func void setCurve( CurveBezier curve )
deClassSynthesizerController::nfSetCurve::nfSetCurve( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "setCurve", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCurveBezier ); // curve
}
void deClassSynthesizerController::nfSetCurve::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	const deClassSynthesizerController &clsAC = *( ( deClassSynthesizerController* )GetOwnerClass() );
	deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	controller.SetCurve( clsAC.GetDS().GetClassCurveBezier()->GetCurve( rt->GetValue( 0 )->GetRealObject() ) );
	
	if( nd.instance ){
		nd.instance->NotifyControllerChangedAt( nd.index );
	}
}

// public func void setCurveConstant( float value )
deClassSynthesizerController::nfSetCurveConstant::nfSetCurveConstant( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "setCurveConstant", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}
void deClassSynthesizerController::nfSetCurveConstant::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	const float value = rt->GetValue( 0 )->GetFloat();
	
	decCurveBezier curve;
	curve.AddPoint( decCurveBezierPoint( decVector2( 0.0f, value ) ) );
	curve.SetInterpolationMode( decCurveBezier::eimConstant );
	controller.SetCurve( curve );
	
	if( nd.instance ){
		nd.instance->NotifyControllerChangedAt( nd.index );
	}
}

// public func void setCurveTime()
deClassSynthesizerController::nfSetCurveTime::nfSetCurveTime( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "setCurveTime", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSynthesizerController::nfSetCurveTime::RunFunction( dsRunTime*, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	
	const deSynthesizer * const synthesizer = nd.synthesizer ? nd.synthesizer : nd.instance->GetSynthesizer();
	const int sampleCount = nd.synthesizer ? nd.synthesizer->GetSampleCount() : nd.instance->GetSampleCount();
	decCurveBezier curve;
	
	if( synthesizer ){
		const float playTime = ( float )sampleCount / ( float )synthesizer->GetSampleRate();
		curve.AddPoint( decCurveBezierPoint( decVector2( 0.0f, controller.GetMinimumValue() ) ) );
		curve.AddPoint( decCurveBezierPoint( decVector2( playTime, controller.GetMaximumValue() ) ) );
		curve.SetInterpolationMode( decCurveBezier::eimLinear );
		
	}else{
		curve.SetInterpolationMode( decCurveBezier::eimConstant );
	}
	
	controller.SetCurve( curve );
	
	if( nd.instance ){
		nd.instance->NotifyControllerChangedAt( nd.index );
	}
}

// public func bool getClamp()
deClassSynthesizerController::nfGetClamp::nfGetClamp( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "getClamp", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassSynthesizerController::nfGetClamp::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	const deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	
	rt->PushBool( controller.GetClamp() );
}

// public func void setClamp( bool clamp )
deClassSynthesizerController::nfSetClamp::nfSetClamp( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "setClamp", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // clamp
}
void deClassSynthesizerController::nfSetClamp::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	const bool clamp = rt->GetValue( 0 )->GetBool();
	
	controller.SetClamp( clamp );
	
	if( nd.instance ){
		nd.instance->NotifyControllerChangedAt( nd.index );
	}
}



// public func void setCurveFrom( SynthesizerController controller )
deClassSynthesizerController::nfSetCurveFrom::nfSetCurveFrom( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "setCurveFrom", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSynthesizerCtrl ); // controller
}
void deClassSynthesizerController::nfSetCurveFrom::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	
	dsValue * const controllerFrom = rt->GetValue( 0 );
	if( ! controllerFrom->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	const sSynthesizerCtrlNatDat &ndFrom = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( controllerFrom ) );
	deSynthesizerController &realControllerFrom = ndFrom.synthesizer
		? *nd.synthesizer->GetControllerAt( ndFrom.index )
		: *nd.instance->GetControllerAt( ndFrom.index );
	
	controller.SetCurve( realControllerFrom.GetCurve() );
	
	if( nd.instance ){
		nd.instance->NotifyControllerChangedAt( nd.index );
	}
}

// public func void setAllFrom( SynthesizerController controller )
deClassSynthesizerController::nfSetAllFrom::nfSetAllFrom( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "setAllFrom", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSynthesizerCtrl ); // controller
}
void deClassSynthesizerController::nfSetAllFrom::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	deSynthesizerController &controller = nd.synthesizer
		? *nd.synthesizer->GetControllerAt( nd.index )
		: *nd.instance->GetControllerAt( nd.index );
	
	dsValue * const controllerFrom = rt->GetValue( 0 );
	if( ! controllerFrom->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	const sSynthesizerCtrlNatDat &ndFrom = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( controllerFrom ) );
	deSynthesizerController &realControllerFrom = ndFrom.synthesizer
		? *nd.synthesizer->GetControllerAt( ndFrom.index )
		: *nd.instance->GetControllerAt( ndFrom.index );
	
	controller.SetValueRange( realControllerFrom.GetMinimumValue(), realControllerFrom.GetMaximumValue() );
	controller.SetCurve( realControllerFrom.GetCurve() );
	controller.SetClamp( realControllerFrom.GetClamp() );
	
	if( nd.instance ){
		nd.instance->NotifyControllerChangedAt( nd.index );
	}
}


// public func bool equals( Object obj )
deClassSynthesizerController::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsSynthesizerCtrl, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassSynthesizerController::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSynthesizerController * const clsSynthesizerCtrl = ( deClassSynthesizerController* )GetOwnerClass();
	const sSynthesizerCtrlNatDat &nd = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( myself ) );
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsSynthesizerCtrl ) ){
		rt->PushBool( false );
		
	}else{
		const sSynthesizerCtrlNatDat &other = *( ( const sSynthesizerCtrlNatDat* )p_GetNativeData( obj ) );
		rt->PushBool( nd.synthesizer == other.synthesizer && nd.instance == other.instance
			&& nd.index == other.index );
	}
}



// Class deClassSynthesizerController
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassSynthesizerController::deClassSynthesizerController( deScriptingDragonScript &ds ) :
dsClass( "SynthesizerController", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSynthesizerCtrlNatDat ) );
}

deClassSynthesizerController::~deClassSynthesizerController(){
}



// Management
///////////////

void deClassSynthesizerController::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsSynthesizerCtrl = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsCurveBezier = pDS.GetClassCurveBezier();
	init.clsSynthesizer = pDS.GetClassSynthesizer();
	init.clsSynthesizerInst = pDS.GetClassSynthesizerInstance();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetSynthesizer( init ) );
	AddFunction( new nfGetSynthesizerInstance( init ) );
	AddFunction( new nfGetControllerIndex( init ) );
	
	AddFunction( new nfGetName( init ) );
	AddFunction( new nfSetName( init ) );
	AddFunction( new nfGetLower( init ) );
	AddFunction( new nfGetUpper( init ) );
	AddFunction( new nfSetRange( init ) );
	AddFunction( new nfGetCurve( init ) );
	AddFunction( new nfSetCurve( init ) );
	AddFunction( new nfSetCurveConstant( init ) );
	AddFunction( new nfSetCurveTime( init ) );
	AddFunction( new nfGetClamp( init ) );
	AddFunction( new nfSetClamp( init ) );
	
	AddFunction( new nfSetCurveFrom( init ) );
	AddFunction( new nfSetAllFrom( init ) );
	
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

void deClassSynthesizerController::GetController( dsRealObject *myself, deSynthesizer *&synthesizer,
deSynthesizerInstance *&instance, int &index ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	const sSynthesizerCtrlNatDat &nd = *( ( sSynthesizerCtrlNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	synthesizer = nd.synthesizer;
	instance = nd.instance;
	index = nd.index;
}

void deClassSynthesizerController::PushController( dsRunTime *rt, deSynthesizer *synthesizer, int index ){
	if( ! rt || ! synthesizer || index < 0 || index >= synthesizer->GetControllerCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sSynthesizerCtrlNatDat &nd = *( ( sSynthesizerCtrlNatDat* )p_GetNativeData(
		rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.synthesizer = synthesizer;
	synthesizer->AddReference();
	nd.instance = NULL;
	nd.index = index;
}

void deClassSynthesizerController::PushController( dsRunTime *rt, deSynthesizerInstance *instance, int index ){
	if( ! rt || ! instance || index < 0 || index >= instance->GetControllerCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sSynthesizerCtrlNatDat &nd = *( ( sSynthesizerCtrlNatDat* )p_GetNativeData(
		rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.synthesizer = NULL;
	nd.instance = instance;
	instance->AddReference();
	nd.index = index;
}
