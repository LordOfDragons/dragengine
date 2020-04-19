/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEMODELBUILDERNAVSPACE_H_
#define _MEMODELBUILDERNAVSPACE_H_

#include <dragengine/resources/model/deModelBuilder.h>

class deNavigationSpace;



/**
 * @brief Navigation Space Model Builder.
 * Model builder creating a model from the faces defined in a navigation space.
 */
class meModelBuilderNavSpace : public deModelBuilder{
private:
	deNavigationSpace *pNavSpace;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a builder using the given navigation space. */
	meModelBuilderNavSpace( deNavigationSpace *navspace );
	/** Cleans up the builder. */
	virtual ~meModelBuilderNavSpace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Build model into the provided model object. */
	virtual void BuildModel( deModel *model );
	
	/** Build nav grid model. */
	void BuildNavGrid( deModel *model );
	/** Build nav mesh model. */
	void BuildNavMesh( deModel *model );
	/*@}*/
};

#endif
