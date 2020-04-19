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

#include "decXmlDocument.h"
#include "decXmlVisitor.h"
#include "visitors/decXmlVisitorCleanCharData.h"
#include "visitors/decXmlVisitorStripComments.h"
#include "../exceptions.h"



// Class decXmlDocument
/////////////////////////

// Constructor, destructor
////////////////////////////

decXmlDocument::decXmlDocument() :
pStandalone( false ){
}

decXmlDocument::~decXmlDocument(){
}



// Management
///////////////

void decXmlDocument::SetEncoding( const char *encoding ){
	pEncoding = encoding;
}

void decXmlDocument::SetDocType( const char *docType ){
	pDocType = docType;
}

void decXmlDocument::SetSystemLiteral( const char *sysLit ){
	pSysLit = sysLit;
}

void decXmlDocument::SetPublicLiteral( const char *pubLit ){
	pPubLit = pubLit;
}

void decXmlDocument::SetStandalone( bool standalone ){
	pStandalone = standalone;
}

decXmlElementTag *decXmlDocument::GetRoot() const{
	const int count = GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		decXmlElement * const element = GetElementAt( i );
		if( element->CanCastToElementTag() ){
			return element->CastToElementTag();
		}
	}
	
	return NULL;
}



void decXmlDocument::Visit( decXmlVisitor &visitor ){
	visitor.VisitDocument( *this );
}



void decXmlDocument::CleanCharData(){
	decXmlVisitorCleanCharData visitor;
	Visit( visitor );
}
void decXmlDocument::StripComments(){
	decXmlVisitorStripComments visitor;
	Visit( visitor );
}



bool decXmlDocument::CanCastToDocument() const{
	return true;
}

decXmlDocument *decXmlDocument::CastToDocument(){
	return this;
}
