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

void deoglShaderSources::SetPathComputeSourceCode( const char *path ){
	pPathSCCompute = path;
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
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "name" ) == 0 ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
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
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( cdata ){
				SetVersion( cdata->GetData() );
				
			}else{
				SetVersion( "120" );
			}
			
		}else if( strcmp( tag->GetName(), "texture" ) == 0 ){
			const decXmlAttValue *attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.texture(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			const decString &attrName = attribute->GetValue();
			
			attribute = pFindAttribute( *tag, "target" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.texture(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			
			pTextureList.Add( attrName, attribute->GetValue().ToInt() );
			
		}else if( strcmp( tag->GetName(), "attribute" ) == 0 ){
			const decXmlAttValue *attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.attribute(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			const decString &attrName = attribute->GetValue();
			
			attribute = pFindAttribute( *tag, "target" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.attribute(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			
			pAttributeList.Add( attrName, attribute->GetValue().ToInt() );
			
		}else if( strcmp( tag->GetName(), "output" ) == 0 ){
			const decXmlAttValue *attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.output(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			const decString &attrName = attribute->GetValue();
			
			attribute = pFindAttribute( *tag, "target" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.output(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			
			pOutputList.Add( attrName, attribute->GetValue().ToInt() );
			
		}else if( strcmp( tag->GetName(), "uniformBlock" ) == 0 ){
			const decXmlAttValue *attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.uniformBlock(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			const decString &attrName = attribute->GetValue();
			
			attribute = pFindAttribute( *tag, "target" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.uniformBlock(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			
			pUniformBlockList.Add( attrName, attribute->GetValue().ToInt() );
			
		}else if( strcmp( tag->GetName(), "shaderStorageBlock" ) == 0 ){
			const decXmlAttValue *attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.shaderStorageBlock(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			const decString &attrName = attribute->GetValue();
			
			attribute = pFindAttribute( *tag, "target" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.shaderStorageBlock(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			
			pShaderStorageBlockList.Add( attrName, attribute->GetValue().ToInt() );
			
		}else if( strcmp( tag->GetName(), "parameter" ) == 0 ){
			const decXmlAttValue *attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.parameter(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			
			pParameterList.Add( attribute->GetValue() );
			
		}else if( strcmp( tag->GetName(), "feedback" ) == 0 ){
			const decXmlAttValue *attribute = pFindAttribute( *tag, "name" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "shader.feedback(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			
			pFeedbackList.Add( attribute->GetValue() );
			
		}else if( strcmp( tag->GetName(), "sourceCode" ) == 0 ){
			const decXmlAttValue *attribute = pFindAttribute( *tag, "unit" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "sourceCode(%i:%i): Missing attribute 'unit'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			const decString &attrName = attribute->GetValue();
			
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			const decString &cdataValue = cdata ? cdata->GetData() : "";
			
			if( attrName == "compute" ){
				SetPathComputeSourceCode( cdataValue );
				
			}else if( attrName == "tessellationControl" ){
				SetPathTessellationControlSourceCode( cdataValue );
				
			}else if( attrName == "tessellationEvaluation" ){
				SetPathTessellationEvaluationSourceCode( cdataValue );
				
			}else if( attrName == "geometry" ){
				SetPathGeometrySourceCode( cdataValue );
				
			}else if( attrName == "vertex" ){
				SetPathVertexSourceCode( cdataValue );
				
			}else if( attrName == "fragment" ){
				SetPathFragmentSourceCode( cdataValue );
				
			}else{
				logger.LogErrorFormat( LOGGING_SOURCE, "sourceCode(%i:%i): Invalid unit '%s'!",
					tag->GetLineNumber(), tag->GetPositionNumber(), attrName.GetString() );
				DETHROW( deeInvalidParam );
			}
			
		}else if( strcmp( tag->GetName(), "program" ) == 0 ){ // DEPRECATED
			const decXmlAttValue *attribute = pFindAttribute( *tag, "unit" );
			if( ! attribute ){
				logger.LogErrorFormat( LOGGING_SOURCE, "program(%i:%i): Missing attribute 'unit'!",
					tag->GetLineNumber(), tag->GetPositionNumber() );
				DETHROW( deeInvalidParam );
			}
			const decString &attrName = attribute->GetValue();
			
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			const decString &cdataValue = cdata ? cdata->GetData() : "";
			
			if( attrName == "geometry" ){
				SetInlineGeometrySourceCode( cdataValue );
				
			}else if( attrName == "vertex" ){
				SetInlineVertexSourceCode( cdataValue );
				
			}else if( attrName == "fragment" ){
				SetInlineFragmentSourceCode( cdataValue );
				
			}else{
				logger.LogErrorFormat( LOGGING_SOURCE, "program(%i:%i): Invalid unit '%s'!",
					tag->GetLineNumber(), tag->GetPositionNumber(), attrName.GetString() );
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
