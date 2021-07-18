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

#ifndef _DENAVIGATIONBLOCKER_H_
#define _DENAVIGATIONBLOCKER_H_

#include "../space/deNavigationSpace.h"
#include "../../deResource.h"
#include "../../../common/math/decMath.h"
#include "../../../common/shape/decShapeList.h"

class deBaseAINavigationBlocker;
class deNavigationBlockerManager;
class deWorld;


/**
 * \brief Blocker for navigation spaces.
 * 
 * Blockers are used to restrict navigation spaces. This can be used to modify naviagion
 * spaces without having to modify their meshes which is the case for placing props in
 * scenes while the navigation space defines the general layout of a room. Blockers can
 * also be used to dynamically alter navigation like doors temporarily blocking off
 * rooms. In this case AI automatically picks new path and the blocker can be disabled
 * later on. Blockers can be moved while active. This should be done moderately since
 * changing the navigation layout all time can be potentially expensive and AI might be
 * confused if path is constantly changing. The already calculates path are not affected
 * by a change. The game scripts have to react to the changes at their own discretion.
 */
class deNavigationBlocker : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNavigationBlocker> Ref;
	
	
	
private:
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pScaling;
	int pLayer;
	deNavigationSpace::eSpaceTypes pSpaceType;
	int pBlockingPriority;
	bool pEnabled;
	
	decShapeList pShapeList;
	
	deBaseAINavigationBlocker *pPeerAI;
	
	deWorld *pParentWorld;
	deNavigationBlocker *pLLWorldPrev;
	deNavigationBlocker *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new blocker. */
	deNavigationBlocker( deNavigationBlockerManager *manager );
	
protected:
	/**
	 * \brief Clean up blocker.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deNavigationBlocker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling( const decVector &scaling );
	
	/** \brief Layer number. */
	inline int GetLayer() const{ return pLayer; }
	
	/** \brief Set layer number. */
	void SetLayer( int layer );
	
	/** \brief Space type to navigate. */
	inline deNavigationSpace::eSpaceTypes GetSpaceType() const{ return pSpaceType; }
	
	/** \brief Set space type to navigate. */
	void SetSpaceType( deNavigationSpace::eSpaceTypes spaceType );
	
	/**
	 * \brief Blocking priority.
	 * 
	 * Blocks navigation meshes with a smaller or equal blocking priority.
	 */
	inline int GetBlockingPriority() const{ return pBlockingPriority; }
	
	/**
	 * \brief Set blocking priority.
	 * 
	 * Blocks navigation meshes with a smaller or equal blocking priority.
	 */
	void SetBlockingPriority( int priority );
	
	/** \brief Blocker is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if blocker is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Shape list. */
	inline decShapeList &GetShapeList(){ return pShapeList; }
	inline const decShapeList &GetShapeList() const{ return pShapeList; }
	
	/** \brief Notifies the peers that the shape list changed. */
	void NotifyShapeListChanged();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief AI system peer. */
	inline deBaseAINavigationBlocker *GetPeerAI() const{ return pPeerAI; }
	
	/** \brief Set AI system peer. */
	void SetPeerAI( deBaseAINavigationBlocker *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous blocker in the parent world linked list. */
	inline deNavigationBlocker *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next blocker in the parent world linked list. */
	void SetLLWorldPrev( deNavigationBlocker *blocker );
	
	/** \brief Next blocker in the parent world linked list. */
	inline deNavigationBlocker *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next blocker in the parent world linked list. */
	void SetLLWorldNext( deNavigationBlocker *blocker );
	/*@}*/
};

#endif
