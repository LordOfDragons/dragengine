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

#ifndef _DEOGLCOMPONENTLISTENER_H_
#define _DEOGLCOMPONENTLISTENER_H_

#include <dragengine/deObject.h>

class deoglRComponent;


/**
 * Render component listener.
 */
class deoglComponentListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component listener. */
	deoglComponentListener();
	
	/** Clean up component listener. */
	virtual ~deoglComponentListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Component has been destroyed. */
	virtual void ComponentDestroyed( deoglRComponent &component );
	
	/** Component changed parent world. */
	virtual void ParentWorldChanged( deoglRComponent &component );
	
	/** Component changed layer mask. */
	virtual void LayerMaskChanged( deoglRComponent &component );
	
	/** Component changed render static. */
	virtual void RenderStaticChanged( deoglRComponent &component );
	
	/** Component geometry changed or dislocated. */
	virtual void BoundariesChanged( deoglRComponent &component );
	
	/** Occlusion mesh geometry changed. */
	virtual void OcclusionMeshChanged( deoglRComponent &component );
	
	/** Textures changed. */
	virtual void TexturesChanged( deoglRComponent &component );
	
	/** TUC changed. */
	virtual void TUCChanged( deoglRComponent &component );
	
	/** Movement hint changed. */
	virtual void MovementHintChanged( deoglRComponent &component );
	
	/** GI importance changed. */
	virtual void GIImportanceChanged( deoglRComponent &component );
	
	/** Visibility changed. */
	virtual void VisibilityChanged( deoglRComponent &component );
	/*@}*/
};

#endif
