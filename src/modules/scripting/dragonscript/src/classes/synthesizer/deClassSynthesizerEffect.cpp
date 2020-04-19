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

#include "deClassSynthesizer.h"
#include "deClassSynthesizerEffect.h"
#include "effects/deClassSEStretch.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectStretch.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectVisitorIdentify.h>

#include <libdscript/exceptions.h>



struct sSynSNatDat{
	deSynthesizerSource *source;
	deSynthesizerEffect *effect;
};



// Native Functions
/////////////////////

// protected func new()
deClassSynthesizerEffect::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsSynEff,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
}
void deClassSynthesizerEffect::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSynSNatDat &nd = *( ( sSynSNatDat* )p_GetNativeData( myself ) );
	nd.source = NULL;
	nd.effect = NULL;
}

// public func destructor()
deClassSynthesizerEffect::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsSynEff,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSynthesizerEffect::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSynSNatDat &nd = *( ( sSynSNatDat* )p_GetNativeData( myself ) );
	
	if( nd.source ){
		nd.source->FreeReference();
		nd.source = NULL;
	}
	
	if( nd.effect ){
		nd.effect->FreeReference();
		nd.effect = NULL;
	}
}



// public func void setEnabled( bool enabled )
deClassSynthesizerEffect::nfSetEnabled::nfSetEnabled( const sInitData &init ) : dsFunction( init.clsSynEff,
"setEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassSynthesizerEffect::nfSetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSynSNatDat &nd = *( ( sSynSNatDat* )p_GetNativeData( myself ) );
	const bool enabled = rt->GetValue( 0 )->GetBool();
	
	if( ! nd.effect ){
		DSTHROW( dueNullPointer );
	}
	
	if( nd.effect->GetEnabled() == enabled ){
		return;
	}
	
	nd.effect->SetEnabled( enabled );
	
	if( nd.source && nd.source->GetSynthesizer() ){
		nd.source->GetSynthesizer()->NotifySourcesChanged();
	}
}

// public func void setStrength( float strength )
deClassSynthesizerEffect::nfSetStrength::nfSetStrength( const sInitData &init ) : dsFunction( init.clsSynEff,
"setStrength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // strength
}
void deClassSynthesizerEffect::nfSetStrength::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSynSNatDat &nd = *( ( sSynSNatDat* )p_GetNativeData( myself ) );
	
	if( ! nd.effect ){
		DSTHROW( dueNullPointer );
	}
	
	nd.effect->SetStrength( rt->GetValue( 0 )->GetFloat() );
	
	if( nd.source && nd.source->GetSynthesizer() ){
		nd.source->GetSynthesizer()->NotifySourcesChanged();
	}
}



// Class deClassSynthesizerEffect
///////////////////////////////////

// Constructor
////////////////

deClassSynthesizerEffect::deClassSynthesizerEffect( deScriptingDragonScript &ds ) :
dsClass( "SynthesizerEffect", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSynSNatDat ) );
}

deClassSynthesizerEffect::~deClassSynthesizerEffect(){
}



// Management
///////////////

void deClassSynthesizerEffect::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsSynEff = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	// add constants
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfSetEnabled( init ) );
	AddFunction( new nfSetStrength( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSynthesizerEffect *deClassSynthesizerEffect::GetEffect( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sSynSNatDat* )p_GetNativeData( myself->GetBuffer() ) )->effect;
}

void deClassSynthesizerEffect::AssignEffect( dsRealObject *myself, deSynthesizerEffect *effect ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	sSynSNatDat &nd = *( ( sSynSNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
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

void deClassSynthesizerEffect::AssignSource( dsRealObject *myself, deSynthesizerSource *source ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	sSynSNatDat &nd = *( ( sSynSNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
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

void deClassSynthesizerEffect::PushEffect( dsRunTime *rt, deSynthesizerSource *source, deSynthesizerEffect *effect ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! effect ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deSynthesizerEffectVisitorIdentify visitor;
	effect->Visit( visitor );
	
	switch( visitor.GetType() ){
	case deSynthesizerEffectVisitorIdentify::eetStretch:
		pDS.GetClassSEStretch()->PushEffect( rt, source, &visitor.CastToStretch() );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
}
