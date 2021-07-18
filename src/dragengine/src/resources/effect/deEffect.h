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

#ifndef _DEEFFECT_H_
#define _DEEFFECT_H_

#include "../../common/math/decMath.h"
#include "../deResource.h"


class deEffectFilterKernel;
class deEffectOverlayImage;
class deEffectColorMatrix;
class deEffectVisitor;
class deEffectManager;
class deBaseGraphicEffect;


/**
 * \brief Effect Class.
 *
 * Effects describe alterations of a world before it is rendered.
 * Such effects can alter the final image ( called post processing )
 * or alter the state of some objects. Various effects can be
 * combined to achieve all sorts of complex effects.
 */
class deEffect : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deEffect> Ref;
	
	
	
private:
	bool pEnabled;
	deBaseGraphicEffect *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new world object with the given resource manager. */
	deEffect( deEffectManager *manager );
	
protected:
	/**
	 * \brief Clean up world.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deEffect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Effect is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if effect is enabled. */
	void SetEnabled( bool enabled );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object. */
	inline deBaseGraphicEffect *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object. */
	void SetPeerGraphic( deBaseGraphicEffect *peer );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit( deEffectVisitor &visitor );
	/*@}*/
	
	
	
private:
	void pNotifyEnabledChanged();
};

#endif
