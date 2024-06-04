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

#ifndef _DEMDLWEIGHTSETLIST_H_
#define _DEMDLWEIGHTSETLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class demdlWeightSet;



/**
 * @brief Weight Set List.
 * List of weight sets. Supports sorting to output the weight sets in the way the model resource expects them.
 */
class demdlWeightSetList{
private:
	decPointerList pList;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new weight set list. */
	demdlWeightSetList();
	/** Cleans up the list. */
	~demdlWeightSetList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the count of weight sets. */
	int GetCount() const;
	/** Retrieves the weight set at the given index. */
	demdlWeightSet *GetAt( int index ) const;
	/** Adds a weight set. */
	void Add( demdlWeightSet *weightSet );
	/** Remove all weight sets. */
	void RemoveAll();
	/** Retrieves the index of the weight set which is equal to the given weight set or -1 if not found. */
	int IndexOfEqual( const demdlWeightSet &weightSet ) const;
	/** Retrieves the largest number of weights in all of the weight sets. */
	int GetLargestWeightCount() const;
	/*@}*/
};

#endif
