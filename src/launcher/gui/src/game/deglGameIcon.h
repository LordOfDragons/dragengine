/* 
 * Drag[en]gine GUI Launcher
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEGLGAMEICON_H_
#define _DEGLGAMEICON_H_

#include "../gui/deglSharedIcon.h"

#include <delauncher/game/icon/delGameIcon.h>


/**
 * Game icon.
 */
class deglGameIcon : public delGameIcon{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<deglGameIcon> Ref;
	
	
	
private:
	deglSharedIcon::Ref pFoxIcon;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create game iconn. */
	deglGameIcon( int size, const char *path );
	
protected:
	/** Cleans up the game. */
	virtual ~deglGameIcon();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** FOX icon or nullptr if unable to load. */
	inline deglSharedIcon *GetFoxIcon() const{ return pFoxIcon; }
	
	/** FOX icon scaled to size or nullptr if unable to load. */
	deglSharedIcon::Ref GetScaledFoxIcon( int size ) const;
	/*@}*/
	
	
	
protected:
	virtual void OnContentChanged();
	
	
	
private:
	void pCreateFoxIcon();
	void pCreatePNGFoxIcon();
};

#endif
