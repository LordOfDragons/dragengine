/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _SEWPMAPPEDLISTENER_H_
#define _SEWPMAPPEDLISTENER_H_

#include "../../skin/seSkinListener.h"

class seWPMapped;


/**
 * Mapped panel listener.
 */
class seWPMappedListener : public seSkinListener{
public:
	typedef deTObjectReference<seWPMappedListener> Ref;
	
	
	
private:
	seWPMapped &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	seWPMappedListener( seWPMapped &panel );
	
	/** Clean up listener. */
	virtual ~seWPMappedListener() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Mapped added or removed. */
	virtual void MappedStructureChanged( seSkin *skin ) override;
	
	/** Mapped changed. */
	virtual void MappedChanged( seSkin *skin, seMapped *mapped ) override;
	
	/** Mapped name changed. */
	virtual void MappedNameChanged( seSkin *skin, seMapped *mapped ) override;
	
	/** Active mapped changed. */
	virtual void ActiveMappedChanged( seSkin *skin ) override;
	/*@}*/
};

#endif
