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

#ifndef _DEOGLSCTRANSPARENT_H_
#define _DEOGLSCTRANSPARENT_H_

class deoglRenderThread;
class deoglCubeMap;
class deoglTexture;
class deoglRenderableDepthTexture;
class deoglRenderableColorTexture;
class deoglRenderableDepthCubeMap;
class deoglRenderableColorCubeMap;



/**
 * \brief Shadow Caster Transparent.
 */
class deoglSCTransparent{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexture *pStaticShadowMap;
	deoglTexture *pStaticColorMap;
	deoglCubeMap *pStaticShadowCubeMap;
	deoglCubeMap *pStaticColorCubeMap;
	int pLastUseStatic;
	bool pHasStatic;
	
	deoglRenderableDepthTexture *pDynamicShadowMap;
	deoglRenderableColorTexture *pDynamicColorMap;
	deoglRenderableDepthCubeMap *pDynamicShadowCubeMap;
	deoglRenderableColorCubeMap *pDynamicColorCubeMap;
	
	int pPlanStaticSize;
	int pPlanDynamicSize;
	int pPlanTransparentSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create transparent shadow caster. */
	deoglSCTransparent( deoglRenderThread &renderThread );
	
	/** \brief Clean up transparent shadow caster. */
	~deoglSCTransparent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Static shadow map or \em NULL if absent. */
	inline deoglTexture *GetStaticShadowMap() const{ return pStaticShadowMap; }
	
	/** \brief Static shadow map with size. */
	deoglTexture *ObtainStaticShadowMapWithSize( int size, bool useFloat );
	
	/** \brief Static color map or \em NULL if absent. */
	inline deoglTexture *GetStaticColorMap() const{ return pStaticColorMap; }
	
	/** \brief Static color map with size. */
	deoglTexture *ObtainStaticColorMapWithSize( int size );
	
	/** \brief Static shadow cube map or \em NULL if absent. */
	inline deoglCubeMap *GetStaticShadowCubeMap() const{ return pStaticShadowCubeMap; }
	
	/** \brief Static shadow cube map with size. */
	deoglCubeMap *ObtainStaticShadowCubeMapWithSize( int size );
	
	/** \brief Static color cube map or \em NULL if absent. */
	inline deoglCubeMap *GetStaticColorCubeMap() const{ return pStaticColorCubeMap; }
	
	/** \brief Static color cube map with size. */
	deoglCubeMap *ObtainStaticColorCubeMapWithSize( int size );
	
	/** \brief Drop static maps if present. */
	void DropStatic();
	
	/** \brief Number of frames elapsed since the last time static maps have been used. */
	inline int GetLastUseStatic() const{ return pLastUseStatic; }
	
	/** \brief Increment last use static maps counter by one. */
	void IncrementLastUseStatic();
	
	/** \brief Reset last use static maps counter. */
	void ResetLastUseStatic();
	
	
	
	/** \brief Dynamic shadow map if present or \em NULL otherwise. */
	inline deoglRenderableDepthTexture *GetDynamicShadowMap() const{ return pDynamicShadowMap; }
	
	/** \brief Obtain dynamic shadow map with size if absent. */
	deoglRenderableDepthTexture *ObtainDynamicShadowMapWithSize( int size, bool useFloat );
	
	/** \brief Dynamic color map if present or \em NULL otherwise. */
	inline deoglRenderableColorTexture *GetDynamicColorMap() const{ return pDynamicColorMap; }
	
	/** \brief Obtain dynamic color map with size if absent. */
	deoglRenderableColorTexture *ObtainDynamicColorMapWithSize( int size );
	
	/** \brief Dynamic shadow cube map if present or \em NULL otherwise. */
	inline deoglRenderableDepthCubeMap *GetDynamicShadowCubeMap() const{ return pDynamicShadowCubeMap; }
	
	/** \brief Obtain dynamic shadow cube map with size if absent. */
	deoglRenderableDepthCubeMap *ObtainDynamicShadowCubeMapWithSize( int size );
	
	/** \brief Dynamic color cube map if present or \em NULL otherwise. */
	inline deoglRenderableColorCubeMap *GetDynamicColorCubeMap() const{ return pDynamicColorCubeMap; }
	
	/** \brief Obtain dynamic shadow cube map with size if absent. */
	deoglRenderableColorCubeMap *ObtainDynamicColorCubeMapWithSize( int size );
	
	/** \brief Drop dynamic shadow map if present. */
	void DropDynamic();
	
	
	
	/** \brief Check if static maps have not been used recently removing them. */
	void Update();
	
	/** \brief Clear transparent shadow caster. */
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
