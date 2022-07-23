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

#ifndef _DEBASEGRAPHICSKYINSTANCE_H_
#define _DEBASEGRAPHICSKYINSTANCE_H_

#include "../../../dragengine_export.h"


/**
 * \brief Graphic module sky instance peer.
 */
class DE_DLL_EXPORT deBaseGraphicSkyInstance{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseGraphicSkyInstance();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicSkyInstance();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Sky changed. */
	virtual void SkyChanged();
	
	/** \brief Order changed. */
	virtual void OrderChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int index );
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	/*@}*/
};

#endif
