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

#include "deClassFont.h"
#include "deClassFontBuilder.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/font/deFontBuilder.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/font/deFontReference.h>


class deClassFontBuilder_Builder : public deFontBuilder{
	dsRunTime *pRT;
	dsValue *pMyself;
	deFont *pFont;
	
public:
	deClassFontBuilder_Builder( dsRunTime *rt, dsValue *myself ) :
	pRT( rt ), pMyself( myself ), pFont( NULL ){
	}
	
	virtual void BuildFont( deFont *font ){
		pFont = font;
		
		try{
			pRT->RunFunction( pMyself, "buildFont", 0 );
			
		}catch( const duException &e ){
			pFont = NULL;
			pRT->PrintExceptionTrace();
			e.PrintError();
			DETHROW( deeInvalidParam );
			
		}catch( ... ){
			pFont = NULL;
			throw;
		}
		
		pFont = NULL;
	}
	
	inline deFont *GetFont() const{ return pFont; }
};


struct sFntBldNatDat{
	deClassFontBuilder_Builder *builder;
};



// Native Functions
/////////////////////

// public constructor new()
deClassFontBuilder::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsFontBuilder, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassFontBuilder::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder = NULL;
}

// public destructor Destructor()
deClassFontBuilder::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsFontBuilder, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassFontBuilder::nfDestructor::RunFunction( dsRunTime*, dsValue* ){
}



// public func Font build( String filename, int fontSize )
deClassFontBuilder::nfBuild::nfBuild( const sInitData &init ) :
dsFunction( init.clsFontBuilder, "build", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFont ){
	p_AddParameter( init.clsString ); // filename
	p_AddParameter( init.clsInteger ); // fontSize
}
void deClassFontBuilder::nfBuild::RunFunction( dsRunTime *rt, dsValue *myself ){
	sFntBldNatDat &nd = *( ( sFntBldNatDat* )p_GetNativeData( myself ) );
	if( nd.builder ){
		DSTHROW( dueInvalidAction );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassFontBuilder* )GetOwnerClass() )->GetDS();
	const char * const filename = rt->GetValue( 0 )->GetString();
	deClassFontBuilder_Builder builder( rt, myself );
	const int fontSize = rt->GetValue( 1 )->GetInt();
	deFontReference font;
	
	nd.builder = &builder;
	
	try{
		font.TakeOver( ds.GetGameEngine()->GetFontManager()->CreateFont( filename, builder ) );
		
	}catch( ... ){
		nd.builder = NULL;
		throw;
	}
	
	nd.builder = NULL;
	ds.GetClassFont()->PushFont( rt, font, fontSize );
}



// abstract protected func void buildFont()
deClassFontBuilder::nfBuildFont::nfBuildFont( const sInitData &init ) :
dsFunction( init.clsFontBuilder, "buildFont", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE | DSTM_ABSTRACT, init.clsVoid ){
}
void deClassFontBuilder::nfBuildFont::RunFunction( dsRunTime*, dsValue* ){
}



// protected func void setLineHeight( int height )
deClassFontBuilder::nfSetLineHeight::nfSetLineHeight( const sInitData &init ) :
dsFunction( init.clsFontBuilder, "setLineHeight", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // height
}
void deClassFontBuilder::nfSetLineHeight::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFontBuilder_Builder * const builder = ( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetFont() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetFont()->SetLineHeight( rt->GetValue( 0 )->GetInt() );
}

// protected func void setIsColorFont( bool isColorFont )
deClassFontBuilder::nfSetIsColorFont::nfSetIsColorFont( const sInitData &init ) :
dsFunction( init.clsFontBuilder, "setIsColorFont", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBoolean ); // isColorFont
}
void deClassFontBuilder::nfSetIsColorFont::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFontBuilder_Builder * const builder = ( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetFont() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetFont()->SetIsColorFont( rt->GetValue( 0 )->GetBool() );
}

// protected func void setUndefinedGlyph( int x, int y, int z, int width, int bearing, int advance )
deClassFontBuilder::nfSetUndefinedGlyph::nfSetUndefinedGlyph( const sInitData &init ) :
dsFunction( init.clsFontBuilder, "setUndefinedGlyph", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // x
	p_AddParameter( init.clsInteger ); // y
	p_AddParameter( init.clsInteger ); // z
	p_AddParameter( init.clsInteger ); // width
	p_AddParameter( init.clsInteger ); // bearing
	p_AddParameter( init.clsInteger ); // advance
}
void deClassFontBuilder::nfSetUndefinedGlyph::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFontBuilder_Builder * const builder = ( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetFont() ){
		DSTHROW( dueInvalidAction );
	}
	
	deFontGlyph &glyph = builder->GetFont()->GetUndefinedGlyph();
	glyph.SetX( rt->GetValue( 0 )->GetInt() );
	glyph.SetY( rt->GetValue( 1 )->GetInt() );
	glyph.SetZ( rt->GetValue( 2 )->GetInt() );
	glyph.SetWidth( rt->GetValue( 3 )->GetInt() );
	glyph.SetBearing( rt->GetValue( 4 )->GetInt() );
	glyph.SetAdvance( rt->GetValue( 5 )->GetInt() );
}

// protected func void setGlyphCount( int count )
deClassFontBuilder::nfSetGlyphCount::nfSetGlyphCount( const sInitData &init ) :
dsFunction( init.clsFontBuilder, "setGlyphCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // count
}
void deClassFontBuilder::nfSetGlyphCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFontBuilder_Builder * const builder = ( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetFont() ){
		DSTHROW( dueInvalidAction );
	}
	
	builder->GetFont()->SetGlyphCount( rt->GetValue( 0 )->GetInt() );
}

// protected func void setGlyphAt( int index, int unicode, int x, int y, int z, int width, int bearing, int advance )
deClassFontBuilder::nfSetGlyphAt::nfSetGlyphAt( const sInitData &init ) :
dsFunction( init.clsFontBuilder, "setGlyphAt", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsInteger ); // unicode
	p_AddParameter( init.clsInteger ); // x
	p_AddParameter( init.clsInteger ); // y
	p_AddParameter( init.clsInteger ); // z
	p_AddParameter( init.clsInteger ); // width
	p_AddParameter( init.clsInteger ); // bearing
	p_AddParameter( init.clsInteger ); // advance
}
void deClassFontBuilder::nfSetGlyphAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFontBuilder_Builder * const builder = ( ( sFntBldNatDat* )p_GetNativeData( myself ) )->builder;
	if( ! builder || ! builder->GetFont() ){
		DSTHROW( dueInvalidAction );
	}
	
	deFontGlyph &glyph = builder->GetFont()->GetGlyphAt( rt->GetValue( 0 )->GetInt() );
	glyph.SetUnicode( rt->GetValue( 1 )->GetInt() );
	glyph.SetX( rt->GetValue( 2 )->GetInt() );
	glyph.SetY( rt->GetValue( 3 )->GetInt() );
	glyph.SetZ( rt->GetValue( 4 )->GetInt() );
	glyph.SetWidth( rt->GetValue( 5 )->GetInt() );
	glyph.SetBearing( rt->GetValue( 6 )->GetInt() );
	glyph.SetAdvance( rt->GetValue( 7 )->GetInt() );
}



// Class deClassFontBuilder
/////////////////////////////

// Constructor, destructor
////////////////////////////

deClassFontBuilder::deClassFontBuilder( deScriptingDragonScript &ds ) :
dsClass( "FontBuilder", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_GUI );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sFntBldNatDat ) );
}

deClassFontBuilder::~deClassFontBuilder(){
}



// Management
///////////////

void deClassFontBuilder::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsFontBuilder = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBoolean = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	init.clsFont = pDS.GetClassFont();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfBuild( init ) );
	AddFunction( new nfBuildFont( init ) );
	AddFunction( new nfSetLineHeight( init ) );
	AddFunction( new nfSetIsColorFont( init ) );
	AddFunction( new nfSetUndefinedGlyph( init ) );
	AddFunction( new nfSetGlyphCount( init ) );
	AddFunction( new nfSetGlyphAt( init ) );
}
