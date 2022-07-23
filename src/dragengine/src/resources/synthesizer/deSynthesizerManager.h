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

#ifndef _DESYNTHESIZERMANAGER_H_
#define _DESYNTHESIZERMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deSynthesizer;


/**
 * \brief Synthesizer resource manager.
 */
class DE_DLL_EXPORT deSynthesizerManager : public deResourceManager{
private:
	deResourceList pSynthesizers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer resource manager. */
	deSynthesizerManager( deEngine *engine );
	
	/** \brief Clean up synthesizer resource manager and report leaking resources. */
	~deSynthesizerManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of synthesizer resources. */
	int GetSynthesizerCount() const;
	
	/** \brief Root synthesizer resource for iteration purpose. */
	deSynthesizer *GetRootSynthesizer() const;
	
	/** \brief Create new synthesizer object. */
	deSynthesizer *CreateSynthesizer();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Synthesizer system peers of all stored resources have to be created. */
	virtual void SystemSynthesizerLoad();
	
	/** \brief Synthesizer system peers of all stored resources have to be freed. */
	virtual void SystemSynthesizerUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only functions.
	 * 
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
