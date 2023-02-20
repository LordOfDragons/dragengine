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

#ifndef _IGDEBROWSEITEMGDPREVIEWLISTENER_H_
#define _IGDEBROWSEITEMGDPREVIEWLISTENER_H_

#include "../igdeIconListBoxReference.h"
#include "../model/igdeListItemReference.h"
#include "../../gamedefinition/preview/igdeGDPreviewListener.h"



/**
 * \brief Create preview listener updating an liste item once finished.
 */
class DE_DLL_EXPORT igdeBrowseItemGDPreviewListener : public igdeGDPreviewListener{
private:
	igdeIconListBoxReference pIconListBox;
	igdeListItemReference pListItem;
	int pIconSize;
	
	
	
public:
	/**  \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeBrowseItemGDPreviewListener( igdeIconListBox *iconListBox, igdeListItem *listItem, int iconSize );
	
protected:
	/**
	 * \brief Clean up listener.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeBrowseItemGDPreviewListener();
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
	virtual void ImageCreated( deImage *image );
	/*@}*/
};

#endif
