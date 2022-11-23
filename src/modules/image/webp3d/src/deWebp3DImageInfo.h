/* 
 * Drag[en]gine WebP-3D Image Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEWEBP3DIMAGEINFO_H_
#define _DEWEBP3DIMAGEINFO_H_

#include <dragengine/systems/modules/image/deBaseImageInfo.h>


/**
 * Image information.
 */
class deWebp3DImageInfo : public deBaseImageInfo{
private:
	decString pFilename;
	
	
	
public:
	int width;
	int height;
	int depth;
	bool hasAlpha;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create image information. */
	deWebp3DImageInfo( const decString &filename );
	
	/** Clean up image information. */
	virtual ~deWebp3DImageInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Has alpha. */
	inline bool GetHasAlpha() const{ return hasAlpha; }
	
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
	/*@}*/
};

#endif
