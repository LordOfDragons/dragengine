/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _RERIGSHAPEHULL_H_
#define _RERIGSHAPEHULL_H_

#include "reRigShape.h"


/**
 * \brief Rig Hull Shape.
 * 
 * Working object for rig box shape.
 */
class reRigShapeHull : public reRigShape{
private:
	decVector *pPoints;
	int pPointCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rig hull shape. */
	reRigShapeHull( deEngine *engine );
	
protected:
	/** \brief Clean up rig shape shape. */
	virtual ~reRigShapeHull();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Point arrays. */
	inline decVector *GetPoints() const{ return pPoints; }
	
	/** \brief Number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/** \brief Point at index. */
	const decVector &GetPointAt( int index ) const;
	
	/** \brief Add point. */
	void AddPoint( const decVector &point );
	
	/** \brief Add point. */
	void InsertPoint( const decVector &point, int index );
	
	/** \brief Set point at index. */
	void SetPointAt( int index, const decVector &point );
	
	/** \brief Remove point. */
	void RemovePoint( int index );
	
	
	
	/** \brief Create copy of shape. */
	virtual reRigShape *Duplicate() const;
	
	/** \brief Uniformly scale shape. */
	virtual void Scale( float scale );
	
	/** \brief Create shape. */
	virtual decShape *CreateShape();
	/*@}*/
};

#endif
