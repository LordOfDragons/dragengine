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

#include "igdeTextStyle.h"

#include <dragengine/common/exceptions.h>



// Class igdeTextStyle
/////////////////////

// Constructor, destructor
////////////////////////////

igdeTextStyle::igdeTextStyle( const char *name ) :
pName( name ),
pColor( 0.0f, 0.0f, 0.0f ),
pBgColor( 1.0f, 1.0f, 1.0f ),
pHasColor( false ),
pHasBgColor( false ),
pBold( false ),
pItalic( false ),
pUnderline( false ),
pStrikeThrough( false ){
}

igdeTextStyle::igdeTextStyle( const char *name, const igdeTextStyle &textStyle ) :
pName( name ),
pColor( textStyle.pColor ),
pBgColor( textStyle.pBgColor ),
pHasColor( textStyle.pHasColor ),
pHasBgColor( textStyle.pHasBgColor ),
pBold( textStyle.pBold ),
pItalic( textStyle.pItalic ),
pUnderline( textStyle.pUnderline ),
pStrikeThrough( textStyle.pStrikeThrough ){
}

igdeTextStyle::~igdeTextStyle(){
}



// Management
///////////////

void igdeTextStyle::SetColor( const decColor &color ){
	pColor = color;
	pHasColor = true;
}

void igdeTextStyle::ClearColor(){
	pHasColor = false;
}

void igdeTextStyle::SetBgColor( const decColor &color ){
	pBgColor = color;
	pHasBgColor = true;
}

void igdeTextStyle::ClearBgColor(){
	pHasBgColor = false;
}

void igdeTextStyle::SetBold( bool bold ){
	pBold = bold;
}

void igdeTextStyle::SetItalic( bool italic ){
	pItalic = italic;
}

void igdeTextStyle::SetUnderline( bool underline ){
	pUnderline = underline;
}

void igdeTextStyle::SetStrikeThrough( bool strikeThrough ){
	pStrikeThrough = strikeThrough;
}



// Operators
//////////////

igdeTextStyle &igdeTextStyle::operator=( const igdeTextStyle &textStyle ){
	pColor = textStyle.pColor;
	pBgColor = textStyle.pBgColor;
	pHasColor = textStyle.pHasColor;
	pHasBgColor = textStyle.pHasBgColor;
	pBold = textStyle.pBold;
	pItalic = textStyle.pItalic;
	pUnderline = textStyle.pUnderline;
	pStrikeThrough = textStyle.pStrikeThrough;
	return *this;
}
