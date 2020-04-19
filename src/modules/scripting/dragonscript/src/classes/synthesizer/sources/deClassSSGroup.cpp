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

#include "deClassSSGroup.h"
#include "../deClassSynthesizer.h"
#include "../deClassSynthesizerSource.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sSSGroupNatDat{
	deSynthesizer *synthesizer;
	deSynthesizerSourceGroup *source;
};



// Native Functions
/////////////////////

// public func new()
deClassSSGroup::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsSSGroup,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSSGroup::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSSGroupNatDat &nd = *( ( sSSGroupNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.synthesizer = NULL;
	nd.source = NULL;
	
	// super call
	deClassSynthesizerSource * const baseClass = ( deClassSynthesizerSource* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create synthesizer source
	nd.source = new deSynthesizerSourceGroup;
	baseClass->AssignSource( myself->GetRealObject(), nd.source );
}

// public func destructor()
deClassSSGroup::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsSSGroup,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSSGroup::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSSGroupNatDat &nd = *( ( sSSGroupNatDat* )p_GetNativeData( myself ) );
	
	if( nd.synthesizer ){
		nd.synthesizer->FreeReference();
		nd.synthesizer = NULL;
	}
	
	if( nd.source ){
		nd.source->FreeReference();
		nd.source = NULL;
	}
}



// public func void targetAddLink( SSGroupTarget target, int link )
deClassSSGroup::nfTargetAddLink::nfTargetAddLink( const sInitData &init ) : dsFunction( init.clsSSGroup,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSSGroupTarget ); // target
	p_AddParameter( init.clsInt ); // link
}
void deClassSSGroup::nfTargetAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sSSGroupNatDat &nd = *( ( sSSGroupNatDat* )p_GetNativeData( myself ) );
	const deClassSSGroup::eTargets target = ( deClassSSGroup::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue( 1 )->GetInt();
	
	switch( target ){
	case deClassSSGroup::etBlendFactor:
		nd.source->GetTargetBlendFactor().AddLink( link );
		break;
		
	case deClassSSGroup::etVolume:
		nd.source->GetTargetVolume().AddLink( link );
		break;
		
	case deClassSSGroup::etPanning:
		nd.source->GetTargetPanning().AddLink( link );
		break;
		
	case deClassSSGroup::etSelection:
		nd.source->GetTargetSelect().AddLink( link );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void targetRemoveAllLinks( SSGroupTarget target )
deClassSSGroup::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsSSGroup,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSSGroupTarget ); // target
}
void deClassSSGroup::nfTargetRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sSSGroupNatDat &nd = *( ( sSSGroupNatDat* )p_GetNativeData( myself ) );
	const deClassSSGroup::eTargets target = ( deClassSSGroup::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( target ){
	case deClassSSGroup::etBlendFactor:
		nd.source->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassSSGroup::etVolume:
		nd.source->GetTargetVolume().RemoveAllLinks();
		break;
		
	case deClassSSGroup::etPanning:
		nd.source->GetTargetPanning().RemoveAllLinks();
		break;
		
	case deClassSSGroup::etSelection:
		nd.source->GetTargetSelect().RemoveAllLinks();
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// public func void setApplicationType( SSGroupApplication applicationType )
deClassSSGroup::nfSetApplicationType::nfSetApplicationType( const sInitData &init ) : dsFunction( init.clsSSGroup,
"setApplicationType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSSGroupApplication ); // applicationType
}
void deClassSSGroup::nfSetApplicationType::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sSSGroupNatDat &nd = *( ( sSSGroupNatDat* )p_GetNativeData( myself ) );
	
	const deSynthesizerSourceGroup::eApplicationTypes type = ( deSynthesizerSourceGroup::eApplicationTypes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	if( type == nd.source->GetApplicationType() ){
		return;
	}
	
	nd.source->SetApplicationType( type );
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// public func void addSource( SynthesizerSource source )
deClassSSGroup::nfAddSource::nfAddSource( const sInitData &init ) : dsFunction( init.clsSSGroup,
"addSource", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSynS ); // source
}
void deClassSSGroup::nfAddSource::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSSGroup &clsSource = * (( deClassSSGroup* )GetOwnerClass() );
	sSSGroupNatDat &nd = *( ( sSSGroupNatDat* )p_GetNativeData( myself ) );
	
	deSynthesizerSource * const source = clsSource.GetSource( rt->GetValue( 0 )->GetRealObject() );
	if( ! source ){
		DSTHROW( dueNullPointer );
	}
	nd.source->AddSource( source );
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void removeSource( SynthesizerSource source )
deClassSSGroup::nfRemoveSource::nfRemoveSource( const sInitData &init ) : dsFunction( init.clsSSGroup,
"removeSource", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSynS ); // source
}
void deClassSSGroup::nfRemoveSource::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSSGroup &clsSource = * (( deClassSSGroup* )GetOwnerClass() );
	sSSGroupNatDat &nd = *( ( sSSGroupNatDat* )p_GetNativeData( myself ) );
	
	deSynthesizerSource * const source = clsSource.GetSource( rt->GetValue( 0 )->GetRealObject() );
	if( ! source ){
		DSTHROW( dueNullPointer );
	}
	nd.source->RemoveSource( source );
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void removeAllSources()
deClassSSGroup::nfRemoveAllSources::nfRemoveAllSources( const sInitData &init ) : dsFunction( init.clsSSGroup,
"removeAllSources", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSSGroup::nfRemoveAllSources::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSSGroupNatDat &nd = *( ( sSSGroupNatDat* )p_GetNativeData( myself ) );
	
	if( nd.source->GetSourceCount() == 0 ){
		return;
	}
	
	nd.source->RemoveAllSources();
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}





// Class deClassSSGroup
/////////////////////////

// Constructor
////////////////

deClassSSGroup::deClassSSGroup( deScriptingDragonScript &ds ) :
dsClass( "SSGroup", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "SynthesizerSource" );
	
	p_SetNativeDataSize( sizeof( sSSGroupNatDat ) );
}

deClassSSGroup::~deClassSSGroup(){
}



// Management
///////////////

void deClassSSGroup::CreateClassMembers( dsEngine *engine ){
	pClsSSGroupTarget = engine->GetClass( "Dragengine.Scenery.SSGroupTarget" );
	pClsSSGroupApplication = engine->GetClass( "Dragengine.Scenery.SSGroupApplication" );
	
	sInitData init;
	init.clsSSGroup = this;
	init.clsSynS = pDS.GetClassSynthesizerSource();
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsSSGroupTarget = pClsSSGroupTarget;
	init.clsSSGroupApplication = pClsSSGroupApplication;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfTargetAddLink( init ) );
	AddFunction( new nfTargetRemoveAllLinks( init ) );
	
	AddFunction( new nfSetApplicationType( init ) );
	AddFunction( new nfAddSource( init ) );
	AddFunction( new nfRemoveSource( init ) );
	AddFunction( new nfRemoveAllSources( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSynthesizerSourceGroup *deClassSSGroup::GetSource( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sSSGroupNatDat* )p_GetNativeData( myself->GetBuffer() ) )->source;
}

void deClassSSGroup::AssignSynthesizer( dsRealObject *myself, deSynthesizer *synthesizer ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS.GetClassSynthesizerSource()->AssignSynthesizer( myself, synthesizer );
	
	sSSGroupNatDat &nd = *( ( sSSGroupNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
	if( synthesizer == nd.synthesizer ){
		return;
	}
	
	if( nd.synthesizer ){
		nd.synthesizer->FreeReference();
	}
	
	nd.synthesizer = synthesizer;
	
	if( synthesizer ){
		synthesizer->AddReference();
	}
}

void deClassSSGroup::PushSource( dsRunTime *rt, deSynthesizer *synthesizer, deSynthesizerSourceGroup *source ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! source ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassSynthesizerSource * const baseClass = ( deClassSynthesizerSource* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sSSGroupNatDat &nd = *( ( sSSGroupNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.synthesizer = NULL;
	nd.source = NULL;
	
	try{
		baseClass->CallBaseClassConstructor( rt, rt->GetValue( 0 ), baseClass->GetFirstConstructor(), 0 );
		
		nd.synthesizer = synthesizer;
		if( synthesizer ){
			synthesizer->AddReference();
		}
		
		nd.source = source;
		source->AddReference();
		
		baseClass->AssignSource( rt->GetValue( 0 )->GetRealObject(), source );
		baseClass->AssignSynthesizer( rt->GetValue( 0 )->GetRealObject(), synthesizer );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
