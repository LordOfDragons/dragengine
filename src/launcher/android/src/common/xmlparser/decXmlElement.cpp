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
