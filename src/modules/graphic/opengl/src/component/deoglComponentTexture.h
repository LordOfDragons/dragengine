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

#ifndef _DEOGLCOMPONENTCOMPONENT_H_
#define _DEOGLCOMPONENTCOMPONENT_H_

#include "../skin/dynamic/deoglDynamicSkinListener.h"

class deoglComponent;
class deoglDynamicSkin;
class deoglRComponentTexture;
class deoglSkin;
class deoglSkinStateController;
class deComponentTexture;



/**
 * Component texture.
 */
class deoglComponentTexture : public deoglDynamicSkinListener{
private:
	deoglComponent &pComponent;
	const int pIndex;
	deoglRComponentTexture *pRTexture;
	
	deoglSkinStateController *pSkinStateController;
	
	deoglSkin *pSkin;
	deoglDynamicSkin *pDynamicSkin;
	
	bool pDirtyTexture;
	bool pDynamicSkinRenderablesChanged;
	bool pDirtyRenderableMapping;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component texture. */
	deoglComponentTexture(deoglComponent &component, int index);
	
	/** Clean up component texture. */
	~deoglComponentTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent component. */
	inline deoglComponent &GetComponent() const{ return pComponent; }
	
	/** Texture index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** Skin state controller. */
	inline deoglSkinStateController *GetSkinStateController() const{ return pSkinStateController; }
	
	/** Dynamic skin. */
	inline deoglDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Render component texture. */
	inline deoglRComponentTexture *GetRTexture() const{ return pRTexture; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	inline bool GetDynamicSkinRenderablesChanged() const{ return pDynamicSkinRenderablesChanged; }
	void SetDynamicSkinRenderablesChanged(bool changed);
	
	void DirtyRenderableMapping();
	
	
	/** Init skin state. */
	void InitSkinState();
	
	/** Advance time. */
	void AdvanceTime(float timeStep);
	
	/** Clear skin state controller. */
	void ClearSkinStateController();
	/*@}*/
	
	
	
	/** \name Dynamic skin listener */
	/*@{*/
	virtual void DynamicSkinDestroyed();
	virtual void DynamicSkinRenderablesChanged();
	virtual void DynamicSkinRenderableChanged(deoglDSRenderable &renderable);
	virtual void DynamicSkinRenderableRequiresSync(deoglDSRenderable &renderable);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Texture changed. */
	void TextureChanged(const deComponentTexture &texture);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
