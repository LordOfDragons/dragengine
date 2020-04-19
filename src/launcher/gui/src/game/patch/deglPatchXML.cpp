/* 
 * Drag[en]gine GUI Launcher
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

#include "deglPatch.h"
#include "deglPatchXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class deglPatchXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

deglPatchXML::deglPatchXML( deLogger *logger, const char *loggerSource ) :
deglBaseXML( logger, loggerSource ){
}

deglPatchXML::~deglPatchXML(){
}



// Management
///////////////

void deglPatchXML::ReadFromFile( decBaseFileReader &reader, deglPatch &patch ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "depatch" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadPatch( *root, patch );
}



// Private Functions
//////////////////////

void deglPatchXML::pReadPatch( const decXmlElementTag &root, deglPatch &patch ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "identifier" ){
			patch.SetIdentifier( decUuid( pGetCDataString( *tag ), false ) );
			
		}else if( tagName == "name" ){
			patch.SetName( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
			
		}else if( tagName == "description" ){
			patch.SetDescription( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
			
		}else if( tagName == "creator" ){
			patch.SetCreator( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
			
		}else if( tagName == "homepage" ){
			patch.SetHomepage( pGetCDataString( *tag ) );
			
		}else if( tagName == "patchDir" ){
			patch.SetPatchDirectory( pGetCDataString( *tag ) );
			
		}else if( tagName == "dataDir" ){
			patch.SetDataDirectory( pGetCDataString( *tag ) );
			
		}else if( tagName == "gameId" ){
			patch.SetGameID( decUuid( pGetCDataString( *tag ), false ) );
			
		}else if( tagName == "requiredPatch" ){
			patch.GetRequiredPatches().Add( decUuid( pGetCDataString( *tag ), false ) );
			
		}else{
			pErrorUnknownTag( root, *tag );
		}
	}
	
	if( ! patch.GetIdentifier() ){
		pErrorMissingTag( root, "identifier" );
		DETHROW( deeInvalidParam );
	}
	if( ! patch.GetGameID() ){
		pErrorMissingTag( root, "gameId" );
		DETHROW( deeInvalidParam );
	}
}
