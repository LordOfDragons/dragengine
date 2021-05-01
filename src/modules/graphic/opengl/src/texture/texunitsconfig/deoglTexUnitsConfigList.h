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



/**
 * @brief Texture Units Configuration List.
 * Manages texture units configurations.
 */
class deoglTexUnitsConfigList{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexUnitsConfig *pRootTUC;
	deoglTexUnitsConfig *pTailTUC;
	int pTUCCount;
	
	deoglTexUnitsConfig *pTUCEmpty;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new manager. */
	deoglTexUnitsConfigList( deoglRenderThread &renderThread );
	/** Cleans up the manager. */
	~deoglTexUnitsConfigList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the texture units configuration count. */
	inline int GetCount() const{ return pTUCCount; }
	/** Retrieves the root texture units configuration. */
	inline deoglTexUnitsConfig *GetRoot() const{ return pRootTUC; }
	
	/**
	 * Retrieves a configuration with the given parameters. Creates the definition if not existing yet.
	 * This adds a usage to the texture units configuration. To remove a usage use the RemoveUsage
	 * function on the returned object.
	 */
	deoglTexUnitsConfig *GetWith( const deoglTexUnitConfig *units, int unitCount );
	/** Retrieves the empty texture units configuration. */
	deoglTexUnitsConfig *GetEmpty();
	
	/** Retrieves the empty texture units configuration without adding a usage. */
	deoglTexUnitsConfig *GetEmptyNoUsage() const;
	
	/** Removes a configuration. This is for deoglTexUnitsConfig only. Do not call directly yourself. */
	void Remove( deoglTexUnitsConfig *config );
	/*@}*/
};

#endif
