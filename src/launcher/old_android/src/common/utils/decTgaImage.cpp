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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decTgaImage.h"
#include "../file/decBaseFileReader.h"
#include "../exceptions.h"



// Structures
///////////////

#define TYPE_NO_IMAGE				0

#define TYPE_COLOR_MAPPED			1
#define TYPE_TRUE_COLOR				2
#define TYPE_GRAYSCALE				3

#define TYPE_RLE_COLOR_MAPPED		9
#define TYPE_RLE_TRUE_COLOR			10
#define TYPE_RLE_GRAYSCALE			11



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



// Class decTgaImage
//////////////////////

// Constructor, destructor
////////////////////////////

decTgaImage::decTgaImage(decBaseFileReader &reader) :
pWidth(0),
pHeight(0),
pPixels(NULL)
{
	try{
		pReadTga(reader);
		
	}catch(const deException &){
		if(pPixels){
			delete [] pPixels;
		}
		throw;
	}
	
}

decTgaImage::~decTgaImage(){
	if(pPixels){
		delete [] pPixels;
	}
}



// Private function
/////////////////////

void decTgaImage::pReadTga(decBaseFileReader &reader){
	bool compressed = false;
	tgaHdr header;
	
	// header
	reader.Read(&header, sizeof(header));
	
	switch(header.imageType){
	case TYPE_TRUE_COLOR:
	case TYPE_GRAYSCALE:
		break;
		
	case TYPE_RLE_TRUE_COLOR:
	case TYPE_RLE_GRAYSCALE:
		compressed = true;
		break;
		
	case TYPE_COLOR_MAPPED:
	case TYPE_RLE_COLOR_MAPPED:
	default:
		// not supported
		DETHROW(deeInvalidFormat);
	}
	
	switch(header.bpp){
	case 8:
	case 24:
	case 32:
		break;
		
	default:
		DETHROW(deeInvalidFormat);
	}
	
	if(header.idLength > 0){
		reader.MovePosition(header.idLength);
	}
	
	// image
	pWidth = header.width;
	pHeight = header.height;
	int y, x, fromY, stepY, fromX, stepX, ry, rx;
	
	if(header.imgDesc & 0x20){ // top -> bottom
		fromY = 0;
		stepY = 1;
		
	}else{ // bottom -> top
		fromY = pHeight - 1;
		stepY = -1;
	}
	
	if(header.imgDesc & 0x10){ // right -> left
		fromX = pWidth - 1;
		stepX = -1;
		
	}else{ // left -> right
		fromX = 0;
		stepX = 1;
	}
	
	pPixels = new sPixel[pWidth * pHeight];
	
	if(compressed){
		int remaining = 0;
		bool isRun = false;
		sPixel copy;
		
		for(ry=fromY,y=0; y<pHeight; ry+=stepY,y++){
			if(header.bpp == 8){
				copy.a = 255;
				sPixel * const row = pPixels + pWidth * ry;
				for(rx=fromX,x=0; x<pWidth; rx+=stepX,x++){
					if(remaining == 0){
						remaining = reader.ReadByte();
						isRun = (remaining & 0x80) == 0x80;
						remaining = (remaining & 0x7f) + 1;
						if(isRun){
							copy.b = copy.g = copy.r = reader.ReadByte();
						}
					}
					
					if(isRun){
						row[rx] = copy;
						
					}else{
						row[rx].b = row[rx].g = row[rx].r = reader.ReadByte();
						row[rx].a = 255;
					}
					
					remaining--;
				}
				
			}else if(header.bpp == 24){
				copy.a = 255;
				sPixel * const row = pPixels + pWidth * ry;
				for(rx=fromX,x=0; x<pWidth; rx+=stepX,x++){
					if(remaining == 0){
						remaining = reader.ReadByte();
						isRun = (remaining & 0x80) == 0x80;
						remaining = (remaining & 0x7f) + 1;
						if(isRun){
							copy.b = reader.ReadByte();
							copy.g = reader.ReadByte();
							copy.r = reader.ReadByte();
						}
					}
					
					if(isRun){
						row[rx] = copy;
						
					}else{
						row[rx].b = reader.ReadByte();
						row[rx].g = reader.ReadByte();
						row[rx].r = reader.ReadByte();
						row[rx].a = 255;
					}
					
					remaining--;
				}
				
			}else{
				sPixel * const row = pPixels + pWidth * ry;
				for(rx=fromX,x=0; x<pWidth; rx+=stepX,x++){
					if(remaining == 0){
						remaining = reader.ReadByte();
						isRun = (remaining & 0x80) == 0x80;
						remaining = (remaining & 0x7f) + 1;
						if(isRun){
							copy.b = reader.ReadByte();
							copy.g = reader.ReadByte();
							copy.r = reader.ReadByte();
							copy.a = reader.ReadByte();
						}
					}
					
					if(isRun){
						row[rx] = copy;
						
					}else{
						row[rx].b = reader.ReadByte();
						row[rx].g = reader.ReadByte();
						row[rx].r = reader.ReadByte();
						row[rx].a = reader.ReadByte();
					}
					
					remaining--;
				}
			}
		}
		
	}else{
		for(ry=fromY,y=0; y<pHeight; ry+=stepY,y++){
			if(header.bpp == 8){
				sPixel * const row = pPixels + pWidth * ry;
				for(rx=fromX,x=0; x<pWidth; rx+=stepX,x++){
					row[rx].b = row[rx].g = row[rx].r = reader.ReadByte();
					row[rx].a = 255;
				}
				
			}else if(header.bpp == 24){
				sPixel * const row = pPixels + pWidth * ry;
				for(rx=fromX,x=0; x<pWidth; rx+=stepX,x++){
					row[rx].b = reader.ReadByte();
					row[rx].g = reader.ReadByte();
					row[rx].r = reader.ReadByte();
					row[rx].a = 255;
				}
				
			}else{
				sPixel * const row = pPixels + pWidth * ry;
				for(rx=fromX,x=0; x<pWidth; rx+=stepX,x++){
					row[rx].b = reader.ReadByte();
					row[rx].g = reader.ReadByte();
					row[rx].r = reader.ReadByte();
					row[rx].a = reader.ReadByte();
				}
			}
		}
	}
}
