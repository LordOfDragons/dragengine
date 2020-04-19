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

#include "igdeConfigurationLocal.h"
#include "igdeConfigurationLocalXML.h"
#include "../gui/igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../module/igdeEditorModuleDefinition.h"
#include "../module/igdeEditorModuleManager.h"

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



// Class igdeConfigurationLocalXML
////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

igdeConfigurationLocalXML::igdeConfigurationLocalXML( deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ){
}

igdeConfigurationLocalXML::~igdeConfigurationLocalXML(){
}



// Management
///////////////

void igdeConfigurationLocalXML::ReadFromFile( decBaseFileReader &reader, igdeConfigurationLocal &config ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser parser( GetLogger() );
	parser.ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "deigde" ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, config );
}

void igdeConfigurationLocalXML::WriteToFile( decBaseFileWriter &writer, const igdeConfigurationLocal &config ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void igdeConfigurationLocalXML::pReadConfig( const decXmlElementTag &root, igdeConfigurationLocal &config ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "recentEditorFiles" ){
			config.GetRecentEditorFiles().ReadFromXml( *tag );
			
		}else if( tag->GetName() == "recentEditors" ){
			pReadRecentEditors( *tag, config );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void igdeConfigurationLocalXML::pReadRecentEditors( const decXmlElementTag &root, igdeConfigurationLocal &config ){
	igdeEditorModuleManager &moduleManger = config.GetWindowMain().GetModuleManager();
	const int count = root.GetElementCount();
	int position = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "editor" ){
			igdeEditorModuleDefinition * const module = moduleManger.GetModuleWithID ( GetCDataString( *tag ) );
			if( module ){
				moduleManger.ChangeModuleRecentUsedPosition( module, position++ );
			}
			
		}else{
			DETHROW( deeInvalidParam );
		}
	}
}



void igdeConfigurationLocalXML::pWriteConfig( decXmlWriter &writer, const igdeConfigurationLocal &config ){
	writer.WriteOpeningTag( "deigde", false, true );
	
	config.GetRecentEditorFiles().WriteToXml( writer, "recentEditorFiles" );
	pWriteRecentEditors( writer, config );
	
	writer.WriteClosingTag( "deigde", true );
}

void igdeConfigurationLocalXML::pWriteRecentEditors( decXmlWriter &writer, const igdeConfigurationLocal &config ){
	igdeEditorModuleManager &moduleManger = config.GetWindowMain().GetModuleManager();
	const int count = moduleManger.GetModuleCount();
	int i;
	
	writer.WriteOpeningTag( "recentEditors" );
	
	for( i=0; i<count; i++ ){
		writer.WriteDataTagString( "editor", moduleManger.GetRecentModuleAt( i )->GetID() );
	}
	
	writer.WriteClosingTag( "recentEditors" );
}
