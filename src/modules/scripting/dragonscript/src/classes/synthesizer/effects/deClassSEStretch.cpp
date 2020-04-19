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

#include "deClassSEStretch.h"
#include "../deClassSynthesizer.h"
#include "../deClassSynthesizerEffect.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectStretch.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sSEStretchNatDat{
	deSynthesizerSource *source;
	deSynthesizerEffectStretch *effect;
};



// Native Functions
/////////////////////

// public func new()
deClassSEStretch::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsSEStretch,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSEStretch::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSEStretchNatDat &nd = *( ( sSEStretchNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.source = NULL;
	nd.effect = NULL;
	
	// super call
	deClassSynthesizerEffect * const baseClass = ( deClassSynthesizerEffect* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create synthesizer effect
	nd.effect = new deSynthesizerEffectStretch;
	baseClass->AssignEffect( myself->GetRealObject(), nd.effect );
}

// public func destructor()
deClassSEStretch::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsSEStretch,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSEStretch::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSEStretchNatDat &nd = *( ( sSEStretchNatDat* )p_GetNativeData( myself ) );
	
	if( nd.source ){
		nd.source->FreeReference();
		nd.source = NULL;
	}
	
	if( nd.effect ){
		nd.effect->FreeReference();
		nd.effect = NULL;
	}
}



// public func void targetAddLink( SEStretchTarget target, int link )
deClassSEStretch::nfTargetAddLink::nfTargetAddLink( const sInitData &init ) : dsFunction( init.clsSEStretch,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSEStretchTarget ); // target
	p_AddParameter( init.clsInt ); // link
}
void deClassSEStretch::nfTargetAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sSEStretchNatDat &nd = *( ( sSEStretchNatDat* )p_GetNativeData( myself ) );
	const deClassSEStretch::eTargets target = ( deClassSEStretch::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue( 1 )->GetInt();
	
	switch( target ){
	case deClassSEStretch::etStrength:
		nd.effect->GetTargetStrength().AddLink( link );
		break;
		
	case deClassSEStretch::etTime:
		nd.effect->GetTargetTime().AddLink( link );
		break;
		
	case deClassSEStretch::etPitch:
		nd.effect->GetTargetPitch().AddLink( link );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.source && nd.source->GetSynthesizer() ){
		nd.source->GetSynthesizer()->NotifySourcesChanged();
	}
}

// public func void targetRemoveAllLinks( SEStretchTarget target )
deClassSEStretch::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsSEStretch,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSEStretchTarget ); // target
}
void deClassSEStretch::nfTargetRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sSEStretchNatDat &nd = *( ( sSEStretchNatDat* )p_GetNativeData( myself ) );
	const deClassSEStretch::eTargets target = ( deClassSEStretch::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( target ){
	case deClassSEStretch::etStrength:
		nd.effect->GetTargetStrength().RemoveAllLinks();
		break;
		
	case deClassSEStretch::etTime:
		nd.effect->GetTargetTime().RemoveAllLinks();
		break;
		
	case deClassSEStretch::etPitch:
		nd.effect->GetTargetPitch().RemoveAllLinks();
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.source && nd.source->GetSynthesizer() ){
		nd.source->GetSynthesizer()->NotifySourcesChanged();
	}
}



// public func void setMinTime( float time )
deClassSEStretch::nfSetMinTime::nfSetMinTime( const sInitData &init ) : dsFunction( init.clsSEStretch,
"setMinTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}
void deClassSEStretch::nfSetMinTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSEStretchNatDat &nd = *( ( sSEStretchNatDat* )p_GetNativeData( myself ) );
	const float time = rt->GetValue( 0 )->GetFloat();
	
	if( fabsf( time - nd.effect->GetMinTime() ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	nd.effect->SetMinTime( time );
	
	if( nd.source && nd.source->GetSynthesizer() ){
		nd.source->GetSynthesizer()->NotifySourcesChanged();
	}
}

// public func void setMaxTime( float time )
deClassSEStretch::nfSetMaxTime::nfSetMaxTime( const sInitData &init ) : dsFunction( init.clsSEStretch,
"setMaxTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // time
}
void deClassSEStretch::nfSetMaxTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSEStretchNatDat &nd = *( ( sSEStretchNatDat* )p_GetNativeData( myself ) );
	const float time = rt->GetValue( 0 )->GetFloat();
	
	if( fabsf( time - nd.effect->GetMaxTime() ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	nd.effect->SetMaxTime( time );
	
	if( nd.source && nd.source->GetSynthesizer() ){
		nd.source->GetSynthesizer()->NotifySourcesChanged();
	}
}

// public func void setMinPitch( float pitch )
deClassSEStretch::nfSetMinPitch::nfSetMinPitch( const sInitData &init ) : dsFunction( init.clsSEStretch,
"setMinPitch", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // pitch
}
void deClassSEStretch::nfSetMinPitch::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSEStretchNatDat &nd = *( ( sSEStretchNatDat* )p_GetNativeData( myself ) );
	const float pitch = rt->GetValue( 0 )->GetFloat();
	
	if( fabsf( pitch - nd.effect->GetMinPitch() ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	nd.effect->SetMinPitch( pitch );
	
	if( nd.source && nd.source->GetSynthesizer() ){
		nd.source->GetSynthesizer()->NotifySourcesChanged();
	}
}

// public func void setMaxPitch( float pitch )
deClassSEStretch::nfSetMaxPitch::nfSetMaxPitch( const sInitData &init ) : dsFunction( init.clsSEStretch,
"setMaxPitch", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // pitch
}
void deClassSEStretch::nfSetMaxPitch::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSEStretchNatDat &nd = *( ( sSEStretchNatDat* )p_GetNativeData( myself ) );
	const float pitch = rt->GetValue( 0 )->GetFloat();
	
	if( fabsf( pitch - nd.effect->GetMaxPitch() ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	nd.effect->SetMaxPitch( pitch );
	
	if( nd.source && nd.source->GetSynthesizer() ){
		nd.source->GetSynthesizer()->NotifySourcesChanged();
	}
}



// Class deClassSEStretch
///////////////////////////

// Constructor
////////////////

deClassSEStretch::deClassSEStretch( deScriptingDragonScript &ds ) :
dsClass( "SEStretch", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "SynthesizerEffect" );
	
	p_SetNativeDataSize( sizeof( sSEStretchNatDat ) );
}

deClassSEStretch::~deClassSEStretch(){
}



// Management
///////////////

void deClassSEStretch::CreateClassMembers( dsEngine *engine ){
	pClsSEStretchTarget = engine->GetClass( "Dragengine.Scenery.SEStretchTarget" );
	
	sInitData init;
	init.clsSEStretch = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsSEStretchTarget = pClsSEStretchTarget;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfTargetAddLink( init ) );
	AddFunction( new nfTargetRemoveAllLinks( init ) );
	
	AddFunction( new nfSetMinTime( init ) );
	AddFunction( new nfSetMaxTime( init ) );
	AddFunction( new nfSetMinPitch( init ) );
	AddFunction( new nfSetMaxPitch( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSynthesizerEffectStretch *deClassSEStretch::GetEffect( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sSEStretchNatDat* )p_GetNativeData( myself->GetBuffer() ) )->effect;
}

void deClassSEStretch::AssignSynthesizer( dsRealObject *myself, deSynthesizerSource *source ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS.GetClassSynthesizerEffect()->AssignSource( myself, source );
	
	sSEStretchNatDat &nd = *( ( sSEStretchNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
	if( source == nd.source ){
		return;
	}
	
	if( nd.source ){
		nd.source->FreeReference();
	}
	
	nd.source = source;
	
	if( source ){
		source->AddReference();
	}
}

void deClassSEStretch::PushEffect( dsRunTime *rt, deSynthesizerSource *source, deSynthesizerEffectStretch *effect ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! effect ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassSynthesizerEffect * const baseClass = ( deClassSynthesizerEffect* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sSEStretchNatDat &nd = *( ( sSEStretchNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.source = NULL;
	nd.effect = NULL;
	
	try{
		baseClass->CallBaseClassConstructor( rt, rt->GetValue( 0 ), baseClass->GetFirstConstructor(), 0 );
		
		nd.source = source;
		if( source ){
			source->AddReference();
		}
		
		nd.effect = effect;
		effect->AddReference();
		
		baseClass->AssignEffect( rt->GetValue( 0 )->GetRealObject(), effect );
		baseClass->AssignSource( rt->GetValue( 0 )->GetRealObject(), source );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
