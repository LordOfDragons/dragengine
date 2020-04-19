/* 
 * Drag[en]gine Console Launcher
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

#include "declConfigXML.h"
#include "declConfiguration.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>



// Class declConfigXML
////////////////////////

// Constructors and Destructors
/////////////////////////////////

declConfigXML::declConfigXML( deLogger *logger, const char *loggerSource ) : declBaseXML( logger, loggerSource ){
}

declConfigXML::~declConfigXML(){
}



// Management
///////////////

void declConfigXML::ReadFromFile( decBaseFileReader &reader, declConfiguration &config ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "delauncherconsole" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, config );
}

void declConfigXML::WriteToFile( decBaseFileWriter &writer, const declConfiguration &config ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void declConfigXML::pWriteConfig( decXmlWriter &writer, const declConfiguration &config ){
	writer.WriteOpeningTag( "delauncherconsole", false, true );
	
	writer.WriteClosingTag( "delauncherconsole", true );
}



void declConfigXML::pReadConfig( const decXmlElementTag &root, declConfiguration &config ){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "DUMMY" ) == 0 ){
				
			}else{
				pErrorUnknownTag( root, *tag );
			}
		}
	}
}
