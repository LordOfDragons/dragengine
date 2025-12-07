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
#include <string.h>
#include <stdlib.h>

#include "deoglDebugSnapshot.h"
#include "deoglDebugSaveTexture.h"
#include "../deoglBasics.h"
#include "../delayedoperation/deoglDelayedFileWrite.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTChoices.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Class deoglDebugSnapshot
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugSnapshot::deoglDebugSnapshot(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pName("Snapshot"),
pEnableStates(false),
pEnableDepth(false),
pEnableDepth2(false),
pEnableDepth3(false),
pEnableStencil(false),
pEnableStencil2(false),
pEnableColor(false),
pEnableDiffuse(false),
pEnableNormal(false),
pEnableReflectivity(false),
pEnableRoughness(false),
pEnableAOSolidity(false),
pEnablePostProcess(false),
pEnableTemporary1(false),
pEnableTemporary2(false),
pEnableTemporary3(false),
pStatesKeyWidth(35){
}

deoglDebugSnapshot::~deoglDebugSnapshot(){
}



// Management
///////////////

void deoglDebugSnapshot::SetName(const char *name){
	pName = name;
}



void deoglDebugSnapshot::SetEnableStates(bool enable){
	pEnableStates = enable;
}

void deoglDebugSnapshot::SetEnableDepth(bool enable){
	pEnableDepth = enable;
}

void deoglDebugSnapshot::SetEnableDepth2(bool enable){
	pEnableDepth2 = enable;
}

void deoglDebugSnapshot::SetEnableDepth3(bool enable){
	pEnableDepth3 = enable;
}

void deoglDebugSnapshot::SetEnableStencil(bool enable){
	pEnableStencil = enable;
}

void deoglDebugSnapshot::SetEnableStencil2(bool enable){
	pEnableStencil2 = enable;
}

void deoglDebugSnapshot::SetEnableColor(bool enable){
	pEnableColor = enable;
}

void deoglDebugSnapshot::SetEnableDiffuse(bool enable){
	pEnableDiffuse = enable;
}

void deoglDebugSnapshot::SetEnableNormal(bool enable){
	pEnableNormal = enable;
}

void deoglDebugSnapshot::SetEnableReflectivity(bool enable){
	pEnableReflectivity = enable;
}

void deoglDebugSnapshot::SetEnableRoughness(bool enable){
	pEnableRoughness = enable;
}

void deoglDebugSnapshot::SetEnableAOSolidity(bool enable){
	pEnableAOSolidity = enable;
}

void deoglDebugSnapshot::SetEnablePostProcess(bool enable){
	pEnablePostProcess = enable;
}

void deoglDebugSnapshot::SetEnableTemporary1(bool enable){
	pEnableTemporary1 = enable;
}

void deoglDebugSnapshot::SetEnableTemporary2(bool enable){
	pEnableTemporary2 = enable;
}

void deoglDebugSnapshot::SetEnableTemporary3(bool enable){
	pEnableTemporary3 = enable;
}



void deoglDebugSnapshot::SetEnableDepthStencil(bool enable){
	pEnableDepth = enable;
	pEnableStencil = enable;
}

void deoglDebugSnapshot::SetEnableDepthStencil2(bool enable){
	pEnableDepth2 = enable;
	pEnableStencil2 = enable;
}

void deoglDebugSnapshot::SetEnableMaterialBuffers(bool enable){
	pEnableDiffuse = enable;
	pEnableNormal = enable;
	pEnableReflectivity = enable;
	pEnableRoughness = enable;
	pEnableAOSolidity = enable;
}

void deoglDebugSnapshot::SetEnableAllBuffers(bool enable){
	pEnableDepth = enable;
	pEnableDepth2 = enable;
	pEnableStencil = enable;
	pEnableStencil2 = enable;
	pEnableColor = enable;
	pEnableDiffuse = enable;
	pEnableNormal = enable;
	pEnableReflectivity = enable;
	pEnableRoughness = enable;
	pEnableAOSolidity = enable;
	pEnablePostProcess = enable;
	pEnableTemporary1 = enable;
	pEnableTemporary2 = enable;
	pEnableTemporary3 = enable;
}



void deoglDebugSnapshot::TakeSnapshot() const{
	if(pEnableStates){
		pTakeSnapshotStates();
	}
	
	deoglDebugSaveTexture &dst = pRenderThread.GetDebug().GetDebugSaveTexture();
	deoglDeferredRendering &defren = pRenderThread.GetDeferredRendering();
	
	if(pEnableDepth || pEnableDepth2 || pEnableDepth3){
		deoglDebugSaveTexture::eDepthTypes depthType;
		
		if(pRenderThread.GetChoices().GetUseInverseDepth()){
			depthType = deoglDebugSaveTexture::edtDepthInverse;
			
		}else{
			depthType = deoglDebugSaveTexture::edtDepth;
		}
		
		if(pEnableDepth){
			dst.SaveDepthArrayTexture(*defren.GetDepthTexture1(), pName + "depth", depthType);
		}
		if(pEnableDepth2){
			dst.SaveDepthArrayTexture(*defren.GetDepthTexture2(), pName + "depth2", depthType);
		}
		if(pEnableDepth3){
			dst.SaveDepthArrayTexture(*defren.GetDepthTexture3(), pName + "depth3", depthType);
		}
	}
	
	if(pEnableStencil){
		dst.SaveStencilArrayTexture(*defren.GetDepthTexture1(), pName + "stencil");
	}
	if(pEnableStencil2){
		dst.SaveStencilArrayTexture(*defren.GetDepthTexture2(), pName + "stencil2");
	}
	
	if(pEnableColor){
		dst.SaveArrayTextureConversion(*defren.GetTextureColor(), pName + "color",
			deoglDebugSaveTexture::ecColorLinear2sRGB);
	}
	
	if(pEnableDiffuse){
		dst.SaveArrayTextureConversion(*defren.GetTextureDiffuse(), pName + "diffuse",
			deoglDebugSaveTexture::ecColorLinear2sRGB);
	}
	
	if(pEnableNormal){
		dst.SaveArrayTextureConversion(*defren.GetTextureNormal(), pName + "normal",
			deoglDebugSaveTexture::ecNormal);
	}
	
	if(pEnableReflectivity){
		dst.SaveArrayTextureConversion(*defren.GetTextureReflectivity(), pName + "reflectivity",
			deoglDebugSaveTexture::ecColorLinear2sRGB);
	}
	
	if(pEnableRoughness){
		dst.SaveArrayTextureConversion(*defren.GetTextureRoughness(), pName + "roughness",
			deoglDebugSaveTexture::ecNoConversion);
	}
	
	if(pEnableAOSolidity){
		dst.SaveArrayTextureConversion(*defren.GetTextureAOSolidity(), pName + "aosolidity",
			deoglDebugSaveTexture::ecNoConversion);
	}
	
	if(pEnablePostProcess){
		dst.SaveArrayTextureConversion(*defren.GetPostProcessTexture(), pName + "postprocess",
			deoglDebugSaveTexture::ecNoConversion);
	}
	
	if(pEnableTemporary1){
		dst.SaveArrayTextureConversion(*defren.GetTextureTemporary1(), pName + "temporary1",
			deoglDebugSaveTexture::ecColorLinear2sRGB);
	}
	
	if(pEnableTemporary2){
		dst.SaveArrayTextureConversion(*defren.GetTextureTemporary2(), pName + "temporary2",
			deoglDebugSaveTexture::ecColorLinear2sRGB);
	}
	
	if(pEnableTemporary3){
		dst.SaveArrayTextureConversion(*defren.GetTextureTemporary3(), pName + "temporary3",
			deoglDebugSaveTexture::ecColorLinear2sRGB);
	}
}



// Private Functions
//////////////////////

void deoglDebugSnapshot::pTakeSnapshotStates() const{
	deoglDelayedFileWrite *file = NULL;
	decBaseFileWriter *writer = NULL;
	
	decPath path;
	path.SetFromUnix(pRenderThread.GetDebug().GetDebugSaveTexture().GetBasePath());
	path.AddUnixPath(pName + "opengl_states.txt");
	
	GLint intValues[4];
	GLfloat floatValues[4];
	GLboolean boolValues[4];
	decString text;
	
	try{
		file = new deoglDelayedFileWrite(path);
		writer = file->GetFileWriter();
		
		pWriteLine(*writer, "// glEnable / glDisable //");
		pWriteLine(*writer, "/////////////////////////");
		pWriteLine(*writer, "");
		pWriteState(*writer, "GL_BLEND", glIsEnabled(GL_BLEND) ? "true": "false");
		pWriteState(*writer, "GL_CULL_FACE", glIsEnabled(GL_CULL_FACE) ? "true": "false");
		pWriteState(*writer, "GL_DEPTH_TEST", glIsEnabled(GL_DEPTH_TEST) ? "true": "false");
		pWriteState(*writer, "GL_POLYGON_OFFSET_FILL", glIsEnabled(GL_POLYGON_OFFSET_FILL) ? "true": "false");
		pWriteState(*writer, "GL_SCISSOR_TEST", glIsEnabled(GL_SCISSOR_TEST) ? "true": "false");
		pWriteState(*writer, "GL_STENCIL_TEST", glIsEnabled(GL_STENCIL_TEST) ? "true": "false");
		pWriteLine(*writer, "");
		
		pWriteLine(*writer, "// States //");
		pWriteLine(*writer, "///////////");
		pWriteLine(*writer, "");
		
		
		
		// glBlendFunc
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_BLEND_SRC_RGB, &intValues[0]));
		pWriteState(*writer, "GL_BLEND_SRC_RGB", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_BLEND_SRC_ALPHA, &intValues[0]));
		pWriteState(*writer, "GL_BLEND_SRC_ALPHA", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_BLEND_DST_RGB, &intValues[0]));
		pWriteState(*writer, "GL_BLEND_DST_RGB", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_BLEND_DST_ALPHA, &intValues[0]));
		pWriteState(*writer, "GL_BLEND_DST_ALPHA", pGLConstantName(intValues[0]));
		pWriteLine(*writer, "");
		
		
		
		// glColorMask
		OGL_CHECK(pRenderThread, glGetBooleanv(GL_COLOR_WRITEMASK, &boolValues[0]));
		text.Format("%s %s %s %s", boolValues[0] ? "true": "false", boolValues[1] ? "true": "false",
			         boolValues[2] ? "true": "false", boolValues[3] ? "true": "false");
		pWriteState(*writer, "GL_COLOR_WRITEMASK", text);
		
		pWriteLine(*writer, "");
		
		
		
		// glCullFace
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_CULL_FACE_MODE, &intValues[0]));
		pWriteState(*writer, "GL_CULL_FACE_MODE", pGLConstantName(intValues[0]));
		
		pWriteLine(*writer, "");
		
		
		
		// glDepthFunc
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_DEPTH_FUNC, &intValues[0]));
		pWriteState(*writer, "GL_DEPTH_FUNC", pGLConstantName(intValues[0]));
		
		pWriteLine(*writer, "");
		
		
		
		// glDepthMask
		OGL_CHECK(pRenderThread, glGetBooleanv(GL_DEPTH_WRITEMASK, &boolValues[0]));
		pWriteState(*writer, "GL_DEPTH_WRITEMASK", boolValues[0] ? "true": "false");
		
		pWriteLine(*writer, "");
		
		
		
		// glPolygonMode
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_POLYGON_MODE, &intValues[0]));
		pWriteState(*writer, "GL_POLYGON_MODE", pGLConstantName(intValues[0]));
		
		pWriteLine(*writer, "");
		
		
		
		// glScissor
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_SCISSOR_BOX, &intValues[0]));
		text.Format("x,y=(%d,%d) w,h=(%d,%d)", intValues[0], intValues[1], intValues[2], intValues[3]);
		pWriteState(*writer, "GL_SCISSOR_BOX", text);
		
		pWriteLine(*writer, "");
		
		
		
		// glStencilFunc
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_BITS, &intValues[0]));
		pWriteState(*writer, "GL_STENCIL_BITS", pGLConstantName(intValues[0]));
		
		const int stencilBits = intValues[0];
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_FUNC, &intValues[0]));
		pWriteState(*writer, "GL_STENCIL_FUNC", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_VALUE_MASK, &intValues[0]));
		text.Format("%02x (%s)", intValues[0], pBitString(intValues[0], stencilBits).GetString());
		pWriteState(*writer, "GL_STENCIL_VALUE_MASK", text);
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_REF, &intValues[0]));
		text.Format("%02x (%s)", intValues[0], pBitString(intValues[0], stencilBits).GetString());
		pWriteState(*writer, "GL_STENCIL_REF", text);
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_BACK_FUNC, &intValues[0]));
		pWriteState(*writer, "GL_STENCIL_BACK_FUNC", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_BACK_VALUE_MASK, &intValues[0]));
		text.Format("%02x (%s)", intValues[0], pBitString(intValues[0], stencilBits).GetString());
		pWriteState(*writer, "GL_STENCIL_BACK_VALUE_MASK", text);
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_BACK_REF, &intValues[0]));
		text.Format("%02x (%s)", intValues[0], pBitString(intValues[0], stencilBits).GetString());
		pWriteState(*writer, "GL_STENCIL_BACK_REF", text);
		
		pWriteLine(*writer, "");
		
		
		
		// glStencilMask
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_WRITEMASK, &intValues[0]));
		text.Format("%02x (%s)", intValues[0], pBitString(intValues[0], stencilBits).GetString());
		pWriteState(*writer, "GL_STENCIL_WRITEMASK", text);
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_BACK_WRITEMASK, &intValues[0]));
		text.Format("%02x (%s)", intValues[0], pBitString(intValues[0], stencilBits).GetString());
		pWriteState(*writer, "GL_STENCIL_BACK_WRITEMASK", text);
		
		pWriteLine(*writer, "");
		
		
		
		// glStencilOp
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_FAIL, &intValues[0]));
		pWriteState(*writer, "GL_STENCIL_FAIL", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &intValues[0]));
		pWriteState(*writer, "GL_STENCIL_PASS_DEPTH_PASS", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &intValues[0]));
		pWriteState(*writer, "GL_STENCIL_PASS_DEPTH_FAIL", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_BACK_FAIL, &intValues[0]));
		pWriteState(*writer, "GL_STENCIL_BACK_FAIL", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS, &intValues[0]));
		pWriteState(*writer, "GL_STENCIL_BACK_PASS_DEPTH_PASS", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL, &intValues[0]));
		pWriteState(*writer, "GL_STENCIL_BACK_PASS_DEPTH_FAIL", pGLConstantName(intValues[0]));
		
		pWriteLine(*writer, "");
		
		
		
		// glViewport
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_VIEWPORT, &intValues[0]));
		text.Format("x,y=(%d,%d) w,h=(%d,%d)", intValues[0], intValues[1], intValues[2], intValues[3]);
		pWriteState(*writer, "GL_VIEWPORT", text);
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &intValues[0]));
		text.Format("w,h=(%d,%d)", intValues[0], intValues[1]);
		pWriteState(*writer, "GL_MAX_VIEWPORT_DIMS", text);
		
		pWriteLine(*writer, "");
		
		
		
		// pglBlendColor
		OGL_CHECK(pRenderThread, glGetFloatv(GL_BLEND_COLOR, &floatValues[0]));
		text.Format("(%g, %g, %g, %g)", floatValues[0], floatValues[1], floatValues[2], floatValues[3]);
		pWriteState(*writer, "GL_BLEND_COLOR", text);
		
		pWriteLine(*writer, "");
		
		
		
		// pglBlendEquation
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_BLEND_EQUATION_RGB, &intValues[0]));
		pWriteState(*writer, "GL_BLEND_EQUATION_RGB", pGLConstantName(intValues[0]));
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &intValues[0]));
		pWriteState(*writer, "GL_BLEND_EQUATION_ALPHA", pGLConstantName(intValues[0]));
		
		pWriteLine(*writer, "");
		
		
		
		// pglPolygonOffset
		OGL_CHECK(pRenderThread, glGetFloatv(GL_POLYGON_OFFSET_FACTOR, &floatValues[0]));
		text.Format("%g", floatValues[0]);
		pWriteState(*writer, "GL_POLYGON_OFFSET_FACTOR", text);
		
		OGL_CHECK(pRenderThread, glGetFloatv(GL_POLYGON_OFFSET_UNITS, &floatValues[0]));
		text.Format("%g", floatValues[0]);
		pWriteState(*writer, "GL_POLYGON_OFFSET_UNITS", text);
		
		//pWriteLine( *writer, "");
		
		
		
		writer->FreeReference();
		writer = NULL;
		
		pRenderThread.GetDelayedOperations().AddFileWrite(file);
		
	}catch(const deException &e){
		if(writer){
			writer->FreeReference();
		}
		if(file){
			delete file;
		}
		pRenderThread.GetLogger().LogException(e);
	}
}

void deoglDebugSnapshot::pWriteLine(decBaseFileWriter &writer, const char *text) const{
	writer.Write(text, (int)strlen(text));
	writer.WriteByte('\n');
}

void deoglDebugSnapshot::pWriteState(decBaseFileWriter &writer, const char *key, const char *value) const{
	decString text(key);
	//text.AppendCharacter( ':' );
	text.AppendCharacter(' ');
	
	const int len = text.GetLength();
	int i;
	for(i=len; i<=pStatesKeyWidth; i++){
		//text.AppendCharacter( ' ' );
		text.AppendCharacter('.');
	}
	
	text.AppendCharacter(' ');
	text.Append(value);
	
	pWriteLine(writer, text);
}

struct sNameTableEntry{
	const int value;
	const char *name;
};
static sNameTableEntry vGLConstantNameTable[] = {
	{GL_ZERO, "GL_ZERO"},
	{GL_ONE, "GL_ONE"},
	{GL_SRC_COLOR, "GL_SRC_COLOR"},
	{GL_ONE_MINUS_SRC_COLOR, "GL_ONE_MINUS_SRC_COLOR"},
	{GL_DST_COLOR, "GL_DST_COLOR"},
	{GL_ONE_MINUS_DST_COLOR, "GL_ONE_MINUS_DST_COLOR"},
	{GL_SRC_ALPHA, "GL_SRC_ALPHA"},
	{GL_ONE_MINUS_SRC_ALPHA, "GL_ONE_MINUS_SRC_ALPHA"},
	{GL_DST_ALPHA, "GL_DST_ALPHA"},
	{GL_ONE_MINUS_DST_ALPHA, "GL_ONE_MINUS_DST_ALPHA"},
	{GL_CONSTANT_COLOR, "GL_CONSTANT_COLOR"},
	{GL_ONE_MINUS_CONSTANT_COLOR, "GL_ONE_MINUS_CONSTANT_COLOR"},
	{GL_CONSTANT_ALPHA, "GL_CONSTANT_ALPHA"},
	{GL_ONE_MINUS_CONSTANT_ALPHA, "GL_ONE_MINUS_CONSTANT_ALPHA"},
	{GL_SRC_ALPHA_SATURATE, "GL_SRC_ALPHA_SATURATE"},
	{GL_SRC1_COLOR, "GL_SRC1_COLOR"},
	{GL_ONE_MINUS_SRC1_COLOR, "GL_ONE_MINUS_SRC1_COLOR"},
	{GL_SRC1_ALPHA, "GL_SRC1_ALPHA"},
	{GL_ONE_MINUS_SRC1_ALPHA, "GL_ONE_MINUS_SRC1_ALPHA"},
	
	{GL_FRONT, "GL_FRONT"},
	{GL_BACK, "GL_BACK"},
	{GL_FRONT_AND_BACK, "GL_FRONT_AND_BACK"},
	
	{GL_NEVER, "GL_NEVER"},
	{GL_LESS, "GL_LESS"},
	{GL_EQUAL, "GL_EQUAL"},
	{GL_LEQUAL, "GL_LEQUAL"},
	{GL_GREATER, "GL_GREATER"},
	{GL_NOTEQUAL, "GL_NOTEQUAL"},
	{GL_GEQUAL, "GL_GEQUAL"},
	{GL_ALWAYS, "GL_ALWAYS"},
	
	{GL_POINT, "GL_POINT"},
	{GL_LINE, "GL_LINE"},
	{GL_FILL, "GL_FILL"},
	
	{GL_KEEP, "GL_KEEP"},
	{GL_REPLACE, "GL_REPLACE"},
	{GL_INCR, "GL_INCR"},
	{GL_INCR_WRAP, "GL_INCR_WRAP"},
	{GL_DECR, "GL_DECR"},
	{GL_DECR_WRAP, "GL_DECR_WRAP"},
	{GL_INVERT, "GL_INVERT"},
	
	{GL_FUNC_ADD, "GL_FUNC_ADD"},
	{GL_FUNC_SUBTRACT, "GL_FUNC_SUBTRACT"},
	{GL_FUNC_REVERSE_SUBTRACT, "GL_FUNC_REVERSE_SUBTRACT"},
	{GL_MIN, "GL_MIN"},
	{GL_MAX, "GL_MAX"},
	
	{0, NULL}
};

decString deoglDebugSnapshot::pGLConstantName(int value) const{
	int i;
	
	for(i=0; vGLConstantNameTable[i].name; i++){
		if(vGLConstantNameTable[i].value == value){
			return decString(vGLConstantNameTable[i].name);
		}
	}
	
	decString text;
	text.Format("%x", value);
	return text;
}

decString deoglDebugSnapshot::pBitString(int value, int maxBits) const{
	decString text;
	int i;
	
	for(i=1; i<=maxBits; i++){
		if(i > 1 && (i - 1) % 4 == 0){
			text.AppendCharacter(' ');
		}
		
		if((value & (1 << (maxBits - i))) == 0){
			text.AppendCharacter('0');
			
		}else{
			text.AppendCharacter('1');
		}
	}
	
	return text;
}
