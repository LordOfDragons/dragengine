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
#ifndef _MELUMIMETER_H_
#define _MELUMIMETER_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>

// predefinitions
class meObject;
class meWorld;
class deLumimeter;
class deEngine;
class deDebugDrawerVolume;



/**
 * @brief Lumimeter for rendering a world.
 * Manages a lumimeter including the wrapped engine lumimeter. Provides also
 * some simple unproject methods to determine a ray shot into the world
 * as seen by this lumimeter. The lumimeter is managed in a lazy way. Hence
 * the engine is not create or updated until it is acquired by the user.
 */
class meLumimeter : public deObject{
private:
	deEngine *pEngine;
	deLumimeter *pLumimeter;
	deDebugDrawerVolume *pDDVolume;
	meWorld *pWorld;
	
	decString pName;
	decDVector pPosition;
	decVector pDirection;
	float pConeInnerAngle;
	float pConeOuterAngle;
	float pConeExponent;
	
	bool pTrackCamera;
	
	meObject *pHostObject;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new lumimeter. */
	meLumimeter( deEngine *engine );
	/** Cleans up the lumimeter object. */
	~meLumimeter();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	/** Retrieves the engine lumimeter. */
	inline deLumimeter *GetLumimeter() const{ return pLumimeter; }
	
	/** Retrieves the parent world. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** Sets the parent world. */
	void SetWorld( meWorld *world );
	
	/** Retrieves the name of the lumimeter. */
	inline const char *GetName() const{ return pName; }
	/** Sets the name of the lumimeter. */
	void SetName( const char *name );
	
	/** Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decDVector &position );
	/** Retrieves the direction. */
	inline const decVector &GetDirection() const{ return pDirection; }
	/** Sets the direction. */
	void SetDirection( const decVector &direction );
	/** Retrieves the cone inner angle. */
	inline float GetConeInnerAngle() const{ return pConeInnerAngle; }
	/** Sets the cone inner angle. */
	void SetConeInnerAngle( float angle );
	/** Retrieves the cone outer angle. */
	inline float GetConeOuterAngle() const{ return pConeOuterAngle; }
	/** Sets the cone outer angle. */
	void SetConeOuterAngle( float angle );
	/** Retrieves the cone exponent. */
	inline float GetConeExponent() const{ return pConeExponent; }
	/** Sets the cone exponent. */
	void SetConeExponent( float exponent );
	
	/** Determines if the sensors tracks the active camera. */
	inline bool GetTrackCamera() const{ return pTrackCamera; }
	/** Sets if the sensor tracks the active camera. */
	void SetTrackCamera( bool trackCamera );
	
	/** Retrieves the host object or NULL if not attached to any object. */
	inline meObject *GetHostObject() const{ return pHostObject; }
	/** Sets the host object or NULL if not attached to any object. */
	void SetHostObject( meObject *object );
	/** Determines if this lumimeter has a host object. */
	bool HasHostObject() const;
	/*@}*/
	
	/** \name Measuring */
	/*@{*/
	/** Measures the luminance. */
	float MeasureLuminance();
	/** Measures the color. */
	decColor MeasureColor();
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateLumimeterPosition();
	void pUpdateDDVGeometry();
	void pUpdateDDVolume();
};

// end of include only once
#endif
