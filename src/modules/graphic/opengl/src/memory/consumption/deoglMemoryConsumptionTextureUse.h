/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLMEMORYCONSUMPTIONTEXTUREUSE_H_
#define _DEOGLMEMORYCONSUMPTIONTEXTUREUSE_H_

#include "deoglMemoryConsumptionGPUUse.h"

#include <dragengine/common/math/decMath.h>


class deoglMemoryConsumptionTexture;
class deoglCapsTextureFormat;


/**
 * Texture memory consumption usage.
 */
class deoglMemoryConsumptionTextureUse{
public:
	deoglMemoryConsumptionGPUUse colorCompressed;
	deoglMemoryConsumptionGPUUse colorUncompressed;
	
	deoglMemoryConsumptionGPUUse depthCompressed;
	deoglMemoryConsumptionGPUUse depthUncompressed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create memory consumption usage. */
	deoglMemoryConsumptionTextureUse( deoglMemoryConsumptionTexture &tracker );
	
	/** Clean up vbo memory consumption. */
	~deoglMemoryConsumptionTextureUse();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Total consumption. */
	unsigned long long Total() const;
	
	/** Total compressed consumption. */
	unsigned long long TotalCompressed() const;
	
	/** Total uncompressed consumption. */
	unsigned long long TotalUncompressed() const;
	
	/** Clear all consumptions. */
	void Clear();
	
	/** Set consumption. */
	void Set( unsigned long long consumption, bool depth, bool compressed );
	
	/** Set compressed consumption. */
	void SetCompressed( unsigned long long consumption, const deoglCapsTextureFormat &format );
	
	/** Set uncompressed consumption. */
	void SetUncompressed( const deoglCapsTextureFormat &format, int width, int height,
		int depth, int mipMapLevels );
	
	/** Calculate base uncompressed consumption. */
	unsigned long long BaseConsumption( const deoglCapsTextureFormat &format,
		int width, int height, int depth ) const;
	
	/** Calculate mip mapped uncompressed consumption. */
	unsigned long long MipMappedConsumption( int levels, int width, int height,
		unsigned long long baseConsumption ) const;
	/*@}*/
};

#endif
