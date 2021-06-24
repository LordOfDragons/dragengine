/* 
 * Drag[en]gine IGDE World Editor
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

#include "meConfiguration.h"
#include "meConfigurationXML.h"
#include "../gui/meWindowMain.h"

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



// Class meConfigurationXML
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

meConfigurationXML::meConfigurationXML( deLogger *logger, const char *loggerSource ) : igdeBaseXML( logger, loggerSource ){
}

meConfigurationXML::~meConfigurationXML(){
}



// Management
///////////////

void meConfigurationXML::ReadFromFile( decBaseFileReader &reader, meConfiguration &config ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "worldEditor" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, config );
}

void meConfigurationXML::WriteToFile( decBaseFileWriter &writer, const meConfiguration &config ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void meConfigurationXML::pWriteConfig( decXmlWriter &writer, const meConfiguration &config ){
	writer.WriteOpeningTag( "worldEditor", false, true );
	
	writer.WriteDataTagFloat( "moveStep", config.GetMoveStep() );
	writer.WriteDataTagInt( "moveSnap", config.GetMoveSnap() ? 1 : 0 );
	writer.WriteDataTagFloat( "rotateStep", config.GetRotateStep() );
	writer.WriteDataTagInt( "rotateSnap", config.GetRotateSnap() ? 1 : 0 );
	writer.WriteDataTagFloat( "scaleStep", config.GetScaleStep() );
	writer.WriteDataTagInt( "scaleSnap", config.GetScaleSnap() ? 1 : 0 );
	writer.WriteDataTagFloat( "sensitivity", config.GetSensitivity() );
	writer.WriteDataTagBool( "enableGI", config.GetEnableGI() );
	config.GetWindowMain().GetRecentFiles().WriteToXml( writer );
	
	writer.WriteClosingTag( "worldEditor", true );
}



void meConfigurationXML::pReadConfig( const decXmlElementTag &root, meConfiguration &config ){
	const int count = root.GetElementCount();
	int i;
	
	config.GetWindowMain().GetRecentFiles().RemoveAllFiles();
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "moveStep" ){
			config.SetMoveStep( GetCDataFloat( *tag ) );
			
		}else if( tag->GetName() == "moveSnap" ){
			config.SetMoveSnap( GetCDataInt( *tag ) != 0 );
			
		}else if( tag->GetName() == "rotateStep" ){
			config.SetRotateStep( GetCDataFloat( *tag ) );
			
		}else if( tag->GetName() == "rotateSnap" ){
			config.SetRotateSnap( GetCDataInt( *tag ) != 0 );
			
		}else if( tag->GetName() == "scaleStep" ){
			config.SetScaleStep( GetCDataFloat( *tag ) );
			
		}else if( tag->GetName() == "scaleSnap" ){
			config.SetScaleSnap( GetCDataInt( *tag ) != 0 );
			
		}else if( tag->GetName() == "sensitivity" ){
			config.SetSensitivity( GetCDataFloat( *tag ) );
			
		}else if( tag->GetName() == "enableGI" ){
			config.SetEnableGI( GetCDataBool( *tag ) );
			
		}else if( tag->GetName() == "recentFiles" ){
			config.GetWindowMain().GetRecentFiles().ReadFromXml( *tag );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}
