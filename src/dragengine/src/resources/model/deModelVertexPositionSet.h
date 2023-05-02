/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEMODELVERTEXPOSITIONSET_H_
#define _DEMODELVERTEXPOSITIONSET_H_

#include "../../common/string/decString.h"


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
class DE_DLL_EXPORT deModelVertexPositionSet{
private:
	decString pName;
	int pBaseSet;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model vertex position set. */
	deModelVertexPositionSet( const char *name );
	
	/** \brief Clean up model vertex position set. */
	~deModelVertexPositionSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Base set index or -1. */
	inline int GetBaseSet() const{ return pBaseSet; }
	
	/** \brief Set base set index or -1. */
	void SetBaseSet( int baseSet );
	/*@}*/
};

#endif
