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

// includes
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decXpmImage.h"
#include "../exceptions.h"


// struct
struct rgba{
	unsigned char r, g, b, a;
};
struct colorEntry{
	rgba color;
	char index[4];
};

// class decXpmImage
//////////////////////
// constructor, destructor
decXpmImage::decXpmImage(const char *xpmData[], bool flip){
	int colorCount = 0;
	int pixelBytes = 0;
	int color;
	int x, y, c, line=0;
	const char *lineData;
	rgba *destData, *destLine;
	colorEntry *colorTable = NULL;
	pWidth = 0;
	pHeight = 0;
	pData = NULL;
	try{
		// image information
		lineData = xpmData[line++];
		pWidth = pReadInt(&lineData);
		pHeight = pReadInt(&lineData);
		colorCount = pReadInt(&lineData);
		pixelBytes = pReadInt(&lineData);
		if(pWidth<1 || pHeight<1 || colorCount<1) DETHROW(deeInvalidFileFormat);
		if(pixelBytes<1 || pixelBytes>4) DETHROW(deeInvalidFileFormat);
		// read color table
		if(!(colorTable = new colorEntry[colorCount])) DETHROW(deeOutOfMemory);
		for(c=0; c<colorCount; c++){
			lineData = xpmData[line++];
			memcpy(&colorTable[c].index[0], lineData, pixelBytes);
			lineData += pixelBytes + 1;
			if(*lineData != 'c') DETHROW(deeInvalidFileFormat);
			lineData += 2;
			if(*lineData == '#'){
				lineData++;
				color = pReadColor(&lineData);
				colorTable[c].color.r = (unsigned char)((color >> 16) & 0xff);
				colorTable[c].color.g = (unsigned char)((color >> 8) & 0xff);
				colorTable[c].color.b = (unsigned char)(color & 0xff);
				colorTable[c].color.a = 255;
			}else if(strncmp(lineData, "None", 4) == 0){
				colorTable[c].color.r = 0;
				colorTable[c].color.g = 0;
				colorTable[c].color.b = 0;
				colorTable[c].color.a = 255;
				lineData += 4;
			}else{
				DETHROW(deeInvalidFileFormat);
			}
		}
		// read pixels
		if(!(destData = new rgba[pWidth*pHeight])) DETHROW(deeOutOfMemory);
		pData = reinterpret_cast<char*>(destData);
		for(y=0; y<pHeight; y++){
			lineData = xpmData[line++];
			destLine = destData + pWidth * (flip ? pHeight - 1 - y : y);
			for(x=0; x<pWidth; x++){
				for(c=0; c<colorCount; c++){
					if(strncmp(&colorTable[c].index[0], lineData, pixelBytes) == 0) break;
				}
				if(c == colorCount) DETHROW(deeInvalidFileFormat);
				lineData += pixelBytes;
				*destLine = colorTable[c].color;
				destLine++;
			}
		}
		// clean up
		delete [] colorTable;
	}catch(const deException &){
		if(pData) delete [] pData;
		if(colorTable) delete [] colorTable;
		throw;
	}
	
}
decXpmImage::~decXpmImage(){
	if(pData) delete [] pData;
}
// private
int decXpmImage::pReadInt(const char **pdata){
	int value = 0;
	char theChar = **pdata;
	if(theChar<'0' || theChar>'9') DETHROW(deeInvalidFileFormat);
	while(true){
		theChar = **pdata;
		if(theChar>='0' && theChar<='9'){
			value = value * 10 + (theChar - '0');
			(*pdata)++;
		}else{
			break;
		}
	}
	if(**pdata==' ' || **pdata=='\t') (*pdata)++;
	return value;
}
int decXpmImage::pReadColor(const char **pdata){
	int value = 0;
	char theChar;
	for(int i=0; i<6; i++){
		theChar = **pdata;
		if(theChar>='0' && theChar<='9'){
			value = value * 16 + (theChar - '0');
			(*pdata)++;
		}else if(theChar>='a' && theChar<='f'){
			value = value * 16 + 10 + (theChar - 'a');
			(*pdata)++;
		}else if(theChar>='A' && theChar<='F'){
			value = value * 16 + 10 + (theChar - 'A');
			(*pdata)++;
		}else{
			DETHROW(deeInvalidFileFormat);
		}
	}
	if(**pdata==' ' || **pdata=='\t') (*pdata)++;
	return value;
}
