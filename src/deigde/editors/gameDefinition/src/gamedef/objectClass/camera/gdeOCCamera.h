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

#ifndef _GDEOCCAMERA_H_
#define _GDEOCCAMERA_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Object class camera.
 */
class gdeOCCamera : public deObject{
private:
	float pFov, pFovRatio, pImageDistance, pViewDistance;
	decVector pPosition, pRotation;
	decString pPropName, pPropPosition, pPropRotation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create camera. */
	gdeOCCamera();
	
	/** \brief Create copy of camera. */
	gdeOCCamera( const gdeOCCamera &camera );
	
	/** \brief Clean up camera. */
	virtual ~gdeOCCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set orientation. */
	void SetRotation( const decVector &orientation );
	
	/** \brief Vertical field  of view in radians for the entire view. */
	inline float GetFov() const{ return pFov; }
	
	/** \brief Set vertical field of view in radians of the entire view. */
	void SetFov( float fov );
	
	/** \brief Aspect ratio of the horizontal field of view to the vertical field of view. */
	inline float GetFovRatio() const{ return pFovRatio; }
	
	/** \brief Set aspect ratio of the horizonral field of view to the vertical field of view. */
	void SetFovRatio( float ratio );
	
	/** \brief Distance to the image plane. */
	inline float GetImageDistance() const{ return pImageDistance; }
	
	/** \brief Set distance to the image plane. */
	void SetImageDistance( float distance );
	
	/** \brief Viewing distance up to which scene geometry is rendered. */
	inline float GetViewDistance() const{ return pViewDistance; }
	
	/** \brief Set view distance up to which scene geometry is rendered. */
	void SetViewDistance( float distance );
	
	
	
	/** \brief Name of the name property. */
	inline const decString &GetPropName() const{ return pPropName; }
	
	/** \brief Set name of the name property. */
	void SetPropName(const char *property);
	
	/** \brief Name of the position property. */
	inline const decString &GetPropPosition() const{ return pPropPosition; }
	
	/** \brief Set name of the position property. */
	void SetPropPosition(const char *property);
	
	/** \brief Name of the rotation property. */
	inline const decString &GetPropRotation() const{ return pPropRotation; }
	
	/** \brief Set name of the rotation property. */
	void SetPropRotation(const char *property);
	/*@}*/
};

#endif
