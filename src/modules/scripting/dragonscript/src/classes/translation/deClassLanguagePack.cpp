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

#include "deClassLanguagePack.h"
#include "../string/deClassUnicodeString.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <libdscript/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/localization/deLanguagePackManager.h>


// Native Structure
/////////////////////

struct sLPNatDat{
	deLanguagePack *langPack;
};



// Native functions
/////////////////////


// Constructors, Destructor
/////////////////////////////

// public func new( String filename )
deClassLanguagePack::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsLP, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // filename
}
void deClassLanguagePack::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sLPNatDat * const nd = ( sLPNatDat* )p_GetNativeData( myself );
	const deScriptingDragonScript &ds = ( ( deClassLanguagePack* )GetOwnerClass() )->GetDS();
	deLanguagePackManager &lpmgr = *ds.GetGameEngine()->GetLanguagePackManager();
	
	// clear ( important )
	nd->langPack = NULL;
	
	// create langPack
	nd->langPack = lpmgr.LoadLanguagePack( rt->GetValue( 0 )->GetString() );
}

// public func destructor()
deClassLanguagePack::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsLP, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassLanguagePack::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sLPNatDat &nd = *( ( sLPNatDat* )p_GetNativeData( myself ) );
	if( ! nd.langPack ){
		return;
	}
	
	nd.langPack->FreeReference();
	nd.langPack = NULL;
}



// management
///////////////

// public func UnicodeString getName()
deClassLanguagePack::nfGetName::nfGetName( const sInitData &init ) :
dsFunction( init.clsLP, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
}
void deClassLanguagePack::nfGetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deLanguagePack &langPack = *( ( ( sLPNatDat* )p_GetNativeData( myself ) )->langPack );
	deScriptingDragonScript &ds = ( ( deClassLanguagePack* )GetOwnerClass() )->GetDS();
	
	ds.GetClassUnicodeString()->PushUnicodeString( rt, langPack.GetName() );
}

// public func UnicodeString getDescription()
deClassLanguagePack::nfGetDescription::nfGetDescription( const sInitData &init ) :
dsFunction( init.clsLP, "getDescription", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
}
void deClassLanguagePack::nfGetDescription::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deLanguagePack &langPack = *( ( ( sLPNatDat* )p_GetNativeData( myself ) )->langPack );
	deScriptingDragonScript &ds = ( ( deClassLanguagePack* )GetOwnerClass() )->GetDS();
	
	ds.GetClassUnicodeString()->PushUnicodeString( rt, langPack.GetDescription() );
}

// public func String getFilename()
deClassLanguagePack::nfGetFilename::nfGetFilename( const sInitData &init ) :
dsFunction( init.clsLP, "getFilename", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassLanguagePack::nfGetFilename::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deLanguagePack &langPack = *( ( ( sLPNatDat* )p_GetNativeData( myself ) )->langPack );
	rt->PushString( langPack.GetFilename() );
}



// public func UnicodeString translate( String name )
deClassLanguagePack::nfTranslate::nfTranslate( const sInitData &init ) :
dsFunction( init.clsLP, "translate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsStr ); // name
}
void deClassLanguagePack::nfTranslate::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deLanguagePack &langPack = *( ( ( sLPNatDat* )p_GetNativeData( myself ) )->langPack );
	deScriptingDragonScript &ds = ( ( deClassLanguagePack* )GetOwnerClass() )->GetDS();
	
	const char * const name = rt->GetValue( 0 )->GetString();
	
	ds.GetClassUnicodeString()->PushUnicodeString( rt, langPack.Translate( name ) );
}

// public func UnicodeString translate2( String name, UnicodeString defaultValue )
deClassLanguagePack::nfTranslate2::nfTranslate2( const sInitData &init ) :
dsFunction( init.clsLP, "translate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsStr ); // name
	p_AddParameter( init.clsUS ); // defaultValue
}
void deClassLanguagePack::nfTranslate2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deLanguagePack &langPack = *( ( ( sLPNatDat* )p_GetNativeData( myself ) )->langPack );
	deScriptingDragonScript &ds = ( ( deClassLanguagePack* )GetOwnerClass() )->GetDS();
	deClassUnicodeString &clsUS = *ds.GetClassUnicodeString();
	
	const char * const name = rt->GetValue( 0 )->GetString();
	const decUnicodeString &defaultValue = clsUS.GetUnicodeString(
		rt->GetValue( 1 )->GetRealObject() );
	
	clsUS.PushUnicodeString( rt, langPack.Translate( name, defaultValue ) );
}



// public func int hashCode()
deClassLanguagePack::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsLP, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassLanguagePack::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deLanguagePack * const langPack = ( ( sLPNatDat* )p_GetNativeData( myself ) )->langPack;
	
	// hash code = memory location
	rt->PushInt( ( intptr_t )langPack );
}

// public func bool equals( Object obj )
deClassLanguagePack::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsLP, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassLanguagePack::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deLanguagePack * const langPack = ( ( sLPNatDat* )p_GetNativeData( myself ) )->langPack;
	deClassLanguagePack * const clsLP = ( deClassLanguagePack* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsLP ) ){
		rt->PushBool( false );
		
	}else{
		const deLanguagePack * const other = ( ( sLPNatDat* )p_GetNativeData( obj ) )->langPack;
		rt->PushBool( langPack->GetName() == other->GetName() );
	}
}



// Class deClassLanguagePack
//////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassLanguagePack::deClassLanguagePack( deScriptingDragonScript &ds ) :
dsClass( "LanguagePack", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sLPNatDat ) );
}

deClassLanguagePack::~deClassLanguagePack(){
}



// Management
///////////////

void deClassLanguagePack::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsLP = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsUS = pDS.GetClassUnicodeString();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetName( init ) );
	AddFunction( new nfGetDescription( init ) );
	AddFunction( new nfGetFilename( init ) );
	
	AddFunction( new nfTranslate( init ) );
	AddFunction( new nfTranslate2( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	CalcMemberOffsets();
}

deLanguagePack *deClassLanguagePack::GetLanguagePack( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sLPNatDat* )p_GetNativeData( myself->GetBuffer() ) )->langPack;
}

void deClassLanguagePack::PushLanguagePack( dsRunTime *rt, deLanguagePack *langPack ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! langPack ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sLPNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->langPack = langPack;
	langPack->AddReference();
}
