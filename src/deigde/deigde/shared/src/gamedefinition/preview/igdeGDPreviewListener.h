/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDPREVIEWLISTENER_H_
#define _IGDEGDPREVIEWLISTENER_H_

#include <dragengine/deObject.h>

class deImage;



/**
 * \brief Listener for creating preview images.
 */
class DE_DLL_EXPORT igdeGDPreviewListener : public deObject{
public:
	/**  \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeGDPreviewListener();
	
	
	
protected:
	/**
	 * \brief Clean up listener.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeGDPreviewListener();
	/*@}*/
	
	
	
public:
	/**  \name Management */
	/*@{*/
	/**
	 * \brief Creating preview image finished.
	 * 
	 * The image is either the created image or an error image if creating the
	 * image failed for some reason. After calling this method the preview
	 * manager release the reference to the listener. The subclass has to
	 * add a reference to the image to hold it.
	 */
	virtual void ImageCreated( deImage *image ) = 0;
	/*@}*/
};

#endif
