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

#ifndef _DEOGLRENDERWINDOWLIST_H_
#define _DEOGLRENDERWINDOWLIST_H_

#include <dragengine/common/collection/decPointerSet.h>

class deGraphicOpenGl;
class deoglRenderWindow;



/**
 * \brief Render window list.
 */
class deoglRenderWindowList{
private:
	deGraphicOpenGl &pOgl;
	decPointerSet pWindows;
	bool pDirty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	deoglRenderWindowList( deGraphicOpenGl &ogl );
	
	/** \brief Clean up list. */
	~deoglRenderWindowList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of windows. */
	int GetCount() const;
	
	/** \brief Window at index. */
	deoglRenderWindow *GetAt( int index ) const;
	
	/** \brief Add window. */
	void Add( deoglRenderWindow *window );
	
	/** \brief Remove window. */
	void Remove( deoglRenderWindow *window );
	
	/** \brief Removes all windows. */
	void RemoveAll();
	
	
	
	/** \brief Update render thread windows including content if required. */
	void SyncToRender();
	/*@}*/
};

#endif
