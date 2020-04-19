/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLPROPFIELDTYPE_H_
#define _DEOGLPROPFIELDTYPE_H_

#include <dragengine/common/math/decMath.h>

class deoglPropField;
class deoglPropFieldCluster;
class deoglRPropFieldType;

class dePropFieldType;



/**
 * \brief Prop Field Type.
 * <date 2015
 */
class deoglPropFieldType{
private:
	deoglPropField &pPropField;
	const dePropFieldType &pType;
	
	deoglRPropFieldType *pRType;
	
	bool pDirtyType;
	bool pDirtyInstances;
	bool pDirtyBendStates;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create prop field type. */
	deoglPropFieldType( deoglPropField &propField, const dePropFieldType &type );
	
	/** \brief Clean up prop field type. */
	~deoglPropFieldType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Prop field. */
	inline deoglPropField &GetPropField() const{ return pPropField; }
	
	/** \brief Prop field type. */
	inline const dePropFieldType &GetType() const{ return pType; }
	
	/** \brief Render prop field type. */
	inline deoglRPropFieldType *GetRType() const{ return pRType; }
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** \brief Type changed. */
	void TypeChanged();
	
	/** \brief Instances changed. */
	void InstancesChanged();
	
	/** \brief Instances to Bend States assignments changed. */
	void AssignmentsChanged();
	
	/** \brief Bend States changed. */
	void BendStatesChanged();
	
	/** \brief Instance count changed. */
	void InstanceCountChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
