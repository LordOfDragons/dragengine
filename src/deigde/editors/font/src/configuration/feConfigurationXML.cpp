/* 
 * Drag[en]gine IGDE Font Editor
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

#include "feConfiguration.h"
#include "feConfigurationXML.h"
#include "../gui/feWindowMain.h"

#include <dragengine/logger/deLogger.h>
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
#include <dragengine/common/exceptions.h>



// Class feConfigurationXML
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

feConfigurationXML::feConfigurationXML( deLogger *logger, const char *loggerSource ) : igdeBaseXML( logger, loggerSource ){
}

feConfigurationXML::~feConfigurationXML(){
}



// Management
///////////////

void feConfigurationXML::ReadFromFile( decBaseFileReader &reader, feConfiguration &config ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "fontEditor" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, config );
}

void feConfigurationXML::WriteToFile( decBaseFileWriter &writer, const feConfiguration &config ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void feConfigurationXML::pWriteConfig( decXmlWriter &writer, const feConfiguration &config ){
	writer.WriteOpeningTag( "fontEditor", false, true );
	
	config.GetWindowMain().GetRecentFiles().WriteToXml( writer );
	
	writer.WriteClosingTag( "fontEditor", true );
}



void feConfigurationXML::pReadConfig( const decXmlElementTag &root, feConfiguration &config ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "recentFiles" ){
			config.GetWindowMain().GetRecentFiles().ReadFromXml( *tag );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}
