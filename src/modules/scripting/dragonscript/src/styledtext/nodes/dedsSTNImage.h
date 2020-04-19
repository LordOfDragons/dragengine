/* 
 * Drag[en]gine DragonScript Script Module
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

// include only once
#ifndef _DEDSSTNIMAGE_H_
#define _DEDSSTNIMAGE_H_

// includes
#include "dedsStyledTextNode.h"

// predefinitions
class deImage;



/**
 * @brief Styled Text Node Image.
 * Image node in a styled text object storing a single image.
 * The image is an engine image object. The image can be NULL
 * in which case this node is an empty node of 0 size.
 */
class dedsSTNImage : public dedsStyledTextNode{
private:
	deImage *pImage;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new image node. */
	dedsSTNImage();
	/** Creates a new image node with the given image. */
	dedsSTNImage( deImage *image );
	/** Cleans up the image node. */
	virtual ~dedsSTNImage();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the image or NULL if not set. */
	inline deImage *GetImage() const{ return pImage; }
	/** Sets the image. */
	void SetImage( deImage *image );
	/*@}*/
};

// end of include only once
#endif
