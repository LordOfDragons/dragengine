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

#ifndef _DEDYNAMICSKIN_H_
#define _DEDYNAMICSKIN_H_

#include "../../deResource.h"

class deDSRenderable;
class deDynamicSkinManager;
class deBaseGraphicDynamicSkin;


/**
 * \brief Dynamic Skin.
 *
 * Defines dynamic properties of a skin resource.
 */
class deDynamicSkin : public deResource{
private:
	deDSRenderable **pRenderables;
	int pRenderableCount;
	int pRenderableSize;
	
	deBaseGraphicDynamicSkin *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new resource. */
	deDynamicSkin( deDynamicSkinManager *manager );
	
protected:
	/**
	 * \brief Clean up resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deDynamicSkin();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of renderables. */
	inline int GetRenderableCount() const{ return pRenderableCount; }
	
	/** \brief Renderable at the given index. */
	deDSRenderable *GetRenderableAt( int index ) const;
	
	/** \brief Renderable with the given name or NULL if not found. */
	deDSRenderable *GetRenderableNamed( const char *name ) const;
	
	/** \brief Determines if a renderable exists. */
	bool HasRenderable( deDSRenderable *renderable ) const;
	
	/** \brief Determines if a renderable with the given name exists. */
	bool HasRenderableNamed( const char *name ) const;
	
	/** \brief Index of the renderable or -1 if not found. */
	int IndexOfRenderable( deDSRenderable *renderable ) const;
	
	/** \brief Index of the renderable with the given name or -1 if not found. */
	int IndexOfRenderableNamed( const char *name ) const;
	
	/** \brief Adds a renderable. */
	void AddRenderable( deDSRenderable *renderable );
	
	/** \brief Removes a renderable. */
	void RemoveRenderable( deDSRenderable *renderable );
	
	/** \brief Removes a renderable with the given name if existing. */
	void RemoveRenderableNamed( const char *name );
	
	/** \brief Removes all renderables. */
	void RemoveAllRenderables();
	
	/** \brief Notifies the peer that the given renderable changed. */
	void NotifyRenderableChanged( int index );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object. */
	inline deBaseGraphicDynamicSkin *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object. */
	void SetPeerGraphic( deBaseGraphicDynamicSkin *peer );
	/*@}*/
};

#endif
