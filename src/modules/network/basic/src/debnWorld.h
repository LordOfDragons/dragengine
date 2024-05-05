/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
