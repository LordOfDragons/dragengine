/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglShaderSources.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGGING_SOURCE "OpenGL"



// Class deoglShaderSources
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderSources::deoglShaderSources(){
}

deoglShaderSources::deoglShaderSources( deLogger &logger, decBaseFileReader &reader ){
	pVersion = "120";
	pFilename = reader.GetFilename();
	
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( &logger ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "shader" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	pParseShader( logger, *root );
}

deoglShaderSources::~deoglShaderSources(){
}



// Management
///////////////

void deoglShaderSources::SetVersion( const char *version ){
	pVersion = version;
}

void deoglShaderSources::SetPathTessellationControlSourceCode( const char *path ){
	pPathSCTessellationControl = path;
}

void deoglShaderSources::SetPathTessellationEvaluationSourceCode( const char* path ){
	pPathSCTessellationEvaluation = path;
}

void deoglShaderSources::SetPathGeometrySourceCode( const char *path ){
	pPathSCGeometry = path;
}

void deoglShaderSources::SetPathVertexSourceCode( const char *path ){
	pPathSCVertex = path;
}

void deoglShaderSources::SetPathFragmentSourceCode( const char *path ){
	pPathSCFragment = path;
}

void deoglShaderSources::SetInlineGeometrySourceCode( const char *sourceCode ){
	pInlineSCGeometry = sourceCode;
}

void deoglShaderSources::SetInlineVertexSourceCode( const char *sourceCode ){
	pInlineSCVertex = sourceCode;
}

void deoglShaderSources::SetInlineFragmentSourceCode( const char *sourceCode ){
	pInlineSCFragment = sourceCode;
}



// Private Functions
//////////////////////

decXmlElementTag *deoglShaderSources::pGetTagAt( const decXmlElementTag &tag, int index ){
	decXmlElement * const element = tag.GetElementAt( index );
	
	if( element->CanCastToElementTag() ){
		return element->CastToElementTag();
		
	}else{
		return NULL;
	}
}

decXmlAttValue *deoglShaderSources::pFindAttribute( const decXmlElementTag &tag, const char *name ){
	int i;
	
	for( i=0; i<tag.GetElementCount(); i++ ){
		decXmlElement * const element = tag.GetElementAt( i );
		
		if( element->CanCastToAttValue() ){
			decXmlAttValue * const value = element->CastToAttValue();
			if( strcmp( value->GetName(), name ) == 0 ){
				return value;
			}
		}
	}
	
	return NULL;
}



void deoglShaderSources::pParseShader( deLogger &logger, const decXmlElementTag &root ){
	decXmlCharacterData *cdata;
	decXmlAttValue *attribute;
	const char *cdataValue;
	const char *attrName;
	int i, attrTarget;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "name" ) == 0 ){
			cdata = tag->GetFirstData();
			if( cdata ){
				if( ! pName.IsEmpty() ){
					logger.LogWarnFormat( LOGGING_SOURCE, "shader.name(%i:%i): Duplicate Shader Name, ignored.",
						tag->GetLineNumber(), tag->GetPositionNumber() );
					
				}else{
					pName = cdata->GetData();
				}
				
			}else{
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.name(%i:%i): Zero-Length Shader Name not allowed!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			
		}else if( strcmp( tag->GetName(), "version" ) == 0 ){
			cdata = tag->GetFirstData();
			if( cdata ){
				SetVersion( cdata->GetData() );
				
			}else{
				SetVersion( "120" );
			}
			
		}else if( strcmp( tag->GetName(), "texture" ) == 0 ){
			attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.texture(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrName = attribute->GetValue();
			
			attribute = pFindAttribute( *tag, "target" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.texture(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrTarget = ( int )strtol( attribute->GetValue(), NULL, 10 );
			
			pTextureList.Add( attrName, attrTarget );
			
		}else if( strcmp( tag->GetName(), "attribute" ) == 0 ){
			attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.attribute(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrName = attribute->GetValue();
			
			attribute = pFindAttribute( *tag, "target" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.attribute(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrTarget = ( int )strtol( attribute->GetValue(), NULL, 10 );
			
			pAttributeList.Add( attrName, attrTarget );
			
		}else if( strcmp( tag->GetName(), "output" ) == 0 ){
			attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.output(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrName = attribute->GetValue();
			
			attribute = pFindAttribute( *tag, "target" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.output(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrTarget = ( int )strtol( attribute->GetValue(), NULL, 10 );
			
			pOutputList.Add( attrName, attrTarget );
			
		}else if( strcmp( tag->GetName(), "uniformBlock" ) == 0 ){
			attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.uniformBlock(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrName = attribute->GetValue();
			
			attribute = pFindAttribute( *tag, "target" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.uniformBlock(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrTarget = ( int )strtol( attribute->GetValue(), NULL, 10 );
			
			pUniformBlockList.Add( attrName, attrTarget );
			
		}else if( strcmp( tag->GetName(), "shaderStorageBlock" ) == 0 ){
			attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.shaderStorageBlock(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrName = attribute->GetValue();
			
			attribute = pFindAttribute( *tag, "target" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.shaderStorageBlock(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrTarget = ( int )strtol( attribute->GetValue(), NULL, 10 );
			
			pShaderStorageBlockList.Add( attrName, attrTarget );
			
		}else if( strcmp( tag->GetName(), "parameter" ) == 0 ){
			attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.parameter(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			
			pParameterList.Add( attribute->GetValue() );
			
		}else if( strcmp( tag->GetName(), "feedback" ) == 0 ){
			attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.feedback(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			
			pFeedbackList.Add( attribute->GetValue() );
			
		}else if( strcmp( tag->GetName(), "sourceCode" ) == 0 ){
			attribute = pFindAttribute( *tag, "unit" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "sourceCode(%i:%i): Missing attribute 'unit'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrName = attribute->GetValue();
			
			cdata = tag->GetFirstData();
			if( cdata ){
				cdataValue = cdata->GetData();
				
			}else{
				cdataValue = "";
			}
			
			if( strcmp( attrName, "tessellationControl" ) == 0 ){
				SetPathTessellationControlSourceCode( cdataValue );
				
			}else if( strcmp( attrName, "tessellationEvaluation" ) == 0 ){
				SetPathTessellationEvaluationSourceCode( cdataValue );
				
			}else if( strcmp( attrName, "geometry" ) == 0 ){
				SetPathGeometrySourceCode( cdataValue );
				
			}else if( strcmp( attrName, "vertex" ) == 0 ){
				SetPathVertexSourceCode( cdataValue );
				
			}else if( strcmp( attrName, "fragment" ) == 0 ){
				SetPathFragmentSourceCode( cdataValue );
				
			}else{
				logger.LogErrorFormat( LOGGING_SOURCE, "sourceCode(%i:%i): Invalid unit '%s'!",
					tag->GetLineNumber(), tag->GetPositionNumber(), attrName );
				DETHROW( deeInvalidParam );
			}
			
		}else if( strcmp( tag->GetName(), "program" ) == 0 ){ // DEPRECATED
			attribute = pFindAttribute( *tag, "unit" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "program(%i:%i): Missing attribute 'unit'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			attrName = attribute->GetValue();
			
			cdata = tag->GetFirstData();
			if( cdata ){
				cdataValue = cdata->GetData();
				
			}else{
				cdataValue = "";
			}
			
			if( strcmp( attrName, "geometry" ) == 0 ){
				SetInlineGeometrySourceCode( cdataValue );
				
			}else if( strcmp( attrName, "vertex" ) == 0 ){
				SetInlineVertexSourceCode( cdataValue );
				
			}else if( strcmp( attrName, "fragment" ) == 0 ){
				SetInlineFragmentSourceCode( cdataValue );
				
			}else{
				logger.LogErrorFormat( LOGGING_SOURCE, "program(%i:%i): Invalid unit '%s'!",
					tag->GetLineNumber(), tag->GetPositionNumber(), attrName );
				DETHROW( deeInvalidParam );
			}
			
		}else{
			logger.LogWarnFormat( LOGGING_SOURCE, "shader(%i:%i): Unknown Tag %s, ignoring.",
				tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString() );
		}
	}
	
	if( pName.IsEmpty() ){
		logger.LogError( LOGGING_SOURCE, "Missing shader name!\n" );
		DETHROW( deeInvalidParam );
	}
}
