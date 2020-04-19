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

#include "igdeLoadTemplate.h"
#include "../template/igdeTemplate.h"
#include "../template/igdeTemplateFile.h"
#include "../template/igdeTemplateReplace.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>



// Class igdeLoadTemplate
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoadTemplate::igdeLoadTemplate( deLogger *logger ) :
igdeBaseXML( logger, "IGDEProjectTemplate" ){
}


// Management
///////////////

void igdeLoadTemplate::Load( decBaseFileReader &reader, igdeTemplate &atemplate ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "projectTemplate" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadTemplate( *root, atemplate );
}



// Private Functions
//////////////////////

void igdeLoadTemplate::pReadTemplate( const decXmlElementTag &root, igdeTemplate &atemplate ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "name" ) == 0 ){
			atemplate.SetName( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "description" ) == 0 ){
			atemplate.SetDescription( ReadMultilineString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "scriptModule" ) == 0 ){
			atemplate.SetScriptModule( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "baseGameDefinition" ) == 0 ){
			atemplate.GetBaseGameDefinitions().Add( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "file" ) == 0 ){
			igdeTemplateFile *file = NULL;
			
			try{
				file = new igdeTemplateFile;
				pReadFile( *tag, *file );
				atemplate.GetFiles().Add( file );
				file->FreeReference();
				
			}catch( const deException & ){
				if( file ){
					file->FreeReference();
				}
				throw;
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void igdeLoadTemplate::pReadFile( const decXmlElementTag &root, igdeTemplateFile &file ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "path" ) == 0 ){
			file.SetPath( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "pattern" ) == 0 ){
			file.SetPattern( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "directory" ) == 0 ){
			const decString directory( GetCDataString( *tag ) );
			
			if( directory == "data" ){
				file.SetDirectory( igdeTemplateFile::edData );
				
			}else if( directory == "project" ){
				file.SetDirectory( igdeTemplateFile::edProject );
				
			}else{
				LogWarnUnknownValue( *tag, directory );
			}
			
		}else if( strcmp( tag->GetName(), "replace" ) == 0 ){
			igdeTemplateReplace *replace = NULL;
			
			try{
				replace = new igdeTemplateReplace;
				pReadReplace( *tag, *replace );
				file.GetReplacements().Add( replace );
				replace->FreeReference();
				
			}catch( const deException & ){
				if( replace ){
					replace->FreeReference();
				}
				throw;
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void igdeLoadTemplate::pReadReplace( const decXmlElementTag &root, igdeTemplateReplace &replace ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "token" ) == 0 ){
			replace.SetToken( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "value" ) == 0 ){
			const decString value( GetCDataString( *tag ) );
			
			if( value == "projectPath" ){
				replace.SetValue( igdeTemplateReplace::evProjectPath );
				
			}else if( value == "dataPath" ){
				replace.SetValue( igdeTemplateReplace::evDataPath );
				
			}else if( value == "projectName" ){
				replace.SetValue( igdeTemplateReplace::evProjectName );
				
			}else if( value == "projectDescription" ){
				replace.SetValue( igdeTemplateReplace::evProjectDescription );
				
			}else if( value == "gameId" ){
				replace.SetValue( igdeTemplateReplace::evGameId );
				
			}else if( value == "gameAliasId" ){
				replace.SetValue( igdeTemplateReplace::evGameAliasId );
				
			}else{
				LogWarnUnknownValue( *tag, value );
			}
			
		}else if( strcmp( tag->GetName(), "format" ) == 0 ){
			const decString format( GetCDataString( *tag ) );
			
			if( format == "text" ){
				replace.SetFormat( igdeTemplateReplace::efText );
				
			}else if( format == "stringC" ){
				replace.SetFormat( igdeTemplateReplace::efStringC );
				
			}else if( format == "xml" ){
				replace.SetFormat( igdeTemplateReplace::efXml );
				
			}else if( format == "xmlMultiline" ){
				replace.SetFormat( igdeTemplateReplace::efXmlMultiline );
				
			}else{
				LogWarnUnknownValue( *tag, format );
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}
