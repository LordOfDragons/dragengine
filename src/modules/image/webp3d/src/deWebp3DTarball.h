/* 
 * Drag[en]gine WebP-3D Image Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEWEBP3DTARBALL_H_
#define _DEWEBP3DTARBALL_H_

#include <webp/decode.h>

class decBaseFileReader;
class decBaseFileWriter;
class deWebp3DImageInfo;
class deImage;
class deWebp3DModule;



/**
 * WebP-3D Tarball Handler.
 */
class deWebp3DTarball{
private:
	struct sImageInfo {
		int width;
		int height;
		bool hasAlpha;
	};
	
	deWebp3DModule &pModule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create handler. */
	deWebp3DTarball( deWebp3DModule &module );
	
	/** Clean up handler. */
	~deWebp3DTarball();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get image properties. */
	void Get3DImageInfos( deWebp3DImageInfo &infos, decBaseFileReader &file );
	
	/** Load image. */
	void Load3DImage( deWebp3DImageInfo &infos, decBaseFileReader &file, deImage &image );
	
	/** Save image. */
	void Save3DImage( decBaseFileWriter &file, const deImage &image );
	
	/** Read image properties from an image in the tarball. */
	void Get2DImageInfos( sImageInfo &infos2D, decBaseFileReader &file, int size );
	
	/** Load image slice from the tarball. */
	void Load2DImage( deWebp3DImageInfo &info3D, decBaseFileReader &file, int size, void *imagedata );
	
	/** Write image slice to a memory file. */
	void Save2DImage( int width, int height, bool hasAlpha, decBaseFileWriter &file, const void *imagedata );
	
	/** Assert status code. .*/
	void Assert( VP8StatusCode statusCode ) const;
	/*@}*/
};

#endif
