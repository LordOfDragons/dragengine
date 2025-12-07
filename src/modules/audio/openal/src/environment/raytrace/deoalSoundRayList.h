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

#ifndef _DEOALSOUNDRAYLIST_H_
#define _DEOALSOUNDRAYLIST_H_

class deoalSoundRay;
class deoalSoundRaySegment;



/**
 * \brief Sound ray list.
 * 
 * List memory is not shrinking to avoid memory allocations.
 */
class deoalSoundRayList{
private:
	deoalSoundRay *pRays;
	int pRayCount;
	int pRaySize;
	
	deoalSoundRaySegment *pSegments;
	int pSegmentCount;
	int pSegmentSize;
	
	deoalSoundRay *pTransmittedRays;
	int pTransmittedRayCount;
	int pTransmittedRaySize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound ray interaction list. */
	deoalSoundRayList();
	
	/** \brief Clean up sound ray interaction list. */
	~deoalSoundRayList();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Number of rays. */
	inline int GetRayCount() const{ return pRayCount; }
	
	/** \brief Ray at index. */
	deoalSoundRay &GetRayAt(int index);
	const deoalSoundRay &GetRayAt(int index) const;
	
	/** \brief Add ray returning index. */
	int AddRay();
	
	/** \brief Number of segments. */
	inline int GetSegmentCount() const{ return pSegmentCount; }
	
	/** \brief Segment at index. */
	deoalSoundRaySegment &GetSegmentAt(int index);
	const deoalSoundRaySegment &GetSegmentAt(int index) const;
	
	/** \brief Add segment returning index. */
	int AddSegment();
	
	/** \brief Number of transmitted rays. */
	inline int GetTransmittedRayCount() const{ return pTransmittedRayCount; }
	
	/** \brief Transmitted ray at index. */
	deoalSoundRay &GetTransmittedRayAt(int index);
	const deoalSoundRay &GetTransmittedRayAt(int index) const;
	
	/** \brief Add transmitted ray returning index. */
	int AddTransmittedRay();
	
	/** \brief Remove all rays. */
	void RemoveAllRays();
	
	/** \brief Reserve space in the array to make future AddRay() and AddSegment() more efficient. */
	void ReserveSize(int rays, int segments, int transmittedRays);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Assign. */
	deoalSoundRayList &operator=(const deoalSoundRayList &list);
	
	/** \brief Append. */
	deoalSoundRayList &operator+=(const deoalSoundRayList &list);
	/*@}*/
};

#endif
