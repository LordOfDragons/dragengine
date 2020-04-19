/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEUNODEDATA_H_
#define _SEUNODEDATA_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class sePropertyNode;



/**
 * \brief Undo action property node data.
 */
class seUPropertyNodeData : public deObject{
private:
	sePropertyNode *pNode;
	decPoint3 pPosition;
	decPoint3 pSize;
	float pRotation;
	float pShearing;
	decTexMatrix2 pMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	seUPropertyNodeData( sePropertyNode *node );
	
protected:
	/** \brief Clean up undo. */
	virtual ~seUPropertyNodeData();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Node. */
	inline sePropertyNode *GetNode() const{ return pNode; }
	
	/** \brief Reference position. */
	inline const decPoint3 &GetPosition() const{ return pPosition; }
	
	/** \brief Reference size. */
	inline const decPoint3 &GetSize() const{ return pSize; }
	
	/** \brief Refrence rotation. */
	inline float GetRotation() const{ return pRotation; }
	
	/** \brief Reference shearing. */
	inline float GetShearing() const{ return pShearing; }
	
	/** \brief Matrix. */
	inline const decTexMatrix2 &GetMatrix() const{ return pMatrix; }
	
	/** \brief Create inverse matrix for the parent or mask parent. */
	decTexMatrix2 CreateInverseParentMatrix() const;
	/*@}*/
};

#endif
