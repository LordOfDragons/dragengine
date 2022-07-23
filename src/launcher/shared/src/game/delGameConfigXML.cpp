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

#include "delGame.h"
#include "delGameManager.h"
#include "delGameConfigXML.h"
#include "icon/delGameIcon.h"
#include "profile/delGameProfile.h"
#include "profile/delGPModule.h"
#include "profile/delGPMParameter.h"
#include "profile/delGPDisableModuleVersion.h"
#include "profile/delGPDisableModuleVersionList.h"
#include "../delLauncher.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>



// Class delGameConfigXML
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameConfigXML::delGameConfigXML( delLauncher &launcher ) :
delSharedConfigXML( launcher.GetLogger(), launcher.GetLogSource() ),
pLauncher( launcher ){
}

delGameConfigXML::~delGameConfigXML(){
}



// Management
///////////////

void delGameConfigXML::ReadFromFile( decBaseFileReader &reader, delGame &game ){
	decXmlDocument::Ref xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "gameConfig" ){
		DETHROW_INFO( deeInvalidParam, "missing root tag 'gameConfig'" );
	}
	
	pReadConfig( *root, game );
}

void delGameConfigXML::WriteToFile( decBaseFileWriter &writer, const delGame &game ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, game );
}



// Private Functions
//////////////////////

void delGameConfigXML::pWriteConfig( decXmlWriter &writer, const delGame &game ){
	writer.WriteOpeningTag( "gameConfig", false, true );
	
	if( game.GetCustomProfile() ){
		WriteProfile( writer, *game.GetCustomProfile(), "customProfile" );
	}
	
	if( game.GetActiveProfile() ){
		writer.WriteDataTagString( "activeProfile", game.GetActiveProfile()->GetName() );
	}
	
	if( ! game.GetRunArguments().IsEmpty() ){
		writer.WriteDataTagString( "runArguments", game.GetRunArguments() );
	}
	
	writer.WriteDataTagBool( "useLatestPatch", game.GetUseLatestPatch() );
	if( game.GetUseCustomPatch() ){
		writer.WriteDataTagString( "useCustomPatch", game.GetUseCustomPatch().ToHexString( false ) );
	}
	
	if( ! game.GetDelgaFile().IsEmpty() ){
		writer.WriteDataTagString( "delgaFile", game.GetDelgaFile() );
	}
	if( ! game.GetAliasIdentifier().IsEmpty() ){
		writer.WriteDataTagString( "aliasIdentifier", game.GetAliasIdentifier() );
	}
	if( ! game.GetTitle().IsEmpty() ){
		writer.WriteDataTagString( "title", game.GetTitle().ToUTF8() );
	}
	
	if( game.GetIcons().GetCount() > 0 ){
		const delGameIcon * const icon = game.GetIcons().GetLargest();
		
		if( icon->GetContent() ){
			const int indexExt = icon->GetPath().FindReverse( '.' );
			decString filenameIcon( "icon" );
			if( indexExt != -1 ){
				filenameIcon += icon->GetPath().GetMiddle( indexExt );
			}
			
			decPath pathIcon;
			pathIcon.SetFromUnix( "/config/user/games" );
			pathIcon.AddComponent( game.GetIdentifier().ToHexString( false ) );
			pathIcon.AddComponent( filenameIcon );
			
			try{
				decBaseFileWriter::Ref::New( pLauncher.GetVFS()->OpenFileForWriting( pathIcon ) )
					->Write( icon->GetContent()->GetPointer(), icon->GetContent()->GetLength() );
				
				writer.WriteOpeningTagStart( "icon" );
				writer.WriteAttributeInt( "size", icon->GetSize() );
				writer.WriteOpeningTagEnd( false, false );
				writer.WriteTextString( filenameIcon );
				writer.WriteClosingTag( "icon", false );
				
			}catch( const deException &e ){
				GetLogger()->LogException( GetLoggerSource(), e );
			}
		}
	}
	
	writer.WriteClosingTag( "gameConfig", true );
}



void delGameConfigXML::pReadConfig( const decXmlElementTag &root, delGame &game ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "customProfile" ){
			delGameProfile::Ref profile( delGameProfile::Ref::New( game.GetLauncher().CreateGameProfile() ) );
			ReadProfile( *tag, profile );
			profile->SetName( "" );
			
			game.SetCustomProfile( profile );
			
		}else if( tagName == "activeProfile" ){
			const decString &name = GetCDataString( *tag );
			if( name.IsEmpty() ){
				game.SetActiveProfile( game.GetCustomProfile() );
				
			}else{
				game.SetActiveProfile( pLauncher.GetGameManager().GetProfiles().GetNamed ( name ) );
				if( ! game.GetActiveProfile() ){
					GetLogger()->LogWarnFormat( GetLoggerSource(), "%s(%i:%i): Profile '%s' does not exist",
						tag->GetName().GetString(), tag->GetLineNumber(), tag->GetPositionNumber(), name.GetString() );
				}
			}
			
		}else if( tagName == "runArguments" ){
			game.SetRunArguments( GetCDataString( *tag ) );
			
		}else if( tagName == "useLatestPatch" ){
			game.SetUseLatestPatch( GetCDataBool( *tag ) );
			
		}else if( tagName == "useCustomPatch" ){
			const decString &value = GetCDataString( *tag );
			game.SetUseCustomPatch( ! value.IsEmpty() ? decUuid( value, false ) : decUuid() );
			
		}else if( tagName == "delgaFile" ){
			// do not replace if present. this is fallback information
			if( game.GetDelgaFile().IsEmpty() ){
				game.SetDelgaFile( GetCDataString( *tag ) );
			}
			
		}else if( tagName == "aliasIdentifier" ){
			// do not replace if present. this is fallback information
			if( game.GetAliasIdentifier().IsEmpty() ){
				game.SetAliasIdentifier( GetCDataString( *tag ) );
			}
			
		}else if( tagName == "title" ){
			// do not replace if present. this is fallback information
			if( game.GetTitle().IsEmpty() ){
				game.SetTitle( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			}
			
		}else if( tagName == "icon" ){
			// do not replace if present. this is fallback information
			if( game.GetIcons().GetCount() == 0 ){
				const decString filenameIcon( GetCDataString( *tag ) );
				const int size = GetAttributeInt( *tag, "size" );
				
				const delGameIcon::Ref icon( delGameIcon::Ref::New(
					pLauncher.CreateGameIcon( size, filenameIcon ) ) );
				
				const decMemoryFile::Ref content( decMemoryFile::Ref::New(
					new decMemoryFile( filenameIcon ) ) );
				
				decPath pathIcon;
				pathIcon.SetFromUnix( "/config/user/games" );
				pathIcon.AddComponent( game.GetIdentifier().ToHexString( false ) );
				pathIcon.AddComponent( filenameIcon );
				
				try{
					const decBaseFileReader::Ref reader( decBaseFileReader::Ref::New(
						pLauncher.GetVFS()->OpenFileForReading( pathIcon ) ) );
					const int length = reader->GetLength();
					content->Resize( length );
					reader->Read( content->GetPointer(), length );
					
					icon->SetContent( content );
					game.GetIcons().Add( icon );
					
				}catch( const deException &e ){
					GetLogger()->LogException( GetLoggerSource(), e );
				}
			}
		}
	}
}
