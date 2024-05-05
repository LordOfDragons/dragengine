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

#include "deSkinPropertyNodeText.h"
#include "deSkinPropertyNodeVisitor.h"
#include "../../../font/deFont.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeText
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeText::deSkinPropertyNodeText() :
pFontSize( 10.0f )
{
	int i;
	for( i=0; i<TextMappedCount; i++ ){
		pTextMapped[ i ] = -1;
	}
}

deSkinPropertyNodeText::~deSkinPropertyNodeText(){
}



// Management
///////////////

void deSkinPropertyNodeText::SetPath( const char *path ){
	pPath = path;
}

void deSkinPropertyNodeText::SetFont( deFont *font ){
	pFont = font;
}

void deSkinPropertyNodeText::SetFontSize( float size ){
	pFontSize = decMath::max( size, 0.0f );
}

void deSkinPropertyNodeText::SetText( const char *text ){
	pText = text;
}

void deSkinPropertyNodeText::SetColor( const decColor &color ){
	pColor = color;
}



int deSkinPropertyNodeText::GetTextMappedFor( eTextMapped mapped ) const{
	return pTextMapped[ mapped ];
}

void deSkinPropertyNodeText::SetTextMappedFor( eTextMapped mapped, int index ){
	DEASSERT_TRUE( index >= -1 )
	pTextMapped[ mapped ] = index;
}



// Visiting
/////////////

void deSkinPropertyNodeText::Visit( deSkinPropertyNodeVisitor &visitor ){
	visitor.VisitText( *this );
}
