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
	deoalPanningCollector(int resolution);
	
	/** \brief Create copy of panning collector. */
	deoalPanningCollector(const deoalPanningCollector &collector);
	
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
	void Add(const decVector &direction, float weight);
	
	/** \brief Calculate panning vector. */
	decVector CalcPanningVector() const;
	
	
	
	/** \brief Debug print. */
	void DebugPrint(deoalAudioThread &audioThread) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set from other panning collector. */
	deoalPanningCollector &operator=(const deoalPanningCollector &collector);
	
	/** \brief Add from other panning collector. */
	deoalPanningCollector &operator+=(const deoalPanningCollector &collector);
	/*@}*/
};

#endif
