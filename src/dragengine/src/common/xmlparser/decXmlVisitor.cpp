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

#include "decXmlVisitor.h"
#include "decXmlElement.h"
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



// Class decXmlVisitor
////////////////////////

// Constructor, destructor
////////////////////////////

decXmlVisitor::decXmlVisitor(){
}

decXmlVisitor::~decXmlVisitor(){
}



// Visiting
/////////////

void decXmlVisitor::VisitElement( decXmlElement &element ){
}

void decXmlVisitor::VisitContainer( decXmlContainer &container ){
	VisitElement( container );
}

void decXmlVisitor::VisitDocument( decXmlDocument &document ){
	VisitContainer( document );
}

void decXmlVisitor::VisitComment( decXmlComment &comment ){
	VisitElement( comment );
}

void decXmlVisitor::VisitPI( decXmlPI &pi ){
	VisitElement( pi );
}

void decXmlVisitor::VisitElementTag( decXmlElementTag &tag ){
	VisitContainer( tag );
}

void decXmlVisitor::VisitCharacterData( decXmlCharacterData &data ){
	VisitElement( data );
}

void decXmlVisitor::VisitEntityReference( decXmlEntityReference &ref ){
	VisitElement( ref );
}

void decXmlVisitor::VisitCharReference( decXmlCharReference &ref ){
	VisitElement( ref );
}

void decXmlVisitor::VisitCDSect( decXmlCDSect &cdsect ){
	VisitElement( cdsect );
}

void decXmlVisitor::VisitAttValue( decXmlAttValue &value ){
	VisitElement( value );
}

void decXmlVisitor::VisitNamespace( decXmlNamespace &ns ){
	VisitElement( ns );
}
