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
	deoalSoundRay &GetRayAt( int index );
	const deoalSoundRay &GetRayAt( int index ) const;
	
	/** \brief Add ray returning index. */
	int AddRay();
	
	/** \brief Number of segments. */
	inline int GetSegmentCount() const{ return pSegmentCount; }
	
	/** \brief Segment at index. */
	deoalSoundRaySegment &GetSegmentAt( int index );
	const deoalSoundRaySegment &GetSegmentAt( int index ) const;
	
	/** \brief Add segment returning index. */
	int AddSegment();
	
	/** \brief Number of transmitted rays. */
	inline int GetTransmittedRayCount() const{ return pTransmittedRayCount; }
	
	/** \brief Transmitted ray at index. */
	deoalSoundRay &GetTransmittedRayAt( int index );
	const deoalSoundRay &GetTransmittedRayAt( int index ) const;
	
	/** \brief Add transmitted ray returning index. */
	int AddTransmittedRay();
	
	/** \brief Remove all rays. */
	void RemoveAllRays();
	
	/** \brief Reserve space in the array to make future AddRay() and AddSegment() more efficient. */
	void ReserveSize( int rays, int segments, int transmittedRays );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Assign. */
	deoalSoundRayList &operator=( const deoalSoundRayList &list );
	
	/** \brief Append. */
	deoalSoundRayList &operator+=( const deoalSoundRayList &list );
	/*@}*/
};

#endif
