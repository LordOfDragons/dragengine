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

#ifndef _DECANVASIMAGE_H_
#define _DECANVASIMAGE_H_

#include "deCanvas.h"
#include "../image/deImageReference.h"


/**
 * \brief Canvas rendering an image.
 */
class DE_DLL_EXPORT deCanvasImage : public deCanvas{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCanvasImage> Ref;
	
	
	
private:
	deImageReference pImage;
	int pRepeatX;
	int pRepeatY;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas. */
	deCanvasImage( deCanvasManager *manager );
	
protected:
	/**
	 * \brief Clean up canvas.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deCanvasImage();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Image or NULL if not set. */
	inline deImage *GetImage() const{ return pImage; }
	
	/** \brief Set image or NULL to unset. */
	void SetImage( deImage *image );
	
	/** \brief Repeat count along X axis. */
	inline int GetRepeatX() const{ return pRepeatX; }
	
	/**
	 * \brief Set repeat count along X axis.
	 * \throws deeInvalidParam \em count is less than 1.
	 */
	void SetRepeatX( int count );
	
	/** \brief Repeat count along Y axis. */
	inline int GetRepeatY() const{ return pRepeatY; }
	
	/**
	 * \brief Set repeat count along Y axis.
	 * \throws deeInvalidParam \em count is less than 1.
	 */
	void SetRepeatY( int count );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	virtual void Visit( deCanvasVisitor &visitor );
	/*@}*/
};

#endif
