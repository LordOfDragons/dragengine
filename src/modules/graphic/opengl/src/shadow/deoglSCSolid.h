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

#ifndef _DEOGLSCSOLID_H_
#define _DEOGLSCSOLID_H_

class deoglRenderThread;
class deoglTexture;
class deoglCubeMap;
class deoglRenderableDepthTexture;
class deoglRenderableDepthCubeMap;



/**
 * \brief Shadow caster solid faces.
 */
class deoglSCSolid{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexture *pStaticMap;
	deoglCubeMap *pStaticCubeMap;
	int pLastUseStatic;
	bool pHasStatic;
	
	deoglRenderableDepthTexture *pDynamicMap;
	deoglRenderableDepthCubeMap *pDynamicCubeMap;
	
	int pPlanStaticSize;
	int pPlanDynamicSize;
	int pPlanTransparentSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create solid shadow caster. */
	deoglSCSolid( deoglRenderThread &renderThread );
	
	/** \brief Clean up solid shadow caster. */
	~deoglSCSolid();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Static shadow map if present or \em NULL otherwise. */
	inline deoglTexture *GetStaticMap() const{ return pStaticMap; }
	
	/** \brief Request static shadow map with size if absent. */
	deoglTexture *ObtainStaticMapWithSize( int size, bool useFloat );
	
	/** \brief Static shadow cube map if present or \em NULL otherwise. */
	inline deoglCubeMap *GetStaticCubeMap() const{ return pStaticCubeMap; }
	
	/** \brief Request static cube map with size if absent. */
	deoglCubeMap *ObtainStaticCubeMapWithSize( int size );
	
	/** \brief Drop static shadow map if present. */
	void DropStatic();
	
	/** \brief Number of frames elapsed since the last time static shadow map has been used. */
	inline int GetLastUseStatic() const{ return pLastUseStatic; }
	
	/** \brief Increment last use static shadow map counter by one. */
	void IncrementLastUseStatic();
	
	/** \brief Reset last use static shadow map counter. */
	void ResetLastUseStatic();
	
	
	
	/** \brief Dynamic shadow map if present or \em NULL otherwise. */
	inline deoglRenderableDepthTexture *GetDynamicMap() const{ return pDynamicMap; }
	
	/** \brief Obtain dynamic shadow map with size if absent. */
	deoglRenderableDepthTexture *ObtainDynamicMapWithSize( int size, bool useFloat );
	
	/** \brief Dynamic shadow cube map if present or \em NULL otherwise. */
	inline deoglRenderableDepthCubeMap *GetDynamicCubeMap() const{ return pDynamicCubeMap; }
	
	/** \brief Obtain dynamic shadow cube map with size if absent. */
	deoglRenderableDepthCubeMap *ObtainDynamicCubeMapWithSize( int size );
	
	/** \brief Drop dynamic shadow map if present. */
	void DropDynamic();
	
	
	
	/** \brief Check if static maps have not been used recently removing them. */
	void Update();
	
	/** \brief Drop all maps. */
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
