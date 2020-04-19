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

// include only once
#ifndef _DEOGLRENDERABLEDEPTHCUBEMAPMANAGER_H_
#define _DEOGLRENDERABLEDEPTHCUBEMAPMANAGER_H_

// predefinitions
class deoglRenderThread;
class deoglRenderableDepthCubeMap;



/**
 * @brief Renderable CubeMap Manager.
 *
 * Manages renderable cubemaps. Maintains a list of renderable cubemaps build
 * upon demand. Renderable cubemaps once created stay around and can be reused.
 * A renderable cubemap is only handed out if the format matches and it is not
 * in use by somebody else already. To give up use of a renderable cubemap use
 * the SetIsUse call with false as the parameter on the returned object.
 */
class deoglRenderableDepthCubeMapManager{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderableDepthCubeMap **pCubeMaps;
	int pCubeMapCount;
	int pCubeMapSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable cubemap manager. */
	deoglRenderableDepthCubeMapManager( deoglRenderThread &renderThread );
	/** Cleans up the renderable cubemap manager. */
	~deoglRenderableDepthCubeMapManager();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of renderable cubemaps. */
	inline int GetCubeMapCount() const{ return pCubeMapCount; }
	/** Retrieves the renderable cubemap for reading only at the given location. */
	const deoglRenderableDepthCubeMap *GetCubeMapAt( int index ) const;
	
	/** Retrieves the renderable cubemap for the given format. */
	deoglRenderableDepthCubeMap *GetCubeMapWith( int size );
	/*@}*/
};

// end of include only once
#endif
