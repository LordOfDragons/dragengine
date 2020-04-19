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
#include "deClassEffectOverlayImage.h"
#include "../graphics/deClassImage.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/effect/deEffectOverlayImage.h>
#include <dragengine/resources/effect/deEffectManager.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sEffOverImgMatrixNatDat{
	deEffectOverlayImage *effect;
};



// Native Functions
/////////////////////

// public func new()
deClassEffectOverlayImage::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsEffOverImg,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEffectOverlayImage::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sEffOverImgMatrixNatDat &nd = *( ( sEffOverImgMatrixNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassEffectOverlayImage* )GetOwnerClass() )->GetDS();
	
	// clear ( important )
	nd.effect = NULL;
	
	// super call
	deClassEffect * const baseClass = ( deClassEffect* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create effect
	nd.effect = ds.GetGameEngine()->GetEffectManager()->CreateEffectOverlayImage();
	baseClass->AssignEffect( myself->GetRealObject(), nd.effect );
}

// public func destructor()
deClassEffectOverlayImage::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsEffOverImg,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEffectOverlayImage::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sEffOverImgMatrixNatDat &nd = *( ( sEffOverImgMatrixNatDat* )p_GetNativeData( myself ) );
	
	if( nd.effect ){
		nd.effect->FreeReference();
		nd.effect = NULL;
	}
}



// Management all types
/////////////////////////

// public func void setImage( Image image )
deClassEffectOverlayImage::nfSetImage::nfSetImage( const sInitData &init ) : dsFunction( init.clsEffOverImg,
"setImage", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsImage ); // image
}
void deClassEffectOverlayImage::nfSetImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sEffOverImgMatrixNatDat &nd = *( ( sEffOverImgMatrixNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassEffectOverlayImage* )GetOwnerClass() )->GetDS();
	
	nd.effect->SetImage( ds.GetClassImage()->GetImage( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void setTransparency( float transparency )
deClassEffectOverlayImage::nfSetTransparency::nfSetTransparency( const sInitData &init ) : dsFunction( init.clsEffOverImg,
"setTransparency", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // transparency
}
void deClassEffectOverlayImage::nfSetTransparency::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sEffOverImgMatrixNatDat &nd = *( ( sEffOverImgMatrixNatDat* )p_GetNativeData( myself ) );
	
	nd.effect->SetTransparency( rt->GetValue( 0 )->GetFloat() );
}



// public func int hashCode()
deClassEffectOverlayImage::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsEffOverImg, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassEffectOverlayImage::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffectOverlayImage * const effect = ( ( sEffOverImgMatrixNatDat* )p_GetNativeData( myself ) )->effect;
	// hash code = memory location
	rt->PushInt( ( intptr_t )effect );
}

// public func bool equals( Object obj )
deClassEffectOverlayImage::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsEffOverImg, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassEffectOverlayImage::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffectOverlayImage * const effect = ( ( sEffOverImgMatrixNatDat* )p_GetNativeData( myself ) )->effect;
	deClassEffectOverlayImage * const clsEffOverImg = ( deClassEffectOverlayImage* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsEffOverImg ) ){
		rt->PushBool( false );
		
	}else{
		deEffectOverlayImage * const otherEffect = ( ( sEffOverImgMatrixNatDat* )p_GetNativeData( obj ) )->effect;
		rt->PushBool( effect == otherEffect );
	}
}



// Class deClassEffectOverlayImage
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEffectOverlayImage::deClassEffectOverlayImage( deScriptingDragonScript &ds ) :
dsClass( "EffectOverlayImage", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Effect" );
	
	p_SetNativeDataSize( sizeof( sEffOverImgMatrixNatDat ) );
}

deClassEffectOverlayImage::~deClassEffectOverlayImage(){
}



// Management
///////////////

void deClassEffectOverlayImage::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsEffOverImg = this;
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
	AddFunction( new nfSetTransparency( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



deEffectOverlayImage *deClassEffectOverlayImage::GetEffect( dsRealObject *myself ) const {
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sEffOverImgMatrixNatDat* )p_GetNativeData( myself->GetBuffer() ) )->effect;
}

void deClassEffectOverlayImage::PushEffect( dsRunTime *rt, deEffectOverlayImage *effect ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! effect ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassEffect * const baseClass = ( deClassEffect* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sEffOverImgMatrixNatDat &nd = *( ( sEffOverImgMatrixNatDat* )p_GetNativeData(
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
