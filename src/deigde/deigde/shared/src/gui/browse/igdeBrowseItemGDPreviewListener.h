/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _IGDEBROWSEITEMGDPREVIEWLISTENER_H_
#define _IGDEBROWSEITEMGDPREVIEWLISTENER_H_

#include "../igdeIconListBoxReference.h"
#include "../model/igdeListItem.h"
#include "../../gamedefinition/preview/igdeGDPreviewListener.h"



/**
 * \brief Create preview listener updating an liste item once finished.
 */
class DE_DLL_EXPORT igdeBrowseItemGDPreviewListener : public igdeGDPreviewListener{
private:
	igdeIconListBoxReference pIconListBox;
	igdeListItem::Ref pListItem;
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
