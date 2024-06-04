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

// include only once
#ifndef _DEOGLRENDERABLEDEPTHARRAYTEXTUREMANAGER_H_
#define _DEOGLRENDERABLEDEPTHARRAYTEXTUREMANAGER_H_

// predefinitions
class deoglRenderThread;
class deoglRenderableDepthArrayTexture;



/**
 * Manages renderable textures. Maintains a list of renderable textures build
 * upon demand. Renderable textures once created stay around and can be reused.
 * A renderable texture is only handed out if the format matches and it is not
 * in use by somebody else already. To give up use of a renderable texture use
 * the SetIsUse call with false as the parameter on the returned object.
 */
class deoglRenderableDepthArrayTextureManager{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderableDepthArrayTexture **pArrayTextures;
	int pArrayTextureCount;
	int pArrayTextureSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderable texture manager. */
	deoglRenderableDepthArrayTextureManager( deoglRenderThread &renderThread );
	
	/** Clean up renderable texture manager. */
	~deoglRenderableDepthArrayTextureManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of renderable textures. */
	inline int GetCount() const{ return pArrayTextureCount; }
	
	/** Renderable texture at index for reading purpose only. */
	const deoglRenderableDepthArrayTexture *GetAt( int index ) const;
	
	/** Renderable texture matching format. */
	deoglRenderableDepthArrayTexture *GetWith( int width, int height, int layerCount,
		bool withStencil, bool useFloat );
	/*@}*/
};

#endif
