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

#include "deClassEffect.h"
#include "deClassEffectColorMatrix.h"
#include "deClassEffectDistortImage.h"
#include "deClassEffectFilterKernel.h"
#include "deClassEffectOverlayImage.h"
#include "../graphics/deClassImage.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/effect/deEffect.h>
#include <dragengine/resources/effect/deEffectManager.h>
#include <dragengine/resources/effect/deEffectFilterKernel.h>
#include <dragengine/resources/effect/deEffectOverlayImage.h>
#include <dragengine/resources/effect/deEffectDistortImage.h>
#include <dragengine/resources/effect/deEffectVisitorIdentify.h>

#include <libdscript/exceptions.h>


// Native Structure
struct sEffNatDat{
	deEffect *effect;
};



// Native Functions
/////////////////////

// public func new()
deClassEffect::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsEffect,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEffect::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sEffNatDat &nd = *( ( sEffNatDat* )p_GetNativeData( myself ) );
	nd.effect = NULL;
}

// public func destructor()
deClassEffect::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsEffect,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEffect::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sEffNatDat &nd = *( ( sEffNatDat* )p_GetNativeData( myself ) );
	
	if( nd.effect ){
		nd.effect->FreeReference();
		nd.effect = NULL;
	}
}



// Management
///////////////

// public func bool getEnabled()
deClassEffect::nfGetEnabled::nfGetEnabled( const sInitData &init ) : dsFunction( init.clsEffect,
"getEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassEffect::nfGetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sEffNatDat &nd = *( ( sEffNatDat* )p_GetNativeData( myself ) );
	if( ! nd.effect ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushBool( nd.effect->GetEnabled() );
}

// public func setEnabled( bool enabled )
deClassEffect::nfSetEnabled::nfSetEnabled( const sInitData &init ) : dsFunction( init.clsEffect,
"setEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassEffect::nfSetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sEffNatDat &nd = *( ( sEffNatDat* )p_GetNativeData( myself ) );
	if( ! nd.effect ){
		DSTHROW( dueNullPointer );
	}
	
	nd.effect->SetEnabled( rt->GetValue( 0 )->GetBool() );
}



// public func int hashCode()
deClassEffect::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsEffect, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassEffect::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffect * const effect = ( ( sEffNatDat* )p_GetNativeData( myself ) )->effect;
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )effect );
}

// public func bool equals( Object obj )
deClassEffect::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsEffect, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassEffect::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEffect * const effect = ( ( sEffNatDat* )p_GetNativeData( myself ) )->effect;
	deClassEffect * const clsEffect = ( deClassEffect* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsEffect ) ){
		rt->PushBool( false );
		
	}else{
		deEffect * const otherEffect = ( ( sEffNatDat* )p_GetNativeData( obj ) )->effect;
		rt->PushBool( effect == otherEffect );
	}
}



// Class deClassEffect
////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEffect::deClassEffect( deScriptingDragonScript &ds ) :
dsClass( "Effect", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sEffNatDat ) );
}
deClassEffect::~deClassEffect(){
}



// Management
///////////////

void deClassEffect::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsEffect = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetEnabled( init ) );
	AddFunction( new nfSetEnabled( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



void deClassEffect::AssignEffect( dsRealObject *myself, deEffect *effect ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	sEffNatDat &nd = *( ( sEffNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
	if( effect == nd.effect ){
		return;
	}
	
	if( nd.effect ){
		nd.effect->FreeReference();
	}
	
	nd.effect = effect;
	
	if( effect ){
		effect->AddReference();
	}
}



deEffect *deClassEffect::GetEffect( dsRealObject *myself ) const {
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sEffNatDat* )p_GetNativeData( myself->GetBuffer() ) )->effect;
}

void deClassEffect::PushEffect( dsRunTime *rt, deEffect *effect ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! effect ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deEffectVisitorIdentify identify;
	effect->Visit( identify );
	
	switch( identify.GetType() ){
	case deEffectVisitorIdentify::eetColorMatrix:
		pDS.GetClassEffectColorMatrix()->PushEffect( rt, &identify.CastToColorMatrix() );
		break;
		
	case deEffectVisitorIdentify::eetDistortImage:
		pDS.GetClassEffectDistortImage()->PushEffect( rt, &identify.CastToDistortImage() );
		break;
		
	case deEffectVisitorIdentify::eetFilterKernel:
		pDS.GetClassEffectFilterKernel()->PushEffect( rt, &identify.CastToFilterKernel() );
		break;
		
	case deEffectVisitorIdentify::eetOverlayImage:
		pDS.GetClassEffectOverlayImage()->PushEffect( rt, &identify.CastToOverlayImage() );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
}
