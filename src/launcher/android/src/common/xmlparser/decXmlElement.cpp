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

#include "decXmlElement.h"
#include "decXmlVisitor.h"
#include "decXmlContainer.h"
#include "decXmlDocument.h"
#include "decXmlComment.h"
#include "decXmlPI.h"
#include "decXmlElementTag.h"
#include "decXmlCharacterData.h"
#include "decXmlEntityReference.h"
#include "decXmlCharReference.h"
#include "decXmlCDSect.h"
#include "decXmlAttValue.h"
#include "decXmlNamespace.h"
#include "../exceptions.h"



// Class decXmlElement
////////////////////////

// Constructor, destructor
////////////////////////////

decXmlElement::decXmlElement() :
pLineNumber( 1 ),
pPositionNumber( 0 ),
pParent( NULL )
{
}

decXmlElement::~decXmlElement(){
}



// Management
///////////////

void decXmlElement::SetLineNumber( int lineNumber ){
	if( lineNumber < 1 ){
		DETHROW( deeInvalidParam );
	}
	pLineNumber = lineNumber;
}

void decXmlElement::SetPositionNumber( int positionNumber ){
	if( positionNumber < 1 ){
		DETHROW( deeInvalidParam );
	}
	pPositionNumber = positionNumber;
}

void decXmlElement::SetParent( decXmlElement *parent ){
	pParent = parent;
}



// Visiting
/////////////

void decXmlElement::Visit( decXmlVisitor &visitor ){
	visitor.VisitElement( *this );
}



// Casting
////////////

bool decXmlElement::CanCastToContainer() const{
	return false;
}

bool decXmlElement::CanCastToDocument() const{
	return false;
}

bool decXmlElement::CanCastToComment() const{
	return false;
}

bool decXmlElement::CanCastToPI() const{
	return false;
}

bool decXmlElement::CanCastToElementTag() const{
	return false;
}

bool decXmlElement::CanCastToCharacterData() const{
	return false;
}

bool decXmlElement::CanCastToEntityReference() const{
	return false;
}

bool decXmlElement::CanCastToCharReference() const{
	return false;
}

bool decXmlElement::CanCastToCDSect() const{
	return false;
}

bool decXmlElement::CanCastToAttValue() const{
	return false;
}

bool decXmlElement::CanCastToNamespace() const{
	return false;
}

decXmlContainer *decXmlElement::CastToContainer(){
	DETHROW( deeInvalidParam );
}

decXmlDocument *decXmlElement::CastToDocument(){
	DETHROW( deeInvalidParam );
}

decXmlComment *decXmlElement::CastToComment(){
	DETHROW( deeInvalidParam );
}

decXmlPI *decXmlElement::CastToPI(){
	DETHROW( deeInvalidParam );
}

decXmlElementTag *decXmlElement::CastToElementTag(){
	DETHROW( deeInvalidParam );
}

decXmlCharacterData *decXmlElement::CastToCharacterData(){
	DETHROW( deeInvalidParam );
}

decXmlEntityReference *decXmlElement::CastToEntityReference(){
	DETHROW( deeInvalidParam );
}

decXmlCharReference *decXmlElement::CastToCharReference(){
	DETHROW( deeInvalidParam );
}

decXmlCDSect *decXmlElement::CastToCDSect(){
	DETHROW( deeInvalidParam );
}

decXmlAttValue *decXmlElement::CastToAttValue(){
	DETHROW( deeInvalidParam );
}

decXmlNamespace *decXmlElement::CastToNamespace(){
	DETHROW( deeInvalidParam );
}
