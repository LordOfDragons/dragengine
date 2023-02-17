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

#ifndef _DEOGLDELAYEDSAVEIMAGE_H_
#define _DEOGLDELAYEDSAVEIMAGE_H_

#include <dragengine/common/file/decPath.h>

class deVirtualFileSystem;
class deGraphicOpenGl;



/**
 * Delayed save image.
 */
class deoglDelayedSaveImage{
private:
	const decPath pPath;
	int pWidth;
	int pHeight;
	int pDepth;
	int pComponentCount;
	int pBitCount;
	char *pData;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create delayed save image object. */
	deoglDelayedSaveImage( const decPath &path, int width, int height, int depth,
		int componentCount, int bitCount, char *data );
	
	/** Clean up delayed save image object. */
	~deoglDelayedSaveImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Save image to virtual file system.
	 * \details This has to be called during main thread.
	 */
	void SaveImage( deGraphicOpenGl &ogl, deVirtualFileSystem &vfs );
	/*@}*/
};

#endif
