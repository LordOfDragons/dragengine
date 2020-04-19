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

// include only once
#ifndef _MEPROPFIELDTYPE_H_
#define _MEPROPFIELDTYPE_H_

// includes
#include <dragengine/common/string/decString.h>

#include <dragengine/deObject.h>

// predefinitions
class mePropField;

class deSkin;
class deModel;
class deEngine;
class dePropFieldType;
class dePropFieldInstance;



/**
 * @brief Prop Field Type.
 *
 * Defines a type in a prop field including a set of instances.
 */
class mePropFieldType : public deObject{
private:
	mePropField *pPropField;
	
	deEngine *pEngine;
	dePropFieldType *pEngPFType;
	
	decString pPathModel;
	deModel *pModel;
	decString pPathSkin;
	deSkin *pSkin;
	float pRotPerForceX;
	float pRotPerForceZ;
	
	dePropFieldInstance *pInstances;
	int pInstanceCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	mePropFieldType( deEngine *engine );
	/** Cleans up the object. */
	virtual ~mePropFieldType();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the prop field or NULL. */
	inline mePropField *GetPropField() const{ return pPropField; }
	/** Sets the prop field or NULL. */
	void SetPropField( mePropField *propField );
	
	/** Retrieves the engine prop field type or NULL. */
	inline dePropFieldType *GetEnginePFType() const{ return pEngPFType; }
	/** Sets the engine prop field type or NULL. */
	void SetEnginePFType( dePropFieldType *engPFType );
	
	/** Creates an engine prop field type. */
	dePropFieldType *CreateEnginePFType();
	/** Updates the engine prop field type. */
	void UpdateEnginePFType();
	
	/** Retrieves the model path. */
	inline const decString &GetPathModel() const{ return pPathModel; }
	/** Sets the model path. */
	void SetPathModel( const char *path );
	/** Retrieves the skin path. */
	inline const decString &GetPathSkin() const{ return pPathSkin; }
	/** Sets the skin path. */
	void SetPathSkin( const char *path );
	/** Retrieves the rotation per force around the X axis. */
	inline float GetRotationPerForceX() const{ return pRotPerForceX; }
	/** Sets the rotation per force aorund the X axis. */
	void SetRotationPerForceX( float rotPerForce );
	/** Retrieves the rotation per force around the Z axis. */
	inline float GetRotationPerForceZ() const{ return pRotPerForceZ; }
	/** Sets the rotation per force aorund the Z axis. */
	void SetRotationPerForceZ( float rotPerForce );
	
	/** Rebuild instances. */
	void RebuildInstances();
	
	/** Notifies the engine object that the type changed. */
	void NotifyTypeChanged();
	/*@}*/
};

// end of include only once
#endif
