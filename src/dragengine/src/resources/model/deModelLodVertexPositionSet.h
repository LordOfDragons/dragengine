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

#ifndef _DEMODELLODVERTEXPOSITIONSET_H_
#define _DEMODELLODVERTEXPOSITIONSET_H_

#include "../../common/math/decMath.h"

class deModelLodVertexPositionSetPosition;


/**
 * \brief Model vertex position set.
 * 
 * Defines an additional named set of positions for all vertices. This is often called a
 * blend shape typically used for facial animation or correcting weighting issues.
 * Position count has to match the vertex count of the model lod this set belongs to.
 * 
 * Vertex position sets are relative to either the original model vertex position or another
 * vertex position set. Vertex position sets are blended like this (for each vertex):
 * 
 *   position = modelLod.vertex.position
 *   
 *   for set in sets
 *     if set.baseSet == -1
 *       position += (set.position[i] - modelLod.vertex.position) * set.weight
 *     else
 *       position += (set.position[i] - sets[set.baseSet].position[i]) * set.weight
 * 
 * Using a base vertex position instead of the original model vertex position allows to
 * create complex vertex position manipulations difficult to do otherwise.
 * 
 * Vertex position sets store only positions for some vertices. All vertices not defined
 * in a vertex position set use the original model vertex position.
 */
class DE_DLL_EXPORT deModelLodVertexPositionSet{
private:
	deModelLodVertexPositionSetPosition *pPositions;
	int pPositionCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model vertex position set. */
	deModelLodVertexPositionSet();
	
	/** \brief Clean up model vertex position set. */
	~deModelLodVertexPositionSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of vertex positions. */
	inline int GetPositionCount() const{ return pPositionCount; }
	
	/** \brief Set count of vertex positions. */
	void SetPositionCount( int count );
	
	/** \brief Vertex position at index. */
	deModelLodVertexPositionSetPosition &GetPositionAt( int index );
	const deModelLodVertexPositionSetPosition &GetPositionAt( int index ) const;
	
	/** \brief Pointer to vertex positions. */
	inline deModelLodVertexPositionSetPosition *GetPositions() const{ return pPositions; }
	/*@}*/
};

#endif
