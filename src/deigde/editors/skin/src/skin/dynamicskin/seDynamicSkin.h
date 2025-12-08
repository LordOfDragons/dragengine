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

#ifndef _SEDYNAMICSKIN_H_
#define _SEDYNAMICSKIN_H_

#include "seDynamicSkinRenderableList.h"

class seSkin;

class deDynamicSkin;



/**
 * @brief Dynamic Skin.
 * Manages a dynamic skin used to test the dynamic part of skin definitions.
 */
class seDynamicSkin{
private:
	seSkin *pParentSkin;
	
	deDynamicSkin::Ref pEngDynamicSkin;
	
	seDynamicSkinRenderableList pRenderableList;
	seDynamicSkinRenderable::Ref pActiveRenderable;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dynamic skin. */
	seDynamicSkin(seSkin *parentSkin);
	/** Cleans up the dynamic skin. */
	~seDynamicSkin();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent skin. */
	inline seSkin *GetParentSkin() const{ return pParentSkin; }
	
	/** Retrieves the engine dynamic skin. */
	inline deDynamicSkin *GetEngineDynamicSkin() const{ return pEngDynamicSkin; }
	
	/** Dispose of all resources. */
	void Dispose();
	/** Updates the dynamic skin. */
	void Update(float elapsed);
	/** Resets the dynamic skin. */
	void Reset();
	/*@}*/
	
	/** @name Renderables */
	/*@{*/
	/** Retrieves the renderable list read-only. */
	inline const seDynamicSkinRenderableList &GetRenderableList() const{ return pRenderableList; }
	/** Adds a new renderable. */
	void AddRenderable(seDynamicSkinRenderable::Ref renderable);
	/** Removes a renderable. */
	void RemoveRenderable(seDynamicSkinRenderable::Ref renderable);
	/** Removes all renderables. */
	void RemoveAllRenderables();
	/** Retrieves the active renderable or NULL if none is active. */
	inline seDynamicSkinRenderable *GetActiveRenderable() const{ return pActiveRenderable; }
	/** Determines if there is an active renderable or not. */
	bool HasActiveRenderable() const;
	/** Sets the active renderable or NULL if none is active. */
	void SetActiveRenderable(seDynamicSkinRenderable::Ref renderable);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
