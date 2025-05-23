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
	DEASSERT_NOTNULL(element)
	
	pElements.Add( element );
	element->SetParent( this );
}

void decXmlContainer::InsertElement(decXmlElement *element, int beforeIndex){
	DEASSERT_NOTNULL(element)
	
	pElements.Insert(element, beforeIndex);
	element->SetParent(this);
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
