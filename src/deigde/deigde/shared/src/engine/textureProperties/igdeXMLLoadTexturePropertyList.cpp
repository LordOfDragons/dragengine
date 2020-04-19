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

#include <stdlib.h>
#include <string.h>

#include "igdeXMLLoadTexturePropertyList.h"
#include "igdeTextureProperty.h"
#include "igdeTexturePropertyList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/logger/deLogger.h>



// Class igdeXMLLoadTexturePropertyList
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeXMLLoadTexturePropertyList::igdeXMLLoadTexturePropertyList( deLogger *logger ) :
igdeBaseXML( logger, "XMLLoadTexturePropertyList" ){
}

igdeXMLLoadTexturePropertyList::~igdeXMLLoadTexturePropertyList(){
}



// Loading
////////////

void igdeXMLLoadTexturePropertyList::ReadFromFile( igdeTexturePropertyList &list,
decBaseFileReader &file ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser parser( GetLogger() );
	parser.ParseXml( &file, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "texturePropertyList" ){
		DETHROW( deeInvalidParam );
	}
	
	pReadList( *root, list );
}



// Private Functions
//////////////////////

void igdeXMLLoadTexturePropertyList::pReadList( const decXmlElementTag &root,
igdeTexturePropertyList &list ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "property" ){
			pReadProperty( *tag, list );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void igdeXMLLoadTexturePropertyList::pReadProperty( const decXmlElementTag &root, igdeTexturePropertyList &list ){
	const int elementCount = root.GetElementCount();
	igdeTextureProperty *property = NULL;
	int i;
	
	try{
		property = new igdeTextureProperty( GetAttributeString( root, "name" ) );
		
		if( list.HasNamed( property->GetName() ) ){
			LogWarnGenericProblemValue( root, property->GetName().GetString(), "Duplicate property definition" );
			return;
		}
		
		for( i=0; i<elementCount; i++ ){
			decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			const decString &tagName = tag->GetName();
			if( tagName == "description" ){
				property->SetDescription( ReadMultilineString( *tag ) );
				
			}else if( tagName == "type" ){
				const char * const identifier = GetCDataString( *tag );
				
				if( strcmp( identifier, "value" ) == 0 ){
					property->SetType( igdeTextureProperty::eptValue );
					
				}else if( strcmp( identifier, "color" ) == 0 ){
					property->SetType( igdeTextureProperty::eptColor );
					
				}else if( strcmp( identifier, "image" ) == 0 ){
					property->SetType( igdeTextureProperty::eptImage );
					
				}else{
					LogErrorUnknownValue( *tag, identifier );
				}
				
			}else if( tagName == "components" ){
				property->SetComponentCount( GetCDataInt( *tag ) );
				
			}else if( tagName == "default" ){
				pReadDefault( *tag, *property );
				
			}else if( tagName == "affects" ){
				pReadAffects( *tag, *property );
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
		list.Add( property );
		
	}catch( const deException & ){
		if( property ){
			delete property;
		}
		throw;
	}
}

void igdeXMLLoadTexturePropertyList::pReadDefault( const decXmlElementTag &root,
igdeTextureProperty &property ){
	const int elementCount = root.GetElementCount();
	decColor defaultColor;
	int i;
	
	for( i=0; i<elementCount; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "value" ){
			property.SetDefaultValue( GetCDataFloat( *tag ) );
			
		}else if( tagName == "red" ){
			defaultColor.r = GetCDataFloat( *tag );
			
		}else if( tagName == "green" ){
			defaultColor.g = GetCDataFloat( *tag );
			
		}else if( tagName == "blue" ){
			defaultColor.b = GetCDataFloat( *tag );
			
		}else if( tagName == "alpha" ){
			defaultColor.a = GetCDataFloat( *tag );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	property.SetDefaultColor( defaultColor );
}

void igdeXMLLoadTexturePropertyList::pReadAffects( const decXmlElementTag &root,
igdeTextureProperty &property ){
	decStringSet &affectedModulesList = property.GetAffectedModules();
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "module" ){
			affectedModulesList.Add( GetCDataString( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}
