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

#include "igdeXMLGameProject.h"
#include "igdeGameProject.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>



// Definitions
////////////////

#define LOGGING_NAME "XMLGameProject"



// Class igdeXMLGameProject
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeXMLGameProject::igdeXMLGameProject( deLogger *logger ) :
igdeBaseXML( logger, LOGGING_NAME ){
}

igdeXMLGameProject::~igdeXMLGameProject(){
}



// Loading and Saving
///////////////////////

void igdeXMLGameProject::Load( decBaseFileReader &reader, igdeGameProject &project ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser parser( GetLogger() );
	parser.ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "gameProject" ){
		DETHROW( deeInvalidParam );
	}
	
	pReadProject( *root, project );
}

void igdeXMLGameProject::Save( decBaseFileWriter &writer, const igdeGameProject &project ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	pWriteProject( xmlWriter, project );
}



// Private Functions
//////////////////////

void igdeXMLGameProject::pReadProject( const decXmlElementTag &root, igdeGameProject &project ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		
		if( tag ){
			if( strcmp( tag->GetName(), "name" ) == 0 ){
				project.SetName( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "description" ) == 0 ){
				project.SetDescription( ReadMultilineString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "dataDirectory" ) == 0 ){
				project.SetPathData( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "cacheDirectory" ) == 0 ){
				project.SetPathCache( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "baseGameDefinition" ) == 0 ){
				project.GetBaseGameDefinitionIDList().Add( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "projectGameDefinition" ) == 0 ){
				project.SetPathProjectGameDefinition( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "scriptModule" ) == 0 ){
				project.SetScriptModule( GetCDataString( *tag ) );
				if( HasAttribute( *tag, "version" ) ){
					project.SetScriptModuleVersion( GetAttributeString( *tag, "version" ) );
				}
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
	}
}



void igdeXMLGameProject::pWriteProject( decXmlWriter &writer, const igdeGameProject &project ){
	writer.WriteOpeningTag( "gameProject" );
	
	writer.WriteDataTagString( "name", project.GetName() );
	WriteMultilineString( writer, "description", project.GetDescription() );
	writer.WriteDataTagString( "dataDirectory", project.GetPathData() );
	writer.WriteDataTagString( "cacheDirectory", project.GetPathCache() );
	
	const decStringList &baseGameDefList = project.GetBaseGameDefinitionIDList();
	const int baseGameDefCount = baseGameDefList.GetCount();
	int i;
	
	for( i=0; i<baseGameDefCount; i++ ){
		writer.WriteDataTagString( "baseGameDefinition", baseGameDefList.GetAt( i ) );
	}
	
	writer.WriteDataTagString( "projectGameDefinition", project.GetPathProjectGameDefinition() );
	
	writer.WriteOpeningTagStart( "scriptModule" );
	if( ! project.GetScriptModuleVersion().IsEmpty() ){
		writer.WriteAttributeString( "version", project.GetScriptModuleVersion() );
	}
	writer.WriteOpeningTagEnd( false, false );
	writer.WriteTextString( project.GetScriptModule() );
	writer.WriteClosingTag( "scriptModule", false );
	
	writer.WriteClosingTag( "gameProject" );
}
