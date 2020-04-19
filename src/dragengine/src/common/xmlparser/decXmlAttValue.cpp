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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decXmlAttValue.h"
#include "decXmlVisitor.h"
#include "../exceptions.h"
#include "../string/decStringList.h"



// Class decXmlAttValue
/////////////////////////

// Constructor, destructor
////////////////////////////

decXmlAttValue::decXmlAttValue( const char *name ){
	SetName( name );
}

decXmlAttValue::~decXmlAttValue(){
}



// Management
///////////////

void decXmlAttValue::SetName( const char *name ){
	pName = name;
	
	const decStringList parts( pName.Split( ':' ) );
	
	if( parts.GetCount() == 2 ){
		pNamespace = parts.GetAt( 0 );
		pLocalName = parts.GetAt( 1 );
		
	}else if( parts.GetCount() < 2 ){
		pNamespace.Empty();
		pLocalName = pName;
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

void decXmlAttValue::SetValue( const char *value ){
	pValue = value;
}



void decXmlAttValue::Visit( decXmlVisitor &visitor ){
	visitor.VisitAttValue( *this );
}



bool decXmlAttValue::CanCastToAttValue() const{
	return true;
}

decXmlAttValue *decXmlAttValue::CastToAttValue(){
	return this;
}
