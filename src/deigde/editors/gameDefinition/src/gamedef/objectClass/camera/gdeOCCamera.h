/* 
 * Drag[en]gine IGDE Game Definition Editor
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
	float pFov;
	float pFovRatio;
	float pImageDistance;
	float pViewDistance;
	decVector pPosition;
	decVector pRotation;
	
	decString pPropName;
	
	
	
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
	void SetPropName( const char *propName );
	/*@}*/
};

#endif
