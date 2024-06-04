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

#include "deSkinPropertyMapped.h"
#include "deSkinPropertyVisitor.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyMapped
///////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyMapped::deSkinPropertyMapped( const char *type ) :
deSkinProperty( type )
{
	int i;
	for( i=0; i<4; i++ ){
		pComponents[ i ] = -1;
	}
}

deSkinPropertyMapped::~deSkinPropertyMapped(){
}



// Management
///////////////

void deSkinPropertyMapped::SetRed( int mapped ){
	DEASSERT_TRUE( mapped >= -1 )
	pComponents[ 0 ] = mapped;
}

void deSkinPropertyMapped::SetGreen( int mapped ){
	DEASSERT_TRUE( mapped >= -1 )
	pComponents[ 1 ] = mapped;
}

void deSkinPropertyMapped::SetBlue( int mapped ){
	DEASSERT_TRUE( mapped >= -1 )
	pComponents[ 2 ] = mapped;
}

void deSkinPropertyMapped::SetAlpha( int mapped ){
	DEASSERT_TRUE( mapped >= -1 )
	pComponents[ 3 ] = mapped;
}

int deSkinPropertyMapped::GetComponent( int component ) const{
	DEASSERT_TRUE( component >= 0 )
	DEASSERT_TRUE( component <= 3 )
	return pComponents[ component ];
}

void deSkinPropertyMapped::SetComponent( int component, int mapped ){
	DEASSERT_TRUE( component >= 0 )
	DEASSERT_TRUE( component <= 3 )
	DEASSERT_TRUE( mapped >= -1 )
	pComponents[ component ] = mapped;
}



// Visiting
/////////////

void deSkinPropertyMapped::Visit( deSkinPropertyVisitor &visitor ){
	visitor.VisitMapped( *this );
}
