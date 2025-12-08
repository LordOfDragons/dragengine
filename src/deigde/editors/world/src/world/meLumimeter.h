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
	deLumimeter::Ref pLumimeter;
	deDebugDrawerVolume *pDDVolume;
	meWorld *pWorld;
	
	decString pName;
	decDVector pPosition;
	decVector pDirection;
	float pConeInnerAngle;
	float pConeOuterAngle;
	float pConeExponent;
	
	bool pTrackCamera;
	
	meObject::Ref pHostObject;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meLumimeter> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new lumimeter. */
	meLumimeter(deEngine *engine);
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
	void SetWorld(meWorld *world);
	
	/** Retrieves the name of the lumimeter. */
	inline const char *GetName() const{ return pName; }
	/** Sets the name of the lumimeter. */
	void SetName(const char *name);
	
	/** Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition(const decDVector &position);
	/** Retrieves the direction. */
	inline const decVector &GetDirection() const{ return pDirection; }
	/** Sets the direction. */
	void SetDirection(const decVector &direction);
	/** Retrieves the cone inner angle. */
	inline float GetConeInnerAngle() const{ return pConeInnerAngle; }
	/** Sets the cone inner angle. */
	void SetConeInnerAngle(float angle);
	/** Retrieves the cone outer angle. */
	inline float GetConeOuterAngle() const{ return pConeOuterAngle; }
	/** Sets the cone outer angle. */
	void SetConeOuterAngle(float angle);
	/** Retrieves the cone exponent. */
	inline float GetConeExponent() const{ return pConeExponent; }
	/** Sets the cone exponent. */
	void SetConeExponent(float exponent);
	
	/** Determines if the sensors tracks the active camera. */
	inline bool GetTrackCamera() const{ return pTrackCamera; }
	/** Sets if the sensor tracks the active camera. */
	void SetTrackCamera(bool trackCamera);
	
	/** Retrieves the host object or NULL if not attached to any object. */
	inline meObject *GetHostObject() const{ return pHostObject; }
	/** Sets the host object or NULL if not attached to any object. */
	void SetHostObject(meObject *object);
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
