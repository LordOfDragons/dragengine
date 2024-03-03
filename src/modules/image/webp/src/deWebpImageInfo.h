/* 
 * Drag[en]gine Webp Image Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEWEBPIMAGEINFO_H_
#define _DEWEBPIMAGEINFO_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/systems/modules/image/deBaseImageInfo.h>

class deWebpModule;
class decBaseFileReader;


/**
 * Image information.
 */
class deWebpImageInfo : public deBaseImageInfo{
private:
	decString pFilename;
	decMemoryFile::Ref pData;
	int pWidth;
	int pHeight;
	bool pHasAlpha;
	bool pIsGrayscale;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create image information. */
	deWebpImageInfo( decBaseFileReader &reader );
	
	/** Clean up image information. */
	virtual ~deWebpImageInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Data. */
	inline const decMemoryFile &GetData() const{ return pData; }
	
	/** Has alpha. */
	inline bool GetHasAlpha() const{ return pHasAlpha; }
	
	/** Use grayscale. */
	inline bool GetIsGrayscale() const{ return pIsGrayscale; }
	
	/** Image width in pixels. */
	virtual int GetWidth();
	
	/** Image height in pixels. */
	virtual int GetHeight();
	
	/** Image depth in pixels. */
	virtual int GetDepth();
	
	/** Image component count. */
	virtual int GetComponentCount();
	
	/** Image bit count. */
	virtual int GetBitCount();
	
	/** Assert status code. .*/
	void Assert( VP8StatusCode statusCode ) const;
	/*@}*/
};

#endif
