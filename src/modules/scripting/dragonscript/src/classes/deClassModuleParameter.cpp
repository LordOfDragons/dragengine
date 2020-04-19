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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libdscript/libdscript.h>
#include "deClassModuleParameter.h"
#include "../deScriptingDragonScript.h"
#include "../deClassPathes.h"
#include "dragengine/systems/modules/deModuleParameter.h"
#include "dragengine/systems/modules/deBaseModule.h"
#include "dragengine/deEngine.h"
#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// native structure
struct sMPNatDat{
	deModuleParameter *param;
};

// native functions
/////////////////////

// public destructor Destructor()
deClassModuleParameter::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsMP, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassModuleParameter::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sMPNatDat *nd = (sMPNatDat*)p_GetNativeData( myself );
	
	if( nd->param ){
		delete nd->param;
		nd->param = NULL;
	}
}



// public func String getName()
deClassModuleParameter::nfGetName::nfGetName( const sInitData &init ) :
dsFunction( init.clsMP, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassModuleParameter::nfGetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	rt->PushString( param.GetName() );
}

// public func String getDescription()
deClassModuleParameter::nfGetDescription::nfGetDescription( const sInitData &init ) :
dsFunction( init.clsMP, "getDescription", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassModuleParameter::nfGetDescription::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	rt->PushString( param.GetDescription() );
}

// public func ModuleParameterType getType()
deClassModuleParameter::nfGetType::nfGetType( const sInitData &init ) :
dsFunction( init.clsMP, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsModuleParameterType ){
}
void deClassModuleParameter::nfGetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	rt->PushValue( ( ( deClassModuleParameter* )GetOwnerClass() )->GetClassModuleParameterType()
		->GetVariable( param.GetType() )->GetStaticValue() );
}

// public func float getMinimumValue()
deClassModuleParameter::nfGetMinimumValue::nfGetMinimumValue( const sInitData &init ) :
dsFunction( init.clsMP, "getMinimumValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassModuleParameter::nfGetMinimumValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	rt->PushFloat( param.GetMinimumValue() );
}

// public func float getMaximumValue()
deClassModuleParameter::nfGetMaximumValue::nfGetMaximumValue( const sInitData &init ) :
dsFunction( init.clsMP, "getMaximumValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassModuleParameter::nfGetMaximumValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	rt->PushFloat( param.GetMaximumValue() );
}

// public func float getValueStepSize()
deClassModuleParameter::nfGetValueStepSize::nfGetValueStepSize( const sInitData &init ) :
dsFunction( init.clsMP, "getValueStepSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassModuleParameter::nfGetValueStepSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	rt->PushFloat( param.GetValueStepSize() );
}

// public func int getSelectionEntryCount()
deClassModuleParameter::nfGetSelectionEntryCount::nfGetSelectionEntryCount( const sInitData &init ) :
dsFunction( init.clsMP, "getSelectionEntryCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassModuleParameter::nfGetSelectionEntryCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	rt->PushInt( param.GetSelectionEntryCount() );
}

// public func String getSelectionEntryValueAt( int index )
deClassModuleParameter::nfGetSelectionEntryValueAt::nfGetSelectionEntryValueAt( const sInitData &init ) :
dsFunction( init.clsMP, "getSelectionEntryValueAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter(init.clsInt); // index
}
void deClassModuleParameter::nfGetSelectionEntryValueAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	int index = rt->GetValue(0)->GetInt();
	rt->PushString( param.GetSelectionEntryAt( index ).value );
}

// public func String getSelectionEntryDisplayNameAt( int index )
deClassModuleParameter::nfGetSelectionEntryDisplayNameAt::nfGetSelectionEntryDisplayNameAt( const sInitData &init ) :
dsFunction( init.clsMP, "getSelectionEntryDisplayNameAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter(init.clsInt); // index
}
void deClassModuleParameter::nfGetSelectionEntryDisplayNameAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	int index = rt->GetValue(0)->GetInt();
	rt->PushString( param.GetSelectionEntryAt( index ).displayName );
}

// public func String getSelectionEntryDescriptionAt( int index )
deClassModuleParameter::nfGetSelectionEntryDescriptionAt::nfGetSelectionEntryDescriptionAt( const sInitData &init ) :
dsFunction( init.clsMP, "getSelectionEntryDescriptionAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter(init.clsInt); // index
}
void deClassModuleParameter::nfGetSelectionEntryDescriptionAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	int index = rt->GetValue(0)->GetInt();
	rt->PushString( param.GetSelectionEntryAt( index ).description );
}

// public func ModuleParameterCategory getCategory()
deClassModuleParameter::nfGetCategory::nfGetCategory( const sInitData &init ) :
dsFunction( init.clsMP, "getCategory", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsModuleParameterCategory ){
}
void deClassModuleParameter::nfGetCategory::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	rt->PushValue( ( ( deClassModuleParameter* )GetOwnerClass() )->GetClassModuleParameterCategory()
		->GetVariable( param.GetCategory() )->GetStaticValue() );
}

// public func String getDisplayName()
deClassModuleParameter::nfGetDisplayName::nfGetDisplayName( const sInitData &init ) :
dsFunction( init.clsMP, "getDisplayName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassModuleParameter::nfGetDisplayName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deModuleParameter &param = *( ( ( sMPNatDat* )p_GetNativeData( myself ) )->param );
	rt->PushString( param.GetDisplayName() );
}



// Class deClassModuleParameter
/////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassModuleParameter::deClassModuleParameter( deEngine *GameEngine, deScriptingDragonScript *ScrMgr ) :
dsClass( "ModuleParameter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! GameEngine || ! ScrMgr ){
		DSTHROW( dueInvalidParam );
	}
	
	p_gameEngine = GameEngine;
	p_scrMgr = ScrMgr;
	
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sMPNatDat));
}

deClassModuleParameter::~deClassModuleParameter(){
}



// Management
///////////////

void deClassModuleParameter::CreateClassMembers( dsEngine *engine ){
	// store classes
	pClsModuleParameterType = engine->GetClass( "Dragengine.ModuleParameterType" );
	pClsModuleParameterCategory = engine->GetClass( "Dragengine.ModuleParameterCategory" );
	
	sInitData init;
	init.clsMP = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsModuleParameterType = pClsModuleParameterType;
	init.clsModuleParameterCategory = pClsModuleParameterCategory;
	
	// add functions
	AddFunction( new nfDestructor( init ) );
	AddFunction( new nfGetName( init ) );
	AddFunction( new nfGetDescription( init ) );
	AddFunction( new nfGetType( init ) );
	AddFunction( new nfGetMinimumValue( init ) );
	AddFunction( new nfGetMaximumValue( init ) );
	AddFunction( new nfGetValueStepSize( init ) );
	AddFunction( new nfGetSelectionEntryCount( init ) );
	AddFunction( new nfGetSelectionEntryValueAt( init ) );
	AddFunction( new nfGetSelectionEntryDisplayNameAt( init ) );
	AddFunction( new nfGetSelectionEntryDescriptionAt( init ) );
	AddFunction( new nfGetCategory( init ) );
	AddFunction( new nfGetDisplayName( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

void deClassModuleParameter::PushParameter( dsRunTime *rt, deBaseModule *module, int index ){
	if( ! rt || ! module ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sMPNatDat &nd = *( ( sMPNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.param = NULL;
	
	try{
		nd.param = new deModuleParameter;
		module->GetParameterInfo( index, *nd.param );
		
	}catch( const duException & ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
