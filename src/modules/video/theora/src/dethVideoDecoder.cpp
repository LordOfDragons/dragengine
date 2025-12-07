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

#include "dethInfos.h"
#include "dethVideoDecoder.h"
#include "dethOggReader.h"
#include "dethStreamReader.h"
#include "deVideoTheora.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/resources/image/deImage.h>



// Class dethVideoDecoder
///////////////////////////

// Constructor, destructor
////////////////////////////

dethVideoDecoder::dethVideoDecoder(deVideoTheora &module, decBaseFileReader *file) :
deBaseVideoDecoder(file),
pModule(module),
pReader(NULL)
{
	(void)pModule;
	dethInfos infos;
	
	try{
		pReader = new dethOggReader(module, *file);
		
		pReader->ReadStreamHeaders(infos);
		if(! infos.GetHeaderFinished()){
			DETHROW(deeInvalidParam);
		}
		if(! pReader->GetStream()){
			DETHROW(deeInvalidParam);
		}
		
		pWidth = infos.GetWidth();
		pHeight = infos.GetHeight();
		pFrameCount = infos.GetFrameCount();
		pFrameRate = infos.GetFrameRate();
		pClrConvMat = infos.GetColorConversionMatrix();
		
		pFrameWidth = infos.GetInfo().frame_width;
		pFrameHeight = infos.GetInfo().frame_height;
		pPictureX = infos.GetInfo().pic_x;
		pPictureY = infos.GetInfo().pic_y;
		
		if(infos.GetInfo().pixel_fmt == TH_PF_420){ // 4:2:0 format
			pInternalPixelFormat = epf420;
			
		}else if(infos.GetInfo().pixel_fmt == TH_PF_422){ // 4:2:2 format
			pInternalPixelFormat = epf422;
			
		}else if(infos.GetInfo().pixel_fmt == TH_PF_444){ // 4:4:4 format
			pInternalPixelFormat = epf444;
			
		}else{
			pInternalPixelFormat = epfUnsupported;
		}
		
		DefConvParams(pConvParams);
		
		/*
		const char *pfstrs[] = {"4:4:4", "4:2:2", "4:2:0", "-"};
		module.LogInfoFormat("Decoder: width=%i height=%i pixelFormat=%i "
			"frameCount=%i frameRate=%g frameWidth=%i frameHeight=%i pictureX=%i "
			"pictureY=%i internalPixelFormat=%s",
				pWidth, pHeight, pPixelFormat, pFrameCount, pFrameRate, pFrameWidth,
				pFrameHeight, pPictureX, pPictureY, pfstrs[pInternalPixelFormat]);
		*/
		
		pReader->Rewind();
		
		//pReader->SeekFrame( 100 );
		//pReader->Rewind();
		
	}catch(const deException &e){
		pCleanUp();
		e.PrintError();
		throw;
	}
}

dethVideoDecoder::~dethVideoDecoder(){
	pCleanUp();
}



// Management
///////////////

int dethVideoDecoder::GetPosition(){
	return pReader->GetCurrentFrame();
}

void dethVideoDecoder::SetPosition(int position){
	pReader->SeekFrame(position);
}

bool dethVideoDecoder::DecodeFrame(void *buffer, int){
	th_ycbcr_buffer tbuffer;
	if(! pReader->GetStream()->GetDecodedFrame(tbuffer)){
		return false;
	}
	
	unsigned char *ptrCb, *ptrLineCb;
	unsigned char *ptrCr, *ptrLineCr;
	unsigned char *ptrY, *ptrLineY;
	int strideY, strideCb, strideCr;
	int x, y, px, py;
	sRGB8 *ptrDest;
	
	// copy the image into the provided buffer. for this a little bit of math is
	// involved since the color planes usually have different sizes which though
	// vary in multiples of 2.
	// 
	// plane 0 = Y
	// plane 1 = Cb
	// plane 2 = Cr
	// 
	// px = picture x
	// py = picture y
	// pw = picture width
	// ph = picture height
	// fw = frame width
	// fh = frame height
	// 
	// plane full width:  planeX = pictureX + imageX
	// plane full height: planeY = pictureY + pictureHeight - 1 - imageY
	// plane half width:  planeX = ( pictureX + imageX ) / 2 
	// plane half height: planeY = ( pictureY + pictureHeight - 1 - imageY ) / 2
	// 
	// r = y + 2(
	
	//pModule.LogInfoFormat( "DecodeFrame %i", frame );
	//pModule.LogInfoFormat( "plane 0: width=%i height=%i stride=%i", tbuffer[ 0 ].width, tbuffer[ 0 ].height, tbuffer[ 0 ].stride );
	//pModule.LogInfoFormat( "plane 1: width=%i height=%i stride=%i", tbuffer[ 1 ].width, tbuffer[ 1 ].height, tbuffer[ 1 ].stride );
	//pModule.LogInfoFormat( "plane 2: width=%i height=%i stride=%i", tbuffer[ 2 ].width, tbuffer[ 2 ].height, tbuffer[ 2 ].stride );
	
	ptrY = tbuffer[0].data;
	ptrCb = tbuffer[1].data;
	ptrCr = tbuffer[2].data;
	strideY = tbuffer[0].stride;
	strideCb = tbuffer[1].stride;
	strideCr = tbuffer[2].stride;
	ptrDest = (sRGB8*)buffer;
	
	if(pInternalPixelFormat == epf444){
		for(y=0; y<pHeight; y++){
			py = pPictureY + pHeight - 1 - y;
			ptrLineY = ptrY + py * strideY;
			ptrLineCb = ptrCb + py * strideCb;
			ptrLineCr = ptrCr + py * strideCr;
			
			for(x=0; x<pWidth; x++){
				px = pPictureX + x;
				ptrDest->red = ptrLineY[px];
				ptrDest->green = ptrLineCb[px];
				ptrDest->blue = ptrLineCr[px];
				ptrDest++;
			}
		}
		
		//ConvertColors( buffer );
		
		return true;
		
	}else if(pInternalPixelFormat == epf422){
		for(y=0; y<pHeight; y++){
			py = pPictureY + pHeight - 1 - y;
			ptrLineY = ptrY + py * strideY;
			ptrLineCb = ptrCb + py * strideCb;
			ptrLineCr = ptrCr + py * strideCr;
			
			for(x=0; x<pWidth; x++){
				px = pPictureX + x;
				ptrDest->red = ptrLineY[px];
				px >>= 1;
				ptrDest->green = ptrLineCb[px];
				ptrDest->blue = ptrLineCr[px];
				ptrDest++;
			}
		}
		
		//ConvertColors( buffer );
		
		return true;
		
	}else if(pInternalPixelFormat == epf420) {
		for(y=0; y<pHeight; y++){
			py = pPictureY + pHeight - 1 - y;
			ptrLineY = ptrY + py * strideY;
			py >>= 1;
			ptrLineCb = ptrCb + py * strideCb;
			ptrLineCr = ptrCr + py * strideCr;
			
			for(x=0; x<pWidth; x++){
				px = pPictureX + x;
				ptrDest->red = ptrLineY[px];
				px >>= 1;
				ptrDest->green = ptrLineCb[px];
				ptrDest->blue = ptrLineCr[px];
				ptrDest++;
			}
		}
		
		//ConvertColors( buffer );
		
		return true;
	}
	
	return false;
}



void dethVideoDecoder::DefConvParams(sConversionParamers &convParams){
	// offsets: 16, 128, 128
	// exculsions: 219, 224, 224
	// kr = 0.299
	// kb = 0.114
	// 
	// Y' = (Y'-offY)/excY
	// Pb = (Cb-offCb)/excCb
	// Pr = (Cr-offCr)/excCr
	// 
	// R' = Y' + c1*Pr
	// G' = Y' + c2*Pb + c3*Pr
	// B' = Y' + c4*Pb
	// 
	// c1 = 2(1-kr)
	// c2 = -2(1-kb)kb / (1-kb-kr)
	// c3 = -2(1-kr)kr / (1-kb-kr)
	// c4 = 2(1-kb)
	// 
	// | R |   | a11 a12 a13 a14 |   | Y' |
	// | G | = | a21 a22 a23 a24 | * | Cb |
	// | B |   | a31 a32 a33 a34 |   | Cr |
	// 
	// a11 = 1/excY
	// a12 = 0
	// a13 = c1/excCr
	// a14 = -offY/excY - c1*offCr/excCr
	// a21 = 1/excY
	// a22 = c2/excCb
	// a23 = c3/excCr
	// a24 = -offY/excY - c2*offCb/excCb - c3*offCr/excCr
	// a31 = 1/excY
	// a32 = c4/excCb
	// a33 = 0
	// a34 = -offY/excY - c4*offCb/excCb
	// 
	// important note for creating the color conversation matrix. the game engine
	// expects the matrix to transform from [0..1] to [0..1] hence both the input
	// YCbCr values and the output RGB values have to be in the range from 0 to 1.
	
	float offY = 16.0f / 255.0f;
	float offCb = 128.0f / 255.0f;
	float offCr = 128.0f / 255.0f;
	
	float excY = 219.0f / 255.0f;
	float excCb = 224.0f / 255.0f;
	float excCr = 224.0f / 255.0f;
	
	float kr = 0.299f;
	float kb = 0.114f;
	
	float c1 = 2.0f * (1.0f - kr);
	float c2 = -2.0f * (1.0f - kb) * kb / (1.0f - kb - kr);
	float c3 = -2.0f * (1.0f - kr) * kr / (1.0f - kb - kr);
	float c4 = 2.0f * (1.0f - kb);
	
	// calculate matrix from parameters
	// | 0 1  2  3 |   | a11 a12 a13 a14 |
	// | 4 5  6  7 | = | a21 a22 a23 a24 |
	// | 8 9 10 11 |   | a31 a32 a33 a34 |
	convParams.matrix[0] = 1.0f / excY;
	convParams.matrix[1] = 0.0f;
	convParams.matrix[2] = c1 / excCr;
	convParams.matrix[3] = -offY / excY - c1 * offCr / excCr;
	
	convParams.matrix[4] = convParams.matrix[0];
	convParams.matrix[5] = c2 / excCb;
	convParams.matrix[6] = c3 / excCr;
	convParams.matrix[7] = -offY / excY - c2 * offCb / excCb - c3 * offCr / excCr;
	
	convParams.matrix[8] = convParams.matrix[0];
	convParams.matrix[9] = c4 / excCb;
	convParams.matrix[10] = 0.0f;
	convParams.matrix[11] = -offY / excY - c4 * offCb / excCb;
	
	/*
	pModule.LogInfoFormat("conversion constants: c1=%f c2=%f c3=%f c4=%f", c1, c2, c3, c4);
	pModule.LogInfo("color conversion matrix:");
	pModule.LogInfoFormat("  | %f %f %f %f |", convParams.matrix[0], convParams.matrix[1], convParams.matrix[2], convParams.matrix[3]);
	pModule.LogInfoFormat("  | %f %f %f %f |", convParams.matrix[4], convParams.matrix[5], convParams.matrix[6], convParams.matrix[7]);
	pModule.LogInfoFormat("  | %f %f %f %f |", convParams.matrix[8], convParams.matrix[9], convParams.matrix[10], convParams.matrix[11]);
	*/
	
	// for the default values above the constants and matrix take these values:
	// c1=1.402000 c2=-0.344136 c3=-0.714136 c4=1.772000
	// | 1.164384  0.000000  1.596027 -0.874202 |                                      
	// | 1.164384 -0.391762 -0.812968  0.531668 |                                     
	// | 1.164384  2.017232  0.000000 -1.085631 |                                      
	
	// set matrix directly for the default case
	convParams.offY = 16;
	convParams.offCb = 128;
	convParams.offCr = 128;
	
	convParams.exclY = 1.0f / 219.0f;
	convParams.exclCb = 1.0f / 224.0f;
	convParams.exclCr = 1.0f / 224.0f;
	
	convParams.c1 = 1.402f;
	convParams.c2 = -0.344136f;
	convParams.c3 = -0.714136f;
	convParams.c4 = 1.772f;
}
/*
void dethVideoDecoder::ConvertColors(void *buffer){
	sRGB8 *pixels = (sRGB8*)buffer;
	int p, pixelCount = pWidth * pHeight;
	float y, pb, pr;
	int value;
	
	for(p=0; p<pixelCount; p++){
		y = (float)(pixels->red - pConvParams.offY) * pConvParams.exclY;
		pb = (float)(pixels->green - pConvParams.offCb) * pConvParams.exclCb;
		pr = (float)(pixels->blue - pConvParams.offCr) * pConvParams.exclCr;
		
		value = (int)((y + pConvParams.c1 * pr) * 255.0f);
		if(value < 0) value = 0;
		if(value > 255) value = 255;
		pixels->red = value;
		
		value = (int)((y + pConvParams.c2 * pb + pConvParams.c3 * pr) * 255.0f);
		if(value < 0) value = 0;
		if(value > 255) value = 255;
		pixels->green = value;
		
		value = (int)((y + pConvParams.c4 * pb) * 255.0f);
		if(value < 0) value = 0;
		if(value > 255) value = 255;
		pixels->blue = value;
		
		pixels++;
	}
}
*/
void dethVideoDecoder::ConvertColors(void *buffer){
	sRGB8 *pixels = (sRGB8*)buffer;
	int p, pixelCount = pWidth * pHeight;
	decColor color;
	int value;
	
	for(p=0; p<pixelCount; p++){
		color.r = (float)pixels->red / 255.0f;
		color.g = (float)pixels->green / 255.0f;
		color.b = (float)pixels->blue / 255.0f;
		
		color = pClrConvMat * color;
		
		value = (int)(color.r * 255.0f);
		if(value < 0) value = 0;
		if(value > 255) value = 255;
		pixels->red = value;
		
		value = (int)(color.g * 255.0f);
		if(value < 0) value = 0;
		if(value > 255) value = 255;
		pixels->green = value;
		
		value = (int)(color.b * 255.0f);
		if(value < 0) value = 0;
		if(value > 255) value = 255;
		pixels->blue = value;
		
		pixels++;
	}
}



// Private Functions
//////////////////////

void dethVideoDecoder::pCleanUp(){
	if(pReader){
		delete pReader;
	}
}
