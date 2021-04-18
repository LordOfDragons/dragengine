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

#ifndef _DEOGLRDYNAMICSKIN_H_
#define _DEOGLRDYNAMICSKIN_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

class deoglRDSRenderable;
class deoglRenderThread;



/**
 * Render dynamic skin.
 */
class deoglRDynamicSkin : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	decObjectList pRenderables;
	int pUpdateNumber;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render dynamic skin. */
	deoglRDynamicSkin( deoglRenderThread &renderThread );
	
protected:
	/** Clean up render dynamic skin. */
	virtual ~deoglRDynamicSkin();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Number of renderables. */
	int GetRenderableCount() const;
	
	/** Renderable at index. */
	deoglRDSRenderable *GetRenderableAt( int index ) const;
	
	/** Named renderable or \em NULL if absent. */
	deoglRDSRenderable *GetRenderableNamed( const char *name ) const;
	
	/** Index of named renderable or -1 if absent. */
	int IndexOfRenderableNamed( const char *name ) const;
	
	/** Remove all renderables. */
	void RemoveAllRenderables();
	
	/** Add renderable. */
	void AddRenderable( deoglRDSRenderable *renderable );
	
	/**
	 * Texture configuration changed.
	 * \details Increments the update number for users to know they need to update their
	 *          texture unit configurations and potentially shaders. This is to be called
	 *          only by deoglRDSRenderable subclasses.
	 */
	void TextureConfigurationChanged();
	
	/** Update the dynamic skin if required. Returns the current update number. */
	int Update();
	/*@}*/
};

#endif
