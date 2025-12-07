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

#ifndef _DEBASEVIDEODECODER_H_
#define _DEBASEVIDEODECODER_H_

#include "../deBaseModule.h"

class decBaseFileReader;


/**
 * \brief Base video image decoder.
 *
 * Video decoders decode video data from a file reader. The decoding is done as individual
 * frames of video data. The file reader is held open as long as the decoder exists. The
 * user decides if the data is read synchronous or asynchronous. The video information is
 * not replicated in the decoder but found in the source video resource.
 */
class DE_DLL_EXPORT deBaseVideoDecoder{
private:
	decBaseFileReader *pFile;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create video image decoder. */
	deBaseVideoDecoder(decBaseFileReader *file);
	
	/** \brief Clean up video image decoder. */
	virtual ~deBaseVideoDecoder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief File. */
	inline decBaseFileReader *GetFile() const{ return pFile; }
	
	/** \brief File position in frames from the beginning. */
	virtual int GetPosition() = 0;
	
	/** \brief Set file position in frames from the beginning. */
	virtual void SetPosition(int position) = 0;
	
	/**
	 * \brief Decode next frame into buffer and advances file position.
	 * 
	 * Depending on the deVideo::GetComponentCount() \em buffer contains 1 to 4 components
	 * per color in the order R, G, B and A. The parameter \em size is provided as fail
	 * check for the decoder to ensure he is expecting the correct buffer size.
	 * 
	 * If successful the file position is advanced. Returns true if the frame
	 * has been decoded successfully. Otherwise \em fals is returned and an error
	 * is signaled using the engine error signaling.
	 */
	virtual bool DecodeFrame(void *buffer, int size) = 0;
	/*@}*/
};

#endif
