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

#ifndef _DEMICROPHONEMANAGER_H_
#define _DEMICROPHONEMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deMicrophone;


/**
 * \brief Scene Microphone Resource Manager.
 *
 */
class deMicrophoneManager : public deResourceManager{
private:
	deResourceList pMicrophones;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new microphone resource manager linked to the given engine. */
	deMicrophoneManager( deEngine *engine );
	
	/** \brief Clean up microphone resource manager and reports leaking resources. */
	~deMicrophoneManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of microphones. */
	int GetMicrophoneCount() const;
	
	/** \brief Root microphone resource for iteration purpose. */
	deMicrophone *GetRootMicrophone() const;
	
	/** \brief Create new microphone. */
	deMicrophone *CreateMicrophone();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemAudioLoad();
	void SystemAudioUnload();
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
