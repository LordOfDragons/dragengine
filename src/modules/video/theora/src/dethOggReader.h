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
#ifndef _DETHOGGREADER_H_
#define _DETHOGGREADER_H_

// includes
#include <ogg/ogg.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class decBaseFileReader;
class dethStreamReader;
class dethInfos;
class deVideoTheora;



/**
 * @brief Ogg File Reader.
 * \author DragonDreams GmbH
 * @version 1.0
 * \date 2024
 */
class dethOggReader{
private:
	deVideoTheora &pModule;
	
	decBaseFileReader &pReader;
	ogg_sync_state pSyncState;
	dethStreamReader *pStream;
	
	int pCurFrame;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new ogg file. */
	dethOggReader(deVideoTheora &module, decBaseFileReader &reader);
	/** Cleans up the ogg file. */
	~dethOggReader();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read streams and find theora header. */
	void ReadStreamHeaders(dethInfos &infos);
	
	/** Reads data from the file returning the amount of data read. */
	int ReadFromFile(char *buffer, int size);
	/** Reads a page. */
	bool ReadPage(ogg_page &page);
	/** Reads next page with the given serial ignoring all others inbetween. */
	bool ReadPage(ogg_page &page, int serial);
	
	/** Retrieves the current frame. */
	inline int GetCurrentFrame() const{return pCurFrame;}
	/** Rewind. */
	void Rewind();
	/** Seek to frame. */
	void SeekFrame(int frame);
	
	/** Retrieves the stream or NULL if not found. */
	inline dethStreamReader *GetStream() const{return pStream;}
	/** Create default color conversion matrix. */
	void DefaultColorConversionMatrix(decColorMatrix3 &matrix);
	/*@}*/
};

// end of include only once
#endif
