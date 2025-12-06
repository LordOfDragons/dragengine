/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deClassLanguagePack.h"
#include "deClassLanguagePackBuilder.h"
#include "../string/deClassUnicodeString.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/localization/deLanguagePackEntry.h>
#include <dragengine/resources/localization/deLanguagePackBuilder.h>
#include <dragengine/resources/localization/deLanguagePackManager.h>


class deClassLanguagePackBuilder_Builder : public deLanguagePackBuilder{
	dsRunTime *pRT;
	dsValue *pMyself;
	deLanguagePack *pLanguagePack;
	
public:
	deClassLanguagePackBuilder_Builder( dsRunTime *rt, dsValue *myself ) :
	pRT( rt ), pMyself( myself ), pLanguagePack( NULL ){
	}
	
	virtual void BuildLanguagePack( deLanguagePack &languagePack ){
		pLanguagePack = &languagePack;
		
		try{
			pRT->RunFunction( pMyself, "buildLanguagePack", 0 );
			
		}catch( const duException &e ){
			pLanguagePack = NULL;
			pRT->PrintExceptionTrace();
			e.PrintError();
			DETHROW( deeInvalidParam );
			
		}catch( ... ){
			pLanguagePack = NULL;
			throw;
		}
		
		pLanguagePack = NULL;
	}
	
	inline deLanguagePack *GetLanguagePack() const{ return pLanguagePack; }
};


struct sFntBldNatDat{
	deClassLanguagePackBuilder_Builder *builder;
};



// Native Functions
/////////////////////

// public constructor new()
deClassLanguagePackBuilder::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsLanguagePackBuilder, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassLanguagePackBuilder::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder = NULL;
}

// public destructor Destructor()
deClassLanguagePackBuilder::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsLanguagePackBuilder, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassLanguagePackBuilder::nfDestructor::RunFunction( dsRunTime*, dsValue* ){
}



// public func LanguagePack build( String filename )
deClassLanguagePackBuilder::nfBuild::nfBuild( const sInitData &init ) :
dsFunction( init.clsLanguagePackBuilder, "build", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLanguagePack ){
	p_AddParameter( init.clsString ); // filename
}
void deClassLanguagePackBuilder::nfBuild::RunFunction( dsRunTime *rt, dsValue *myself ){
	sFntBldNatDat &nd = *( ( sFntBldNatDat* )p_GetNativeData( myself ) );
	if( nd.builder ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassLanguagePackBuilder* )GetOwnerClass() )->GetDS();
	const char * const filename = rt->GetValue( 0 )->GetString();
	deClassLanguagePackBuilder_Builder builder( rt, myself );
	deLanguagePack::Ref languagePack;
	
	nd.builder = &builder;
	
	try{
		languagePack.TakeOver( ds.GetGameEngine()->GetLanguagePackManager()->CreateLanguagePack( filename, builder ) );
		
	}catch( ... ){
		nd.builder = NULL;
		throw;
	}
	
	nd.builder = NULL;
	ds.GetClassLanguagePack()->PushLanguagePack( rt, languagePack );
}



// abstract protected func void buildLanguagePack()
deClassLanguagePackBuilder::nfBuildLanguagePack::nfBuildLanguagePack( const sInitData &init ) :
dsFunction( init.clsLanguagePackBuilder, "buildLanguagePack", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE | DSTM_ABSTRACT, init.clsVoid ){
}
void deClassLanguagePackBuilder::nfBuildLanguagePack::RunFunction( dsRunTime*, dsValue* ){
}



// protected func void setName( UnicodeString name )
deClassLanguagePackBuilder::nfSetName::nfSetName( const sInitData &init ) :
dsFunction( init.clsLanguagePackBuilder, "setName", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsUnicodeString ); // name
}
void deClassLanguagePackBuilder::nfSetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassLanguagePackBuilder_Builder * const builder = ( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetLanguagePack() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassLanguagePackBuilder* )GetOwnerClass() )->GetDS();
	builder->GetLanguagePack()->SetName( ds.GetClassUnicodeString()->GetUnicodeString(
		rt->GetValue( 0 )->GetRealObject() ) );
}

// protected func void setDescription( UnicodeString description )
deClassLanguagePackBuilder::nfSetDescription::nfSetDescription( const sInitData &init ) :
dsFunction( init.clsLanguagePackBuilder, "setDescription", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsUnicodeString ); // description
}
void deClassLanguagePackBuilder::nfSetDescription::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassLanguagePackBuilder_Builder * const builder = ( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetLanguagePack() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassLanguagePackBuilder* )GetOwnerClass() )->GetDS();
	builder->GetLanguagePack()->SetDescription( ds.GetClassUnicodeString()->GetUnicodeString(
		rt->GetValue( 0 )->GetRealObject() ) );
}

// protected func void setMissingText( UnicodeString missingText )
deClassLanguagePackBuilder::nfSetMissingText::nfSetMissingText( const sInitData &init ) :
dsFunction( init.clsLanguagePackBuilder, "setMissingText", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsUnicodeString ); // missingText
}
void deClassLanguagePackBuilder::nfSetMissingText::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassLanguagePackBuilder_Builder * const builder = ( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetLanguagePack() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassLanguagePackBuilder* )GetOwnerClass() )->GetDS();
	builder->GetLanguagePack()->SetMissingText( ds.GetClassUnicodeString()->GetUnicodeString(
		rt->GetValue( 0 )->GetRealObject() ) );
}

// protected func void setEntryCount( int count )
deClassLanguagePackBuilder::nfSetEntryCount::nfSetEntryCount( const sInitData &init ) :
dsFunction( init.clsLanguagePackBuilder, "setEntryCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // count
}
void deClassLanguagePackBuilder::nfSetEntryCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassLanguagePackBuilder_Builder * const builder = ( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetLanguagePack() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetLanguagePack()->SetEntryCount( rt->GetValue( 0 )->GetInt() );
}

// protected func void setEntryAt( int index, String name, UnicodeString text )
deClassLanguagePackBuilder::nfSetEntryAt::nfSetEntryAt( const sInitData &init ) :
dsFunction( init.clsLanguagePackBuilder, "setEntryAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsString ); // name
	p_AddParameter( init.clsUnicodeString ); // text
}
void deClassLanguagePackBuilder::nfSetEntryAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassLanguagePackBuilder_Builder * const builder = ( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetLanguagePack() ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassLanguagePackBuilder* )GetOwnerClass() )->GetDS();
	deLanguagePackEntry &entry = builder->GetLanguagePack()->GetEntryAt( rt->GetValue( 0 )->GetInt() );
	entry.SetName( rt->GetValue( 1 )->GetString() );
	entry.SetText( ds.GetClassUnicodeString()->GetUnicodeString( rt->GetValue( 2 )->GetRealObject() ) );
}



// Class deClassLanguagePackBuilder
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassLanguagePackBuilder::deClassLanguagePackBuilder( deScriptingDragonScript &ds ) :
dsClass( "LanguagePackBuilder", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sFntBldNatDat ) );
}

deClassLanguagePackBuilder::~deClassLanguagePackBuilder(){
}



// Management
///////////////

void deClassLanguagePackBuilder::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsLanguagePackBuilder = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBoolean = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	init.clsLanguagePack = pDS.GetClassLanguagePack();
	init.clsUnicodeString = pDS.GetClassUnicodeString();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfBuild( init ) );
	AddFunction( new nfBuildLanguagePack( init ) );
	AddFunction( new nfSetName( init ) );
	AddFunction( new nfSetDescription( init ) );
	AddFunction( new nfSetMissingText( init ) );
	AddFunction( new nfSetEntryCount( init ) );
	AddFunction( new nfSetEntryAt( init ) );
}
