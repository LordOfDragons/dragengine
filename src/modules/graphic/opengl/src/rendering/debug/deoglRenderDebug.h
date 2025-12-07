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

#ifndef _DEOGLRENDERDEBUG_H_
#define _DEOGLRENDERDEBUG_H_

#include "../deoglRenderBase.h"
#include "deoglDynamicTBOFloat32.h"
#include "deoglDynamicTBOFloat8.h"

class deoglArrayTexture;
class deoglDebugFont;
class deoglRComponent;
class deoglRenderPlan;
class deoglTexture;
class deoglDebugFont;



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
	
	deoglDynamicTBOFloat32::Ref pTBORenderText1;
	deoglDynamicTBOFloat8::Ref pTBORenderText2;
	
	deoglDynamicTBOFloat32::Ref pTBORenderRectangle1;
	deoglDynamicTBOFloat8::Ref pTBORenderRectangle2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderDebug(deoglRenderThread &renderThread);
	
	/** Clean up renderer. */
	~deoglRenderDebug() override;
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Debug font. */
	inline deoglDebugFont &GetDebugFont() const{ return *pDebugFont; }
	
	
	
	/** Display texture. */
	void DisplayTexture(deoglRenderPlan &plan, deoglTexture *texture, bool gammaCorrect);
	
	/** Display texture level. */
	void DisplayTextureLevel(deoglRenderPlan &plan, deoglTexture *texture, int level, bool gammaCorrect);
	
	/** Display array texture layer. */
	void DisplayArrayTextureLayer(deoglRenderPlan &plan, deoglArrayTexture *texture, int layer, bool gammaCorrect);
	
	/** Display array texture layer. */
	void DisplayArrayTextureLayerLevel(deoglRenderPlan &plan, deoglArrayTexture *texture, int layer, int level, bool gammaCorrect);
	
	
	
	/** Render component static information as colored boxes. */
	void RenderComponentsStatic(sRenderParameters &params);
	
	/** Render component using a colored box using the component extends. */
	void RenderComponentBox(sRenderParameters &params, deoglRComponent &component, const decColor &color);
	
	
	
	/** Begin render text. */
	void BeginRenderText();
	
	/** Add rendered text to TBO. */
	void AddRenderText(const decPoint &viewport, const char *text, int x, int y, const decColor &color);
	
	/** Finish render text. */
	void EndRenderText();
	
	
	
	/** Begin render filled rectangle. */
	void BeginRenderRectangle();
	
	/** Add rendered filled rectangle to TBO. */
	void AddRenderRectangle(const decPoint &viewport, int x1, int y1, int x2, int y2, const decColor &color);
	
	/** Finish render filled rectangle. */
	void EndRenderRectangle();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
