/* 
 * Drag[en]gine Occlusion Mesh Module
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

#ifndef _DEOCCMWEIGHTSET_H_
#define _DEOCCMWEIGHTSET_H_


/**
 * @brief Weight Set.
 */
class deoccmWeightSet{
private:
	struct sWeight{
		int bone;
		float weight;
	};
	
private:
	sWeight *pWeights;
	int pCount;
	int pGroupedIndex;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new weight set. */
	deoccmWeightSet();
	/** Cleans up the weight set. */
	~deoccmWeightSet();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of of weights. */
	inline int GetCount() const{ return pCount; }
	/** Retrieves the bone index for the given weight. */
	int GetBoneAt( int index ) const;
	/** Retrieves the factor for the given weight. */
	float GetWeightAt( int index ) const;
	/** Retrieves the factor for the given bone index or 0 if not found. */
	float GetWeightFor( int bone ) const;
	/**
	 * Sets the factor for a given bone. If the bone index
	 * does not exist it is added otherwise the factor changed.
	 */
	void Set( int bone, float weight );
	/** Retrieves the grouped index. */
	inline int GetGroupedIndex() const{ return pGroupedIndex; }
	/** Sets the grouped index. */
	void SetGroupedIndex( int index );
	/** Removes all weights. */
	void RemoveAll();
	/** Normalizes the weights alike that all weight factors summed up equal 1.0 */
	void Normalize();
	/** Determines if this weight set is equal to another one. */
	bool Equals( const deoccmWeightSet &weight ) const;
	/*@}*/
};

#endif
