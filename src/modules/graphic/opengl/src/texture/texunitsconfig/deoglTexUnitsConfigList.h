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

#ifndef _DEOGLTEXUNITSCONFIGLIST_H_
#define _DEOGLTEXUNITSCONFIGLIST_H_

class deoglRenderThread;
class deoglTexUnitConfig;
class deoglTexUnitsConfig;
class deoglShaderParameterBlock;


/**
 * Texture Units Configuration List.
 */
class deoglTexUnitsConfigList{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexUnitsConfig *pRootTUC;
	deoglTexUnitsConfig *pTailTUC;
	int pTUCCount;
	
	deoglTexUnitsConfig *pTUCEmpty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create list. */
	deoglTexUnitsConfigList( deoglRenderThread &renderThread );
	
	/** Clean up list. */
	~deoglTexUnitsConfigList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Count of texture units configurations. */
	inline int GetCount() const{ return pTUCCount; }
	
	/** Root texture units configuration. */
	inline deoglTexUnitsConfig *GetRoot() const{ return pRootTUC; }
	
	/**
	 * Texture units configuration matching parameters. Create texture units configuration
	 * if absent. Adds usage to texture units configuration. To remove a usage use RemoveUsage.
	 */
	deoglTexUnitsConfig *GetWith( const deoglTexUnitConfig *units,
		int unitCount, deoglShaderParameterBlock *paramBlock );
	
	/** Empty texture units configuration. */
	deoglTexUnitsConfig *GetEmpty();
	
	/** Empty texture units configuration without adding usage. */
	deoglTexUnitsConfig *GetEmptyNoUsage() const;
	
	/** Remove texture units configuration. For use by deoglTexUnitsConfig only. */
	void Remove( deoglTexUnitsConfig *config );
	/*@}*/
};

#endif
