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

#ifndef _DEOGLSCAMBIENT_H_
#define _DEOGLSCAMBIENT_H_

class deoglRenderThread;
class deoglTexture;
class deoglCubeMap;
class deoglRenderableDepthTexture;



/**
 * \brief Shadow caster ambient map.
 */
class deoglSCAmbient{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexture *pStaticMap;
	deoglCubeMap *pStaticCubeMap;
	int pLastUseStatic;
	bool pHasStatic;
	
	deoglRenderableDepthTexture *pDynamicMap;
	
	int pPlanStaticSize;
	int pPlanDynamicSize;
	int pPlanTransparentSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ambient caster. */
	deoglSCAmbient( deoglRenderThread &renderThread );
	
	/** \brief Clean up ambient caster. */
	~deoglSCAmbient();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	
	/** \brief Static map if present or \em NULL otherwise. */
	inline deoglTexture *GetStaticMap() const{ return pStaticMap; }
	
	/** \brief Request static map with size if absent. */
	deoglTexture *ObtainStaticMapWithSize( int size, bool useFloat );
	
	/** \brief Static cube map if present or \em NULL otherwise. */
	inline deoglCubeMap *GetStaticCubeMap() const{ return pStaticCubeMap; }
	
	/** \brief Request static cube map with size if absent. */
	deoglCubeMap *ObtainStaticCubeMapWithSize( int size );
	
	/** \brief Drop static maps if present. */
	void DropStatic();
	
	/** \brief Number of frames elapsed since the last time static map has been used. */
	inline int GetLastUseStatic() const{ return pLastUseStatic; }
	
	/** \brief Increment last use static map counter by one. */
	void IncrementLastUseStatic();
	
	/** \brief Reset last use static map counter. */
	void ResetLastUseStatic();
	
	
	
	
	/** \brief Dynamic map if present or \em NULL otherwise. */
	inline deoglRenderableDepthTexture *GetDynamicMap() const{ return pDynamicMap; }
	
	/** \brief Obtain dynamic map with size if absent. */
	deoglRenderableDepthTexture *GetDynamicMap( int size );
	
	/** \brief Drop dynamic map if present. */
	void DropDynamic();
	
	
	
	/** \brief Check if static maps have not been used recently removing them. */
	void Update();
	
	/** \brief Clear ambient caster. */
	void Clear();
	
	
	
	/** \brief Plan static map size. */
	inline int GetPlanStaticSize() const{ return pPlanStaticSize; }
	
	/** \brief Set plan static map size. */
	void SetPlanStaticSize( int size );
	
	/** \brief Plan dynamic map size. */
	inline int GetPlanDynamicSize() const{ return pPlanDynamicSize; }
	
	/** \brief Set plan dynamic map size. */
	void SetPlanDynamicSize( int size );
	
	/** \brief Plan transparent map size. */
	inline int GetPlanTransparentSize() const{ return pPlanTransparentSize; }
	
	/** \brief Set plan transparent map size. */
	void SetPlanTransparentSize( int size );
	/*@}*/
};

#endif
