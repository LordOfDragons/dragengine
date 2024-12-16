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

#include "devkFormat.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>


// Class devkFormat
/////////////////////

// Constructor, destructor
////////////////////////////

devkFormat::devkFormat(){
}

devkFormat::devkFormat(VkFormat format, int bitsPerPixel, bool isDepth, bool isDepthFloat,
	bool isStencil, bool isCompressed, const char *name) :
pFormat(format),
pBitsPerPixel(bitsPerPixel),
pIsDepth(isDepth),
pIsDepthFloat(isDepthFloat),
pIsStencil(isStencil),
pIsCompressed(isCompressed),
pName(name){
}

devkFormat::devkFormat(const devkFormat &format){
	*this = format;
}

devkFormat::~devkFormat(){
}

// Management
///////////////

void devkFormat::SetFormat(VkFormat format){
	pFormat = format;
}

void devkFormat::SetBitsPerPixel(int bitsPerPixel){
	pBitsPerPixel = bitsPerPixel;
}

void devkFormat::SetIsDepth(bool isDepth){
	pIsDepth = isDepth;
}

void devkFormat::SetIsDepthFloat(bool isDepthFloat){
	pIsDepthFloat = isDepthFloat;
}

void devkFormat::SetIsStencil(bool isStencil){
	pIsStencil = isStencil;
}

void devkFormat::SetIsCompressed(bool isCompressed){
	pIsCompressed = isCompressed;
}

void devkFormat::SetCanSample(bool canSample){
	pCanSample = canSample;
}

void devkFormat::SetCanSampleFilter(bool canSampleFilter){
	pCanSampleFilter = canSampleFilter;
}

void devkFormat::SetCanImageStore(bool canImageStore){
	pCanImageStore = canImageStore;
}

void devkFormat::SetCanImageStoreAtomic(bool canImageStoreAtomic){
	pCanImageStoreAtomic = canImageStoreAtomic;
}

void devkFormat::SetCanAttach(bool canAttach){
	pCanAttach = canAttach;
}

void devkFormat::SetCanAttachBlend(bool canAttachBlend){
	pCanAttachBlend = canAttachBlend;
}

void devkFormat::SetName(const char *name){
	pName = name;
}

void devkFormat::WriteToString(decUnicodeString &buffer) const
{
	buffer.AppendFromUTF8(pName);
	buffer.AppendFromUTF8(",\n");
}

devkFormat &devkFormat::operator=(const devkFormat &format){
	pFormat = format.pFormat;
	pBitsPerPixel = format.pBitsPerPixel;
	pIsDepth = format.pIsDepth;
	pIsDepthFloat = format.pIsDepthFloat;
	pIsStencil = format.pIsStencil;
	pIsCompressed = format.pIsCompressed;
	pCanSample = format.pCanSample;
	pCanSampleFilter = format.pCanSampleFilter;
	pCanImageStore = format.pCanImageStore;
	pCanImageStoreAtomic = format.pCanImageStoreAtomic;
	pCanAttach = format.pCanAttach;
	pCanAttachBlend = format.pCanAttachBlend;
	pName = format.pName;
	return *this;
}
