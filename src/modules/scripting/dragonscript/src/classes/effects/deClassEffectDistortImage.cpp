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
#include "deClassEffectDistortImage.h"
#include "../graphics/deClassImage.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/effect/deEffectDistortImage.h>
#include <dragengine/resources/effect/deEffectManager.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sEffDistImgMatrixNatDat{
	deEffectDistortImage *effect;
};



// Native Functions
/////////////////////

// public func new()
deClassEffectDistortImage::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsEffDistImg,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEffectDistortImage::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sEffDistImgMatrixNatDat &nd = *( ( sEffDistImgMatrixNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassEffectDistortImage* )GetOwnerClass() )->GetDS();
	
	// clear ( important )
	nd.effect = NULL;
	
	// super call
	deClassEffect * const baseClass = ( deClassEffect* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create effect
	nd.effect = ds.GetGameEngine()->GetEffectManager()->CreateEffectDistortImage();
	baseClass->AssignEffect( myself->GetRealObject(), nd.effect );
}

// public func destructor()
deClassEffectDistortImage::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsEffDistImg,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEffectDistortImage::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sEffDistImgMatrixNatDat &nd = *( ( sEffDistImgMatrixNatDat* )p_GetNativeData( myself ) );
	
	if( nd.effect ){
		nd.effect->FreeReference();
		nd.effect = NULL;
	}
}



// Management all types
/////////////////////////

// public func void setImage( Image image )
deClassEffectDistortImage::nfSetImage::nfSetImage( const sInitData &init ) : dsFunction( init.clsEffDistImg,
"setImage", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsImage ); // image
}
void deClassEffectDistortImage::nfSetImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sEffDistImgMatrixNatDat &nd = *( ( sEffDistImgMatrixNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassEffectDistortImage* )GetOwnerClass() )->GetDS();
	
	nd.effect->SetImage( ds.GetClassImage()->GetImage( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void setStrength( float u, float v )
deClassEffectDistortImage::nfSetStrength::nfSetStrength( const sInitData &init ) : dsFunction( init.clsEffDistImg,
"setStrength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // u
	p_AddParameter( init.clsFlt ); // v
}
void deClassEffectDistortImage::nfSetStrength::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sEffDistImgMatrixNatDat &nd = *( ( sEffDistImgMatrixNatDat* )p_GetNativeData( myself ) );
	
	const float strengthU = rt->GetValue( 0 )->GetFloat();
	const float strengthV = rt->GetValue( 1 )->GetFloat();
	
	nd.effect->SetStrengthU( strengthU );
	nd.effect->SetStrengthV( strengthV );
}



// public func int hashCode()
deClassEffectDistortImage::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsEffDistImg, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassEffectDistortImage::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffectDistortImage * const effect = ( ( sEffDistImgMatrixNatDat* )p_GetNativeData( myself ) )->effect;
	// hash code = memory location
	rt->PushInt( ( intptr_t )effect );
}

// public func bool equals( Object obj )
deClassEffectDistortImage::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsEffDistImg, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassEffectDistortImage::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffectDistortImage * const effect = ( ( sEffDistImgMatrixNatDat* )p_GetNativeData( myself ) )->effect;
	deClassEffectDistortImage * const clsEffDistImg = ( deClassEffectDistortImage* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsEffDistImg ) ){
		rt->PushBool( false );
		
	}else{
		deEffectDistortImage * const otherEffect = ( ( sEffDistImgMatrixNatDat* )p_GetNativeData( obj ) )->effect;
		rt->PushBool( effect == otherEffect );
	}
}



// Class deClassEffectDistortImage
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEffectDistortImage::deClassEffectDistortImage( deScriptingDragonScript &ds ) :
dsClass( "EffectDistortImage", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Effect" );
	
	p_SetNativeDataSize( sizeof( sEffDistImgMatrixNatDat ) );
}

deClassEffectDistortImage::~deClassEffectDistortImage(){
}



// Management
///////////////

void deClassEffectDistortImage::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsEffDistImg = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsImage = pDS.GetClassImage();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfSetImage( init ) );
	AddFunction( new nfSetStrength( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



deEffectDistortImage *deClassEffectDistortImage::GetEffect( dsRealObject *myself ) const {
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sEffDistImgMatrixNatDat* )p_GetNativeData( myself->GetBuffer() ) )->effect;
}

void deClassEffectDistortImage::PushEffect( dsRunTime *rt, deEffectDistortImage *effect ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! effect ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassEffect * const baseClass = ( deClassEffect* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sEffDistImgMatrixNatDat &nd = *( ( sEffDistImgMatrixNatDat* )p_GetNativeData(
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
