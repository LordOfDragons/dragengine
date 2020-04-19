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

#ifndef _DEBASEAIWORLD_H_
#define _DEBASEAIWORLD_H_

class deNavigationBlocker;
class deNavigationSpace;
class deNavigator;



/**
 * \brief AI Module World Peer.
 */
class deBaseAIWorld{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseAIWorld();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAIWorld();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Update world. */
	virtual void Update( float elapsed );
	
	/** \brief Navigation space has been added. */
	virtual void NavigationSpaceAdded( deNavigationSpace *navspace );
	
	/** \brief Navigation space has been removed. */
	virtual void NavigationSpaceRemoved( deNavigationSpace *navspace );
	
	/** \brief All navigation spaces have been removed. */
	virtual void AllNavigationSpacesRemoved();
	
	/** \brief Navigation blocker has been added. */
	virtual void NavigationBlockerAdded( deNavigationBlocker *blocker );
	
	/** \brief Navigation blocker has been removed. */
	virtual void NavigationBlockerRemoved( deNavigationBlocker *blocker );
	
	/** \brief All navigation blockers have been removed. */
	virtual void AllNavigationBlockersRemoved();
	
	/** \brief Navigator has been added. */
	virtual void NavigatorAdded( deNavigator *navigator );
	
	/** \brief Navigator has been removed. */
	virtual void NavigatorRemoved( deNavigator *navigator );
	
	/** \brief Remove all navigators. */
	virtual void AllNavigatorsRemoved();
	
	/** \brief Height Terrain changed. */
	virtual void HeightTerrainChanged();
	/*@}*/
};

#endif
