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
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstanceManager.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>



struct sSynINatDat{
	deSynthesizerInstance *instance;
};



// Native Functions
/////////////////////

// public func new()
deClassSynthesizerInstance::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsSynI,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSynthesizerInstance::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	const deClassSynthesizerInstance &clsSyn = *( ( deClassSynthesizerInstance* )GetOwnerClass() );
	deSynthesizerInstanceManager &syninstmgr = *clsSyn.GetDS().GetGameEngine()->GetSynthesizerInstanceManager();
	sSynINatDat &nd = *( ( sSynINatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.instance = NULL;
	
	// create synthesizer
	nd.instance = syninstmgr.CreateSynthesizerInstance();
}

// public func destructor()
deClassSynthesizerInstance::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsSynI,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSynthesizerInstance::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSynINatDat &nd = *( ( sSynINatDat* )p_GetNativeData( myself ) );
	
	if( nd.instance ){
		nd.instance->FreeReference();
		nd.instance = NULL;
	}
}



// public func Synthesizer getSynthesizer()
deClassSynthesizerInstance::nfGetSynthesizer::nfGetSynthesizer( const sInitData &init ) : dsFunction( init.clsSynI,
"getSynthesizer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSyn ){
}
void deClassSynthesizerInstance::nfGetSynthesizer::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSynthesizerInstance &instance = *( ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance );
	const deScriptingDragonScript &ds = ( ( deClassSynthesizerInstance* )GetOwnerClass() )->GetDS();
	
	ds.GetClassSynthesizer()->PushSynthesizer( rt, instance.GetSynthesizer() );
}

// public func void setSynthesizer( Synthesizer synthesizer )
deClassSynthesizerInstance::nfSetSynthesizer::nfSetSynthesizer( const sInitData &init ) : dsFunction( init.clsSynI,
"setSynthesizer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSyn ); // synthesizer
}
void deClassSynthesizerInstance::nfSetSynthesizer::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSynthesizerInstance &instance = *( ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance );
	const deScriptingDragonScript &ds = ( ( deClassSynthesizerInstance* )GetOwnerClass() )->GetDS();
	dsRealObject * const objSynthesizer = rt->GetValue( 0 )->GetRealObject();
	
	instance.SetSynthesizer( ds.GetClassSynthesizer()->GetSynthesizer( objSynthesizer ) );
}

// public func int getSampleCount()
deClassSynthesizerInstance::nfGetSampleCount::nfGetSampleCount( const sInitData &init ) : dsFunction( init.clsSynI,
"getSampleCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSynthesizerInstance::nfGetSampleCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSynthesizerInstance &instance = *( ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance );
	rt->PushInt( instance.GetSampleCount() );
}

// public func void setSampleCount( int sampleCount )
deClassSynthesizerInstance::nfSetSampleCount::nfSetSampleCount( const sInitData &init ) : dsFunction( init.clsSynI,
"setSampleCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // sampleCount
}
void deClassSynthesizerInstance::nfSetSampleCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSynthesizerInstance &instance = *( ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance );
	instance.SetSampleCount( rt->GetValue( 0 )->GetInt() );
}



// public func int getControllerCount()
deClassSynthesizerInstance::nfGetControllerCount::nfGetControllerCount( const sInitData &init ) :
dsFunction( init.clsSynI, "getControllerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSynthesizerInstance::nfGetControllerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSynthesizerInstance &instance = *( ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance );
	
	rt->PushInt( instance.GetControllerCount() );
}

// public func SynthesizerController getControllerAt( int index )
deClassSynthesizerInstance::nfGetControllerAt::nfGetControllerAt( const sInitData &init ) :
dsFunction( init.clsSynI, "getControllerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSynController ){
	p_AddParameter( init.clsInt ); // controller
}
void deClassSynthesizerInstance::nfGetControllerAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSynthesizerInstance * const instance = ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance;
	const deScriptingDragonScript &ds = ( ( deClassSynthesizerInstance* )GetOwnerClass() )->GetDS();
	const int index = rt->GetValue( 0 )->GetInt();
	
	if( index < 0 ){
		ds.GetClassSynthesizerController()->PushController( rt, instance,
			instance->GetControllerCount() + index );
		
	}else{
		ds.GetClassSynthesizerController()->PushController( rt, instance, index );
	}
}

// public func SynthesizerController getControllerNamed( String name )
deClassSynthesizerInstance::nfGetControllerNamed::nfGetControllerNamed( const sInitData &init ) :
dsFunction( init.clsSynI, "getControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSynController ){
	p_AddParameter( init.clsString ); // name
}
void deClassSynthesizerInstance::nfGetControllerNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSynthesizerInstance * const instance = ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance;
	const deScriptingDragonScript &ds = ( ( deClassSynthesizerInstance* )GetOwnerClass() )->GetDS();
	const int index = instance->IndexOfControllerNamed( rt->GetValue( 0 )->GetString() );
	
	if( index == -1 ){
		rt->PushObject( NULL, ds.GetClassSynthesizerController() );
		
	}else{
		ds.GetClassSynthesizerController()->PushController( rt, instance, index );
	}
}

// public func int indexOfControllerNamed( String name )
deClassSynthesizerInstance::nfIndexOfControllerNamed::nfIndexOfControllerNamed( const sInitData &init ) :
dsFunction( init.clsSynI, "indexOfControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsString ); // name
}
void deClassSynthesizerInstance::nfIndexOfControllerNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSynthesizerInstance * const instance = ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance;
	rt->PushInt( instance->IndexOfControllerNamed( rt->GetValue( 0 )->GetString() ) );
}

// public func void copyControllerStates( SynthesizerInstance instance, int count )
deClassSynthesizerInstance::nfCopyControllerStates::nfCopyControllerStates( const sInitData &init ) :
dsFunction( init.clsSynI, "copyControllerStates", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSynI ); // instance
	p_AddParameter( init.clsInt ); // count
}
void deClassSynthesizerInstance::nfCopyControllerStates::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSynthesizerInstance &instance = *( ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance );
	const deClassSynthesizerInstance &clsAr = *( ( deClassSynthesizerInstance* )GetOwnerClass() );
	
	const deSynthesizerInstance * const sourceInstance =
		clsAr.GetSynthesizerInstance( rt->GetValue( 0 )->GetRealObject() );
	int count = rt->GetValue( 1 )->GetInt();
	
	if( count < 0 ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! sourceInstance ){
		DSTHROW( dueNullPointer );
	}
	
	if( sourceInstance->GetControllerCount() < count ){
		count = sourceInstance->GetControllerCount();
	}
	if( instance.GetControllerCount() < count ){
		count = instance.GetControllerCount();
	}
	
	int i;
	
	for( i=0; i<count; i++ ){
		instance.GetControllerAt( i )->SetCurve( sourceInstance->GetControllerAt( i )->GetCurve() );
		instance.NotifyControllerChangedAt( i );
	}
}

// public func void copyControllerStates( SynthesizerInstance instance, int offset, int count )
deClassSynthesizerInstance::nfCopyControllerStates2::nfCopyControllerStates2( const sInitData &init ) :
dsFunction( init.clsSynI, "copyControllerStates", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSynI ); // instance
	p_AddParameter( init.clsInt ); // offset
	p_AddParameter( init.clsInt ); // count
}
void deClassSynthesizerInstance::nfCopyControllerStates2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSynthesizerInstance &instance = *( ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance );
	const deClassSynthesizerInstance &clsAr = *( ( deClassSynthesizerInstance* )GetOwnerClass() );
	
	const deSynthesizerInstance * const sourceInstance = clsAr.GetSynthesizerInstance( rt->GetValue( 0 )->GetRealObject() );
	const int offset = rt->GetValue( 1 )->GetInt();
	int count = rt->GetValue( 2 )->GetInt();
	
	if( offset < 0 ){
		DSTHROW( dueInvalidParam );
	}
	if( count < 0 ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! sourceInstance ){
		DSTHROW( dueNullPointer );
	}
	
	if( sourceInstance->GetControllerCount() < count ){
		count = sourceInstance->GetControllerCount();
	}
	if( instance.GetControllerCount() - offset < count ){
		count = instance.GetControllerCount() - offset;
	}
	
	int i;
	
	for( i=0; i<count; i++ ){
		instance.GetControllerAt( offset + i )->SetCurve( sourceInstance->GetControllerAt( i )->GetCurve() );
		instance.NotifyControllerChangedAt( i );
	}
}

// public func void copyNamedControllerStates( SynthesizerInstance instance )
deClassSynthesizerInstance::nfCopyNamedControllerStates::nfCopyNamedControllerStates( const sInitData &init ) :
dsFunction( init.clsSynI, "copyNamedControllerStates", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSynI ); // instance
}
void deClassSynthesizerInstance::nfCopyNamedControllerStates::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSynthesizerInstance &instance = *( ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance );
	const deClassSynthesizerInstance &clsAr = *( ( deClassSynthesizerInstance* )GetOwnerClass() );
	
	const deSynthesizerInstance * const sourceInstance = clsAr.GetSynthesizerInstance( rt->GetValue( 0 )->GetRealObject() );
	if( ! sourceInstance ){
		DSTHROW( dueNullPointer );
	}
	
	const int count = sourceInstance->GetControllerCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deSynthesizerController &sourceController = *sourceInstance->GetControllerAt( i );
		const int index = instance.IndexOfControllerNamed( sourceController.GetName() );
		if( index == -1 ){
			continue;
		}
		
		instance.GetControllerAt( index )->SetCurve( sourceController.GetCurve() );
		instance.NotifyControllerChangedAt( index );
	}
}



// public func int hashCode()
deClassSynthesizerInstance::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsSynI,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassSynthesizerInstance::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSynthesizerInstance * const instance = ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance;
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )instance );
}

// public func bool equals( Object obj )
deClassSynthesizerInstance::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsSynI,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassSynthesizerInstance::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSynthesizerInstance * const instance = ( ( sSynINatDat* )p_GetNativeData( myself ) )->instance;
	deClassSynthesizerInstance * const clsSyn = ( deClassSynthesizerInstance* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsSyn ) ){
		rt->PushBool( false );
		
	}else{
		deSynthesizerInstance * const otherInstance = ( ( sSynINatDat* )p_GetNativeData( obj ) )->instance;
		
		rt->PushBool( instance == otherInstance );
	}
}



// Class deClassSynthesizerInstance
//////////////////////////////////

// Constructor
////////////////

deClassSynthesizerInstance::deClassSynthesizerInstance( deScriptingDragonScript &ds ) :
dsClass( "SynthesizerInstance", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSynINatDat ) );
}

deClassSynthesizerInstance::~deClassSynthesizerInstance(){
}



// Management
///////////////

void deClassSynthesizerInstance::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsSynI = this;
	init.clsSyn = pDS.GetClassSynthesizer();
	init.clsSynController = pDS.GetClassSynthesizerController();
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetSynthesizer( init ) );
	AddFunction( new nfSetSynthesizer( init ) );
	AddFunction( new nfGetSampleCount( init ) );
	AddFunction( new nfSetSampleCount( init ) );
	
	AddFunction( new nfGetControllerCount( init ) );
	AddFunction( new nfGetControllerAt( init ) );
	AddFunction( new nfGetControllerNamed( init ) );
	AddFunction( new nfIndexOfControllerNamed( init ) );
	AddFunction( new nfCopyControllerStates( init ) );
	AddFunction( new nfCopyControllerStates2( init ) );
	AddFunction( new nfCopyNamedControllerStates( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSynthesizerInstance *deClassSynthesizerInstance::GetSynthesizerInstance( dsRealObject *object ) const{
	if( ! object ){
		return NULL;
	}
	
	return ( ( sSynINatDat* )p_GetNativeData( object->GetBuffer() ) )->instance;
}

void deClassSynthesizerInstance::PushSynthesizerInstance( dsRunTime *rt, deSynthesizerInstance *instance ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! instance ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sSynINatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->instance = instance;
	instance->AddReference();
}
