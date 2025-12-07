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

#ifndef _DEOGLRENDERABLECOLORTEXTUREMANAGER_H_
#define _DEOGLRENDERABLECOLORTEXTUREMANAGER_H_

class deoglRenderThread;
class deoglRenderableColorTexture;



/**
 * Renderable Color Texture Manager.
 * Manages renderable textures. Maintains a list of renderable textures build
 * upon demand. Renderable textures once created stay around and can be reused.
 * A renderable texture is only handed out if the format matches and it is not
 * in use by somebody else already. To give up use of a renderable texture use
 * the SetIsUse call with false as the parameter on the returned object.
 */
class deoglRenderableColorTextureManager{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderableColorTexture **pTextures;
	int pTextureCount;
	int pTextureSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable texture manager. */
	deoglRenderableColorTextureManager(deoglRenderThread &renderThread);
	/** Cleans up the renderable texture manager. */
	~deoglRenderableColorTextureManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of renderable textures. */
	inline int GetTextureCount() const{return pTextureCount;}
	/** Retrieves the renderable texture for reading only at the given location. */
	const deoglRenderableColorTexture *GetTextureAt(int index) const;
	
	/** Retrieves the renderable texture for the given format. */
	deoglRenderableColorTexture *GetTextureWith(int width, int height, int componentCount, bool isFloat);
	/*@}*/
};

#endif
