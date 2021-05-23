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

#ifndef _DEOGLRENDERPLANLIGHT_H_
#define _DEOGLRENDERPLANLIGHT_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglRLight;



/**
 * \brief Render plan light.
 */
class deoglRenderPlanLight{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRLight *pLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render plan light. */
	deoglRenderPlanLight( deoglRenderThread &renderThread );
	
	/** \brief Clean up render plan light. */
	~deoglRenderPlanLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Light. */
	inline deoglRLight *GetLight() const{ return pLight; }
	
	/** \brief Set light. */
	void SetLight( deoglRLight *light );
	/*@}*/
};

#endif
