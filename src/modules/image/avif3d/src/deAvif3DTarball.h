/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEAVIF3DTARBALL_H_
#define _DEAVIF3DTARBALL_H_

#include <dragengine/common/string/decString.h>

class decBaseFileReader;
class decBaseFileWriter;
class deImage;
class deAvif3DImageInfo;
class deAvif3DModule;


/**
 * AVIF-3D Tarball.
 */
class deAvif3DTarball{
private:
	deAvif3DModule &pModule;
	
	struct sImageInfo{
		int width;
		int height;
		bool hasAlpha;
		bool isGrayscale;
		int bitCount;
	};
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create tarball. */
	explicit deAvif3DTarball(deAvif3DModule &module);
	
	/** Clean up tarball. */
	~deAvif3DTarball();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Get 3D image info. */
	void Get3DImageInfos(deAvif3DImageInfo &infos, decBaseFileReader &file);
	
	/** Load 3D image from tarball. */
	void Load3DImage(deAvif3DImageInfo &infos, decBaseFileReader &file, deImage &image);
	
	/** Save 3D image to tarball. */
	void Save3DImage(decBaseFileWriter &file, const deImage &image);
	/*@}*/
	
	
private:
	/** Get 2D image info from AVIF data. */
	void Get2DImageInfos(sImageInfo &info2D, decBaseFileReader &file, int size);
	
	/** Load 2D image from tarball. */
	void Load2DImage(deAvif3DImageInfo &info3D, decBaseFileReader &file, int size, void *imagedata);
};

#endif
