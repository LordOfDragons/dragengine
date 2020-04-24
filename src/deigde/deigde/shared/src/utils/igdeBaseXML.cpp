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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeBaseXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlCDSect.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>



// Class igdeBaseXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

igdeBaseXML::igdeBaseXML( deLogger *logger, const char *loggerSource ) :
pLogger( logger ),
pLoggerSource( loggerSource )
{
	if( ! logger ){
		DETHROW( deeInvalidParam );
	}
	logger->AddReference();
}

igdeBaseXML::~igdeBaseXML(){
	if( pLogger ){
		pLogger->FreeReference();
	}
}



// Management
///////////////

bool igdeBaseXML::HasAttribute( const decXmlElementTag &tag, const char *name ) const{
	return tag.FindAttribute( name ) != NULL;
}

const decString &igdeBaseXML::GetAttributeString( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue * const value = tag.FindAttribute( name );
	
	if( value ){
		return value->GetValue();
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

int igdeBaseXML::GetAttributeInt( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue * const value = tag.FindAttribute( name );
	
	if( value ){
		return value->GetValue().ToInt();
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

float igdeBaseXML::GetAttributeFloat( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue * const value = tag.FindAttribute( name );
	
	if( value ){
		return value->GetValue().ToFloat();
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

double igdeBaseXML::GetAttributeDouble( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue * const value = tag.FindAttribute( name );
	
	if( value ){
		return value->GetValue().ToDouble();
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

bool igdeBaseXML::GetAttributeBool( const decXmlElementTag &tag, const char *name ) const{
	const decXmlAttValue * const value = tag.FindAttribute( name );
	
	if( value ){
		return value->GetValue() == "true" || value->GetValue() == "yes" || value->GetValue() == "1";
		
	}else{
		pLogger->LogErrorFormat( pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}



bool igdeBaseXML::GetCDataBool( const decXmlElementTag &tag ) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData() == "true" || cdata->GetData() == "yes" || cdata->GetData() == "1";
	}
	
	return 0;
}

int igdeBaseXML::GetCDataInt( const decXmlElementTag &tag ) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData().ToInt();
	}
	
	return 0;
}

float igdeBaseXML::GetCDataFloat( const decXmlElementTag &tag ) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData().ToFloat();
	}
	
	return 0.0f;
}

double igdeBaseXML::GetCDataDouble( const decXmlElementTag &tag ) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData().ToDouble();
	}
	
	return 0.0;
}

const char *igdeBaseXML::GetCDataString( const decXmlElementTag &tag ) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData();
	}
	
	return "";
}



decString igdeBaseXML::ReadMultilineString( const decXmlElementTag &tag ){
	const int count = tag.GetElementCount();
	bool skipLeadingWhitespaces = false;
	int i;
	
	decString string;
	
	for( i=0; i<count; i++ ){
		decXmlElement &element = *tag.GetElementAt( i );
		
		if( element.CanCastToCharacterData() ){
			if( skipLeadingWhitespaces ){
				string.Append( element.CastToCharacterData()->GetData().GetTrimmedLeft() );
				
			}else{
				string.Append( element.CastToCharacterData()->GetData() );
			}
			skipLeadingWhitespaces = false;
			
		}else if( element.CanCastToCDSect() ){
			if( skipLeadingWhitespaces ){
				string.Append( element.CastToCDSect()->GetData().GetTrimmedLeft() );
				
			}else{
				string.Append( element.CastToCDSect()->GetData() );
			}
			skipLeadingWhitespaces = false;
			
		}else if( element.CanCastToElementTag() ){
			const decString &name = element.CastToElementTag()->GetName();
			
			if( name.EqualsInsensitive( "br" ) ){
				string.AppendCharacter( '\n' );
				skipLeadingWhitespaces = true;
				
			}else{
				break;
			}
			
		}else if( element.CanCastToAttValue() ){
			continue;
			
		}else if( element.CanCastToComment() ){
			continue;
			
		}else{
			break;
		}
	}
	
	return string;
}

void igdeBaseXML::ReadVector( const decXmlElementTag &tag, decVector &vector ){
	const decXmlAttValue *value;
	
	value = tag.FindAttribute( "x" );
	if( value ){
		vector.x = value->GetValue().ToFloat();
	}
	
	value = tag.FindAttribute( "y" );
	if( value ){
		vector.y = value->GetValue().ToFloat();
	}
	
	value = tag.FindAttribute( "z" );
	if( value ){
		vector.z = value->GetValue().ToFloat();
	}
}

void igdeBaseXML::ReadVector2( const decXmlElementTag &tag, decVector2 &vector ){
	const decXmlAttValue *value;
	
	value = tag.FindAttribute( "x" );
	if( value ){
		vector.x = value->GetValue().ToFloat();
	}
	
	value = tag.FindAttribute( "y" );
	if( value ){
		vector.y = value->GetValue().ToFloat();
	}
}

void igdeBaseXML::ReadCurveBezier( const decXmlElementTag &root, decCurveBezier &curve ){
	const int elementCount = root.GetElementCount();
	int i;
	
	curve.RemoveAllPoints();
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "interpolation" ) == 0 ){
			const char * const type = GetCDataString( *tag );
			
			if( strcmp( type, "constant" ) == 0 ){
				curve.SetInterpolationMode( decCurveBezier::eimConstant );
				
			}else if( strcmp( type, "linear" ) == 0 ){
				curve.SetInterpolationMode( decCurveBezier::eimLinear );
				
			}else if( strcmp( type, "bezier" ) == 0 ){
				curve.SetInterpolationMode( decCurveBezier::eimBezier );
				
			}else{
				LogErrorUnknownValue( *tag, type );
			}
			
		}else if( strcmp( tag->GetName(), "point" ) == 0 ){
			ReadCurveBezierPoint( *tag, curve );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void igdeBaseXML::ReadCurveBezierPoint( const decXmlElementTag &root, decCurveBezier &curve ){
	const int elementCount = root.GetElementCount();
	bool hasHandle1 = false;
	bool hasHandle2 = false;
	decVector2 coordinates;
	decVector2 handle1;
	decVector2 handle2;
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "coordinates" ) == 0 ){
			ReadVector2( *tag, coordinates );
			
		}else if( strcmp( tag->GetName(), "handle1" ) == 0 ){
			ReadVector2( *tag, handle1 );
			hasHandle1 = true;
			
		}else if( strcmp( tag->GetName(), "handle2" ) == 0 ){
			ReadVector2( *tag, handle2 );
			hasHandle2 = true;
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	if( ! hasHandle1 ){
		handle1 = coordinates;
	}
	if( ! hasHandle2 ){
		handle2 = coordinates;
	}
	curve.AddPoint( decCurveBezierPoint( coordinates, handle1, handle2 ) );
}

void igdeBaseXML::ReadColor( const decXmlElementTag &tag, decColor &color ){
	const decXmlAttValue *value;
	
	value = tag.FindAttribute( "r" );
	if( value ){
		color.r = value->GetValue().ToFloat();
	}
	
	value = tag.FindAttribute( "g" );
	if( value ){
		color.g = value->GetValue().ToFloat();
	}
	
	value = tag.FindAttribute( "b" );
	if( value ){
		color.b = value->GetValue().ToFloat();
	}
}

void igdeBaseXML::ReadColor4( const decXmlElementTag &tag, decColor &color ){
	const decXmlAttValue *value;
	
	value = tag.FindAttribute( "r" );
	if( value ){
		color.r = value->GetValue().ToFloat();
	}
	
	value = tag.FindAttribute( "g" );
	if( value ){
		color.g = value->GetValue().ToFloat();
	}
	
	value = tag.FindAttribute( "b" );
	if( value ){
		color.b = value->GetValue().ToFloat();
	}
	
	value = tag.FindAttribute( "a" );
	if( value ){
		color.a = value->GetValue().ToFloat();
	}
}

void igdeBaseXML::ReadPoint( const decXmlElementTag &tag, decPoint &point ){
	const decXmlAttValue *value;
	
	value = tag.FindAttribute( "x" );
	if( value ){
		point.x = value->GetValue().ToInt();
	}
	
	value = tag.FindAttribute( "y" );
	if( value ){
		point.y = value->GetValue().ToInt();
	}
}

void igdeBaseXML::ReadPoint3( const decXmlElementTag &tag, decPoint3 &point ){
	const decXmlAttValue *value;
	
	value = tag.FindAttribute( "x" );
	if( value ){
		point.x = value->GetValue().ToInt();
	}
	
	value = tag.FindAttribute( "y" );
	if( value ){
		point.y = value->GetValue().ToInt();
	}
	
	value = tag.FindAttribute( "z" );
	if( value ){
		point.z = value->GetValue().ToInt();
	}
}



void igdeBaseXML::WriteMultilineString( decXmlWriter &writer, const char *name, const char *string ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	if( string[ 0 ] ){
		writer.WriteOpeningTag( name, false, true, false );
		WriteTextMultilineString( writer, string );
		writer.WriteClosingTag( name, false );
		
	}else{
		writer.WriteOpeningTag( name, true );
	}
}

void igdeBaseXML::WriteTextMultilineString( decXmlWriter &writer, const char *string ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	const decString s( string );
	const int len = s.GetLength();
	int begin = 0;
	int i;
	
	for( i=0; i<len; i++ ){
		if( s.GetAt( i ) == '\n' ){
			writer.WriteTextString( s.GetMiddle( begin, i ) );
			begin = i + 1;
			writer.WriteOpeningTag( "br", true, false, false );
		}
	}
	
	if( begin < len ){
		writer.WriteTextString( s.GetMiddle( begin ) );
	}
}

void igdeBaseXML::WriteVector( decXmlWriter &writer, const char *name, const decVector &vector ){
	writer.WriteOpeningTagStart( name );
	writer.WriteAttributeFloat( "x", vector.x );
	writer.WriteAttributeFloat( "y", vector.y );
	writer.WriteAttributeFloat( "z", vector.z );
	writer.WriteOpeningTagEnd( true );
}

void igdeBaseXML::WriteVector2( decXmlWriter &writer, const char *name, const decVector2 &vector ){
	writer.WriteOpeningTagStart( name );
	writer.WriteAttributeFloat( "x", vector.x );
	writer.WriteAttributeFloat( "y", vector.y );
	writer.WriteOpeningTagEnd( true );
}

void igdeBaseXML::WriteCurveBezier( decXmlWriter &writer, const char *name, const decCurveBezier &curve ){
	const int pointCount = curve.GetPointCount();
	int p;
	
	writer.WriteOpeningTag( name, false, true );
	
	if( curve.GetInterpolationMode() == decCurveBezier::eimConstant ){
		writer.WriteDataTagString( "interpolation", "constant" );
		
	}else if( curve.GetInterpolationMode() == decCurveBezier::eimLinear ){
		writer.WriteDataTagString( "interpolation", "linear" );
		
	}else{
		writer.WriteDataTagString( "interpolation", "bezier" );
	}
	
	for( p=0; p<pointCount; p++ ){
		if( curve.GetInterpolationMode() == decCurveBezier::eimBezier ){
			WriteCurveBezierPoint( writer, "point", curve.GetPointAt( p ) );
			
		}else{
			WriteCurveBezierPointNoHandles( writer, "point", curve.GetPointAt( p ) );
		}
	}
	
	writer.WriteClosingTag( name, true );
}

void igdeBaseXML::WriteCurveBezierPoint( decXmlWriter &writer, const char *name,
const decCurveBezierPoint &point ){
	writer.WriteOpeningTag( name, false, true );
	WriteVector2( writer, "coordinates", point.GetPoint() );
	WriteVector2( writer, "handle1", point.GetHandle1() );
	WriteVector2( writer, "handle2", point.GetHandle2() );
	writer.WriteClosingTag( name, true );
}

void igdeBaseXML::WriteCurveBezierPointNoHandles( decXmlWriter &writer, const char *name,
const decCurveBezierPoint &point ){
	writer.WriteOpeningTag( name, false, true );
	WriteVector2( writer, "coordinates", point.GetPoint() );
	writer.WriteClosingTag( name, true );
}

void igdeBaseXML::WriteColor( decXmlWriter &writer, const char *name, const decColor &color ){
	writer.WriteOpeningTagStart( name );
	writer.WriteAttributeFloat( "r", color.r );
	writer.WriteAttributeFloat( "g", color.g );
	writer.WriteAttributeFloat( "b", color.b );
	writer.WriteOpeningTagEnd( true );
}

void igdeBaseXML::WriteColor4( decXmlWriter &writer, const char *name, const decColor &color ){
	writer.WriteOpeningTagStart( name );
	writer.WriteAttributeFloat( "r", color.r );
	writer.WriteAttributeFloat( "g", color.g );
	writer.WriteAttributeFloat( "b", color.b );
	writer.WriteAttributeFloat( "a", color.a );
	writer.WriteOpeningTagEnd( true );
}



void igdeBaseXML::LogWarnUnknownTag( const decXmlElementTag &root, const decXmlElementTag &tag ){
	pLogger->LogWarnFormat( pLoggerSource.GetString(), "%s(%i:%i): Unknown Tag %s",
		root.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(),
		tag.GetName().GetString() );
}

void igdeBaseXML::LogErrorUnknownTag( const decXmlElementTag &root, const decXmlElementTag &tag ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Unknown Tag %s",
		root.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(),
		tag.GetName().GetString() );
	DETHROW( deeInvalidParam );
}

void igdeBaseXML::LogWarnUnknownValue( const decXmlElementTag &tag, const char *value ){
	pLogger->LogWarnFormat( pLoggerSource.GetString(), "%s(%i:%i): Unknown Value %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(), value );
}

void igdeBaseXML::LogErrorUnknownValue( const decXmlElementTag &tag, const char *value ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Unknown Value %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(), value );
	DETHROW( deeInvalidParam );
}

void igdeBaseXML::LogWarnGenericProblemValue( const decXmlElementTag &tag,
const char *value, const char *problem ){
	pLogger->LogWarnFormat( pLoggerSource.GetString(), "%s(%i:%i): Value %s: %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(),
		value, problem );
}

void igdeBaseXML::LogErrorGenericProblemValue( const decXmlElementTag &tag,
const char *value, const char *problem ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Value %s: %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(),
		value, problem );
	DETHROW( deeInvalidParam );
}

void igdeBaseXML::LogWarnGenericProblem( const decXmlElementTag &tag, const char *problem ){
	pLogger->LogWarnFormat( pLoggerSource.GetString(), "%s(%i:%i): %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(), problem );
}

void igdeBaseXML::LogErrorGenericProblem( const decXmlElementTag &tag, const char *problem ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(), problem );
	DETHROW( deeInvalidParam );
}

void igdeBaseXML::LogWarnExceptionTag( const decXmlElementTag &tag,
const deException &exception ){
	const decStringList output = exception.FormatOutput();
	const int count = output.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pLogger->LogWarn( pLoggerSource.GetString(), output.GetAt( i ).GetString() );
	}
}

void igdeBaseXML::LogErrorExceptionTag( const decXmlElementTag &tag,
const deException &exception ){
	const decStringList output = exception.FormatOutput();
	const int count = output.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pLogger->LogError( pLoggerSource.GetString(), output.GetAt( i ).GetString() );
	}
}



void igdeBaseXML::LogWarnMissingTag( const decXmlElementTag &root, const char *tagName ){
	pLogger->LogWarnFormat( pLoggerSource.GetString(), "%s(%i:%i): Missing Tag %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(), tagName );
}

void igdeBaseXML::LogErrorMissingTag( const decXmlElementTag &root, const char *tagName ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Missing Tag %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(), tagName );
	DETHROW( deeInvalidParam );
}

void igdeBaseXML::LogWarnDuplicateTag( const decXmlElementTag &root, const char *tagName ){
	pLogger->LogWarnFormat( pLoggerSource.GetString(), "%s(%i:%i): Duplicate Tag %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(), tagName );
}

void igdeBaseXML::LogErrorDuplicateTag( const decXmlElementTag &root, const char *tagName ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Duplicate Tag %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(), tagName );
	DETHROW( deeInvalidParam );
}

void igdeBaseXML::LogWarnGenericProblemTag( const decXmlElementTag &root,
const char *tagName, const char *problem ){
	pLogger->LogWarnFormat( pLoggerSource.GetString(), "%s(%i:%i): Tag %s: %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(),
		tagName, problem );
}

void igdeBaseXML::LogErrorGenericProblemTag( const decXmlElementTag &root, const char *tagName,
const char *problem ){
	pLogger->LogErrorFormat( pLoggerSource.GetString(), "%s(%i:%i): Tag %s: %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(),
		tagName, problem );
	DETHROW( deeInvalidParam );
}
