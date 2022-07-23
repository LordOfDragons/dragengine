/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRENDERTASKCONFIG_H_
#define _DEOGLRENDERTASKCONFIG_H_

class deoglRenderTaskConfigTexture;


/**
 * Render task configuration.
 */
class deoglRenderTaskConfig{
private:
	deoglRenderTaskConfigTexture *pTextures;
	int pTextureCount;
	int pTextureSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task config. */
	deoglRenderTaskConfig();
	
	/** Clean up render task config. */
	~deoglRenderTaskConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** Texture at index. */
	deoglRenderTaskConfigTexture &GetTextureAt( int index ) const;
	
	/** Add texture. */
	deoglRenderTaskConfigTexture &AddTexture();
	
	/** Remove all textures. */
	void RemoveAllTextures();
	/*@}*/
};

#endif
