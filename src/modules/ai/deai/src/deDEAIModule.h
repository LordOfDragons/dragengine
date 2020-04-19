/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDEAI_H_
#define _DEDEAI_H_

#include "dragengine/systems/modules/ai/deBaseAIModule.h"


class dedaiDeveloperMode;
class dedaiCommandExecuter;



/**
 * \brief DEAI AI module.
 */
class deDEAIModule : public deBaseAIModule{
private:
	dedaiDeveloperMode *pDeveloperMode;
	dedaiCommandExecuter *pCommandExecuter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new ai module. */
	deDEAIModule( deLoadableModule &loadableModule );
	/** \brief Cleans up the ai module. */
	virtual ~deDEAIModule();
	/*@}*/
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initializes the ai module. */
	virtual bool Init();
	/** \brief Shuts down the ai module and cleans up. */
	virtual void CleanUp();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for a world object. */
	virtual deBaseAIWorld *CreateWorld( deWorld *world );
	
	/** \brief Create peer for a navigation space object. */
	virtual deBaseAINavigationSpace *CreateNavigationSpace( deNavigationSpace *navspace );
	
	/** \brief Create peer for a navigation blocker object. */
	virtual deBaseAINavigationBlocker *CreateNavigationBlocker( deNavigationBlocker *blocker );
	
	/** \brief Create peer for a navigator object. */
	virtual deBaseAINavigator *CreateNavigator( deNavigator *navigator );
	
	/** \brief Create peer for height terrain. */
	virtual deBaseAIHeightTerrain *CreateHeightTerrain( deHeightTerrain &heightTerrain );
	
	/** \brief Retrieves the developer mode. */
	inline dedaiDeveloperMode &GetDeveloperMode() const{ return *pDeveloperMode; }
	/*@}*/
	
	/** \name Debugging */
	/*@{*/
	/**
	 * \brief Sends a command to the module and retrieves an answer from it.
	 * \details At least the 'help' command has to be understood answering a
	 *          list of possible commands. The command is provided in the 'command'
	 *          parameter and the answer has to be written into 'answer'. The default
	 *          implementation simply answers only to help with itself.
	 * \param[in] command The command to send to the module as a list of arguments.
	 * \param[out] answer Unicode string to hold the answer returned from the module.
	 */
	virtual void SendCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/*@}*/
};

#endif
