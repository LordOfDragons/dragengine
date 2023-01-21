/* 
 * Drag[en]gine OpenGL Graphic Module
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
#ifndef _DEOGLSMOKEEMITTER_H_
#define _DEOGLSMOKEEMITTER_H_

// includes
#include "../deoglGL.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicSmokeEmitter.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class deoglWorld;
class deGraphicOpenGl;

class deSmokeEmitter;



/**
 * Smoke Emitter Peer.
 *
 * Peer for the smoke emitter resource.
 */
class deoglSmokeEmitter : public deBaseGraphicSmokeEmitter{
private:
	deGraphicOpenGl *pOgl;
	deoglWorld *pParentWorld;
	deSmokeEmitter *pSmokeEmitter;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	bool pDirtyExtends;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	deoglSmokeEmitter( deGraphicOpenGl *ogl, deSmokeEmitter *smokeEmitter );
	/** Cleans up the peer. */
	virtual ~deoglSmokeEmitter();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the opengl object. */
	inline deGraphicOpenGl *GetOpenGL() const{ return pOgl; }
	/** Retrieves the prop field. */
	inline deSmokeEmitter *GetSmokeEmitter() const{ return pSmokeEmitter; }
	
	/** Retrieves the parent world. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	/** Sets the parent world. */
	void SetParentWorld( deoglWorld *parentWorld );
	
	/** Retrieves the minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	/** Retrieves the maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** Updates the smoke emitter if required. */
	void Update( float elapsed );
	/** Prepares the smoke emitter for rendering. */
	void PrepareForRender();
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/** Volume position changed. */
	virtual void VolumePositionChanged();
	/** Skin changed. */
	virtual void SkinChanged();
	/** Points changed. */
	virtual void PointsChanged();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateExtends();
};

// end of include only once
#endif
