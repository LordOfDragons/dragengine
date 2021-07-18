/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delPatch.h"
#include "delPatchXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class delPatchXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

delPatchXML::delPatchXML( deLogger *logger, const char *loggerSource ) :
delBaseXML( logger, loggerSource ){
}

delPatchXML::~delPatchXML(){
}



// Management
///////////////

void delPatchXML::ReadFromFile( decBaseFileReader &reader, delPatch &patch ){
	const decXmlDocument::Ref xmlDoc( decXmlDocument::Ref::With( new decXmlDocument ) );
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "depatch" ){
		DETHROW_INFO( deeInvalidParam, "missing root tag 'depatch'" );
	}
	
	pReadPatch( *root, patch );
}



// Private Functions
//////////////////////

void delPatchXML::pReadPatch( const decXmlElementTag &root, delPatch &patch ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "identifier" ){
			patch.SetIdentifier( decUuid( GetCDataString( *tag ), false ) );
			
		}else if( tagName == "name" ){
			patch.SetName( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( tagName == "description" ){
			patch.SetDescription( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( tagName == "creator" ){
			patch.SetCreator( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( tagName == "homepage" ){
			patch.SetHomepage( GetCDataString( *tag ) );
			
		}else if( tagName == "patchDir" ){
			patch.SetPatchDirectory( GetCDataString( *tag ) );
			
		}else if( tagName == "dataDir" ){
			patch.SetDataDirectory( GetCDataString( *tag ) );
			
		}else if( tagName == "gameId" ){
			patch.SetGameID( decUuid( GetCDataString( *tag ), false ) );
			
		}else if( tagName == "requiredPatch" ){
			patch.GetRequiredPatches().Add( decUuid( GetCDataString( *tag ), false ) );
			
		}else{
			ErrorUnknownTag( root, *tag );
		}
	}
	
	if( ! patch.GetIdentifier() ){
		ErrorMissingTag( root, "identifier" );
		DETHROW_INFO( deeInvalidParam, "missing tag");
	}
	if( ! patch.GetGameID() ){
		ErrorMissingTag( root, "gameId" );
		DETHROW_INFO( deeInvalidParam, "missing tag" );
	}
}
