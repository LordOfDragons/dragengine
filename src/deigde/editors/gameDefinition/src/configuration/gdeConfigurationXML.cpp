/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeConfiguration.h"
#include "gdeConfigurationXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class gdeConfigurationXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

gdeConfigurationXML::gdeConfigurationXML( deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ){
}

gdeConfigurationXML::~gdeConfigurationXML(){
}



// Management
///////////////

void gdeConfigurationXML::ReadFromFile( decBaseFileReader &reader, gdeConfiguration &config ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "gameDefinitionEditor" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, config );
}

void gdeConfigurationXML::WriteToFile( decBaseFileWriter &writer, const gdeConfiguration &config ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void gdeConfigurationXML::pWriteConfig( decXmlWriter &writer, const gdeConfiguration &config ){
	writer.WriteOpeningTag( "gameDefinitionEditor", false, true );
	
	writer.WriteClosingTag( "gameDefinitionEditor", true );
}



void gdeConfigurationXML::pReadConfig( const decXmlElementTag &root, gdeConfiguration &config ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "example" ) == 0 ){
			//config.SetEXAMPLE( GetCDataFloat( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}
