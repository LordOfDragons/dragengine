/* 
 * Drag[en]gine JPEG Image Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
