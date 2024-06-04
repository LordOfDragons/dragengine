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

#ifndef _DEBASEGRAPHICIMAGE_H_
#define _DEBASEGRAPHICIMAGE_H_

#include "../../../dragengine_export.h"


/**
 * \brief Graphic module image peer.
 */
class DE_DLL_EXPORT deBaseGraphicImage{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseGraphicImage();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Image data changed. */
	virtual void ImageDataChanged();
	
	/** \brief Image data has been restored from original file. */
	virtual void ImageDataRestored();
	
	/** \brief Image data has been released. */
	virtual void ImageDataReleased();
	
	/**
	 * \brief Graphic module requires image data to remain loaded.
	 * 
	 * Used to optimized memory consumption. Default implementation returns false.
	 */
	virtual bool RetainImageData();
	/*@}*/
};

#endif
