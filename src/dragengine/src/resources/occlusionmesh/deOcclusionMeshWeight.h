/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEOCCLUSIONMESHWEIGHT_H_
#define _DEOCCLUSIONMESHWEIGHT_H_


/**
 * \brief Occlusion Mesh Weight.
 * A single weight in a weight set. The bone index has to be 0 or larger, the weight in the range from 0 to 1.
 */
class deOcclusionMeshWeight{
private:
	int pBone;
	float pWeight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new weight. */
	deOcclusionMeshWeight();
	
	/** \brief Clean up weight. */
	~deOcclusionMeshWeight();
	/*@}*/
	
	
	
	/** \name Weights Management */
	/*@{*/
	/** \brief Index of the bone. */
	inline int GetBone() const{ return pBone; }
	
	/** \brief Set index of the bone. */
	void SetBone( int bone );
	
	/** \brief Weight in the range from 0 to 1. */
	inline float GetWeight() const{ return pWeight; }
	
	/** \brief Set weight in the range from 0 to 1. */
	void SetWeight( float weight );
	/*@}*/
};

#endif
