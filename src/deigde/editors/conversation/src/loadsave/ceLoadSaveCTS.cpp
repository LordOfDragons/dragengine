/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceLoadSaveCTS.h"
#include "ceLoadSaveCTA.h"
#include "ceLoadSaveCTGS.h"
#include "ceLoadSaveSystem.h"
#include "../conversation/ceConversation.h"
#include "../conversation/actor/ceConversationActor.h"
#include "../conversation/actor/controller/ceActorController.h"
#include "../conversation/actor/parameters/ceActorParameter.h"
#include "../conversation/coordsystem/ceCoordSystem.h"
#include "../conversation/playback/command/cePlaybackCommand.h"
#include "../conversation/prop/ceProp.h"
#include "../gui/ceWindowMain.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class ceLoadSaveCTS
////////////////////////

// Constructor, destructor
////////////////////////////

ceLoadSaveCTS::ceLoadSaveCTS( ceLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ){
	pLSSys = lssys;
	
	pName = "Conversation Test Setup";
	pPattern = ".dects";
}



// Loading and saving
///////////////////////

void ceLoadSaveCTS::LoadCTS( ceConversation &conversation, decBaseFileReader &reader ){
	decXmlDocument::Ref xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "conversationTestSetup" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadCTS( *root, conversation );
	
	conversation.NotifyPlaybackCommandListChanged();
	conversation.NotifyPlaybackVarListChanged();
}

void ceLoadSaveCTS::SaveCTS( const ceConversation &conversation, decBaseFileWriter &writer ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteCTS( xmlWriter, conversation );
}



// Private Functions
//////////////////////

void ceLoadSaveCTS::pWriteCTS( decXmlWriter &writer, const ceConversation &conversation ){
	int i;
	
	writer.WriteOpeningTag( "conversationTestSetup", false, true );
	
	const ceConversationActorList &actorList = conversation.GetActorList();
	const int actorCount = actorList.GetCount();
	for( i=0; i<actorCount; i++ ){
		pLSSys->GetLSCTA()->WriteActor( writer, *actorList.GetAt( i ), "actor" );
	}
	
	const ceCoordSystemList &coordSystemList = conversation.GetCoordSystemList();
	const int coordSystemCount = coordSystemList.GetCount();
	for( i=0; i<coordSystemCount; i++ ){
		pWriteCoordSystem( writer, *coordSystemList.GetAt( i ) );
	}
	
	const cePropList &propList = conversation.GetPropList();
	const int propCount = propList.GetCount();
	for( i=0; i<propCount; i++ ){
		pWriteProp( writer, *propList.GetAt( i ) );
	}
	
	pLSSys->GetLSCTGS()->WriteGameState( writer, *conversation.GetPlayback(), "gameState" );
	
	writer.WriteClosingTag( "conversationTestSetup", true );
}

void ceLoadSaveCTS::pWriteCoordSystem( decXmlWriter &writer, const ceCoordSystem &coordSystem ){
	writer.WriteOpeningTag( "coordinateSystem" );
	
	writer.WriteDataTagString( "id", coordSystem.GetID() );
	writer.WriteDataTagString( "aliasID", coordSystem.GetAliasID() );
	
	const decVector &position = coordSystem.GetPosition();
	writer.WriteOpeningTagStart( "position" );
	writer.WriteAttributeFloat( "x", position.x );
	writer.WriteAttributeFloat( "y", position.y );
	writer.WriteAttributeFloat( "z", position.z );
	writer.WriteOpeningTagEnd( true );
	
	const decVector &orientation = coordSystem.GetOrientation();
	writer.WriteOpeningTagStart( "orientation" );
	writer.WriteAttributeFloat( "x", orientation.x );
	writer.WriteAttributeFloat( "y", orientation.y );
	writer.WriteAttributeFloat( "z", orientation.z );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteClosingTag( "coordinateSystem" );
}

void ceLoadSaveCTS::pWriteProp( decXmlWriter &writer, const ceProp &prop ){
	writer.WriteOpeningTag( "prop" );
	
	writer.WriteDataTagString( "name", prop.GetName() );
	writer.WriteDataTagString( "objectClass", prop.GetObjectClass() );
	
	const decVector &position = prop.GetPosition();
	writer.WriteOpeningTagStart( "position" );
	writer.WriteAttributeFloat( "x", position.x );
	writer.WriteAttributeFloat( "y", position.y );
	writer.WriteAttributeFloat( "z", position.z );
	writer.WriteOpeningTagEnd( true );
	
	const decVector &orientation = prop.GetOrientation();
	writer.WriteOpeningTagStart( "orientation" );
	writer.WriteAttributeFloat( "x", orientation.x );
	writer.WriteAttributeFloat( "y", orientation.y );
	writer.WriteAttributeFloat( "z", orientation.z );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteDataTagBool( "visible", prop.GetVisible() );
	
	writer.WriteClosingTag( "prop" );
}



void ceLoadSaveCTS::pReadCTS( const decXmlElementTag &root, ceConversation &conversation ){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	conversation.RemoveAllActors();
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "actor" ) == 0 ){
				ceConversationActor * const actor = new ceConversationActor(
					*conversation.GetEnvironment() );
				
				try{
					pLSSys->GetLSCTA()->ReadActor( *tag, *actor );
					conversation.AddActor( actor );
					actor->FreeReference();
					
				}catch( const deException & ){
					if( actor ){
						actor->FreeReference();
					}
					throw;
				}
				
			}else if( strcmp( tag->GetName(), "coordinateSystem" ) == 0 ){
				pReadCoordSystem( *tag, conversation );
				
			}else if( strcmp( tag->GetName(), "prop" ) == 0 ){
				pReadProp( *tag, conversation );
				
			}else if( strcmp( tag->GetName(), "gameState" ) == 0 ){
				pLSSys->GetLSCTGS()->ReadGameState( *tag, *conversation.GetPlayback() );
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
	}
}

void ceLoadSaveCTS::pReadCoordSystem( const decXmlElementTag &root, ceConversation &conversation ){
	const int elementCount = root.GetElementCount();
	ceCoordSystem *coordSystem = NULL;
	int i;
	
	try{
		coordSystem = new ceCoordSystem;
		
		for( i=0; i<elementCount; i++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "id" ) == 0 ){
				coordSystem->SetID( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "aliasID" ) == 0 ){
				coordSystem->SetAliasID( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "position" ) == 0 ){
				decVector position;
				ReadVector( *tag, position );
				coordSystem->SetPosition( position );
				
			}else if( strcmp( tag->GetName(), "orientation" ) == 0 ){
				decVector orientation;
				ReadVector( *tag, orientation );
				coordSystem->SetOrientation( orientation );
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
		conversation.AddCoordSystem( coordSystem );
		coordSystem->FreeReference();
		
	}catch( const deException & ){
		if( coordSystem ){
			coordSystem->FreeReference();
		}
		throw;
	}
}

void ceLoadSaveCTS::pReadProp( const decXmlElementTag &root, ceConversation &conversation ){
	const int elementCount = root.GetElementCount();
	ceProp *prop = NULL;
	int i;
	
	try{
		prop = new ceProp;
		
		for( i =0; i <elementCount; i++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( strcmp( tag->GetName(), "name" ) == 0 ){
				prop->SetName( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "objectClass" ) == 0 ){
				prop->SetObjectClass( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "position" ) == 0 ){
				decVector position;
				ReadVector( *tag, position );
				prop->SetPosition( position );
				
			}else if( strcmp( tag->GetName(), "orientation" ) == 0 ){
				decVector orientation;
				ReadVector( *tag, orientation );
				prop->SetOrientation( orientation );
				
			}else if( strcmp( tag->GetName(), "visible" ) == 0 ){
				prop->SetVisible( GetCDataBool( *tag ) );
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
		conversation.AddProp( prop );
		prop->FreeReference();
		
	}catch( const deException & ){
		if( prop ){
			prop->FreeReference();
		}
		throw;
	}
}
