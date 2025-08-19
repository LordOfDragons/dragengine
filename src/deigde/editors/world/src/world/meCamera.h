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

#ifndef _MECAMERA_H_
#define _MECAMERA_H_

#include <deigde/gui/igdeCamera.h>

class meObject;
class meWorld;
class deDebugDrawerShape;


/**
 * Camera for rendering a world.
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
	decDMatrix pHostMatrix, pInvHostMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new camera. */
	meCamera( deEngine *engine );
	/** Cleans up the camera object. */
	~meCamera() override;
	/*@}*/
	
	/** \name Management */
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
	
	/** Matrix transforming from host object to camera. */
	inline const decDMatrix &GetHostMatrix() const{ return pHostMatrix; }
	
	/** Matrix transforming from camera to host object. */
	inline const decDMatrix &GetInvHostMatrix() const{ return pInvHostMatrix; }
	
	/** Set matrix transforming from host object to camera (and inverse). */
	void SetHostMatrix(const decDMatrix &matrix);
	
	/** Resets the camera. */
	void Reset() override;
	
	/** Geometry changed. */
	void GeometryChanged() override;
	/** Adaption changed. */
	void AdaptionChanged() override;
	/*@}*/
	
private:
	void pUpdateDDVGeometry();
	void pUpdateDDVolume();
};

#endif
