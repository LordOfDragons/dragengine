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

#ifndef _DESKYINSTANCE_H_
#define _DESKYINSTANCE_H_

#include "deSkyReference.h"
#include "../deResource.h"
#include "../../common/utils/decLayerMask.h"

class deSkyInstanceManager;
class deSkyController;
class deBaseGraphicSkyInstance;
class deWorld;


/**
 * \brief Sky instance.
 * 
 * Every sky contains a set of layers and controllers indicating how the final sky looks
 * like. While skies define how the sky looks like the actual sky is produced using per
 * instance parameters.
 */
class DE_DLL_EXPORT deSkyInstance : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSkyInstance> Ref;
	
	
	
private:
	deSkyReference pSky;
	int pOrder;
	
	deSkyController *pControllers;
	int pControllerCount;
	
	decLayerMask pLayerMask;
	
	deBaseGraphicSkyInstance *pPeerGraphic;
	
	deWorld *pParentWorld;
	deSkyInstance *pLLWorldPrev;
	deSkyInstance *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky instance. */
	deSkyInstance( deSkyInstanceManager *manager );
	
protected:
	/**
	 * \brief Clean up the sky instance.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSkyInstance();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sky or NULL if not set. */
	inline deSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky or NULL to clear. */
	void SetSky( deSky *sky );
	
	/** \brief Render order with higher order covering lower order. */
	inline int GetOrder() const{ return pOrder; }
	
	/** \brief Set render order with higher order covering lower order. */
	void SetOrder( int order );
	
	
	
	/** \brief Number of controllers. */
	inline int GetControllerCount() const{ return pControllerCount; }
	
	/**
	 * \brief Controller at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater or equal
	 * than GetControllerCount().
	 */
	deSkyController &GetControllerAt( int index ) const;
	
	/** \brief Index of named controller or -1 if absent. */
	int IndexOfControllerNamed( const char *name ) const;
	
	/** \brief Notify peer controller changed. */
	void NotifyControllerChangedAt( int index );
	
	
	
	/** \brief Layer mask. */
	const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic peer or NULL if not set. */
	inline deBaseGraphicSkyInstance *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic peer or NULL if not set. */
	void SetPeerGraphic( deBaseGraphicSkyInstance *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous sky in the parent world linked list. */
	inline deSkyInstance *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next sky in the parent world linked list. */
	void SetLLWorldPrev( deSkyInstance *sky );
	
	/** \brief Next sky in the parent world linked list. */
	inline deSkyInstance *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next sky in the parent world linked list. */
	void SetLLWorldNext( deSkyInstance *sky );
	/*@}*/
};

#endif
