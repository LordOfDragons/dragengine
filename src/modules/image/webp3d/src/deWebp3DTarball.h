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
		bool isGrayscale;
	};
	
	deWebp3DModule &pModule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create handler. */
	deWebp3DTarball(deWebp3DModule &module);
	
	/** Clean up handler. */
	~deWebp3DTarball();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get image properties. */
	void Get3DImageInfos(deWebp3DImageInfo &infos, decBaseFileReader &file);
	
	/** Load image. */
	void Load3DImage(deWebp3DImageInfo &infos, decBaseFileReader &file, deImage &image);
	
	/** Save image. */
	void Save3DImage(decBaseFileWriter &file, const deImage &image);
	
	/** Read image properties from an image in the tarball. */
	void Get2DImageInfos(sImageInfo &infos2D, decBaseFileReader &file, int size);
	
	/** Load image slice from the tarball. */
	void Load2DImage(deWebp3DImageInfo &info3D, decBaseFileReader &file, int size, void *imagedata);
	
	/** Write image slice to a memory file. */
	void Save2DImage(int width, int height, bool hasAlpha, decBaseFileWriter &file, const void *imagedata);
	
	/** Assert status code. .*/
	void Assert(VP8StatusCode statusCode) const;
	/*@}*/
};

#endif
