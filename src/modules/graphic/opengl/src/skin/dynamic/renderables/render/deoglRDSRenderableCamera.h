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

#ifndef _DEOGLRRDSRENDERABLECAMERA_H_
#define _DEOGLRRDSRENDERABLECAMERA_H_

#include <dragengine/deObject.h>

#include "deoglRDSRenderable.h"

class deoglRCamera;



/**
 * \brief Render dynamic skin camera renderable.
 */
class deoglRDSRenderableCamera : public deoglRDSRenderable{
private:
	deoglRCamera *pCamera;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render dynamic skin camera renderable. */
	deoglRDSRenderableCamera( deoglRDynamicSkin &dynamicSkin );
	
	/** \brief Clean up render dynamic skin camera renderable. */
	virtual ~deoglRDSRenderableCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set camera. */
	void SetCamera( deoglRCamera *camera );
	
	/** \brief Prepare for render. */
	virtual void PrepareForRender();
	
	/**
	 * \brief Get render plan to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglRenderPlan *GetRenderPlan();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
