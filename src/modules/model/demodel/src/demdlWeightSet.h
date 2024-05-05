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

#ifndef _DEMDLWEIGHTSET_H_
#define _DEMDLWEIGHTSET_H_


/**
 * @brief Weight Set.
 */
class demdlWeightSet{
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
	demdlWeightSet();
	/** Cleans up the weight set. */
	~demdlWeightSet();
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
	bool Equals( const demdlWeightSet &weight ) const;
	/*@}*/
};

#endif
