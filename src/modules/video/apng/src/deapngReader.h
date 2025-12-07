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

#ifndef _DEAPNGREADER_H_
#define _DEAPNGREADER_H_

#include <png.h>

#include <dragengine/common/math/decMath.h>

class decString;
class decBaseFileReader;
class deVideoApng;



/**
 * Animated PNG reader.
 */
class deapngReader{
private:
	deVideoApng &pModule;
	
	decBaseFileReader *pReader;
	
	png_structp pReadStruct;
	png_infop pInfoStruct;
	
	int pWidth;
	int pHeight;
	int pComponentCount;
	int pFrameCount;
	float pFrameRate;
	int pFirstFrame;
	int pRowLength;
	int pImageSize;
	
	int pCurFrame;
	
	png_uint_32 pLastFrameX;
	png_uint_32 pLastFrameY;
	png_uint_32 pLastFrameWidth;
	png_uint_32 pLastFrameHeight;
	unsigned char pLastFrameDop;
	
	png_bytep pAccumData;
	png_bytep *pAccumRows;
	png_bytep pFrameData;
	png_bytep *pFrameRows;
	png_bytep pLastFrameData;
	png_bytep *pLastFrameRows;
	
	bool pErrorState;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create reader. */
	deapngReader(deVideoApng &module, decBaseFileReader *reader);
	
	/** Clean up reader. */
	~deapngReader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Animated PNG module. */
	inline deVideoApng &GetModule() const{return pModule;}
	
	/** File reader. */
	inline decBaseFileReader *GetReader() const{return pReader;}
	
	/** Width in pixels. */
	inline int GetWidth() const{return pWidth;}
	
	/** Height in pixels. */
	inline int GetHeight() const{return pHeight;}
	
	/** Component count. */
	inline int GetComponentCount() const{return pComponentCount;}
	
	/** Frame count. */
	inline int GetFrameCount() const{return pFrameCount;}
	
	/** Frame rate. */
	inline float GetFrameRate() const{return pFrameRate;}
	
	/** First frame (depends on first frame hidden). */
	inline int GetFirstFrame() const{return pFirstFrame;}
	
	
	
	/** Current frame. */
	inline int GetCurrentFrame() const{return pCurFrame;}
	
	
	
	/** Rewinds to the beginning. */
	void Rewind();
	
	/** Seek to frame. */
	void SeekFrame(int frame);
	
	/** Read frame image into accum image and advance to next frame. */
	void ReadImage();
	
	/** Copy accum image. */
	void CopyAccumImage(void *buffer, int size) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pReadHeader();
	void pReadImage();
	void pEnterErrorState();
};

#endif
