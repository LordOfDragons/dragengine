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

#ifndef _DEOGLDECAL_H_
#define _DEOGLDECAL_H_

#include "../skin/dynamic/deoglDynamicSkinListener.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicDecal.h>
#include "deoglRDecal.h"

class deoglDynamicSkin;
class deoglComponent;

class deGraphicOpenGl;
class deDecal;



/**
 * Decal peer.
 */
class deoglDecal : public deBaseGraphicDecal, deoglDynamicSkinListener{
public:
	deGraphicOpenGl &pOgl;
	const deDecal &pDecal;
	
	deoglRDecal::Ref pRDecal;
	
	deoglDynamicSkin *pDynamicSkin;
	
	bool pDirtyVBO;
	bool pDirtyGeometry;
	bool pDirtyTransform;
	bool pDirtySkin;
	bool pDirtyDynamicSkin;
	bool pDirtyVisibility;
	bool pDirtyParamBlocks;
	bool pDirtyRenderableMapping;
	bool pDirtyStaticTexture;
	
	bool pDynamicSkinRequiresSync;
	
	bool pNotifyTextureChanged;
	bool pNotifyTUCChanged;
	
	deoglComponent *pParentComponent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglDecal(deGraphicOpenGl &ogl, const deDecal &decal);
	
	/** Clean up peer. */
	~deoglDecal() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Decal engine resource. */
	inline const deDecal &GetDecal() const{ return pDecal; }
	
	
	
	/** Render decal. */
	inline const deoglRDecal::Ref &GetRDecal() const{ return pRDecal; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Parent component or \em NULL. */
	inline deoglComponent *GetParentComponent() const{ return pParentComponent; }
	
	/** Set parent component or \em NULL. */
	void SetParentComponent(deoglComponent *component);
	/*@}*/
	
	
	
	/** \name Dynamic skin listener */
	/*@{*/
	void DynamicSkinDestroyed() override;
	void DynamicSkinRenderablesChanged() override;
	void DynamicSkinRenderableChanged(deoglDSRenderable &renderable) override;
	void DynamicSkinRenderableRequiresSync(deoglDSRenderable &renderable) override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position, orientation or size changed. */
	virtual void GeometryChanged();
	
	/** Texture coordinates transformation changed. */
	virtual void TransformChanged();
	
	/** Skin changed. */
	virtual void SkinChanged();
	
	/** Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	
	/** Visible changed. */
	virtual void VisibleChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pSyncSkin();
	void pSyncDynamicSkin();
	
	void pRequiresSync();
};

#endif
