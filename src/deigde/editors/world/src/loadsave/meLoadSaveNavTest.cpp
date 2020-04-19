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

#include "meLoadSaveNavTest.h"
#include "../world/meWorld.h"
#include "../world/pathfinding/mePathFindTest.h"
#include "../world/pathfinding/types/mePathFindTestType.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <deigde/triggersystem/igdeTriggerTarget.h>



// Class meLoadSaveNavTest
////////////////////////////

// Constructor, destructor
////////////////////////////

meLoadSaveNavTest::meLoadSaveNavTest( meLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ){
	pLSSys = lssys;
	
	pName = "Navigation Test";
	pPattern = ".dent";
}



// Loading and saving
///////////////////////

void meLoadSaveNavTest::LoadNavTest( meWorld &world, decBaseFileReader &reader ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "navigationTest" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadNavTest( *root, world );
	
	world.NotifyPathFindTestChanged();
}

void meLoadSaveNavTest::SaveNavTest( const meWorld &world, decBaseFileWriter &writer ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteNavTest( xmlWriter, world );
}



// Private Functions
//////////////////////

void meLoadSaveNavTest::pWriteNavTest( decXmlWriter &writer, const meWorld &world ){
	const mePathFindTest &pathFindTest = *world.GetPathFindTest();
	
	writer.WriteOpeningTag( "navigationTest", false, true );
	
	// start position
	const decDVector &startPosition = pathFindTest.GetStartPosition();
	writer.WriteOpeningTagStart( "startPosition" );
	writer.WriteAttributeFloat( "x", startPosition.x );
	writer.WriteAttributeFloat( "y", startPosition.y );
	writer.WriteAttributeFloat( "z", startPosition.z );
	writer.WriteOpeningTagEnd( true );
	
	// goal position
	const decDVector &goalPosition = pathFindTest.GetGoalPosition();
	writer.WriteOpeningTagStart( "goalPosition" );
	writer.WriteAttributeFloat( "x", goalPosition.x );
	writer.WriteAttributeFloat( "y", goalPosition.y );
	writer.WriteAttributeFloat( "z", goalPosition.z );
	writer.WriteOpeningTagEnd( true );
	
	// layer
	writer.WriteDataTagInt( "layer", pathFindTest.GetLayer() );
	
	// space type
	if( pathFindTest.GetSpaceType() == deNavigationSpace::estGrid ){
		writer.WriteDataTagString( "spaceType", "grid" );
		
	}else if( pathFindTest.GetSpaceType() == deNavigationSpace::estMesh ){
		writer.WriteDataTagString( "spaceType", "mesh" );
		
	}else{ // deNavigationSpace::estVolume
		writer.WriteDataTagString( "spaceType", "volume" );
	}
	
	// blocking cost
	writer.WriteDataTagInt( "blockingCost", ( int )pathFindTest.GetBlockingCost() );
	
	// cost types
	const mePathFindTestTypeList &typeList = pathFindTest.GetTypeList();
	const int typeCount = typeList.GetCount();
	int i;
	
	for( i=0; i<typeCount; i++ ){
		pWriteNavTestType( writer, *typeList.GetAt( i ) );
	}
	
	writer.WriteClosingTag( "navigationTest", true );
}

void meLoadSaveNavTest::pWriteNavTestType( decXmlWriter &writer, const mePathFindTestType &type ){
	writer.WriteOpeningTag( "type", false, true );
	
	writer.WriteDataTagInt( "number", type.GetTypeNumber() );
	writer.WriteDataTagString( "name", type.GetName() );
	
	writer.WriteDataTagFloat( "fixCost", type.GetFixCost() );
	writer.WriteDataTagFloat( "costPerMeter", type.GetCostPerMeter() );
	
	writer.WriteClosingTag( "type", true );
}



void meLoadSaveNavTest::pReadNavTest( const decXmlElementTag &root, meWorld &world ){
	mePathFindTest &pathFindTest = *world.GetPathFindTest();
	mePathFindTestTypeList &typeList = pathFindTest.GetTypeList();
	const int elementCount = root.GetElementCount();
	int e;
	
	typeList.RemoveAll();
	
	for( e=0; e<elementCount; e++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "startPosition" ) == 0 ){
				pathFindTest.SetStartPosition( decDVector( GetAttributeDouble( *tag, "x" ),
					GetAttributeDouble( *tag, "y" ), GetAttributeDouble( *tag, "z" ) ) );
				
			}else if( strcmp( tag->GetName(), "goalPosition" ) == 0 ){
				pathFindTest.SetGoalPosition( decDVector( GetAttributeDouble( *tag, "x" ),
					GetAttributeDouble( *tag, "y" ), GetAttributeDouble( *tag, "z" ) ) );
				
			}else if( strcmp( tag->GetName(), "layer" ) == 0 ){
				pathFindTest.SetLayer( GetCDataInt( *tag ) );
				
			}else if( strcmp( tag->GetName(), "spaceType" ) == 0 ){
				const char * const spaceType = GetCDataString( *tag );
				
				if( spaceType ){
					if( strcmp( spaceType, "grid" ) == 0 ){
						pathFindTest.SetSpaceType( deNavigationSpace::estGrid );
						
					}else if( strcmp( spaceType, "mesh" ) == 0 ){
						pathFindTest.SetSpaceType( deNavigationSpace::estMesh );
						
					}else if( strcmp( spaceType, "volume" ) == 0 ){
						pathFindTest.SetSpaceType( deNavigationSpace::estVolume );
						
					}else{
						LogWarnUnknownValue( *tag, spaceType );
					}
					
				}else{
					LogWarnUnknownValue( *tag, "" );
				}
				
			}else if( strcmp( tag->GetName(), "type" ) == 0 ){
				pReadNavTestType( *tag, world );
				
			}else if( strcmp( tag->GetName(), "blockingCost" ) == 0 ){
				pathFindTest.SetBlockingCost( ( float )GetCDataInt( *tag ) );
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
	}
}

void meLoadSaveNavTest::pReadNavTestType( const decXmlElementTag &root, meWorld &world ){
	const int elementCount = root.GetElementCount();
	mePathFindTestType *type = NULL;
	int e;
	
	try{
		type = new mePathFindTestType( 0 );
		
		for( e=0; e<elementCount; e++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( e );
			
			if( tag ){
				if( strcmp( tag->GetName(), "number" ) == 0 ){
					type->SetTypeNumber( GetCDataInt( *tag ) );
					
				}else if( strcmp( tag->GetName(), "name" ) == 0 ){
					type->SetName( GetCDataString( *tag ) );
					
				}else if( strcmp( tag->GetName(), "fixCost" ) == 0 ){
					type->SetFixCost( GetCDataFloat( *tag ) );
					
				}else if( strcmp( tag->GetName(), "costPerMeter" ) == 0 ){
					type->SetCostPerMeter( GetCDataFloat( *tag ) );
					
				}else{
					LogWarnUnknownTag( root, *tag );
				}
			}
		}
		
		world.GetPathFindTest()->GetTypeList().Add( type );
		type->FreeReference();
		
		world.GetPathFindTest()->NotifyTypesChanged();
		
	}catch( const deException & ){
		if( type ){
			type->FreeReference();
		}
		throw;
	}
}
