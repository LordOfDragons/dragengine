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

#ifndef _DECANVASVIEW_H_
#define _DECANVASVIEW_H_

#include "deCanvas.h"


/**
 * \brief Canvas containing other canvas.
 */
class DE_DLL_EXPORT deCanvasView : public deCanvas{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCanvasView> Ref;
	
	
	
private:
	deCanvas *pCanvasRoot;
	deCanvas *pCanvasTail;
	int pCanvasCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas. */
	deCanvasView( deCanvasManager *manager );
	
protected:
	/**
	 * \brief Clean up canvas.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deCanvasView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of child canvas. */
	inline int GetCanvasCount() const{ return pCanvasCount; }
	
	/** \brief Root canvas or NULL if there is none. */
	inline deCanvas *GetRootCanvas() const{ return pCanvasRoot; }
	
	/**
	 * \brief Add child canvas.
	 * \throws deeInvalidParam \em canvas is NULL.
	 * \throws deeInvalidParam \em canvas has a parent view.
	 */
	void AddCanvas( deCanvas *canvas );
	
	/**
	 * \brief Remove child canvas.
	 * \throws deeInvalidParam \em canvas is NULL.
	 * \throws deeInvalidParam \em canvas parent view is not this canvas.
	 */
	void RemoveCanvas( deCanvas *canvas );
	
	/** \brief Remove all child canvas. */
	void RemoveAllCanvas();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	virtual void Visit( deCanvasVisitor &visitor );
	/*@}*/
};

#endif
