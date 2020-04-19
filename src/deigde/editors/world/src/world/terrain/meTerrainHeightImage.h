/* 
 * Drag[en]gine IGDE World Editor
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
#ifndef _METERRAINHEIGHTIMAGE_H_
#define _METERRAINHEIGHTIMAGE_H_

// includes
#include "dragengine/resources/image/deImage.h"



/**
 * @brief Height Image Wrapper.
 *
 * Wraps a height image delivering the height value without having to worry about
 * the underlaying bit count of the height image. The image is not held so you
 * have to hold the image yourself. The data is only wrapped in a shallow way
 * without any checks done so do the checks yourself.
 */
class meTerrainHeightImage{
private:
	int pWidth;
	int pHeight;
	sGrayscale8 *pData8;
	sGrayscale16 *pData16;
	sGrayscale32 *pData32;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new terrain height image wrapper. */
	meTerrainHeightImage( deImage *image );
	/** Cleans up the terrain height image wrapper. */
	~meTerrainHeightImage();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the width. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height. */
	inline int GetHeight() const{ return pHeight; }
	/**
	 * Retrieves the height at the given position. No checking is done on
	 * the parameters so make sure they are inside the boundaries.
	 */
	float GetHeightAt( int x, int y ) const;
	/**
	 * Retrieves the height at the given position. No checking is done on
	 * the parameters so make sure they are inside the boundaries.
	 */
	float GetHeightAt( int index ) const;
	/**
	 * Sets the height at the given position. No checking is done on
	 * the parameters so make sure they are inside the boundaries.
	 */
	void SetHeightAt( int x, int y, float value );
	/**
	 * Sets the height at the given position. No checking is done on
	 * the parameters so make sure they are inside the boundaries.
	 */
	void SetHeightAt( int index, float value );
	/*@}*/
};

// end of include only once
#endif
