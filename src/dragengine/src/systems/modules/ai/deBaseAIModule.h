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

#ifndef _DEBASEAIMODULE_H_
#define _DEBASEAIMODULE_H_

#include "../deBaseModule.h"

class deHeightTerrain;
class deBaseAIHeightTerrain;
class deBaseAIAI;
class deBaseAINavigationBlocker;
class deBaseAINavigationSpace;
class deBaseAINavigator;
class deBaseAIWorld;
class deNavigationBlocker;
class deNavigationSpace;
class deNavigator;
class deWorld;



/**
 * \brief Base AI module interface.
 */
class DE_DLL_EXPORT deBaseAIModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new AI module. */
	deBaseAIModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up AI module. */
	virtual ~deBaseAIModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize ai module. */
	virtual bool Init() = 0;
	
	/** \brief Shut down ai module and clean up. */
	virtual void CleanUp() = 0;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for a world object. */
	virtual deBaseAIWorld *CreateWorld( deWorld *world ) = 0;
	
	/** \brief Create peer for a navigation space object. */
	virtual deBaseAINavigationSpace *CreateNavigationSpace( deNavigationSpace *navspace ) = 0;
	
	/** \brief Create peer for a navigation blocker object. */
	virtual deBaseAINavigationBlocker *CreateNavigationBlocker( deNavigationBlocker *blocker ) = 0;
	
	/** \brief Create peer for a navigator object. */
	virtual deBaseAINavigator *CreateNavigator( deNavigator *navigator ) = 0;
	
	/** \brief Create peer for height terrain. */
	virtual deBaseAIHeightTerrain *CreateHeightTerrain( deHeightTerrain &heightTerrain ) = 0;
	/*@}*/
};

#endif
