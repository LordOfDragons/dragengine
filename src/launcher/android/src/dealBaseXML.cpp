/* 
 * Drag[en]gine Android Launcher
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

#include "dealBaseXML.h"
#include "logger/deLogger.h"
#include "common/xmlparser/decXmlWriter.h"
#include "common/xmlparser/decXmlDocument.h"
#include "common/xmlparser/decXmlCharacterData.h"
#include "common/xmlparser/decXmlElementTag.h"
#include "common/xmlparser/decXmlAttValue.h"
#include "common/xmlparser/decXmlVisitor.h"
#include "common/xmlparser/decXmlParser.h"
#include "common/exceptions.h"



// Class dealBaseXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

dealBaseXML::dealBaseXML( deLogger *logger, const char *loggerSource ){
	if( ! logger || ! loggerSource ) DETHROW( deeInvalidParam );
	
	pLoggerSource = loggerSource;
	
	pLogger = logger;
	logger->AddReference();
}

dealBaseXML::~dealBaseXML(){
	pLogger->FreeReference();
}



// Management
///////////////


bool dealBaseXML::pHasAttribute( const decXmlElementTag &tag, const char *name ) const{
	return tag.FindAttribute( name ) != NULL;
}

const char *dealBaseXML::pGetAttributeString( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue *value = tag.FindAttribute( name );
	
	if( value ){
		return value->GetValue();
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(),
			"Missing Attribute %s in tag %s", name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

int dealBaseXML::pGetAttributeInt( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue *value = tag.FindAttribute( name );
	
	if( value ){
		return ( int )strtol( value->GetValue(), NULL, 10 );
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(),
			"Missing Attribute %s in tag %s", name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

float dealBaseXML::pGetAttributeFloat( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue *value = tag.FindAttribute( name );
	
	if( value ){
		return strtof( value->GetValue(), NULL );
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(),
			"Missing Attribute %s in tag %s", name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

int dealBaseXML::pGetCDataInt( const decXmlElementTag &tag ) const{
	decXmlCharacterData *cdata = tag.GetFirstData();
	
	if( cdata ){
		return ( int )strtol( cdata->GetData(), NULL, 10 );
	}
	
	return 0;
}

float dealBaseXML::pGetCDataFloat( const decXmlElementTag &tag ) const{
	decXmlCharacterData *cdata = tag.GetFirstData();
	
	if( cdata ){
		return strtof( cdata->GetData(), NULL );
	}
	
	return 0.0f;
}

const char *dealBaseXML::pGetCDataString( const decXmlElementTag &tag ) const{
	decXmlCharacterData *cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData();
	}
	
	return "";
}



void dealBaseXML::pErrorUnknownTag( const decXmlElementTag &root, const decXmlElementTag &tag ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Unknown Tag %s",
		root.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(),
		tag.GetName().GetString() );
}

void dealBaseXML::pErrorUnknownValue( const decXmlElementTag &tag, const char *value ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Unknown Value %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(), value );
}

void dealBaseXML::pErrorMissingTag( const decXmlElementTag &root, const char *tagName ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Missing Tag %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(), tagName );
}
