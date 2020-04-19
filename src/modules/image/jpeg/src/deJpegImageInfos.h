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

#ifndef _DEJPEGIMAGEINFO_H_
#define _DEJPEGIMAGEINFO_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/modules/image/deBaseImageInfo.h>

// include last to avoid conflicts with INT32 typedef and windows
#include "jpeglib.h"



class decBaseFileReader;
class deJpegModule;



/**
 * @brief JPEG Image Information.
 */
class deJpegImageInfo : public deBaseImageInfo{
private:
	deJpegModule *pModule;
	
	decString pFilename;
	int pFileSize;
	int pFilePosition;
	
	jpeg_decompress_struct pDecompress;
	jpeg_error_mgr pErrorMgr;
	jpeg_source_mgr pSourceMgr;
	
	JOCTET *pDataBuffer;
	int pDataBufferPosition;
	decBaseFileReader *pReader;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new information object. */
	deJpegImageInfo( deJpegModule *module, const char *filename );
	/** Cleans up the information object. */
	virtual ~deJpegImageInfo();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deJpegModule *GetModule() const{ return pModule; }
	
	/** Retrieves the filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Retrieves the decompress structure. */
	inline jpeg_decompress_struct &GetDecompress(){ return pDecompress; }
	inline jpeg_decompress_struct *GetDecompressPtr(){ return &pDecompress; }
	
	/** Retrieves the error manager structure. */
	inline jpeg_error_mgr &GetErrorMgr(){ return pErrorMgr; }
	inline jpeg_error_mgr *GetErrorMgrPtr(){ return &pErrorMgr; }
	
	/** Retrieves the source manager structure. */
	inline jpeg_source_mgr &GetSourceMgr(){ return pSourceMgr; }
	inline jpeg_source_mgr *GetSourceMgrPtr(){ return &pSourceMgr; }
	/*@}*/
	
	/** @name Data Buffer */
	/*@{*/
	/** Retrieves the data buffer. */
	inline JOCTET *GetDataBuffer() const{ return pDataBuffer; }
	/** Retrieves the file size. */
	inline int GetFileSize() const{ return pFileSize; }
	/** Retrieves the file position. */
	inline int GetFilePosition() const{ return pFilePosition; }
	
	/** Init reading into data buffer. */
	void InitRead( decBaseFileReader *reader );
	/** Read data from file into data buffer returning the number of bytes read. */
	void ReadNext();
	/** Skip data from file adjusting the source manager accordingly. */
	void SkipNext( int bytes );
	/** Close reader. */
	void CloseReader();
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/** Retrieves the width in pixels. */
	virtual int GetWidth();
	/** Retrieves the height in pixels. */
	virtual int GetHeight();
	/** Retrieves the depth in pixels. */
	virtual int GetDepth();
	/** Retrieves the number of components. */
	virtual int GetComponentCount();
	/** Retrieves the bit count of each component. */
	virtual int GetBitCount();
	/*@}*/
};

#endif
