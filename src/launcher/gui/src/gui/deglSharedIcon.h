/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLSHAREDICON_H_
#define _DEGLSHAREDICON_H_

#include "foxtoolkit.h"

#include <dragengine/deObject.h>


/**
 * FOX icons are not reference counted. This makes sharing them problematic.
 */
class deglSharedIcon : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<deglSharedIcon> Ref;
	
	
	
private:
	FXIcon *pIcon;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared icon. */
	deglSharedIcon( FXIcon *icon );
	
protected:
	/** Clean up shared icon. */
	virtual ~deglSharedIcon();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Icon. */
	inline FXIcon *GetIcon() const{ return pIcon; }
	/*@}*/
};

#endif
