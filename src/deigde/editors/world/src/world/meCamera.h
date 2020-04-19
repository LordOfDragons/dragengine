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
#ifndef _MECAMERA_H_
#define _MECAMERA_H_

// includes
#include <deigde/gui/igdeCamera.h>

// predefinitions
class meObject;
class meWorld;
class deDebugDrawerShape;



/**
 * @brief Camera for rendering a world.
 *
 * Manages a camera including the wrapped engine camera. Provides also
 * some simple unproject methods to determine a ray shot into the world
 * as seen by this camera. The camera is managed in a lazy way. Hence
 * the engine is not create or updated until it is acquired by the user.
 */
class meCamera : public igdeCamera{
private:
	deDebugDrawerShape *pDDVolume;
	meWorld *pWorld;
	
	meObject *pHostObject;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new camera. */
	meCamera( deEngine *engine );
	/** Cleans up the camera object. */
	virtual ~meCamera();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Dispose of the camera. */
	void Dispose();
	
	/** Retrieves the parent world. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** Sets the parent world. */
	void SetWorld( meWorld *world );
	
	/** Retrieves the host object or NULL if not attached to any object. */
	inline meObject *GetHostObject() const{ return pHostObject; }
	/** Sets the host object or NULL if not attached to any object. */
	void SetHostObject( meObject *object );
	/** Determines if this camera has a host object. */
	bool HasHostObject() const;
	
	/** Resets the camera. */
	virtual void Reset();
	
	/** Geometry changed. */
	virtual void GeometryChanged();
	/** Adaption changed. */
	virtual void AdaptionChanged();
	/*@}*/
	
private:
	void pUpdateDDVGeometry();
	void pUpdateDDVolume();
};

// end of include only once
#endif
