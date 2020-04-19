/* 
 * Drag[en]gine Basic Network Module
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

#ifndef _DEBNWORLD_H_
#define _DEBNWORLD_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/network/deBaseNetworkWorld.h>

class deWorld;
class debnState;
class debnNetwork;
class deNetworkBasic;



/**
 * \brief World class.
 */
class debnWorld : public deBaseNetworkWorld{
private:
	deWorld *pWorld;
	
	bool pCanCreateStates;
	
	debnState **pStateSlots;
	int pStateSlotCount;
	
	debnNetwork *pParentNetwork;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create world. */
	debnWorld( deNetworkBasic *oal, deWorld *world );
	
	/** \brief Clean up world. */
	virtual ~debnWorld();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief World. */
	inline deWorld *GetWorld() const{ return pWorld; }
	
	/** \brief User is allowed to create states. */
	inline bool GetCanCreateStates() const{ return pCanCreateStates; }
	
	/** \brief Set if user is allowed to create states. */
	void SetCanCreateStates( bool canCreateStates );
	
	/** \brief Initialize network states. */
	void InitStates();
	
	/** \brief Parent network or NULL. */
	inline debnNetwork *GetParentNetwork() const{ return pParentNetwork; }
	
	/** \brief Set parent world or NULL. */
	void SetParentNetwork( debnNetwork *network );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief NetworkState has been added. */
	virtual void NetworkStateAdded( deNetworkState *state );
	
	/** \brief NetworkState has been removed. */
	virtual void NetworkStateRemoved( deNetworkState *state );
	
	/** \brief All states have been removed. */
	virtual void AllNetworkStatesRemoved();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	int pNextFreeSlot();
};

#endif
