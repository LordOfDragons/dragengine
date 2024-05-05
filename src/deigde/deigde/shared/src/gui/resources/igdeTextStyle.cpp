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
