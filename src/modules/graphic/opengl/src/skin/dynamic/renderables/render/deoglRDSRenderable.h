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
 * \brief Render dynamic skin renderable.
 */
class deoglRDSRenderable : public deObject{
private:
	decString pName;
	deoglRDynamicSkin &pDynamicSkin;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render dynamic skin renderable. */
	deoglRDSRenderable( deoglRDynamicSkin &dynamicSkin );
	
	/** \brief Clean up render dynamic skin renderable. */
	virtual ~deoglRDSRenderable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dynamic skin. */
	inline deoglRDynamicSkin &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Prepare for render. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask ) = 0;
	
	/**
	 * \brief Get value if support or default value.
	 * \details Default implementation returns default value.
	 */
	virtual float GetRenderValue( float defaultValue );
	
	/**
	 * \brief Get color if support or default color.
	 * \details Default implementation returns default value.
	 */
	virtual decColor GetRenderColor( const decColor &defaultColor );
	
	/**
	 * \brief Get texture to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglTexture *GetRenderTexture();
	
	/**
	 * \brief Get cubemap to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglCubeMap *GetRenderCubeMap();
	
	/**
	 * \brief Get render plan to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglRenderPlan *GetRenderPlan() const;
	/*@}*/
};

#endif
