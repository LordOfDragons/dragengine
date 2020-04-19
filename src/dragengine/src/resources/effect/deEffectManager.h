/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEEFFECTMANAGER_H_
#define _DEEFFECTMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deEffect;
class deEffectFilterKernel;
class deEffectOverlayImage;
class deEffectColorMatrix;
class deEffectDistortImage;


/**
 * \brief Effect Resource Manager.
 *
 */
class deEffectManager : public deResourceManager{
private:
	deResourceList pEffects;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new effect resource manager linked to the given engine. */
	deEffectManager( deEngine *engine );
	
	/** \brief Clean up effect resource manager and reports leaking resources. */
	virtual ~deEffectManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of scenes. */
	int GetEffectCount() const;
	
	/** \brief Root effect resource for iteration purpose. */
	deEffect *GetRootEffect() const;
	
	/** \brief Create new effect filter kernel. */
	deEffectFilterKernel *CreateEffectFilterKernel();
	
	/** \brief Create new effect overlay image. */
	deEffectOverlayImage *CreateEffectOverlayImage();
	
	/** \brief Create new color matrix effect. */
	deEffectColorMatrix *CreateEffectColorMatrix();
	
	/** \brief Create new distort image effect. */
	deEffectDistortImage *CreateEffectDistortImage();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad();
	void SystemGraphicUnload();
	//void SystemPhysicsLoad();
	//void SystemPhysicsUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
