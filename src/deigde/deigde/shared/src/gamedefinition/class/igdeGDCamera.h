/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDCAMERA_H_
#define _IGDEGDCAMERA_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>



/**
 * \brief Game Definition Camera.
 *
 * Contains informations about a camera attached to an object.
 */
class igdeGDCamera{
private:
	float pFov;
	float pFovRatio;
	float pImageDistance;
	float pViewDistance;
	decVector pPosition;
	decQuaternion pOrientation;
	
	decString pPropName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition camera. */
	igdeGDCamera();
	
	/** \brief Create game definition camera as a copy of another game definition camera. */
	igdeGDCamera( const igdeGDCamera &camera );
	
	/** \brief Clean up game definition camera. */
	~igdeGDCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
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
