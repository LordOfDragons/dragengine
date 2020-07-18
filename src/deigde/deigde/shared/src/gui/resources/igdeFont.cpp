/* 
 * Drag[en]gine IGDE
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
#include <string.h>

#include "igdeFont.h"
#include "../native/toolkit.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Struct igdeFont.sConfiguration
///////////////////////////////////

igdeFont::sConfiguration::sConfiguration() :
size( 10.0f ),
bold( false ),
italic( false ),
underline( false ),
strikeThrough( false ){
}

igdeFont::sConfiguration::sConfiguration( const igdeFont::sConfiguration &config ) :
name( config.name ),
size( config.size ),
bold( config.bold ),
italic( config.italic ),
underline( config.underline ),
strikeThrough( config.strikeThrough ){
}

igdeFont::sConfiguration &igdeFont::sConfiguration::operator=( const igdeFont::sConfiguration &config ){
	name = config.name;
	size = config.size;
	bold = config.bold;
	italic = config.italic;
	underline = config.underline;
	strikeThrough = config.strikeThrough;
	return *this;
}

bool igdeFont::sConfiguration::operator==( const sConfiguration &config ) const{
	return name == config.name
		&& fabsf( size - config.size ) <= 0.01f
		&& bold == config.bold
		&& italic == config.italic
		&& underline == config.underline
		&& strikeThrough == config.strikeThrough;
}



// Class igdeFont
///////////////////

// Constructor, destructor
////////////////////////////

igdeFont::igdeFont( igdeEnvironment &environment, const sConfiguration &config ) :
pEnvironment( environment ),
pNativeFont( NULL ),
pName( config.name ),
pSize( config.size ),
pBold( config.bold ),
pItalic( config.italic ),
pUnderline( config.underline ),
pStrikeThrough( config.strikeThrough )
{
	if( config.size <= 0.0f ){
		DETHROW( deeInvalidParam );
	}
	
	pNativeFont = igdeNativeFont::CreateNativeFont( *this );
}

igdeFont::~igdeFont(){
	if( pNativeFont ){
		( ( igdeNativeFont* )pNativeFont )->DestroyNativeFont();
	}
}



// Management
///////////////

void igdeFont::GetConfig( sConfiguration &config ) const{
	config.name = pName;
	config.size = pSize;
	config.bold = pBold;
	config.italic = pItalic;
	config.underline = pUnderline;
	config.strikeThrough = pStrikeThrough;
}

decPoint igdeFont::TextSize( const char *text ) const{
	if( ! text ){
		DETHROW( deeInvalidParam );
	}
	return ( ( igdeNativeFont* )pNativeFont )->TextSize( text );
}

deFont *igdeFont::GetEngineFont(){
	if( ! pEngineFont ){
		pEngineFont.TakeOver( ( ( igdeNativeFont* )pNativeFont )->CreateEngineFont() );
	}
	return pEngineFont;
}
