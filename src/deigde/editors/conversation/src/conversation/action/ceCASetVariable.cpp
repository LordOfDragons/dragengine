/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceCASetVariable.h"

#include <dragengine/common/exceptions.h>



// Class ceCASetVariable
//////////////////////////

// Constructor, destructor
////////////////////////////

ceCASetVariable::ceCASetVariable() :
ceConversationAction( eatSetVariable ),
pName( "Variable" ),
pOperator( eopSet ),
pValue( 0 ){
}

ceCASetVariable::ceCASetVariable( const ceCASetVariable &action ) :
ceConversationAction( action ),
pName( action.pName ),
pOperator( action.pOperator ),
pValue( action.pValue ),
pValueVariable( action.pValueVariable ){
}

ceCASetVariable::~ceCASetVariable(){
}



// Management
///////////////

void ceCASetVariable::SetName( const char *name ){
	pName = name;
}

void ceCASetVariable::SetOperator( eOperators aOperator ){
	if( aOperator < eopSet || aOperator > eopRandom ){
		DETHROW( deeInvalidParam );
	}
	pOperator = aOperator;
}

void ceCASetVariable::SetValue( int value ){
	pValue = value;
}

void ceCASetVariable::SetValueVariable( const char *value ){
	pValueVariable = value;
}



ceConversationAction *ceCASetVariable::CreateCopy() const{
	return new ceCASetVariable( *this );
}
