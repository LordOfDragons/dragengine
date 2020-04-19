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

#include "decXmlElementTag.h"
#include "decXmlAttValue.h"
#include "decXmlCDSect.h"
#include "decXmlCharacterData.h"
#include "decXmlVisitor.h"
#include "../exceptions.h"
#include "../string/decStringList.h"



// Class decXmlElementTag
///////////////////////////

// Constructor, destructor
////////////////////////////

decXmlElementTag::decXmlElementTag( const char *name ){
	SetName( name );
}

decXmlElementTag::~decXmlElementTag(){
}



// Management
///////////////

void decXmlElementTag::SetName( const char *name ){
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



decXmlCharacterData *decXmlElementTag::GetFirstData() const{
	const int count = GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		decXmlElement &element = *GetElementAt( i );
		
		if( element.CanCastToCharacterData() ){
			return element.CastToCharacterData();
			
		}else if( element.CanCastToCDSect() ){
			return element.CastToCDSect();
		}
	}
	
	return NULL;
}

decXmlElementTag *decXmlElementTag::GetElementIfTag( int index ) const{
	decXmlElement &element = *GetElementAt( index );
	
	if( element.CanCastToElementTag() ){
		return element.CastToElementTag();
		
	}else{
		return NULL;
	}
}

decXmlAttValue *decXmlElementTag::FindAttribute( const char *name ) const{
	const int count = GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		decXmlElement &element = *GetElementAt( i );
		if( ! element.CanCastToAttValue() ){
			continue;
		}
		
		decXmlAttValue * const attribute = element.CastToAttValue();
		if( attribute->GetName() == name ){
			return attribute;
		}
	}
	
	return NULL;
}



void decXmlElementTag::Visit( decXmlVisitor &visitor ){
	visitor.VisitElementTag( *this );
}


bool decXmlElementTag::CanCastToElementTag() const{
	return true;
}

decXmlElementTag *decXmlElementTag::CastToElementTag(){
	return this;
}
