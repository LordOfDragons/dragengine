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

#include "deoglTextureCompression.h"
#include "../pixelbuffer/deoglPixelBuffer.h"
#include "../pixelbuffer/deoglPixelBufferMipMap.h"
#include "../../../squish/squish.h"

#include <dragengine/common/exceptions.h>



// Class deoglTextureCompression
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTextureCompression::deoglTextureCompression(){
	pDecompressedData = NULL;
	pCompressedData = NULL;
	
	pDecompressedDataMipMap = NULL;
	pCompressedDataMipMap = NULL;
	
	pFastCompression = true;
}

deoglTextureCompression::~deoglTextureCompression(){
}



// Management
///////////////

void deoglTextureCompression::SetDecompressedData(deoglPixelBuffer *pixelBuffer){
	pDecompressedData = pixelBuffer;
}

void deoglTextureCompression::SetCompressedData(deoglPixelBuffer *pixelBuffer){
	pCompressedData = pixelBuffer;
}



void deoglTextureCompression::SetDecompressedDataMipMap(deoglPixelBufferMipMap *pixelBufferMipMap){
	pDecompressedDataMipMap = pixelBufferMipMap;
}

void deoglTextureCompression::SetCompressedDataMipMap(deoglPixelBufferMipMap *pixelBufferMipMap){
	pCompressedDataMipMap = pixelBufferMipMap;
}



void deoglTextureCompression::SetFastCompression(bool fastCompression){
	pFastCompression = fastCompression;
}



void deoglTextureCompression::Compress(){
	if(! pCompressedData){
		DETHROW(deeInvalidParam);
	}
	
	if(pCompressedData->GetFormat() == deoglPixelBuffer::epfDXT1){
		CompressDXT1();
		
	}else if(pCompressedData->GetFormat() == deoglPixelBuffer::epfDXT3){
		CompressDXT3();
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deoglTextureCompression::CompressDXT1(){
	if(! pDecompressedData || ! pCompressedData){
		DETHROW(deeInvalidParam);
	}
	if(pDecompressedData->GetWidth() != pCompressedData->GetWidth()
	 || pDecompressedData->GetHeight() != pCompressedData->GetHeight()
	 || pDecompressedData->GetDepth() != pCompressedData->GetDepth()){
		DETHROW(deeInvalidParam);
	}
	if(pCompressedData->GetFormat() != deoglPixelBuffer::epfDXT1){
		DETHROW(deeInvalidParam);
	}
	
	pCompressSquish(*pDecompressedData, *pCompressedData, pGetQualitySquishFlags() | squish::kDxt1);
}

void deoglTextureCompression::CompressDXT3(){
	if(! pDecompressedData || ! pCompressedData){
		DETHROW(deeInvalidParam);
	}
	if(pDecompressedData->GetWidth() != pCompressedData->GetWidth()
	 || pDecompressedData->GetHeight() != pCompressedData->GetHeight()
	 || pDecompressedData->GetDepth() != pCompressedData->GetDepth()){
		DETHROW(deeInvalidParam);
	}
	if(pCompressedData->GetFormat() != deoglPixelBuffer::epfDXT3){
		DETHROW(deeInvalidParam);
	}
	
	pCompressSquish(*pDecompressedData, *pCompressedData, pGetQualitySquishFlags() | squish::kDxt3);
}



void deoglTextureCompression::CompressMipMap(){
	if(! pCompressedDataMipMap){
		DETHROW(deeInvalidParam);
	}
	
	const deoglPixelBuffer &pixelBuffer = *pCompressedDataMipMap->GetPixelBuffer(0);
	
	if(pixelBuffer.GetFormat() == deoglPixelBuffer::epfDXT1){
		CompressMipMapDXT1();
		
	}else if(pixelBuffer.GetFormat() == deoglPixelBuffer::epfDXT3){
		CompressMipMapDXT3();
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deoglTextureCompression::CompressMipMapDXT1(){
	if(! pDecompressedDataMipMap || ! pCompressedDataMipMap){
		DETHROW(deeInvalidParam);
	}
	if(pDecompressedDataMipMap->GetPixelBufferCount() != pCompressedDataMipMap->GetPixelBufferCount()){
		DETHROW(deeInvalidParam);
	}
	
	const deoglPixelBuffer &pixelBufferDecompressed = *pDecompressedDataMipMap->GetPixelBuffer(0);
	const deoglPixelBuffer &pixelBufferCompressed = *pCompressedDataMipMap->GetPixelBuffer(0);
	
	if(pixelBufferDecompressed.GetWidth() != pixelBufferCompressed.GetWidth()
	 || pixelBufferDecompressed.GetHeight() != pixelBufferCompressed.GetHeight()
	 || pixelBufferDecompressed.GetDepth() != pixelBufferCompressed.GetDepth()){
		DETHROW(deeInvalidParam);
	}
	if(pixelBufferCompressed.GetFormat() != deoglPixelBuffer::epfDXT1){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pDecompressedDataMipMap->GetPixelBufferCount();
	const int flags = pGetQualitySquishFlags() | squish::kDxt1;
	int i;
	
	for(i=0; i<count; i++){
		pCompressSquish(*pDecompressedDataMipMap->GetPixelBuffer(i),
			*pCompressedDataMipMap->GetPixelBuffer( i ), flags );
	}
}

void deoglTextureCompression::CompressMipMapDXT3(){
	if(! pDecompressedDataMipMap || ! pCompressedDataMipMap){
		DETHROW(deeInvalidParam);
	}
	if(pDecompressedDataMipMap->GetPixelBufferCount() != pCompressedDataMipMap->GetPixelBufferCount()){
		DETHROW(deeInvalidParam);
	}
	
	const deoglPixelBuffer &pixelBufferDecompressed = *pDecompressedDataMipMap->GetPixelBuffer(0);
	const deoglPixelBuffer &pixelBufferCompressed = *pCompressedDataMipMap->GetPixelBuffer(0);
	
	if(pixelBufferDecompressed.GetWidth() != pixelBufferCompressed.GetWidth()
	 || pixelBufferDecompressed.GetHeight() != pixelBufferCompressed.GetHeight()
	 || pixelBufferDecompressed.GetDepth() != pixelBufferCompressed.GetDepth()){
		DETHROW(deeInvalidParam);
	}
	if(pixelBufferCompressed.GetFormat() != deoglPixelBuffer::epfDXT3){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pDecompressedDataMipMap->GetPixelBufferCount();
	const int flags = pGetQualitySquishFlags() | squish::kDxt3;
	int i;
	
	for(i=0; i<count; i++){
		pCompressSquish(*pDecompressedDataMipMap->GetPixelBuffer(i),
			*pCompressedDataMipMap->GetPixelBuffer( i ), flags );
	}
}



// Private Functions
//////////////////////

int deoglTextureCompression::pGetQualitySquishFlags(){
	int flags = 0;
	
	if(pFastCompression){
		flags |= squish::kColourRangeFit;
		
	}else{
		flags |= squish::kColourClusterFit; // high quality
		//flags |= squish::kColourIterativeClusterFit; // very high quality
	}
	
	flags |= squish::kColourMetricPerceptual;
	
	return flags;
}

void deoglTextureCompression::pCompressSquish(const deoglPixelBuffer &pixelBufferFrom, deoglPixelBuffer &pixelBufferTo, int flags){
	squish::u8 *ptrCompressed = (squish::u8*)pixelBufferTo.GetPointer();
	const int width = pixelBufferFrom.GetWidth();
	const int height = pixelBufferFrom.GetHeight();
	const int depth = pixelBufferFrom.GetDepth();
	const int strideLayer = width * height;
	const int blockSize = pixelBufferTo.GetUnitSize();
	const deoglPixelBuffer::sByte1 *ptrByte1 = NULL;
	const deoglPixelBuffer::sByte2 *ptrByte2 = NULL;
	const deoglPixelBuffer::sByte3 *ptrByte3 = NULL;
	const deoglPixelBuffer::sByte4 *ptrByte4 = NULL;
	int x, y, z, x2, y2, bx, by, bi, bm;
	squish::u8 blockData[64];
	int dataOffset;
	int blockMask;
	
	for(bi=0, x=0; x<16; x++){
		blockData[bi++] = 0;
		blockData[bi++] = 0;
		blockData[bi++] = 0;
		blockData[bi++] = 255; // for kDxt1 alpha has to be 255 or else hell breaks loose
	}
	
	if(pixelBufferFrom.GetFormat() == deoglPixelBuffer::epfByte1){
		ptrByte1 = pixelBufferFrom.GetPointerByte1();
		
	}else if(pixelBufferFrom.GetFormat() == deoglPixelBuffer::epfByte2){
		ptrByte2 = pixelBufferFrom.GetPointerByte2();
		
	}else if(pixelBufferFrom.GetFormat() == deoglPixelBuffer::epfByte3){
		ptrByte3 = pixelBufferFrom.GetPointerByte3();
		
	}else if(pixelBufferFrom.GetFormat() == deoglPixelBuffer::epfByte4){
		ptrByte4 = pixelBufferFrom.GetPointerByte4();
		
	}else{
		DETHROW(deeInvalidParam);
	}
	
	for(z=0; z<depth; z++){
		for(y=0; y<height; y+=4){
			y2 = y + 4;
			
			for(x=0; x<width; x+=4){
				x2 = x + 4;
				
				// fill block data. padding pixels have undefined value and their mask cleared
				blockMask = 0;
				bi = 0;
				bm = 1; // bm is the mask bit to set shifted with each pixel
				
				for(by=y; by<y2; by++){
					if(by < height){
						dataOffset = strideLayer * z + width * by + x;
						
						for(bx=x; bx<x2; bx++){
							if(bx < width){
								blockMask |= bm;
								
								if(ptrByte1){
									blockData[bi++] = (squish::u8)ptrByte1[dataOffset].r;
									bi += 3;
									
								}else if(ptrByte2){
									blockData[bi++] = (squish::u8)ptrByte2[dataOffset].r;
									blockData[bi++] = (squish::u8)ptrByte2[dataOffset].g;
									bi += 2;
									
								}else if(ptrByte3){
									blockData[bi++] = (squish::u8)ptrByte3[dataOffset].r;
									blockData[bi++] = (squish::u8)ptrByte3[dataOffset].g;
									blockData[bi++] = (squish::u8)ptrByte3[dataOffset].b;
									bi++;
									
								}else if(ptrByte4){
									blockData[bi++] = (squish::u8)ptrByte4[dataOffset].r;
									blockData[bi++] = (squish::u8)ptrByte4[dataOffset].g;
									blockData[bi++] = (squish::u8)ptrByte4[dataOffset].b;
									blockData[bi++] = (squish::u8)ptrByte4[dataOffset].a;
								}
								
							}else{
								bi += 4;
							}
							
							bm <<= 1;
							dataOffset++;
						}
						
					}else{
						break; // this line and all following lines are padding
					}
				}
				
				// compress using squish and write next block to the compressed data pixel buffer
				squish::CompressMasked(&blockData[0], blockMask, ptrCompressed, flags);
				ptrCompressed += blockSize;
			}
		}
	}
}
