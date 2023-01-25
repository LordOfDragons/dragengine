/* 
 * Drag[en]gine PNG Image Module
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

#ifndef _DEPNGIMAGEINFO_H_
#define _DEPNGIMAGEINFO_H_

#include "png.h"

#include <dragengine/systems/modules/image/deBaseImageInfo.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class dePngModule;



// image information class
class dePngImageInfo : public deBaseImageInfo{
public:
	struct sFeedback{
		dePngModule *module;
		decString filename;
	};
	
public:
	sFeedback feedback;
	
	decString filename;
	png_structp readStruct;
	png_infop infoStruct;
	
	int width;
	int height;
	int componentCount;
	int bitCount;
	
public:
	// constructor, destructor
	dePngImageInfo(const char *aFilename);
	~dePngImageInfo();
	// management
	int GetWidth();
	int GetHeight();
	int GetDepth();
	int GetComponentCount();
	int GetBitCount();
};

#endif
