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
