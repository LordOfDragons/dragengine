/* 
 * Drag[en]gine Game Engine
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decXmlCharReference.h"
#include "decXmlVisitor.h"
#include "../exceptions.h"



// Class decXmlCharReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

decXmlCharReference::decXmlCharReference( const char *data, eRadices radix ) :
pData( data ),
pRadix( erDecimal ){
	SetRadix( radix );
}

decXmlCharReference::~decXmlCharReference(){
}



// Management
///////////////

void decXmlCharReference::SetData( const char *data ){
	pData = data;
}

void decXmlCharReference::SetRadix( eRadices radix ){
	if( radix < erDecimal || radix > erHexadecimal ){
		DETHROW( deeInvalidParam );
	}
	pRadix = radix;
}



void decXmlCharReference::Visit( decXmlVisitor &visitor ){
	visitor.VisitCharReference( *this );
}

bool decXmlCharReference::CanCastToCharReference() const{
	return true;
}

decXmlCharReference *decXmlCharReference::CastToCharReference(){
	return this;
}
