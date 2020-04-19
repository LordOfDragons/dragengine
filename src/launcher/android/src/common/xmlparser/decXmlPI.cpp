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

#include "decXmlPI.h"
#include "decXmlVisitor.h"
#include "../exceptions.h"



// Class decXmlPI
///////////////////

// Constructor, destructor
////////////////////////////

decXmlPI::decXmlPI( const char *target ) :
pTarget( target ){
}

decXmlPI::~decXmlPI(){
}



// Management
///////////////

void decXmlPI::SetTarget( const char *target ){
	pTarget = target;
}

void decXmlPI::SetCommand( const char *command ){
	pCommand = command;
}



void decXmlPI::Visit( decXmlVisitor &visitor ){
	visitor.VisitPI( *this );
}

bool decXmlPI::CanCastToPI() const{
	return true;
}

decXmlPI *decXmlPI::CastToPI(){
	return this;
}
