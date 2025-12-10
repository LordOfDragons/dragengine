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
#include <inttypes.h>
#include <string.h>

#include "deoglShaderUnitSourceCode.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>


// Class deoglShaderUnitSourceCode
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderUnitSourceCode::deoglShaderUnitSourceCode(deGraphicOpenGl &ogl, const decPath &path) :
pStage(0)
{
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	deVirtualFileSystem &vfs = ogl.GetVFS();
	decBaseFileReader::Ref reader(vfs.OpenFileForReading(path));
	decXmlParser(ogl.GetGameEngine()->GetLogger()).ParseXml(reader, xmlDoc);
	const uint64_t modTime = (uint64_t)reader->GetModificationTime();
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	const decXmlElementTag * const root = xmlDoc->GetRoot();
	DEASSERT_NOTNULL(root)
	DEASSERT_TRUE(root->GetName() == "shaderSource")
	
	const int count = root->GetElementCount();
	int i;
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = pGetTagAt(*root, i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "name"){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			DEASSERT_NOTNULL(cdata)
			
			pName = cdata->GetData();
			
		}else if(tagName == "source"){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			DEASSERT_NOTNULL(cdata)
			
			reader = vfs.OpenFileForReading(decPath::AbsolutePathUnix(
				cdata->GetData(), path.GetParent().GetPathUnix()));
			
			const int length = reader->GetLength();
			pSourceCode.Set(' ', length);
			reader->Read((char*)pSourceCode.GetString(), length);
			
		}else if(tagName == "define"){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			DEASSERT_NOTNULL(cdata)
			
			pDefines.Add(cdata->GetData());
			
		}else if(tagName == "stage"){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			DEASSERT_NOTNULL(cdata)
			
			const decString &value = cdata->GetData();
			if(value == "compute"){
				pStage = GL_COMPUTE_SHADER;
				
			}else if(value == "vertex"){
				pStage = GL_VERTEX_SHADER;
				
			}else if(value == "geometry"){
				pStage = GL_GEOMETRY_SHADER;
				
			}else if(value == "fragment"){
				pStage = GL_FRAGMENT_SHADER;
				
			}else if(value == "tessellateControl"){
				pStage = GL_TESS_CONTROL_SHADER;
				
			}else if(value == "tessellateEvaluate"){
				pStage = GL_TESS_EVALUATION_SHADER;
				
			}else{
				ogl.LogErrorFormat("%s %s(%d:%d): Invalid value '%s'.",
					path.GetPathUnix().GetString(), tagName.GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), value.GetString());
				DETHROW(deeInvalidParam);
			}
			
		}else if(tagName == "output" || tagName == "attribute"
		|| tagName == "uniformBlock" || tagName == "shaderStorageBlock"){
			// ignored on purpose
			
		}else{
			ogl.LogErrorFormat("%s %s(%d:%d): Unknown Tag %s.",
				path.GetPathUnix().GetString(), root->GetName().GetString(),
				tag->GetLineNumber(), tag->GetPositionNumber(), tagName.GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	DEASSERT_FALSE(pName.IsEmpty())
	DEASSERT_FALSE(pSourceCode.IsEmpty())
	DEASSERT_FALSE(pStage == 0)
	
	pValidationString.Format("%s: %" PRIu64, pName.GetString(), modTime);
}

const char *deoglShaderUnitSourceCode::GetLogStageName() const{
	switch(pStage){
	case GL_COMPUTE_SHADER:
		return "compute";
		
	case GL_VERTEX_SHADER:
		return "vertex";
		
	case GL_GEOMETRY_SHADER:
		return "geometry";
		
	case GL_FRAGMENT_SHADER:
		return "fragment";
		
	case GL_TESS_CONTROL_SHADER:
		return "tessellate control";
		
	case GL_TESS_EVALUATION_SHADER:
		return "tessellate evaluate";
		
	default:
		return "?";
	}
}


// Private Functions
//////////////////////

const decXmlElementTag *deoglShaderUnitSourceCode::pGetTagAt(
const decXmlElementTag & tag, int index) const{
	decXmlElement * const element = tag.GetElementAt(index);
	return element->CanCastToElementTag() ? element->CastToElementTag() : nullptr;
}
