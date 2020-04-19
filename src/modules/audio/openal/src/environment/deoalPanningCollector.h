/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALPANNINGCOLLECTOR_H_
#define _DEOALPANNINGCOLLECTOR_H_

#include <dragengine/common/math/decMath.h>

class deoalAudioThread;


/**
 * \brief Panning collector.
 * 
 * Collects panning direction information using a cube map approach. Rays are inserted into
 * the collector by finding the cube map face pixel they are intersecting. Each cube map
 * face pixel stores the maximum weight of all contributing rays. Using the maximum approach
 * avoids problems with rays bulking up due to ray tracing with small ray count. Once all
 * rays are collected the panning collector calculates the final panning vector which is
 * a direction vector with magnitude between 0 and 1 indicating the strength of directivity.
 * 
 * The axes map to the faces the following way:
 * - Face positive XAxis: coord.x = ZAxis, coord.y = YAxis
 * - Face negative XAxis: coord.x = -ZAxis, coord.y = YAxis
 * - Face positive YAxis: coord.x = XAxis, coord.y = ZAxis
 * - Face negative YAxis: coord.x = XAxis, coord.y = -ZAxis
 * - Face positive ZAxis: coord.x = XAxis, coord.y = YAxis
 * - Face negative ZAxis: coord.x = -XAxis, coord.y = YAxis
 */
class deoalPanningCollector{
private:
	int pResolution;
	float *pPixels;
	int pPixelCount;
	int pPixelsPerFace;
	float *pPixelsXPos;
	float *pPixelsXNeg;
	float *pPixelsYPos;
	float *pPixelsYNeg;
	float *pPixelsZPos;
	float *pPixelsZNeg;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panning collector. */
	deoalPanningCollector( int resolution );
	
	/** \brief Create copy of panning collector. */
	deoalPanningCollector( const deoalPanningCollector &collector );
	
	/** \brief Clean up panning collector. */
	~deoalPanningCollector();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Resolution. */
	inline int GetResolution() const{ return pResolution; }
	
	
	
	/** \brief Clear collector. */
	void Clear();
	
	/** \brief Add weighted direction. */
	void Add( const decVector &direction, float weight );
	
	/** \brief Calculate panning vector. */
	decVector CalcPanningVector() const;
	
	
	
	/** \brief Debug print. */
	void DebugPrint( deoalAudioThread &audioThread ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set from other panning collector. */
	deoalPanningCollector &operator=( const deoalPanningCollector &collector );
	
	/** \brief Add from other panning collector. */
	deoalPanningCollector &operator+=( const deoalPanningCollector &collector );
	/*@}*/
};

#endif
