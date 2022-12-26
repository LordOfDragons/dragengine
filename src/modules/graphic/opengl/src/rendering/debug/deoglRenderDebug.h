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
class deoglTexture;
class deoglDebugFont;
class deoglDynamicTBOFloat32;
class deoglDynamicTBOFloat8;



/**
 * Debug rendering helper.
 */
class deoglRenderDebug : public deoglRenderBase{
private:
	struct sVBODataGlyph{
		decVector2 position;
		decVector2 texCoord;
		decColor color;
	};
	
	const deoglPipeline *pPipelineXRay;
	
	const deoglPipeline *pPipelineOutTexLayer;
	const deoglPipeline *pPipelineOutArrTex;
	
	const deoglPipeline *pPipelineRenderText;
	const deoglPipeline *pPipelineRectangle;
	
	deoglDebugFont *pDebugFont;
	
	deoglDynamicTBOFloat32 *pTBORenderText1;
	deoglDynamicTBOFloat8 *pTBORenderText2;
	
	deoglDynamicTBOFloat32 *pTBORenderRectangle1;
	deoglDynamicTBOFloat8 *pTBORenderRectangle2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderDebug( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	virtual ~deoglRenderDebug();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Debug font. */
	inline deoglDebugFont &GetDebugFont() const{ return *pDebugFont; }
	
	
	
	/** Display texture. */
	void DisplayTexture( deoglRenderPlan &plan, deoglTexture *texture, bool gammaCorrect );
	
	/** Display texture level. */
	void DisplayTextureLevel( deoglRenderPlan &plan, deoglTexture *texture, int level, bool gammaCorrect );
	
	/** Display array texture layer. */
	void DisplayArrayTextureLayer( deoglRenderPlan &plan, deoglArrayTexture *texture, int layer, bool gammaCorrect );
	
	/** Display array texture layer. */
	void DisplayArrayTextureLayerLevel( deoglRenderPlan &plan, deoglArrayTexture *texture, int layer, int level, bool gammaCorrect );
	
	
	
	/** Render component static information as colored boxes. */
	void RenderComponentsStatic( sRenderParameters &params );
	
	/** Render component using a colored box using the component extends. */
	void RenderComponentBox( sRenderParameters &params, deoglRComponent &component, const decColor &color );
	
	
	
	/** Begin render text. */
	void BeginRenderText();
	
	/** Add rendered text to TBO. */
	void AddRenderText( deoglRenderPlan &plan, const char *text, int x, int y, const decColor &color );
	
	/** Finish render text. */
	void EndRenderText();
	
	
	
	/** Begin render filled rectangle. */
	void BeginRenderRectangle();
	
	/** Add rendered filled rectangle to TBO. */
	void AddRenderRectangle( deoglRenderPlan &plan, int x1, int y1, int x2, int y2, const decColor &color );
	
	/** Finish render filled rectangle. */
	void EndRenderRectangle();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
