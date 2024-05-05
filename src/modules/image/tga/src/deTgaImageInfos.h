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

// include only once
#ifndef _DETGAIMAGEINFO_H_
#define _DETGAIMAGEINFO_H_

// includes
#include "dragengine/systems/modules/image/deBaseImageInfo.h"
#include "dragengine/common/math/decMath.h"

// predefinitions

// structures
#pragma pack(1)
struct tgaHdr{
	// header
	unsigned char idLength;
	unsigned char clrMapType;
	unsigned char imageType;
	// color map info
	unsigned short firstClrMapID;
	unsigned short countClrMapEntries;
	unsigned char entrySize;
	// image info
	unsigned short posX;
	unsigned short posY;
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char imgDesc;
};
#pragma pack()

// image information class
class deTgaImageInfo : public deBaseImageInfo{
public:
	tgaHdr header;
public:
	// constructor, destructor
	deTgaImageInfo();
	~deTgaImageInfo();
	// management
	int GetWidth();
	int GetHeight();
	int GetDepth();
	int GetComponentCount();
	int GetBitCount();
};

// end of include only once
#endif
