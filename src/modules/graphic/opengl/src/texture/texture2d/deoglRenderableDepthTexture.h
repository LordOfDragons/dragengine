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

#ifndef _DEOGLRENDERABLEDEPTHTEXTURE_H_
#define _DEOGLRENDERABLEDEPTHTEXTURE_H_

class deoglRenderThread;
class deoglTexture;



/**
 * Renderable depth texture.
 * 
 * Stores a renderable texture of any kind. Renderable textures can not be
 * used by more than one entity at the same time. They can also not change
 * the size of format once created. They can though be reused if the entity
 * holding the texture gives it up.
 */
class deoglRenderableDepthTexture{
private:
	int pWidth;
	int pHeight;
	bool pWithStencil;
	bool pUseFloat;
	bool pInUse;
	
	deoglTexture *pTexture;
	
	int pMemoryUsageGPU;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderable texture. */
	deoglRenderableDepthTexture(deoglRenderThread &renderThread,
		int width, int height, bool withStencil, bool useFloat);
	
	/** Clean up renderable texture. */
	~deoglRenderableDepthTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Width. */
	inline int GetWidth() const{return pWidth;}
	
	/** Height. */
	inline int GetHeight() const{return pHeight;}
	
	/** With stencil buffer. */
	inline bool GetWithStencil() const{return pWithStencil;}
	
	/** Use float depth. */
	inline bool GetUseFloat() const{return pUseFloat;}
	
	
	
	/** Format matche. */
	bool Matches(int width, int height, bool withStencil, bool useFloat) const;
	
	/** Texture is in use. */
	inline bool GetInUse() const{return pInUse;}
	
	/** Set if texture is in use. */
	void SetInUse(bool inUse);
	
	
	
	/** Texture. */
	inline deoglTexture *GetTexture() const{return pTexture;}
	
	
	
	/** GPU memory usage. */
	inline int GetMemoryUsageGPU() const{return pMemoryUsageGPU;}
	
	/** Update GPU memory consumption. */
	void UpdateMemoryUsage();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
