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

#ifndef _DEOGLTERRAINMASKIMAGE_H_
#define _DEOGLTERRAINMASKIMAGE_H_

#include <dragengine/resources/image/deImage.h>


/**
 * \brief Mask Image Wrapper.
 *
 * Wraps a mask image delivering the mask value without having to worry about the underlaying
 * bit count of the mask image. The image is not held so you have to hold the image yourself.
 * The data is only wrapped in a shallow way without any checks done so do the checks yourself.
 * 
 * \warning This class retains image data in the constructor and release it in the destructor.
 */
class deoglTerrainMaskImage{
private:
	deImage &pImage;
	int pWidth;
	int pHeight;
	sGrayscale8 *pData8;
	sGrayscale16 *pData16;
	sGrayscale32 *pData32;
	bool pRetained;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create terrain mask image wrapper. */
	deoglTerrainMaskImage( deImage &image );
	
	/** \brief Clean up terrain mask image wrapper. */
	~deoglTerrainMaskImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height. */
	inline int GetHeight() const{ return pHeight; }
	
	/**
	 * \brief Mask value at position.
	 * 
	 * No checking is done on the parameters so make sure they are inside the boundaries.
	 */
	float GetMaskValueAt( int x, int y ) const;
	
	/**
	 * \brief Mask value at position.
	 * 
	 * No checking is done on the parameters so make sure they are inside the boundaries.
	 */
	float GetMaskValueAt( int index ) const;
	/*@}*/
};

#endif
