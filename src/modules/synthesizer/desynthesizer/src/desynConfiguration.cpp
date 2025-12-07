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

#include "deDESynthesizer.h"
#include "desynConfiguration.h"

#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>



// Class desynConfiguration
/////////////////////////////

// Constructor, destructor
////////////////////////////

desynConfiguration::desynConfiguration( deDESynthesizer &module ) :
pModule( module ),
pStreamBufSizeThreshold( 700000 ) // see desynSound.cpp
{
}

desynConfiguration::~desynConfiguration(){
}



// Parmeters
//////////////

void desynConfiguration::SetStreamBufSizeThreshold( int threshold ){
	if( threshold < 0 ){
		DETHROW( deeInvalidParam );
	}
	pStreamBufSizeThreshold = threshold;
}



void desynConfiguration::LoadConfig(){
	deVirtualFileSystem &vfs = pModule.GetVFS();
	const decPath path( decPath::CreatePathNative( "/config/desynthesizer.xml" ) );
	if( ! vfs.ExistsFile( path ) ){
		return;
	}
	
	pLoadConfig(decBaseFileReader::Ref::New(vfs.OpenFileForReading(path)));
}

void desynConfiguration::SaveConfig(){
}



// Private Functions
//////////////////////

void desynConfiguration::pCleanUp(){
}



decXmlElementTag *desynConfiguration::pGetTagAt( const decXmlElementTag &tag, int index ){
	decXmlElement &element = *tag.GetElementAt( index );
	
	if( element.CanCastToElementTag() ){
		return element.CastToElementTag();
		
	}else{
		return NULL;
	}
}

decXmlAttValue *desynConfiguration::pFindAttribute( const decXmlElementTag &tag, const char *name ){
	const int count = tag.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		decXmlElement &element = *tag.GetElementAt( i );
		
		if( element.CanCastToAttValue() ){
			decXmlAttValue * const value = element.CastToAttValue();
			
			if( strcmp( value->GetName(), name ) == 0 ){
				return value;
			}
		}
	}
	
	return NULL;
}

const char *desynConfiguration::pGetAttributeString( const decXmlElementTag &tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	
	if( value ){
		return value->GetValue();
		
	}else{
		pModule.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.", name,
			tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

int desynConfiguration::pGetAttributeInt( const decXmlElementTag &tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	
	if( value ){
		return ( int )strtol( value->GetValue(), NULL, 10 );
		
	}else{
		pModule.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.", name,
			tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

float desynConfiguration::pGetAttributeFloat( const decXmlElementTag &tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	
	if( value ){
		return strtof( value->GetValue(), NULL );
		
	}else{
		pModule.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.", name,
			tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

const char *desynConfiguration::pGetCData( const decXmlElementTag &tag, const char *defaultValue ){
	decXmlCharacterData * const cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData();
		
	}else{
		return defaultValue;
	}
}

bool desynConfiguration::pGetCDataBool( const decXmlElementTag &tag, bool defaultValue ){
	if( defaultValue ){
		return strcmp( pGetCData( tag, "1" ), "1" ) == 0;
		
	}else{
		return strcmp( pGetCData( tag, "0" ), "1" ) == 0;
	}
}

int desynConfiguration::pGetCDataInt( const decXmlElementTag &tag, int defaultValue ){
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData().ToInt();
		
	}else{
		return defaultValue;
	}
}



void desynConfiguration::pLoadConfig( decBaseFileReader *file ){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser( pModule.GetGameEngine()->GetLogger() ).ParseXml( file, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "config" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	// read properties
	const int elementCount = root->GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = pGetTagAt( *root, i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "property" ) == 0 ){
			const decString name( pGetAttributeString( *tag, "name" ) );
			
			if( strcmp( name, "streamBufSizeThreshold" ) == 0 ){
				SetStreamBufSizeThreshold( pGetCDataInt( *tag, pStreamBufSizeThreshold ) );
				
			}else{
				pModule.LogWarnFormat( "desynthesizer.xml %s(%i:%i): Invalid property name %s.",
					tag->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), name.GetString() );
			}
		}else{
			pModule.LogWarnFormat( "desynthesizer.xml %s(%i:%i): Unknown Tag %s.",
				root->GetName().GetString(), tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
		}
	}
}
