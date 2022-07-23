/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEDSRENDERABLECAMERA_H_
#define _DEDSRENDERABLECAMERA_H_

#include "deDSRenderable.h"
#include "../../../camera/deCameraReference.h"


/**
 * \brief Dynamic Skin Renderable Camera.
 *
 * All pixels covered by this renderable are rendered using the given
 * camera. Post processing effects are not applied since they are
 * screen space which is not given here.
 */
class DE_DLL_EXPORT deDSRenderableCamera : public deDSRenderable{
private:
	deCameraReference pCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new renderable. */
	deDSRenderableCamera( const char *name );
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderableCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Camera or NULL if not set. */
	inline deCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Set camera or NULL if not set. */
	void SetCamera( deCamera *camera );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the renderable. */
	virtual void Visit( deDSRenderableVisitor &visitor );
};

#endif
