/* 
 * Drag[en]gine Android Launcher
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

#include "decXmlCDSect.h"
#include "decXmlVisitor.h"
#include "../exceptions.h"



// Class decXmlCDSect
////////////////////////

// Constructor, destructor
////////////////////////////

decXmlCDSect::decXmlCDSect( const char *data ) :
decXmlCharacterData( data ){
}

decXmlCDSect::~decXmlCDSect(){
}



// Management
void decXmlCDSect::Visit( decXmlVisitor &visitor ){
	visitor.VisitCDSect( *this );
}

bool decXmlCDSect::CanCastToCDSect() const{
	return true;
}

decXmlCDSect *decXmlCDSect::CastToCDSect(){
	return this;
}
