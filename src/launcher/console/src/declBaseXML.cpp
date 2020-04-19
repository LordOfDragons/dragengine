/* 
 * Drag[en]gine Console Launcher
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

#include "declBaseXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>



// Class declBaseXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

declBaseXML::declBaseXML( deLogger *logger, const char *loggerSource ){
	if( ! logger || ! loggerSource ){
		DETHROW( deeInvalidParam );
	}
	
	pLoggerSource = loggerSource;
	
	pLogger = logger;
	logger->AddReference();
}

declBaseXML::~declBaseXML(){
	pLogger->FreeReference();
}



// Management
///////////////


bool declBaseXML::pHasAttribute( const decXmlElementTag &tag, const char *name ) const{
	return tag.FindAttribute( name ) != NULL;
}

const char *declBaseXML::pGetAttributeString( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue *value = tag.FindAttribute( name );
	
	if( value ){
		return value->GetValue();
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

int declBaseXML::pGetAttributeInt( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue *value = tag.FindAttribute( name );
	
	if( value ){
		return ( int )strtol( value->GetValue(), NULL, 10 );
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

float declBaseXML::pGetAttributeFloat( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue *value = tag.FindAttribute( name );
	
	if( value ){
		return ( float )strtod( value->GetValue(), NULL );
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

bool declBaseXML::pGetAttributeBool( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue * const value = tag.FindAttribute( name );
	if( ! value ){
		pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
	
	return strcmp( value->GetValue(), "true" ) == 0
	|| strcmp( value->GetValue(), "yes" ) == 0
	|| strcmp( value->GetValue(), "1" ) == 0;
}



int declBaseXML::pGetCDataInt( const decXmlElementTag &tag ) const{
	decXmlCharacterData *cdata = tag.GetFirstData();
	
	if( cdata ){
		return ( int )strtol( cdata->GetData(), NULL, 10 );
	}
	
	return 0;
}

float declBaseXML::pGetCDataFloat( const decXmlElementTag &tag ) const{
	decXmlCharacterData *cdata = tag.GetFirstData();
	
	if( cdata ){
		return ( float )strtod( cdata->GetData(), NULL );
	}
	
	return 0.0f;
}

const char *declBaseXML::pGetCDataString( const decXmlElementTag &tag ) const{
	decXmlCharacterData *cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData();
	}
	
	return "";
}

bool declBaseXML::pGetCDataBool( const decXmlElementTag &tag ) const{
	decXmlCharacterData * const cdata = tag.GetFirstData();
	if( ! cdata ){
		return false;
	}
	
	return strcmp( cdata->GetData(), "true" ) == 0
	|| strcmp( cdata->GetData(), "yes" ) == 0
	|| strcmp( cdata->GetData(), "1" ) == 0;
}



void declBaseXML::pErrorUnknownTag( const decXmlElementTag &root, const decXmlElementTag &tag ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Unknown Tag %s",
		root.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(),
		tag.GetName().GetString() );
}

void declBaseXML::pErrorUnknownValue( const decXmlElementTag &tag, const char *value ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Unknown Value %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(), value );
}

void declBaseXML::pErrorMissingTag( const decXmlElementTag &root, const char *tagName ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Missing Tag %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(), tagName );
}
