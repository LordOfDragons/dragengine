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
#include "deClassEffectColorMatrix.h"
#include "../math/deClassColorMatrix.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/effect/deEffectColorMatrix.h>
#include <dragengine/resources/effect/deEffectManager.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sEffClrMatMatrixNatDat{
	deEffectColorMatrix *effect;
};



// Native Functions
/////////////////////

// public func new()
deClassEffectColorMatrix::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsEffClrMat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEffectColorMatrix::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sEffClrMatMatrixNatDat &nd = *( ( sEffClrMatMatrixNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassEffectColorMatrix* )GetOwnerClass() )->GetDS();
	
	// clear ( important )
	nd.effect = NULL;
	
	// super call
	deClassEffect * const baseClass = ( deClassEffect* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create effect
	nd.effect = ds.GetGameEngine()->GetEffectManager()->CreateEffectColorMatrix();
	baseClass->AssignEffect( myself->GetRealObject(), nd.effect );
}

// public func destructor()
deClassEffectColorMatrix::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsEffClrMat,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEffectColorMatrix::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sEffClrMatMatrixNatDat &nd = *( ( sEffClrMatMatrixNatDat* )p_GetNativeData( myself ) );
	
	if( nd.effect ){
		nd.effect->FreeReference();
		nd.effect = NULL;
	}
}



// Management all types
/////////////////////////

// public func ColorMatrix getColorMatrix()
deClassEffectColorMatrix::nfGetColorMatrix::nfGetColorMatrix( const sInitData &init ) : dsFunction( init.clsEffClrMat,
"getColorMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClrMat ){
}
void deClassEffectColorMatrix::nfGetColorMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sEffClrMatMatrixNatDat &nd = *( ( sEffClrMatMatrixNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassEffectColorMatrix* )GetOwnerClass() )->GetDS();
	
	ds.GetClassColorMatrix()->PushColorMatrix( rt, nd.effect->GetColorMatrix() );
}

// public func void setColorMatrix( ColorMatrix matrix )
deClassEffectColorMatrix::nfSetColorMatrix::nfSetColorMatrix( const sInitData &init ) : dsFunction( init.clsEffClrMat,
"setColorMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsClrMat ); // matrix
}
void deClassEffectColorMatrix::nfSetColorMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sEffClrMatMatrixNatDat &nd = *( ( sEffClrMatMatrixNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassEffectColorMatrix* )GetOwnerClass() )->GetDS();
	
	const decColorMatrix &colorMatrix = ds.GetClassColorMatrix()->GetColorMatrix( rt->GetValue( 0 )->GetRealObject() );
	nd.effect->SetColorMatrix( colorMatrix );
}



// public func int hashCode()
deClassEffectColorMatrix::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsEffClrMat, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassEffectColorMatrix::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffectColorMatrix * const effect = ( ( sEffClrMatMatrixNatDat* )p_GetNativeData( myself ) )->effect;
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )effect );
}

// public func bool equals( Object obj )
deClassEffectColorMatrix::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsEffClrMat, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassEffectColorMatrix::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffectColorMatrix * const effect = ( ( sEffClrMatMatrixNatDat* )p_GetNativeData( myself ) )->effect;
	deClassEffectColorMatrix * const clsEffClrMat = ( deClassEffectColorMatrix* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsEffClrMat ) ){
		rt->PushBool( false );
		
	}else{
		deEffectColorMatrix * const otherEffect = ( ( sEffClrMatMatrixNatDat* )p_GetNativeData( obj ) )->effect;
		rt->PushBool( effect == otherEffect );
	}
}



// Class deClassEffectColorMatrix
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEffectColorMatrix::deClassEffectColorMatrix( deScriptingDragonScript &ds ) :
dsClass( "EffectColorMatrix", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Effect" );
	
	p_SetNativeDataSize( sizeof( sEffClrMatMatrixNatDat ) );
}

deClassEffectColorMatrix::~deClassEffectColorMatrix(){
}



// Management
///////////////

void deClassEffectColorMatrix::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsEffClrMat = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsClrMat = pDS.GetClassColorMatrix();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetColorMatrix( init ) );
	AddFunction( new nfSetColorMatrix( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



deEffectColorMatrix *deClassEffectColorMatrix::GetEffect( dsRealObject *myself ) const {
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sEffClrMatMatrixNatDat* )p_GetNativeData( myself->GetBuffer() ) )->effect;
}

void deClassEffectColorMatrix::PushEffect( dsRunTime *rt, deEffectColorMatrix *effect ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! effect ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassEffect * const baseClass = ( deClassEffect* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sEffClrMatMatrixNatDat &nd = *( ( sEffClrMatMatrixNatDat* )p_GetNativeData(
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
