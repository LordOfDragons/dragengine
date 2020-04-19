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
#ifndef _MEHTVVARIATION_H_
#define _MEHTVVARIATION_H_

// includes
#include <dragengine/common/string/decString.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

// predefinitions
class meHTVegetationLayer;

class deEngine;
class deSkin;
class deModel;



/**
 * @brief Height Terrain Vegetation Variation.
 *
 * Defines a variation in a layer of vegetation on a height terrain. A
 * vegetation layer variation composes of a model, skin and optional a
 * rig defining the plant to render.
 */
class meHTVVariation : public deObject{
private:
	meHTVegetationLayer *pVLayer;
	
	deEngine *pEngine;
	
	decString pPathModel;
	deModel *pModel;
	decString pPathSkin;
	deSkin *pSkin;
	float pRotationPerForce;
	float pRestitution;
	
	float pRandomScaleMin;
	float pRandomScaleMax;
	float pRandomRotationMin;
	float pRandomRotationMax;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	meHTVVariation( deEngine *engine );
	/** Cleans up the object. */
	virtual ~meHTVVariation();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent height terrain vegetation layer or NULL. */
	inline meHTVegetationLayer *GetVLayer() const{ return pVLayer; }
	/** Sets the parent height terrain vegetation layer or NULL. */
	void SetVLayer( meHTVegetationLayer *vlayer );
	
	/** Retrieves the model path. */
	inline const decString &GetPathModel() const{ return pPathModel; }
	/** Sets the model path. */
	void SetPathModel( const char *path );
	/** Retrieves the engine model or NULL if not valid. */
	inline deModel *GetModel() const{ return pModel; }
	/** Retrieves the skin path. */
	inline const decString &GetPathSkin() const{ return pPathSkin; }
	/** Sets the skin path. */
	void SetPathSkin( const char *path );
	/** Retrieves the engine skin or NULL if not valid. */
	inline deSkin *GetSkin() const{ return pSkin; }
	/** Retrieves the rotation per force. */
	inline float GetRotationPerForce() const{ return pRotationPerForce; }
	/** Sets the rotation per force. */
	void SetRotationPerForce( float rotationPerForce );
	/** Retrieves the restitution. */
	inline float GetRestitution() const{ return pRestitution; }
	/** Sets the restitution. */
	void SetRestitution( float resitution );
	
	/** Retrieves the minimum random scaling. */
	inline float GetMinimumRandomScaling() const{ return pRandomScaleMin; }
	/** Sets the minimum random scaling. */
	void SetMinimumRandomScaling( float scaling );
	/** Retrieves the maximum random scaling. */
	inline float GetMaximumRandomScaling() const{ return pRandomScaleMax; }
	/** Sets the maximum random scaling. */
	void SetMaximumRandomScaling( float scaling );
	/** Retrieves the minimum random rotation. */
	inline float GetMinimumRandomRotation() const{ return pRandomRotationMin; }
	/** Sets the minimum random rotation. */
	void SetMinimumRandomRotation( float rotation );
	/** Retrieves the maximum random rotation. */
	inline float GetMaximumRandomRotation() const{ return pRandomRotationMax; }
	/** Sets the maximum random rotation. */
	void SetMaximumRandomRotation( float rotation );
	
	/** Notifies that this variation changed. */
	void NotifyChanged();
	/*@}*/
};

// end of include only once
#endif
