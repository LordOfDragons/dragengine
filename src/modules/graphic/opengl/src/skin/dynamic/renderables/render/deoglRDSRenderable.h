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

#ifndef _DEOGLRRDSRENDERABLE_H_
#define _DEOGLRRDSRENDERABLE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deoglCubeMap;
class deoglTexture;
class deoglRDynamicSkin;
class deoglRenderPlan;
class deoglRenderPlanMasked;



/**
 * Render dynamic skin renderable.
 */
class deoglRDSRenderable : public deObject{
public:
	enum eType{
		etValue,
		etColor,
		etImage,
		etVideoFrame,
		etCanvas,
		etCamera
	};
	
	
	
private:
	const eType pType;
	decString pName;
	deoglRDynamicSkin &pDynamicSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render dynamic skin renderable. */
	deoglRDSRenderable( eType type, deoglRDynamicSkin &dynamicSkin );
	
	/** Clean up render dynamic skin renderable. */
	virtual ~deoglRDSRenderable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Type. */
	inline const eType GetType() const{ return pType; }
	
	/** Dynamic skin. */
	inline deoglRDynamicSkin &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Prepare for render. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask ) = 0;
	
	/** Render if required. */
	virtual void Render( const deoglRenderPlanMasked *renderPlanMask );
	
	/**
	 * Get value if support or default value.
	 * \details Default implementation returns default value.
	 */
	virtual float GetRenderValue( float defaultValue );
	
	/**
	 * Get color if support or default color.
	 * \details Default implementation returns default value.
	 */
	virtual decColor GetRenderColor( const decColor &defaultColor );
	
	/**
	 * Get texture to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglTexture *GetRenderTexture();
	
	/**
	 * Get cubemap to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglCubeMap *GetRenderCubeMap();
	
	/**
	 * Get render plan to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglRenderPlan *GetRenderPlan() const;
	/*@}*/
};

#endif
