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
	deoalSoundRay( const deoalSoundRay &ray );
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Index of first segment. */
	inline int GetFirstSegment() const{ return pFirstSegment; }
	
	/** \brief Set index of first segment. */
	void SetFirstSegment( int index );
	
	/** \brief Number of segments. */
	inline int GetSegmentCount() const{ return pSegmentCount; }
	
	/** \brief Set number of segments. */
	void SetSegmentCount( int count );
		
	/** \brief Increment number of segments by one. */
	void IncSegmentCount();

	/** \brief Index of first transmitted ray. */
	inline int GetFirstTransmittedRay() const{ return pFirstTransmittedRay; }
	
	/** \brief Set index of first transmitted ray. */
	void SetFirstTransmittedRay( int index );
	
	/** \brief Number of transmitted rays. */
	inline int GetTransmittedRayCount() const{ return pTransmittedRayCount; }
	
	/** \brief Set number of transmitted rays. */
	void SetTransmittedRayCount( int count );
	
	/** \brief Increment number of transmitted rays by one. */
	void IncTransmittedRayCount();
	
	/** \brief Ray is outside. */
	inline bool GetOutside() const{ return pOutside; }
	
	/** \brief Set if ray is outside. */
	void SetOutside( bool outside );
	
	
	
	/** \brief Set from other ray. */
	deoalSoundRay &operator=( const deoalSoundRay &ray );
	/*@}*/
};

#endif
