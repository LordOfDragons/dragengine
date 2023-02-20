/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassEffect.h"
#include "deClassEffectFilterKernel.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/effect/deEffectFilterKernel.h>
#include <dragengine/resources/effect/deEffectManager.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sEffFilKerMatrixNatDat{
	deEffectFilterKernel *effect;
};



// Native Functions
/////////////////////

// public func new()
deClassEffectFilterKernel::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsEffFilKer,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEffectFilterKernel::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sEffFilKerMatrixNatDat &nd = *( ( sEffFilKerMatrixNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassEffectFilterKernel* )GetOwnerClass() )->GetDS();
	
	// clear ( important )
	nd.effect = NULL;
	
	// super call
	deClassEffect * const baseClass = ( deClassEffect* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create effect
	nd.effect = ds.GetGameEngine()->GetEffectManager()->CreateEffectFilterKernel();
	baseClass->AssignEffect( myself->GetRealObject(), nd.effect );
}

// public func destructor()
deClassEffectFilterKernel::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsEffFilKer,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEffectFilterKernel::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sEffFilKerMatrixNatDat &nd = *( ( sEffFilKerMatrixNatDat* )p_GetNativeData( myself ) );
	
	if( nd.effect ){
		nd.effect->FreeReference();
		nd.effect = NULL;
	}
}



// Management all types
/////////////////////////

// public func void setKernelSize( int rows, int cols )
deClassEffectFilterKernel::nfSetKernelSize::nfSetKernelSize( const sInitData &init ) : dsFunction( init.clsEffFilKer,
"setKernelSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // rows
	p_AddParameter( init.clsInt ); // cols
}
void deClassEffectFilterKernel::nfSetKernelSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sEffFilKerMatrixNatDat &nd = *( ( sEffFilKerMatrixNatDat* )p_GetNativeData( myself ) );
	
	const int rows = rt->GetValue( 0 )->GetInt();
	const int cols = rt->GetValue( 1 )->GetInt();
	
	nd.effect->SetKernelSize( rows, cols );
}

// public func void setKernelValue( int row, int col, float value )
deClassEffectFilterKernel::nfSetKernelValue::nfSetKernelValue( const sInitData &init ) : dsFunction( init.clsEffFilKer,
"setKernelValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // row
	p_AddParameter( init.clsInt ); // col
	p_AddParameter( init.clsFlt ); // value
}
void deClassEffectFilterKernel::nfSetKernelValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sEffFilKerMatrixNatDat &nd = *( ( sEffFilKerMatrixNatDat* )p_GetNativeData( myself ) );
	
	const int row = rt->GetValue( 0 )->GetInt();
	const int col = rt->GetValue( 1 )->GetInt();
	const float value = rt->GetValue( 2 )->GetFloat();
	
	nd.effect->SetKernelValueAt( row, col, value );
}

// public func float getScale()
deClassEffectFilterKernel::nfGetScale::nfGetScale( const sInitData &init ) : dsFunction( init.clsEffFilKer,
"getScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassEffectFilterKernel::nfGetScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deEffectFilterKernel &effect = *( ( ( sEffFilKerMatrixNatDat* )p_GetNativeData( myself ) )->effect );
	rt->PushFloat( effect.GetScale() );
}

// public func void setScale( float scale )
deClassEffectFilterKernel::nfSetScale::nfSetScale( const sInitData &init ) : dsFunction( init.clsEffFilKer,
"setScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // scale
}
void deClassEffectFilterKernel::nfSetScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffectFilterKernel &effect = *( ( ( sEffFilKerMatrixNatDat* )p_GetNativeData( myself ) )->effect );
	effect.SetScale( rt->GetValue( 0 )->GetFloat() );
}



// public func int hashCode()
deClassEffectFilterKernel::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsEffFilKer, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassEffectFilterKernel::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffectFilterKernel * const effect = ( ( sEffFilKerMatrixNatDat* )p_GetNativeData( myself ) )->effect;
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )effect );
}

// public func bool equals( Object obj )
deClassEffectFilterKernel::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsEffFilKer, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassEffectFilterKernel::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffectFilterKernel * const effect = ( ( sEffFilKerMatrixNatDat* )p_GetNativeData( myself ) )->effect;
	deClassEffectFilterKernel * const clsEffFilKer = ( deClassEffectFilterKernel* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsEffFilKer ) ){
		rt->PushBool( false );
		
	}else{
		deEffectFilterKernel * const otherEffect = ( ( sEffFilKerMatrixNatDat* )p_GetNativeData( obj ) )->effect;
		rt->PushBool( effect == otherEffect );
	}
}



// Class deClassEffectFilterKernel
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEffectFilterKernel::deClassEffectFilterKernel( deScriptingDragonScript &ds ) :
dsClass( "EffectFilterKernel", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Effect" );
	
	p_SetNativeDataSize( sizeof( sEffFilKerMatrixNatDat ) );
}

deClassEffectFilterKernel::~deClassEffectFilterKernel(){
}



// Management
///////////////

void deClassEffectFilterKernel::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsEffFilKer = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfSetKernelSize( init ) );
	AddFunction( new nfSetKernelValue( init ) );
	AddFunction( new nfGetScale( init ) );
	AddFunction( new nfSetScale( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



deEffectFilterKernel *deClassEffectFilterKernel::GetEffect( dsRealObject *myself ) const {
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sEffFilKerMatrixNatDat* )p_GetNativeData( myself->GetBuffer() ) )->effect;
}

void deClassEffectFilterKernel::PushEffect( dsRunTime *rt, deEffectFilterKernel *effect ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! effect ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassEffect * const baseClass = ( deClassEffect* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sEffFilKerMatrixNatDat &nd = *( ( sEffFilKerMatrixNatDat* )p_GetNativeData(
		rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.effect = NULL;
	
	try{
		baseClass->CallBaseClassConstructor( rt, rt->GetValue( 0 ), baseClass->GetFirstConstructor(), 0 );
		
		nd.effect = effect;
		effect->AddReference();
		
		baseClass->AssignEffect( rt->GetValue( 0 )->GetRealObject(), effect );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
