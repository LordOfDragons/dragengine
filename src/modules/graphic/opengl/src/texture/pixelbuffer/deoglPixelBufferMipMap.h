/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLPIXELBUFFERMIPMAP_H_
#define _DEOGLPIXELBUFFERMIPMAP_H_

#include "deoglPixelBuffer.h"



/**
 * Pixel Buffer Mip Map.
 * Mip map chain of pixel buffers.
 */
class deoglPixelBufferMipMap : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglPixelBufferMipMap> Ref;
	
	
	
private:
	deoglPixelBuffer::Ref *pPixelBuffers;
	int pPixelBufferCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new pixel buffer mip map. */
	deoglPixelBufferMipMap( deoglPixelBuffer::ePixelFormats format, int width, int height, int depth, int maxLevel );
	
protected:
	/** Cleans up the opengl array texture. */
	virtual ~deoglPixelBufferMipMap();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the number of pixel buffers which is the mip map level count. */
	inline int GetPixelBufferCount() const{ return pPixelBufferCount; }
	/** Retrieves the pixel buffer for a mip map level. */
	const deoglPixelBuffer::Ref &GetPixelBuffer( int level ) const;
	
	/** Reduce maximum mip map level count. */
	void ReducePixelBufferCount( int reduceByCount );
	
	/** Create mip maps from the base level using simple box filtering. */
	void CreateMipMaps();
	
	/**
	 * Create mip maps from the base level using simple box filtering.
	 * Only components with their mask set to true are processed.
	 */
	void CreateMipMaps( bool maskRed, bool maskGreen, bool maskBlue, bool maskAlpha );
	
	/** Create mip maps from the base level using maximum filtering. */
	void CreateMipMapsMax();
	
	/**
	 * Create mip maps from the base level using maximum filtering.
	 * Only components with their mask set to true are processed.
	 */
	void CreateMipMapsMax( bool maskRed, bool maskGreen, bool maskBlue, bool maskAlpha );
	
	/** Create mip maps from the base level using minimum filtering. */
	void CreateMipMapsMin();
	
	/**
	 * Create mip maps from the base level using minimum filtering.
	 * Only components with their mask set to true are processed.
	 */
	void CreateMipMapsMin( bool maskRed, bool maskGreen, bool maskBlue, bool maskAlpha );
	
	/**
	 * Create normal mip maps from the base level using simple box filtering. In addition
	 * the variance while downsampling the normals is stored in the alpha channel of the
	 * respective mip map levels.
	 */
	void CreateNormalMipMaps();
	/**
	 * Create roughness mip maps from the base level using box filtering on the roughness
	 * values using the normal values to adjust the roughness at distance. Processes only
	 * the alpha value (for the time being).
	 */
	void CreateRoughnessMipMaps( deoglPixelBufferMipMap &normalPixeBufferMipMap );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pGetTypeParams( int pixelBufferType, int &componentCount, bool &floatData ) const;
};

#endif
