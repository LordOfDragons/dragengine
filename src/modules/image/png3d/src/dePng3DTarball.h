/* 
 * Drag[en]gine 3D PNG Image Module
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

#ifndef _DEPNG3DTARBALL_H_
#define _DEPNG3DTARBALL_H_

#include <png.h>

class decBaseFileReader;
class decBaseFileWriter;
class dePng3DImageInfo;
class deImage;
class dePng3DModule;



/**
 * @brief 3D-PNG Tarball Handler.
 */
class dePng3DTarball{
private:
	struct sImageInfos{
		int width;
		int height;
		int bitCount;
		int componentCount;
	};
	
	dePng3DModule *pModule;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new tarball handler. */
	dePng3DTarball( dePng3DModule *module );
	/** Cleans up the tarball handler. */
	~dePng3DTarball();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determine image properties. */
	void Get3DImageInfos( dePng3DImageInfo &infos, decBaseFileReader &file );
	/** Load image. */
	void Load3DImage( dePng3DImageInfo &infos, decBaseFileReader &file, deImage &image );
	/** Save image. */
	void Save3DImage( decBaseFileWriter &file, const deImage &image );
	
	/** Read image properties from an image in the tarball. */
	void Get2DImageInfos( dePng3DImageInfo &infos3D, sImageInfos &infos2D, decBaseFileReader &file );
	/** Load image slice from the tarball. */
	void Load2DImage( dePng3DImageInfo &infos3D, decBaseFileReader &file, png_bytep *rows );
	/** Write image slice to a memory file. */
	void Save2DImage( int width, int height, decBaseFileWriter &file, png_bytep *rows, int pngColorType, int pngBitCount );
	/*@}*/
};

// end of include only once
#endif
