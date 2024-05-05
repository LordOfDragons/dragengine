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

#include "deSkinPropertyConstructed.h"
#include "deSkinPropertyVisitor.h"
#include "node/deSkinPropertyNodeGroup.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyConstructed
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyConstructed::deSkinPropertyConstructed( const char *type ) :
deSkinProperty( type ),
pContent( NULL ),
pTileX( false ),
pTileY( false ),
pBitCount( 8 )
{
	pContent = new deSkinPropertyNodeGroup;
	pContent->SetSize( decVector( 256.0f, 256.0f, 1.0f ) );
}

deSkinPropertyConstructed::~deSkinPropertyConstructed(){
	if( pContent ){
		delete pContent;
	}
}



// Management
///////////////

void deSkinPropertyConstructed::SetColor( const decColor &color ){
	pColor = color;
}

void deSkinPropertyConstructed::SetTileX( bool tileX ){
	pTileX = tileX;
}

void deSkinPropertyConstructed::SetTileY( bool tileY ){
	pTileY = tileY;
}

void deSkinPropertyConstructed::SetBitCount( int bitCount ){
	switch( bitCount ){
	case 8:
	case 16:
	case 32:
		pBitCount = bitCount;
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



// Visiting
/////////////

void deSkinPropertyConstructed::Visit( deSkinPropertyVisitor &visitor ){
	visitor.VisitConstructed( *this );
}
