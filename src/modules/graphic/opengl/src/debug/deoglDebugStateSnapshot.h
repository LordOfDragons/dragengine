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
	bool pEnabledRasterizerDiscard;
	bool pEnabledScissorTest;
	bool pEnabledStencilTest;
	
	GLint pBlendSrcRgb;
	GLint pBlendSrcAlpha;
	GLint pBlendDstRgb;
	GLint pBlendDstAlpha;
	
	GLboolean pColorWriteMask[ 4 ];
	GLint pCullFaceMode;
	GLint pDepthFunc;
	GLboolean pDepthWriteMask;
	GLint pPolygonMode;
	GLint pScissorBox[ 4 ];
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
	GLint pViewport[ 4 ];
	
	GLfloat pBlendColor[ 4 ];
	GLint pBlendEquationRgb;
	GLint pBlendEquationAlpha;
	
	GLfloat pPolygonOffsetFactor;
	GLfloat pPolygonOffsetUnits;
	
	int pStatesKeyWidth;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create state snapshot. */
	deoglDebugStateSnapshot( deoglRenderThread &renderThread );
	
	/** Clean up state snapshot. */
	~deoglDebugStateSnapshot();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Log state. */
	void LogState() const;
	
	/** Compare this state with another state logging difference. */
	void LogDifference( const deoglDebugStateSnapshot &state ) const;
	/*@}*/
	
	
	
private:
	decString pLogState( const char *key, const char *value ) const;
	decString pGLConstantName( int value ) const;
	decString pBitString( int value, int maxBits ) const;
};

#endif
