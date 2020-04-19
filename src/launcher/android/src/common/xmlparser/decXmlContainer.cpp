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

#include <stdlib.h>

#include "decXmlContainer.h"
#include "decXmlElement.h"
#include "decXmlVisitor.h"
#include "../exceptions.h"



// Class decXmlContainer
//////////////////////////

// Constructor, destructor
////////////////////////////

decXmlContainer::decXmlContainer(){
}

decXmlContainer::~decXmlContainer(){
	RemoveAllElements();
}



// Management
///////////////

int decXmlContainer::GetElementCount() const{
	return pElements.GetCount();
}

bool decXmlContainer::IsEmpty() const{
	return pElements.GetCount() == 0;
}

decXmlElement *decXmlContainer::GetElementAt( int index ) const{
	return ( decXmlElement* )pElements.GetAt( index );
}

void decXmlContainer::AddElement( decXmlElement *element ){
	if( ! element ){
		DETHROW( deeInvalidParam );
	}
	
	pElements.Add( element );
	element->SetParent( this );
}

void decXmlContainer::RemoveElement( decXmlElement *element ){
	if( ! pElements.Has( element ) ){
		DETHROW( deeInvalidParam );
	}
	
	element->SetParent( NULL );
	pElements.Remove( element );
}

void decXmlContainer::RemoveAllElements(){
	const int count = pElements.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( decXmlElement* )pElements.GetAt( i ) )->SetParent( NULL );
	}
	pElements.RemoveAll();
}

int decXmlContainer::IndexOfElement( decXmlElement *element ){
	return pElements.IndexOf( element );
}

bool decXmlContainer::HasElement( decXmlElement *element ){
	return pElements.Has( element );
}

void decXmlContainer::VisitElements( decXmlVisitor &visitor ){
	const int count = pElements.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( decXmlElement* )pElements.GetAt( i ) )->Visit( visitor );
	}
}



void decXmlContainer::Visit( decXmlVisitor &visitor ){
	visitor.VisitContainer( *this );
}



bool decXmlContainer::CanCastToContainer() const{
	return true;
}

decXmlContainer *decXmlContainer::CastToContainer(){
	return this;
}
