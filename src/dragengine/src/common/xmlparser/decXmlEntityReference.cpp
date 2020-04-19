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

#include "decXmlEntityReference.h"
#include "decXmlVisitor.h"
#include "../exceptions.h"



// class decXmlEntityReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

decXmlEntityReference::decXmlEntityReference( const char *name ) :
pName( name ){
}

decXmlEntityReference::~decXmlEntityReference(){
}



// Management
///////////////

void decXmlEntityReference::SetName( const char *name ){
	pName = name;
}



void decXmlEntityReference::Visit( decXmlVisitor &visitor ){
	visitor.VisitEntityReference( *this );
}

bool decXmlEntityReference::CanCastToEntityReference() const{
	return true;
}

decXmlEntityReference *decXmlEntityReference::CastToEntityReference(){
	return this;
}
