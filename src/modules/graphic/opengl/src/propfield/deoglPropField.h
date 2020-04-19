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

#ifndef _DEOGLPROPFIELD_H_
#define _DEOGLPROPFIELD_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicPropField.h>

class deoglRPropField;
class deoglPropFieldType;

class deGraphicOpenGl;
class dePropField;
class dePropFieldType;



/**
 * \brief Prop field peer.
 */
class deoglPropField : public deBaseGraphicPropField{
private:
	deGraphicOpenGl &pOgl;
	dePropField &pPropField;
	
	deoglRPropField *pRPropField;
	
	decPointerList pTypes;
	
	int pLODLevel;
	int pBestLOD;
	bool pCheckLOD;
	
	bool pDirtyPosition;
	bool pDirtyExtends;
	bool pDirtyTypes;
	bool pDirtyInstances;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deoglPropField( deGraphicOpenGl &ogl, dePropField &propField );
	
	/** \brief Clean up peer. */
	virtual ~deoglPropField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Opengl object. */
	inline deGraphicOpenGl &GetOpenGL() const{ return pOgl; }
	
	/** \brief Prop field. */
	inline const dePropField &GetPropField() const{ return pPropField; }
	
	
	
	/** \brief Render prop field. */
	inline deoglRPropField *GetRPropField() const{ return pRPropField; }
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** \brief LOD level. */
	inline int GetLODLevel() const{ return pLODLevel; }
	
	/** \brief Set lod level not doing the updates yet. */
	void SetLODLevel( int level );
	
	/** \brief Test lod level against the given camera point. */
	void TestLODLevel( const decDVector &camera );
	
	/** \brief Update instance counts. */
	void UpdateInstanceCounts();
	
	
	
	/** \brief Update prop field if required. */
	void Update();
	
	
	
	/** \brief Number of types. */
	int GetTypeCount() const;
	
	/** \brief Type at index. */
	deoglPropFieldType &GetTypeAt( int index ) const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Ground changed. */
	virtual void GroundChanged();
	
	
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	
	
	/** \brief Type has been added. */
	virtual void TypeAdded( int index, dePropFieldType *type );
	
	/** \brief Type has been removed. */
	virtual void TypeRemoved( int index, dePropFieldType *type );
	
	/** \brief All types have been removed. */
	virtual void AllTypesRemoved();
	
	/** \brief Type changed. */
	virtual void TypeChanged( int index, dePropFieldType *type );
	
	
	
	/** \brief Instances changed. */
	virtual void InstancesChanged( int index, dePropFieldType *type );
	
	/** \brief Instances to Bend States assignments changed. */
	virtual void AssignmentsChanged( int index, dePropFieldType *type );
	
	/** \brief Bend States changed. */
	virtual void BendStatesChanged( int index, dePropFieldType *type );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
