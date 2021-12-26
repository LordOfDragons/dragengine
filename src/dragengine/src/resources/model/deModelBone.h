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

#ifndef _DEMODELBONE_H_
#define _DEMODELBONE_H_

#include "../../common/string/decString.h"
#include "../../common/math/decMath.h"


/**
 * \brief Model Bone.
 */
class DE_DLL_EXPORT deModelBone{
private:
	decString pName;
	int pParent;
	decVector pPosition;
	decQuaternion pOrientation;
	decMatrix pMatrix;
	decMatrix pInverseMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model bone with the given name. */
	deModelBone( const char *name );
	
	/** \brief Clean up model bone. */
	~deModelBone();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Index of the parent bone or -1 if a top level bone. */
	inline int GetParent() const{ return pParent; }
	
	/** \brief Set index of the parent bone or -1 if a top level bone. */
	void SetParent( int bone );
	
	/** \brief Position relative to the parent or the model if there is no parent. */
	inline decVector GetPosition() const{ return pPosition; }
	
	/** \brief Set position relative to the parent or the model if there is no parent. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation relative to the parent or the model if there is no parent. */
	inline decQuaternion GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation relative to the parent or the model if there is no parent. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Matrix transforming from local bone space to model space in reference pose. */
	inline decMatrix GetMatrix() const{ return pMatrix; }
	
	/** \brief Matrix transforming from model space to local bone space in reference pose. */
	inline decMatrix GetInverseMatrix() const{ return pInverseMatrix; }
	
	/** \brief Set matrix transforming from local bone space to model space in reference pose. */
	void SetMatrix( const decMatrix &matrix );
	/*@}*/
};

#endif
