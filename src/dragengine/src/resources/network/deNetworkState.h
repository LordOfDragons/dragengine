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

#ifndef _DENETWORKSTATE_H_
#define _DENETWORKSTATE_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"
#include "../../common/collection/decObjectOrderedSet.h"


class deNetworkStateManager;
class deNetworkValue;
class deBaseNetworkState;
class deBaseScriptingNetworkState;
class deWorld;


/**
 * \brief Network State Class.
 *
 * Stores the state of a game object. States can be positional if attached
 * to a World or positionless if attached to a network player. Only attached
 * network states are updated and monitored.
 */
class DE_DLL_EXPORT deNetworkState : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkState> Ref;
	
	
	
private:
	decObjectOrderedSet pValues;
	bool pReadOnly;
	
	deBaseNetworkState *pPeerNetwork;
	deBaseScriptingNetworkState *pPeerScripting;
	
	deWorld *pParentWorld;
	deNetworkState *pLLWorldPrev;
	deNetworkState *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network state object. */
	deNetworkState( deNetworkStateManager *manager, bool readOnly );
	
protected:
	/**
	 * \brief Clean up network state object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deNetworkState();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Resource is a valid parent. */
	bool IsValidParent( deResource *parent ) const;
	
	/** \brief Network state is read only. */
	inline bool GetReadOnly() const{ return pReadOnly; }
	/*@}*/
	
	
	
	/** \name Values */
	/*@{*/
	/** \brief Count of values. */
	int GetValueCount() const;
	
	/** \brief Index of value. */
	int IndexOfValue( deNetworkValue *value ) const;
	
	/** \brief Value at index. */
	deNetworkValue *GetValueAt( int index ) const;
	
	/** \brief Add value. */
	void AddValue( deNetworkValue *value );
	
	/** \brief Notify network peer value changed. */
	void NotifyValueChanged( int index ) const;
	
	/** \brief Notify network peer precision changed. */
	void NotifyPrecisionChanged( int index ) const;
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Network system peer object. */
	inline deBaseNetworkState *GetPeerNetwork() const{ return pPeerNetwork; }
	
	/** \brief Set network system peer object. */
	void SetPeerNetwork ( deBaseNetworkState *peer );
	
	/** \brief Scripting system peer object. */
	inline deBaseScriptingNetworkState *GetPeerScripting() const{ return pPeerScripting; }
	
	/** \brief Set scripting system peer object. */
	void SetPeerScripting( deBaseScriptingNetworkState *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous networkState in the parent world linked list. */
	inline deNetworkState *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next networkState in the parent world linked list. */
	void SetLLWorldPrev( deNetworkState *networkState );
	
	/** \brief Next networkState in the parent world linked list. */
	inline deNetworkState *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next networkState in the parent world linked list. */
	void SetLLWorldNext( deNetworkState *networkState );
	/*@}*/
};

#endif
