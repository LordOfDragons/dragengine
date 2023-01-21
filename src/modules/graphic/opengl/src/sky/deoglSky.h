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

#ifndef _DEOGLSKY_H_
#define _DEOGLSKY_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicSky.h>

class deoglRSky;
class deoglSkyLayer;
class deGraphicOpenGl;

class deSky;



/**
 * Sky peer.
 */
class deoglSky : public deBaseGraphicSky{
private:
	deGraphicOpenGl &pOgl;
	const deSky &pSky;
	
	deoglRSky *pRSky;
	
	bool pDirtyParameters;
	unsigned int pUpdateTracker;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create sky. */
	deoglSky( deGraphicOpenGl &ogl, const deSky &sky );
	
	/** Clean up sky. */
	virtual ~deoglSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Sky. */
	inline const deSky &GetSky() const{ return pSky; }
	
	/** Render sky. */
	inline deoglRSky *GetRSky() const{ return pRSky; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	/** Update tracker state. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Parameters changed. */
	virtual void ParametersChanged();
	/*@}*/
};

#endif
