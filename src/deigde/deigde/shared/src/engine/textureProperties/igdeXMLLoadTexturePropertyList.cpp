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
	decXmlDocument::Ref xmlDoc;
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
