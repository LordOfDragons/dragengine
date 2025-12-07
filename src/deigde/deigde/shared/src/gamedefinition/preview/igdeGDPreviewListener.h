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

#ifndef _IGDEGDPREVIEWLISTENER_H_
#define _IGDEGDPREVIEWLISTENER_H_

#include <dragengine/deObject.h>

class deImage;



/**
 * \brief Listener for creating preview images.
 */
class DE_DLL_EXPORT igdeGDPreviewListener : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDPreviewListener> Ref;


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
	virtual void ImageCreated(deImage *image) = 0;
	/*@}*/
};

#endif
