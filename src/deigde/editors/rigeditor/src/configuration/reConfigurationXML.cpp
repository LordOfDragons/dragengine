/* 
 * Drag[en]gine IGDE Rig Editor
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

#include "reConfiguration.h"
#include "reConfigurationXML.h"
#include "../gui/reWindowMain.h"

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



// Class reConfigurationXML
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

reConfigurationXML::reConfigurationXML( deLogger *logger, const char *loggerSource ) : igdeBaseXML( logger, loggerSource ){
}

reConfigurationXML::~reConfigurationXML(){
}



// Management
///////////////

void reConfigurationXML::ReadFromFile( decBaseFileReader &reader, reConfiguration &config ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "rigEditor" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, config );
}

void reConfigurationXML::WriteToFile( decBaseFileWriter &writer, const reConfiguration &config ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void reConfigurationXML::pWriteConfig( decXmlWriter &writer, const reConfiguration &config ){
	writer.WriteOpeningTag( "rigEditor", false, true );
	
	writer.WriteDataTagFloat( "gridSize", config.GetGridSize() );
	writer.WriteDataTagInt( "snapToGrid", config.GetSnapToGrid() ? 1 : 0 );
	writer.WriteDataTagFloat( "sensitivity", config.GetSensitivity() );
	writer.WriteDataTagInt( "rotationSnapAngle", config.GetRotSnapAngle() ? 1 : 0 );
	
	config.GetWindowMain().GetRecentFiles().WriteToXml( writer );
	
	writer.WriteClosingTag( "rigEditor", true );
}



void reConfigurationXML::pReadConfig( const decXmlElementTag &root, reConfiguration &config ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "gridSize" ){
			config.SetGridSize( GetCDataFloat( *tag ) );
			
		}else if( tag->GetName() == "snapToGrid" ){
			config.SetSnapToGrid( GetCDataInt( *tag ) != 0 );
			
		}else if( tag->GetName() == "sensitivity" ){
			config.SetSensitivity( GetCDataFloat( *tag ) );
			
		}else if( tag->GetName() == "rotationSnapAngle" ){
			config.SetRotSnapAngle( GetCDataFloat( *tag ) );
			
		}else if( tag->GetName() == "recentFiles" ){
			config.GetWindowMain().GetRecentFiles().ReadFromXml( *tag );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}
