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

#ifndef _DEJPEGENCODER_H_
#define _DEJPEGENCODER_H_

#include "jpeglib.h"



class decBaseFileWriter;
class deJpegModule;



/**
 * @brief JPEG Encoder.
 */
class deJpegEncoder{
private:
	deJpegModule *pModule;
	
	jpeg_compress_struct pEncode;
	jpeg_error_mgr pErrorMgr;
	jpeg_destination_mgr pDestMgr;
	
	JOCTET *pDataBuffer;
	decBaseFileWriter *pWriter;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new information object. */
	deJpegEncoder( deJpegModule *module );
	/** Cleans up the information object. */
	~deJpegEncoder();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deJpegModule *GetModule() const{ return pModule; }
	
	/** Retrieves the encode structure. */
	inline jpeg_compress_struct &GetEncode(){ return pEncode; }
	inline jpeg_compress_struct *GetEncodePtr(){ return &pEncode; }
	
	/** Retrieves the error manager structure. */
	inline jpeg_error_mgr &GetErrorMgr(){ return pErrorMgr; }
	inline jpeg_error_mgr *GetErrorMgrPtr(){ return &pErrorMgr; }
	
	/** Retrieves the destination manager structure. */
	inline jpeg_destination_mgr &GetDestinationMgr(){ return pDestMgr; }
	inline jpeg_destination_mgr *GetDestinationMgrPtr(){ return &pDestMgr; }
	/*@}*/
	
	/** @name Data Buffer */
	/*@{*/
	/** Retrieves the data buffer. */
	inline JOCTET *GetDataBuffer() const{ return pDataBuffer; }
	
	/** Init writing into data buffer. */
	void InitWrite( decBaseFileWriter *writer );
	/** Reset write buffer. */
	void ResetBuffer();
	/** Write entire data buffer into the file. */
	void WriteEntireBuffer();
	/** Write remaining data buffer into the file. */
	void WriteRemaining();
	/** Close the writer. */
	void CloseReader();
	/*@}*/
};

#endif
