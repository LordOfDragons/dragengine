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

#ifndef _DEOGLRENDERDEBUG_H_
#define _DEOGLRENDERDEBUG_H_

#include "../deoglRenderBase.h"

class deoglArrayTexture;
class deoglDebugFont;
class deoglRComponent;
class deoglRenderPlan;
class deoglShaderProgram;
class deoglTexture;
class deoglDebugFont;



/**
 * \brief Debug rendering helper.
 */
class deoglRenderDebug : public deoglRenderBase{
private:
	deoglShaderProgram *pShaderXRay;
	deoglShaderProgram *pShaderSolid;
	
	deoglShaderProgram *pShaderSphere;
	
	deoglShaderProgram *pShaderOutTex;
	deoglShaderProgram *pShaderOutTexLayer;
	deoglShaderProgram *pShaderOutArrTex;
	
	deoglShaderProgram *pShaderRectangle;
	
	deoglDebugFont *pDebugFont;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderDebug( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderDebug();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** \brief Debug font. */
	inline deoglDebugFont &GetDebugFont() const{ return *pDebugFont; }
	
	
	
	/** \brief Display texture. */
	void DisplayTexture( deoglRenderPlan &plan, deoglTexture *texture, bool gammaCorrect );
	
	/** \brief Display texture level. */
	void DisplayTextureLevel( deoglRenderPlan &plan, deoglTexture *texture, int level, bool gammaCorrect );
	
	/** \brief Display array texture layer. */
	void DisplayArrayTextureLayer( deoglRenderPlan &plan, deoglArrayTexture *texture, int layer, bool gammaCorrect );
	
	
	
	/** \brief Render component static information as colored boxes. */
	void RenderComponentsStatic( sRenderParameters &params );
	
	/** \brief Render component using a colored box using the component extends. */
	void RenderComponentBox( sRenderParameters &params, deoglRComponent &component, const decColor &color );
	
	
	
	/** \brief Render text using the debug font. */
	void RenderText( deoglRenderPlan &plan, const char *text, int x, int y, const decColor &color );
	
	/** \brief Render filled rectangle. */
	void RenderRectangle( deoglRenderPlan &plan, int x1, int y1, int x2, int y2, const decColor &color );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
