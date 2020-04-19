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
