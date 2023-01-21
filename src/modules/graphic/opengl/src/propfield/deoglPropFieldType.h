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
 * Prop Field Type.
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
	/** Create prop field type. */
	deoglPropFieldType( deoglPropField &propField, const dePropFieldType &type );
	
	/** Clean up prop field type. */
	~deoglPropFieldType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Prop field. */
	inline deoglPropField &GetPropField() const{ return pPropField; }
	
	/** Prop field type. */
	inline const dePropFieldType &GetType() const{ return pType; }
	
	/** Render prop field type. */
	inline deoglRPropFieldType *GetRType() const{ return pRType; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Type changed. */
	void TypeChanged();
	
	/** Instances changed. */
	void InstancesChanged();
	
	/** Instances to Bend States assignments changed. */
	void AssignmentsChanged();
	
	/** Bend States changed. */
	void BendStatesChanged();
	
	/** Instance count changed. */
	void InstanceCountChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
