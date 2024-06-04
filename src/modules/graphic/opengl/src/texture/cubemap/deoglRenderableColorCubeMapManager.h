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
#ifndef _DEOGLRENDERABLECOLORCUBEMAPMANAGER_H_
#define _DEOGLRENDERABLECOLORCUBEMAPMANAGER_H_

// predefinitions
class deoglRenderThread;
class deoglRenderableColorCubeMap;



/**
 * Renderable Color Cube Manager.
 *
 * Manages renderable cubemaps. Maintains a list of renderable cubemaps build
 * upon demand. Renderable cubemaps once created stay around and can be reused.
 * A renderable cubemap is only handed out if the format matches and it is not
 * in use by somebody else already. To give up use of a renderable cubemap use
 * the SetIsUse call with false as the parameter on the returned object.
 */
class deoglRenderableColorCubeMapManager{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderableColorCubeMap **pCubeMaps;
	int pCubeMapCount;
	int pCubeMapSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable cubemap manager. */
	deoglRenderableColorCubeMapManager( deoglRenderThread &renderThread );
	/** Cleans up the renderable cubemap manager. */
	~deoglRenderableColorCubeMapManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of renderable cubemaps. */
	inline int GetCubeMapCount() const{ return pCubeMapCount; }
	/** Retrieves the renderable cubemap for reading only at the given location. */
	const deoglRenderableColorCubeMap *GetCubeMapAt( int index ) const;
	
	/** Retrieves the renderable cubemap for the given format. */
	deoglRenderableColorCubeMap *GetCubeMapWith( int size, int componentCount, bool isFloat );
	/*@}*/
};

// end of include only once
#endif
