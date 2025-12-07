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
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
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
	writer.WriteAttributeFloat( "x", ( float )startPosition.x );
	writer.WriteAttributeFloat( "y", ( float )startPosition.y );
	writer.WriteAttributeFloat( "z", ( float )startPosition.z );
	writer.WriteOpeningTagEnd( true );
	
	// goal position
	const decDVector &goalPosition = pathFindTest.GetGoalPosition();
	writer.WriteOpeningTagStart( "goalPosition" );
	writer.WriteAttributeFloat( "x", ( float )goalPosition.x );
	writer.WriteAttributeFloat( "y", ( float )goalPosition.y );
	writer.WriteAttributeFloat( "z", ( float )goalPosition.z );
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
