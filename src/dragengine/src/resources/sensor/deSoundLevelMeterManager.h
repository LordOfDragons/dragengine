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

#ifndef _DESOUNDLEVELMETERMANAGER_H_
#define _DESOUNDLEVELMETERMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deSoundLevelMeter;


/**
 * \brief Sound level meter resourse manager.
 */
class deSoundLevelMeterManager : public deResourceManager{
private:
	deResourceList pSoundLevelMeters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound level meter resource manager. */
	deSoundLevelMeterManager( deEngine *engine );
	
	/** \brief Clean up sound level meter resource manager and report leaking resources. */
	virtual ~deSoundLevelMeterManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of sound level meters. */
	int GetSoundLevelMeterCount() const;
	
	/** \brief Root sound level meter resource for iteration purpose. */
	deSoundLevelMeter *GetRootSoundLevelMeter() const;
	
	/** \brief Create sound level meter. */
	deSoundLevelMeter *CreateSoundLevelMeter();
	
	/** \brief Report and release leaking resources. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Audio System Peers of all stored resources have to be created. */
	virtual void SystemAudioLoad();
	
	/** \brief Audio System Peers of all stored resources have to be freed. */
	virtual void SystemAudioUnload();
	
	/** \brief Scripting System Peers of all stored resources have to be created. */
	virtual void SystemScriptingLoad();
	
	/** \brief Scripting System Peers of all stored resources have to be freed. */
	virtual void SystemScriptingUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * \warning Only for use by deSoundLevelMeter. Never be call directly.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
