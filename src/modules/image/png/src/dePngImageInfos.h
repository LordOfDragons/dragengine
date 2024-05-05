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

#ifndef _DEPNGIMAGEINFO_H_
#define _DEPNGIMAGEINFO_H_

#include "png.h"

#include <dragengine/systems/modules/image/deBaseImageInfo.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class dePngModule;



// image information class
class dePngImageInfo : public deBaseImageInfo{
public:
	struct sFeedback{
		dePngModule *module;
		decString filename;
	};
	
public:
	sFeedback feedback;
	
	decString filename;
	png_structp readStruct;
	png_infop infoStruct;
	
	int width;
	int height;
	int componentCount;
	int bitCount;
	
public:
	// constructor, destructor
	dePngImageInfo(const char *aFilename);
	~dePngImageInfo();
	// management
	int GetWidth();
	int GetHeight();
	int GetDepth();
	int GetComponentCount();
	int GetBitCount();
};

#endif
