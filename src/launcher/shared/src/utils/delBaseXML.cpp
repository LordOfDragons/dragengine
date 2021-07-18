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

#include "delBaseXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class delBaseXML
/////////////////////

// Constructors and Destructors
/////////////////////////////////

delBaseXML::delBaseXML( deLogger *logger, const char *loggerSource ) :
pLogger( logger ),
pLoggerSource( loggerSource )
{
	if( ! logger ){
		DETHROW_INFO( deeNullPointer, "logger" );
	}
	if( ! loggerSource ){
		DETHROW_INFO( deeNullPointer, "loggerSource" );
	}
}

delBaseXML::~delBaseXML(){
}



// Management
///////////////

bool delBaseXML::HasAttribute( const decXmlElementTag &tag, const char *name ) const{
	return tag.FindAttribute( name ) != NULL;
}

const decString &delBaseXML::GetAttributeString( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue * const value = tag.FindAttribute( name );
	if( value ){
		return value->GetValue();
	}
	
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
		name, tag.GetName().GetString() );
	DETHROW( deeInvalidParam );
}

int delBaseXML::GetAttributeInt( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue * const value = tag.FindAttribute( name );
	if( value ){
		return value->GetValue().ToInt();
	}
	
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
		name, tag.GetName().GetString() );
	DETHROW( deeInvalidParam );
}

float delBaseXML::GetAttributeFloat( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue * const value = tag.FindAttribute( name );
	if( value ){
		return value->GetValue().ToFloat();
	}
	
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
		name, tag.GetName().GetString() );
	DETHROW( deeInvalidParam );
}

int delBaseXML::GetCDataInt( const decXmlElementTag &tag ) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	return cdata ? cdata->GetData().ToInt() : 0;
}

float delBaseXML::GetCDataFloat( const decXmlElementTag &tag ) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	return cdata ? cdata->GetData().ToFloat() : 0.0f;
}

const decString &delBaseXML::GetCDataString( const decXmlElementTag &tag ) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	static const decString empty;
	return cdata ? cdata->GetData() : empty;
}

bool delBaseXML::GetCDataBool( const decXmlElementTag &tag ) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	return cdata && ( cdata->GetData() == "true" || cdata->GetData() == "1" );
}



void delBaseXML::ErrorUnknownTag( const decXmlElementTag &root, const decXmlElementTag &tag ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Unknown Tag %s",
		root.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(),
		tag.GetName().GetString() );
}

void delBaseXML::ErrorUnknownValue( const decXmlElementTag &tag, const char *value ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Unknown Value %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(), value );
}

void delBaseXML::ErrorMissingTag( const decXmlElementTag &root, const char *tagName ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Missing Tag %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(), tagName );
}
