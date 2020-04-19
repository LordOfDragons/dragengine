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

#ifndef _DESYNTHESIZERINSTANCEMANAGER_H_
#define _DESYNTHESIZERINSTANCEMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deSynthesizerInstance;


/**
 * \brief Synthesizer Instance Resource Manager.
 */
class deSynthesizerInstanceManager : public deResourceManager{
private:
	deResourceList pInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer instance resource manager. */
	deSynthesizerInstanceManager( deEngine *engine );
	
	/** \brief Clean up synthesizer instance resource manager and report leaking resources. */
	virtual ~deSynthesizerInstanceManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of synthesizer instance resource. */
	int GetSynthesizerInstanceCount() const;
	
	/** \brief Root synthesizer instance resource for iteration purpose. */
	deSynthesizerInstance *GetRootSynthesizerInstance() const;
	
	/** \brief Create synthesizer instance. */
	deSynthesizerInstance *CreateSynthesizerInstance();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Synthesizer system peers of all stored resources have to be created. */
	virtual void SystemSynthesizerLoad();
	
	/** \brief Synthesizer system peers of all stored resources have to be freed. */
	virtual void SystemSynthesizerUnload();
	
	/** \brief Audio system peers of all stored resources have to be created. */
	virtual void SystemAudioLoad();
	
	/** \brief Audio system peers of all stored resources have to be freed. */
	virtual void SystemAudioUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * \brief Those functions are only for resource objects and should never be called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
