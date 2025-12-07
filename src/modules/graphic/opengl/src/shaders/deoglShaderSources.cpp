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

#include "deoglShaderSources.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
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

deoglShaderSources::deoglShaderSources(deLogger &logger, decBaseFileReader &reader){
	pFilename = reader.GetFilename();
	
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(&logger).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(! root || strcmp(root->GetName(), "shader") != 0){
		DETHROW(deeInvalidParam);
	}
	pParseShader(logger, *root);
}

deoglShaderSources::~deoglShaderSources(){
}



// Management
///////////////

void deoglShaderSources::AddParameter(const char *name, int location){
	pParameterList.Add(name);
	pParameterLocations.Add(location);
}

void deoglShaderSources::SetPathComputeSourceCode(const char *path){
	pPathSCCompute = path;
}

void deoglShaderSources::SetPathTessellationControlSourceCode(const char *path){
	pPathSCTessellationControl = path;
}

void deoglShaderSources::SetPathTessellationEvaluationSourceCode(const char* path){
	pPathSCTessellationEvaluation = path;
}

void deoglShaderSources::SetPathGeometrySourceCode(const char *path){
	pPathSCGeometry = path;
}

void deoglShaderSources::SetPathVertexSourceCode(const char *path){
	pPathSCVertex = path;
}

void deoglShaderSources::SetPathFragmentSourceCode(const char *path){
	pPathSCFragment = path;
}



// Private Functions
//////////////////////

decXmlElementTag *deoglShaderSources::pGetTagAt(const decXmlElementTag &tag, int index){
	decXmlElement * const element = tag.GetElementAt(index);
	
	if(element->CanCastToElementTag()){
		return element->CastToElementTag();
		
	}else{
		return NULL;
	}
}

decXmlAttValue *deoglShaderSources::pFindAttribute(const decXmlElementTag &tag, const char *name){
	int i;
	
	for(i=0; i<tag.GetElementCount(); i++){
		decXmlElement * const element = tag.GetElementAt(i);
		
		if(element->CanCastToAttValue()){
			decXmlAttValue * const value = element->CastToAttValue();
			if(strcmp(value->GetName(), name) == 0){
				return value;
			}
		}
	}
	
	return NULL;
}



void deoglShaderSources::pParseShader(deLogger &logger, const decXmlElementTag &root){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = pGetTagAt(root, i);
		if(! tag){
			continue;
		}
		
		if(strcmp(tag->GetName(), "name") == 0){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if(cdata){
				if(! pName.IsEmpty()){
					logger.LogWarnFormat(LOGGING_SOURCE, "shader.name(%i:%i): Duplicate Shader Name, ignored.",
						tag->GetLineNumber(), tag->GetPositionNumber());
					
				}else{
					pName = cdata->GetData();
				}
				
			}else{
				logger.LogErrorFormat(LOGGING_SOURCE, "shader.name(%i:%i): Zero-Length Shader Name not allowed!",
					tag->GetLineNumber(), tag->GetPositionNumber());
				DETHROW(deeInvalidParam);
			}
			
		}else if(strcmp(tag->GetName(), "texture") == 0){
			const decXmlAttValue *attribute = pFindAttribute(*tag, "name");
			if(! attribute){
				logger.LogErrorFormat(LOGGING_SOURCE, "shader.texture(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber());
				DETHROW(deeInvalidParam);
			}
			const decString &attrName = attribute->GetValue();
			
			attribute = pFindAttribute(*tag, "target");
			if(! attribute){
				logger.LogErrorFormat(LOGGING_SOURCE, "shader.texture(%i:%i): Missing attribute 'target'!",
					tag->GetLineNumber(), tag->GetPositionNumber());
				DETHROW(deeInvalidParam);
			}
			
			pTextureList.Add(attrName, attribute->GetValue().ToInt());
			
		}else if(strcmp(tag->GetName(), "parameter") == 0){
			const decXmlAttValue * const attribute = pFindAttribute(*tag, "name");
			if(! attribute){
				logger.LogErrorFormat(LOGGING_SOURCE, "shader.parameter(%i:%i): Missing attribute 'name'!",
					tag->GetLineNumber(), tag->GetPositionNumber());
				DETHROW(deeInvalidParam);
			}
			
			const decXmlAttValue * const attrLocation = pFindAttribute(*tag, "location");
			if(!attrLocation){
				logger.LogErrorFormat(LOGGING_SOURCE, "shader.parameter(%i:%i): Missing attribute 'location'!",
					tag->GetLineNumber(), tag->GetPositionNumber());
				DETHROW(deeInvalidParam);
			}
			
			AddParameter(attribute->GetValue(), attrLocation->GetValue().ToIntValid());
			
		}else if(strcmp(tag->GetName(), "sourceCode") == 0){
			const decXmlAttValue *attribute = pFindAttribute(*tag, "unit");
			if(! attribute){
				logger.LogErrorFormat(LOGGING_SOURCE, "sourceCode(%i:%i): Missing attribute 'unit'!",
					tag->GetLineNumber(), tag->GetPositionNumber());
				DETHROW(deeInvalidParam);
			}
			const decString &attrName = attribute->GetValue();
			
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			const char * const cdataValue = cdata ? cdata->GetData().GetString() : "";
			
			if(attrName == "compute"){
				SetPathComputeSourceCode(cdataValue);
				
			}else if(attrName == "tessellationControl"){
				SetPathTessellationControlSourceCode(cdataValue);
				
			}else if(attrName == "tessellationEvaluation"){
				SetPathTessellationEvaluationSourceCode(cdataValue);
				
			}else if(attrName == "geometry"){
				SetPathGeometrySourceCode(cdataValue);
				
			}else if(attrName == "vertex"){
				SetPathVertexSourceCode(cdataValue);
				
			}else if(attrName == "fragment"){
				SetPathFragmentSourceCode(cdataValue);
				
			}else{
				logger.LogErrorFormat(LOGGING_SOURCE, "sourceCode(%i:%i): Invalid unit '%s'!",
					tag->GetLineNumber(), tag->GetPositionNumber(), attrName.GetString());
				DETHROW(deeInvalidParam);
			}
			
		}else{
			logger.LogWarnFormat(LOGGING_SOURCE, "shader(%i:%i): Unknown Tag %s, ignoring.",
				tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
	
	if(pName.IsEmpty()){
		logger.LogError(LOGGING_SOURCE, "Missing shader name!\n");
		DETHROW(deeInvalidParam);
	}
}
