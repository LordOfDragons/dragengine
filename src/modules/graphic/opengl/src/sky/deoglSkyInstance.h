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

#ifndef _DEOGLSKYINSTANCE_H_
#define _DEOGLSKYINSTANCE_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicSkyInstance.h>

class deoglRSkyInstance;
class deoglSky;

class deGraphicOpenGl;
class deSkyInstance;
class deoglWorld;



/**
 * Sky peer.
 */
class deoglSkyInstance : public deBaseGraphicSkyInstance{
private:
	deGraphicOpenGl &pOgl;
	const deSkyInstance &pInstance;
	
	deoglRSkyInstance *pRInstance;
	deoglWorld *pParentWorld;
	
	deoglSky *pOglSky;
	float pEnvMapTimer;
	
	bool pDirtySky;
	bool pDirtyControllers;
	bool pDirtyLayerMask;
	unsigned int pSkyUpdateState;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new sky. */
	deoglSkyInstance( deGraphicOpenGl &ogl, const deSkyInstance &instance );
	
	/** Cleans up the sky. */
	virtual ~deoglSkyInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Sky instance. */
	inline const deSkyInstance &GetInstance() const{ return pInstance; }
	
	
	
	/** Render instance. */
	inline deoglRSkyInstance *GetRInstance() const{ return pRInstance; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	/** Update sky instance. */
	void Update( float elapsed );
	
	
	
	/** Parent world or \em NULL if not in a world. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	
	/**
	 * Set parent world or \em NULL if not in a world.
	 * \details For use by deoglWorld only.
	 */
	void SetParentWorld( deoglWorld *world );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Sky changed. */
	virtual void SkyChanged();
	
	/** order changed. */
	virtual void OrderChanged();
	
	/** Controller changed. */
	virtual void ControllerChanged( int index );
	
	/** Layer mask changed. */
	virtual void LayerMaskChanged();
	/*@}*/
};

#endif
