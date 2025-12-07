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

#ifndef _DEOALSOUNDRAY_H_
#define _DEOALSOUNDRAY_H_

#include <dragengine/common/math/decMath.h>



/**
 * \brief Parameters of a sound ray.
 */
class deoalSoundRay{
private:
	int pFirstSegment;
	int pSegmentCount;
	int pFirstTransmittedRay;
	int pTransmittedRayCount;
	bool pOutside;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound ray. */
	deoalSoundRay();
	
	/** \brief Create copy of sound ray. */
	deoalSoundRay(const deoalSoundRay &ray);
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Index of first segment. */
	inline int GetFirstSegment() const{ return pFirstSegment; }
	
	/** \brief Set index of first segment. */
	void SetFirstSegment(int index);
	
	/** \brief Number of segments. */
	inline int GetSegmentCount() const{ return pSegmentCount; }
	
	/** \brief Set number of segments. */
	void SetSegmentCount(int count);
		
	/** \brief Increment number of segments by one. */
	void IncSegmentCount();

	/** \brief Index of first transmitted ray. */
	inline int GetFirstTransmittedRay() const{ return pFirstTransmittedRay; }
	
	/** \brief Set index of first transmitted ray. */
	void SetFirstTransmittedRay(int index);
	
	/** \brief Number of transmitted rays. */
	inline int GetTransmittedRayCount() const{ return pTransmittedRayCount; }
	
	/** \brief Set number of transmitted rays. */
	void SetTransmittedRayCount(int count);
	
	/** \brief Increment number of transmitted rays by one. */
	void IncTransmittedRayCount();
	
	/** \brief Ray is outside. */
	inline bool GetOutside() const{ return pOutside; }
	
	/** \brief Set if ray is outside. */
	void SetOutside(bool outside);
	
	
	
	/** \brief Set from other ray. */
	deoalSoundRay &operator=(const deoalSoundRay &ray);
	/*@}*/
};

#endif
