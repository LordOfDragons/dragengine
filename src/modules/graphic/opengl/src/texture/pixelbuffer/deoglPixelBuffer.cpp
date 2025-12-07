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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglPixelBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deoglPixelBuffer
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglPixelBuffer::deoglPixelBuffer(ePixelFormats format, int width, int height, int depth){
	if(width < 1 || height < 1 || depth < 1 || format < 0 || format >= EPF_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	pFormat = format;
	pWidth = width;
	pHeight = height;
	pDepth = depth;
	pPixels = NULL;
	
	if(format == epfByte1){
		pUnitSize = 1;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_RED;
		pGLPixelType = GL_UNSIGNED_BYTE;
		
	}else if(format == epfByte2){
		pUnitSize = 2;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_RG;
		pGLPixelType = GL_UNSIGNED_BYTE;
		
	}else if(format == epfByte3){
		pUnitSize = 3;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_RGB;
		pGLPixelType = GL_UNSIGNED_BYTE;
		
	}else if(format == epfByte4){
		pUnitSize = 4;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_RGBA;
		pGLPixelType = GL_UNSIGNED_BYTE;
		
	}else if(format == epfFloat1){
		pUnitSize = 4;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_RED;
		pGLPixelType = GL_FLOAT;
		
	}else if(format == epfFloat2){
		pUnitSize = 8;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_RG;
		pGLPixelType = GL_FLOAT;
		
	}else if(format == epfFloat3){
		pUnitSize = 12;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_RGB;
		pGLPixelType = GL_FLOAT;
		
	}else if(format == epfFloat4){
		pUnitSize = 16;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_RGBA;
		pGLPixelType = GL_FLOAT;
		
	}else if(format == epfInt1){
		pUnitSize = 4;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_RED;
		pGLPixelType = GL_UNSIGNED_INT;
		
	}else if(format == epfDepth){
		pUnitSize = 4;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_DEPTH_COMPONENT;
		pGLPixelType = GL_FLOAT;
		
	}else if(format == epfStencil){
		pUnitSize = 1;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_STENCIL_INDEX;
		pGLPixelType = GL_UNSIGNED_BYTE;
		
	}else if(format == epfDepthStencil){
		pUnitSize = 4;
		pStrideLine = pUnitSize * width;
		pStrideLayer = pStrideLine * height;
		pImageSize = pStrideLayer * depth;
		pCompressed = false;
		pGLPixelFormat = GL_DEPTH_STENCIL;
		pGLPixelType = GL_UNSIGNED_INT_24_8;
		
	}else if(format == epfDXT1){
		const int blockCountX = ((width - 1) >> 2) + 1;
		const int blockCountY = ((height - 1) >> 2) + 1;
		
		pUnitSize = 8;
		pStrideLine = pUnitSize * blockCountX;
		pStrideLayer = pStrideLine * blockCountY;
		pImageSize = pStrideLayer * depth;
		pCompressed = true;
		pGLPixelFormat = 0; // unsupported
		pGLPixelType = 0; // unsupported
		
	}else if(format == epfDXT3){
		const int blockCountX = ((width - 1) >> 2) + 1;
		const int blockCountY = ((height - 1) >> 2) + 1;
		
		pUnitSize = 16;
		pStrideLine = pUnitSize * blockCountX;
		pStrideLayer = pStrideLine * blockCountY;
		pImageSize = pStrideLayer * depth;
		pCompressed = true;
		pGLPixelFormat = 0; // unsupported
		pGLPixelType = 0; // unsupported
		
	}else{
		DETHROW(deeInvalidParam);
	}
	
	pPixels = new GLubyte[pImageSize];
}

deoglPixelBuffer::deoglPixelBuffer(const deoglPixelBuffer &pixelBuffer) :
pWidth(pixelBuffer.pWidth),
pHeight(pixelBuffer.pHeight),
pDepth(pixelBuffer.pDepth),
pFormat(pixelBuffer.pFormat),
pUnitSize(pixelBuffer.pUnitSize),
pStrideLine(pixelBuffer.pStrideLine),
pStrideLayer(pixelBuffer.pStrideLayer),
pImageSize(pixelBuffer.pImageSize),
pCompressed(pixelBuffer.pCompressed),
pGLPixelFormat(pixelBuffer.pGLPixelFormat),
pGLPixelType(pixelBuffer.pGLPixelType),
pPixels(NULL)
{
	pPixels = new GLubyte[pImageSize];
	memcpy(pPixels, pixelBuffer.pPixels, pImageSize);
}

deoglPixelBuffer::~deoglPixelBuffer(){
	if(pPixels){
		delete [] (GLubyte*)pPixels;
	}
}



// Management
///////////////

deoglPixelBuffer::sByte1 *deoglPixelBuffer::GetPointerByte1() const{
	if(pFormat != epfByte1){
		DETHROW(deeInvalidParam);
	}
	
	return (sByte1*)pPixels;
}

deoglPixelBuffer::sByte2 *deoglPixelBuffer::GetPointerByte2() const{
	if(pFormat != epfByte2){
		DETHROW(deeInvalidParam);
	}
	
	return (sByte2*)pPixels;
}

deoglPixelBuffer::sByte3 *deoglPixelBuffer::GetPointerByte3() const{
	if(pFormat != epfByte3){
		DETHROW(deeInvalidParam);
	}
	
	return (sByte3*)pPixels;
}

deoglPixelBuffer::sByte4 *deoglPixelBuffer::GetPointerByte4() const{
	if(pFormat != epfByte4){
		DETHROW(deeInvalidParam);
	}
	
	return (sByte4*)pPixels;
}

deoglPixelBuffer::sFloat1 *deoglPixelBuffer::GetPointerFloat1() const{
	if(pFormat != epfFloat1){
		DETHROW(deeInvalidParam);
	}
	
	return (sFloat1*)pPixels;
}

deoglPixelBuffer::sFloat2 *deoglPixelBuffer::GetPointerFloat2() const{
	if(pFormat != epfFloat2){
		DETHROW(deeInvalidParam);
	}
	
	return (sFloat2*)pPixels;
}

deoglPixelBuffer::sFloat3 *deoglPixelBuffer::GetPointerFloat3() const{
	if(pFormat != epfFloat3){
		DETHROW(deeInvalidParam);
	}
	
	return (sFloat3*)pPixels;
}

deoglPixelBuffer::sFloat4 *deoglPixelBuffer::GetPointerFloat4() const{
	if(pFormat != epfFloat4){
		DETHROW(deeInvalidParam);
	}
	
	return (sFloat4*)pPixels;
}

deoglPixelBuffer::sInt1 *deoglPixelBuffer::GetPointerInt1() const{
	DEASSERT_TRUE(pFormat == epfInt1)
	return (sInt1*)pPixels;
}

deoglPixelBuffer::sDepth *deoglPixelBuffer::GetPointerDepth() const{
	if(pFormat != epfDepth){
		DETHROW(deeInvalidParam);
	}
	
	return (sDepth*)pPixels;
}

deoglPixelBuffer::sStencil *deoglPixelBuffer::GetPointerStencil() const{
	if(pFormat != epfStencil){
		DETHROW(deeInvalidParam);
	}
	
	return (sStencil*)pPixels;
}

deoglPixelBuffer::sDepthStencil *deoglPixelBuffer::GetPointerDepthStencil() const{
	if(pFormat != epfDepthStencil){
		DETHROW(deeInvalidParam);
	}
	
	return (sDepthStencil*)pPixels;
}

deoglPixelBuffer::sDXT1 *deoglPixelBuffer::GetPointerDXT1() const{
	if(pFormat != epfDXT1){
		DETHROW(deeInvalidParam);
	}
	
	return (sDXT1*)pPixels;
}

deoglPixelBuffer::sDXT3 *deoglPixelBuffer::GetPointerDXT3() const{
	if(pFormat != epfDXT3){
		DETHROW(deeInvalidParam);
	}
	
	return (sDXT3*)pPixels;
}



void deoglPixelBuffer::SetToIntColor(int red, int green, int blue, int alpha){
	const int pixelcount = pWidth * pHeight * pDepth;
	int i;
	
	switch(pFormat){
	case epfByte1:{
		sByte1 * const pixels = (sByte1*)pPixels;
		const GLubyte gred = (GLubyte)red;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
		}
		}break;
		
	case epfByte2:{
		sByte2 * const pixels = (sByte2*)pPixels;
		const GLubyte gred = (GLubyte)red;
		const GLubyte ggreen = (GLubyte)green;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
		}
		}break;
		
	case epfByte3:{
		sByte3 * const pixels = (sByte3*)pPixels;
		const GLubyte gred = (GLubyte)red;
		const GLubyte ggreen = (GLubyte)green;
		const GLubyte gblue = (GLubyte)blue;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
		}
		}break;
		
	case epfByte4:{
		sByte4 * const pixels = (sByte4*)pPixels;
		const GLubyte gred = (GLubyte)red;
		const GLubyte ggreen = (GLubyte)green;
		const GLubyte gblue = (GLubyte)blue;
		const GLubyte galpha = (GLubyte)alpha;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
			pixels[i].a = galpha;
		}
		}break;
		
	case epfFloat1:{
		sFloat1 * const pixels = (sFloat1*)pPixels;
		const GLfloat gred = (GLfloat)red / (GLfloat)255.0;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
		}
		}break;
		
	case epfFloat2:{
		sFloat2 * const pixels = (sFloat2*)pPixels;
		const GLfloat gred = (GLfloat)red / (GLfloat)255.0;
		const GLfloat ggreen = (GLfloat)green / (GLfloat)255.0;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
		}
		}break;
		
	case epfFloat3:{
		sFloat3 * const pixels = (sFloat3*)pPixels;
		const GLfloat gred = (GLfloat)red / (GLfloat)255.0;
		const GLfloat ggreen = (GLfloat)green / (GLfloat)255.0;
		const GLfloat gblue = (GLfloat)blue / (GLfloat)255.0;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
		}
		}break;
		
	case epfFloat4:{
		sFloat4 * const pixels = (sFloat4*)pPixels;
		const GLfloat gred = (GLfloat)red / (GLfloat)255.0;
		const GLfloat ggreen = (GLfloat)green / (GLfloat)255.0;
		const GLfloat gblue = (GLfloat)blue / (GLfloat)255.0;
		const GLfloat galpha = (GLfloat)alpha / (GLfloat)255.0;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
			pixels[i].a = galpha;
		}
		}break;
		
	case epfInt1:{
		sInt1 * const pixels = (sInt1*)pPixels;
		const GLuint gred = (GLuint)red;
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
		}
		}break;
		
	default:
		DETHROW_INFO(deeInvalidParam, "unsupported format");
	}
}



void deoglPixelBuffer::SetToUIntColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha){
	const int pixelcount = pWidth * pHeight * pDepth;
	int i;
	
	switch(pFormat){
	case epfByte1:{
		sByte1 * const pixels = (sByte1*)pPixels;
		const GLubyte gred = (GLubyte)red;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
		}
		}break;
		
	case epfByte2:{
		sByte2 * const pixels = (sByte2*)pPixels;
		const GLubyte gred = (GLubyte)red;
		const GLubyte ggreen = (GLubyte)green;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
		}
		}break;
		
	case epfByte3:{
		sByte3 * const pixels = (sByte3*)pPixels;
		const GLubyte gred = (GLubyte)red;
		const GLubyte ggreen = (GLubyte)green;
		const GLubyte gblue = (GLubyte)blue;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
		}
		}break;
		
	case epfByte4:{
		sByte4 * const pixels = (sByte4*)pPixels;
		const GLubyte gred = (GLubyte)red;
		const GLubyte ggreen = (GLubyte)green;
		const GLubyte gblue = (GLubyte)blue;
		const GLubyte galpha = (GLubyte)alpha;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
			pixels[i].a = galpha;
		}
		}break;
		
	case epfFloat1:{
		sFloat1 * const pixels = (sFloat1*)pPixels;
		const GLfloat gred = (GLfloat)red / (GLfloat)255.0;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
		}
		}break;
		
	case epfFloat2:{
		sFloat2 * const pixels = (sFloat2*)pPixels;
		const GLfloat gred = (GLfloat)red / (GLfloat)255.0;
		const GLfloat ggreen = (GLfloat)green / (GLfloat)255.0;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
		}
		}break;
		
	case epfFloat3:{
		sFloat3 * const pixels = (sFloat3*)pPixels;
		const GLfloat gred = (GLfloat)red / (GLfloat)255.0;
		const GLfloat ggreen = (GLfloat)green / (GLfloat)255.0;
		const GLfloat gblue = (GLfloat)blue / (GLfloat)255.0;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
		}
		}break;
		
	case epfFloat4:{
		sFloat4 * const pixels = (sFloat4*)pPixels;
		const GLfloat gred = (GLfloat)red / (GLfloat)255.0;
		const GLfloat ggreen = (GLfloat)green / (GLfloat)255.0;
		const GLfloat gblue = (GLfloat)blue / (GLfloat)255.0;
		const GLfloat galpha = (GLfloat)alpha / (GLfloat)255.0;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
			pixels[i].a = galpha;
		}
		}break;
		
	case epfInt1:{
		sInt1 * const pixels = (sInt1*)pPixels;
		const GLuint gred = (GLuint)red;
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
		}
		}break;
		
	default:
		DETHROW_INFO(deeInvalidParam, "unsupported format");
	}
}

void deoglPixelBuffer::SetToFloatColor(float red, float green, float blue, float alpha){
	const int pixelcount = pWidth * pHeight * pDepth;
	int i;
	
	switch(pFormat){
	case epfByte1:{
		sByte1 * const pixels = (sByte1*)pPixels;
		const GLubyte gred = (GLubyte)(red * 255.0f);
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
		}
		}break;
		
	case epfByte2:{
		sByte2 * const pixels = (sByte2*)pPixels;
		const GLubyte gred = (GLubyte)(red * 255.0f);
		const GLubyte ggreen = (GLubyte)(green * 255.0f);
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
		}
		}break;
		
	case epfByte3:{
		sByte3 * const pixels = (sByte3*)pPixels;
		const GLubyte gred = (GLubyte)(red * 255.0f);
		const GLubyte ggreen = (GLubyte)(green * 255.0f);
		const GLubyte gblue = (GLubyte)(blue * 255.0f);
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
		}
		}break;
		
	case epfByte4:{
		sByte4 * const pixels = (sByte4*)pPixels;
		const GLubyte gred = (GLubyte)(red * 255.0f);
		const GLubyte ggreen = (GLubyte)(green * 255.0f);
		const GLubyte gblue = (GLubyte)(blue * 255.0f);
		const GLubyte galpha = (GLubyte)(alpha * 255.0f);
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
			pixels[i].a = galpha;
		}
		}break;
		
	case epfFloat1:
	case epfDepth:{
		sFloat1 * const pixels = (sFloat1*)pPixels;
		const GLfloat gred = (GLfloat)red;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
		}
		}break;
		
	case epfFloat2:{
		sFloat2 * const pixels = (sFloat2*)pPixels;
		const GLfloat gred = (GLfloat)red;
		const GLfloat ggreen = (GLfloat)green;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
		}
		}break;
		
	case epfFloat3:{
		sFloat3 * const pixels = (sFloat3*)pPixels;
		const GLfloat gred = (GLfloat)red;
		const GLfloat ggreen = (GLfloat)green;
		const GLfloat gblue = (GLfloat)blue;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
		}
		}break;
		
	case epfFloat4:{
		sFloat4 * const pixels = (sFloat4*)pPixels;
		const GLfloat gred = (GLfloat)red;
		const GLfloat ggreen = (GLfloat)green;
		const GLfloat gblue = (GLfloat)blue;
		const GLfloat galpha = (GLfloat)alpha;
		
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
			pixels[i].g = ggreen;
			pixels[i].b = gblue;
			pixels[i].a = galpha;
		}
		}break;
		
	case epfInt1:{
		sInt1 * const pixels = (sInt1*)pPixels;
		const GLuint gred = (GLuint)(red * 4294967295.0f);
		for(i=0; i<pixelcount; i++){
			pixels[i].r = gred;
		}
		}break;
		
	default:
		DETHROW_INFO(deeInvalidParam, "unsupported format");
	}
}

void deoglPixelBuffer::SetToDepthStencil(float depth, int stencil){
	const int pixelcount = pWidth * pHeight * pDepth;
	int i;
	
	if(pFormat == epfDepthStencil){
		const int depthInt = (int)(depth * 16777215.0f);
		const GLuint depthStencil =
			(((GLuint)decMath::clamp(depthInt, 0, 16777215)) << 8)
			| (GLuint)(stencil & 0xff);
		
		sDepthStencil * const pixels = (sDepthStencil*)pPixels;
		for(i=0; i<pixelcount; i++){
			pixels[i].depthStencil = depthStencil;
		}
		
	}else{
		DETHROW_INFO(deeInvalidParam, "unsupported format");
	}
}
