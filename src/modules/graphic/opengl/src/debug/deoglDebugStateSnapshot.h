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

#ifndef _DEOGLDEBUGSTATESNAPSHOT_H_
#define _DEOGLDEBUGSTATESNAPSHOT_H_

#include "../deoglBasics.h"

#include <dragengine/common/string/decString.h>

class deoglRenderThread;


/**
 * OpenGL state snapshot.
 * 
 * Supports comparing snapshots to find differences.
 */
class deoglDebugStateSnapshot{
private:
	deoglRenderThread &pRenderThread;
	
	bool pEnabledBlend;
	bool pEnabledCullFace;
	bool pEnabledDepthTest;
	bool pEnabledPolygonOffsetFill;
	bool pEnabledScissorTest;
	bool pEnabledStencilTest;
	
	GLint pBlendSrcRgb;
	GLint pBlendSrcAlpha;
	GLint pBlendDstRgb;
	GLint pBlendDstAlpha;
	
	GLboolean pColorWriteMask[4];
	GLint pCullFaceMode;
	GLint pDepthFunc;
	GLboolean pDepthWriteMask;
	GLint pPolygonMode;
	GLint pScissorBox[4];
	GLint pStencilBits;
	GLint pStencilFunc;
	GLint pStencilValueMask;
	GLint pStencilRef;
	GLint pStencilBackFunc;
	GLint pStencilBackValueMask;
	GLint pStencilBackRef;
	GLint pStencilWriteMask;
	GLint pStencilBackWriteMask;
	GLint pStencilFail;
	GLint pStencilPassDepthPass;
	GLint pStencilPassDepthFail;
	GLint pStencilBackFail;
	GLint pStencilBackPassDepthPass;
	GLint pStencilBackPassDepthFail;
	GLint pViewport[4];
	
	GLfloat pBlendColor[4];
	GLint pBlendEquationRgb;
	GLint pBlendEquationAlpha;
	
	GLfloat pPolygonOffsetFactor;
	GLfloat pPolygonOffsetUnits;
	
	int pStatesKeyWidth;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create state snapshot. */
	deoglDebugStateSnapshot(deoglRenderThread &renderThread);
	
	/** Clean up state snapshot. */
	~deoglDebugStateSnapshot();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Log state. */
	void LogState() const;
	
	/** Compare this state with another state logging difference. */
	void LogDifference(const deoglDebugStateSnapshot &state) const;
	/*@}*/
	
	
	
private:
	decString pLogState(const char *key, const char *value) const;
	decString pGLConstantName(int value) const;
	decString pBitString(int value, int maxBits) const;
};

#endif
