/* 
 * Drag[en]gine IGDE
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

#include "igdeConfiguration.h"
#include "igdeConfigurationXML.h"
#include "../gui/igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/preview/igdeGDPreviewManager.h>

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



// Class igdeConfigurationXML
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

igdeConfigurationXML::igdeConfigurationXML( deLogger *logger, const char *loggerSource ) : igdeBaseXML( logger, loggerSource ){
}

igdeConfigurationXML::~igdeConfigurationXML(){
}



// Management
///////////////

void igdeConfigurationXML::ReadFromFile( decBaseFileReader &reader, igdeConfiguration &config ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser parser( GetLogger() );
	parser.ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "deigde" ) != 0 ) DETHROW( deeInvalidParam );
	
	pReadConfig( *root, config );
}

void igdeConfigurationXML::WriteToFile( decBaseFileWriter &writer, const igdeConfiguration &config ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void igdeConfigurationXML::pReadConfig( const decXmlElementTag &root, igdeConfiguration &config ){
	decStringList &recentProjects = config.GetRecentProjectList();
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "windowMain" ){
			pReadWindowMain( *tag, config.GetWindowMain() );
			
		}else if( tag->GetName() == "maxRecentProjectEntries" ){
			config.SetMaxRecentProjectEntries( GetCDataInt( *tag ) );
			
		}else if( tag->GetName() == "recentProject" ){
			recentProjects.Add( GetCDataString( *tag ) );
		}
	}
	
	while( recentProjects.GetCount() > config.GetMaxRecentProjectEntries() ){
		recentProjects.RemoveFrom( recentProjects.GetCount() - 1 );
	}
}

void igdeConfigurationXML::pReadWindowMain( const decXmlElementTag &root, igdeWindowMain &window ){
	const int count = root.GetElementCount();
	decPoint position, size;
	bool hasPosition = false;
	bool hasSize = false;
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "x" ){
			position.x = GetCDataInt( *tag );
			hasPosition = true;
			
		}else if( tag->GetName() == "y" ){
			position.y = GetCDataInt( *tag );
			hasPosition = true;
			
		}else if( tag->GetName() == "width" ){
			size.x = GetCDataInt( *tag );
			hasSize = true;
			
		}else if( tag->GetName() == "height" ){
			size.y = GetCDataInt( *tag );
			hasSize = true;
		}
	}
	
	if( hasPosition ){
		window.SetPosition( position );
	}
	if( hasSize ){
		window.SetSize( size );
	}
}



void igdeConfigurationXML::pWriteConfig( decXmlWriter &writer, const igdeConfiguration &config ){
	writer.WriteOpeningTag( "deigde", false, true );
	
	pWriteWindowMain( writer, config.GetWindowMain(), "windowMain" );
	
	writer.WriteDataTagInt( "maxRecentProjectEntries", config.GetMaxRecentProjectEntries() );
	const decStringList &recentProjectList = config.GetRecentProjectList();
	const int recentProjectCount = recentProjectList.GetCount();
	int i;
	for( i=0; i<recentProjectCount; i++ ){
		writer.WriteDataTagString( "recentProject", recentProjectList.GetAt( i ).GetString() );
	}
	
	writer.WriteClosingTag( "deigde", true );
}

void igdeConfigurationXML::pWriteWindowMain( decXmlWriter &writer, const igdeWindowMain &window, const char *tagName ){
	writer.WriteOpeningTag( tagName, false, true );
	
	writer.WriteDataTagInt( "x", window.GetPosition().x );
	writer.WriteDataTagInt( "y", window.GetPosition().y );
	writer.WriteDataTagInt( "width", window.GetSize().x );
	writer.WriteDataTagInt( "height", window.GetSize().y );
	
	writer.WriteClosingTag( tagName, true );
}
