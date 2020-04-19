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

#ifndef _DEDAINAVSPACE_H_
#define _DEDAINAVSPACE_H_

#include <dragengine/systems/modules/ai/deBaseAINavigationSpace.h>

class dedaiSpace;
class dedaiWorld;
class deDEAIModule;

class deNavigationSpace;



/**
 * \brief Navigation space peer.
 */
class dedaiNavSpace : public deBaseAINavigationSpace{
private:
	deDEAIModule &pDEAI;
	const deNavigationSpace &pNavigationSpace;
	
	dedaiWorld *pParentWorld;
	
	dedaiSpace *pSpace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedaiNavSpace( deDEAIModule &deai, const deNavigationSpace &navigationSpace );
	
	/** \brief Clean up peer. */
	virtual ~dedaiNavSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief AI module. */
	inline deDEAIModule &GetDEAI() const{ return pDEAI; }
	
	/** \brief Navigation space. */
	inline const deNavigationSpace &GetNavigationSpace() const{ return pNavigationSpace; }
	
	
	
	/** \brief Parent world or \em NULL. */
	inline dedaiWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL. */
	void SetParentWorld( dedaiWorld *world );
	
	
	
	/** \brief Space. */
	inline dedaiSpace *GetSpace() const{ return pSpace; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Layer changed. */
	virtual void LayerChanged();
	
	/** \brief Type changed. */
	virtual void TypeChanged();
	
	/** \brief Snapping parameters changed. */
	virtual void SnappingChanged();
	
	/** \brief Blocker shape list changed. */
	virtual void BlockerShapeChanged();
	
	/** \brief Layout changed. */
	virtual void LayoutChanged();
	
	/** \brief Blocking priority changed. */
	virtual void BlockingPriorityChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
