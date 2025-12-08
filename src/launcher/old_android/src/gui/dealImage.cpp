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

#include "dealImage.h"
#include "dealDisplay.h"
#include "../dealLauncher.h"
#include "../common/exceptions.h"
#include "../common/math/decMath.h"
#include "../common/file/decMemoryFile.h"
#include "../common/file/decMemoryFileReader.h"
#include "../common/utils/decTgaImage.h"
#include "../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealImage
////////////////////

// Constructors, destructors
//////////////////////////////

dealImage::dealImage(dealDisplay &display, const char *filename) :
pDisplay(display),

pTexture(0),
pWidth(0),
pHeight(0)
{
	try{
		pLoadImage(filename);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dealImage::~dealImage(){
	pCleanUp();
}



// Management
///////////////



// Private functions
//////////////////////

void dealImage::pCleanUp(){
	if(pTexture != 0){
		glDeleteTextures(1, &pTexture);
	}
}

void dealImage::pLoadImage(const char *filename){
	decMemoryFile::Ref memoryFileImage = NULL;
	decMemoryFileReader::Ref tgaImageRader = NULL;
	decTgaImage *tgaImage = NULL;
	
	try{
		memoryFileImage.TakeOver(new decMemoryFile(filename));
		pDisplay.GetLauncher().LoadAsset(filename, *memoryFileImage);
		
		tgaImageRader.TakeOver(new decMemoryFileReader(memoryFileImage));
		
		tgaImage = new decTgaImage(*tgaImageRader);
		pCreateTexture(*tgaImage);
		
		delete tgaImage;
	}catch(const deException &){
		if(tgaImage){
			delete tgaImage;
		}
		throw;
	}
}

void dealImage::pCreateTexture(decTgaImage &tgaImage){
	if(pTexture != 0){
		DETHROW(deeInvalidParam);
	}
	
	glGenTextures(1, &pTexture);
	if(pTexture == 0){
		DETHROW(deeInvalidAction);
	}
	
	pWidth = tgaImage.GetWidth();
	pHeight = tgaImage.GetHeight();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pTexture);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pWidth, pHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tgaImage.GetPixels());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}
