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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	mePropFieldType( deEngine *engine );
	/** Cleans up the object. */
	virtual ~mePropFieldType();
	/*@}*/
	
	/** \name Management */
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
