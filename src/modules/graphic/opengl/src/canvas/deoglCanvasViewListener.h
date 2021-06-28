/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLCANVASVIEWLISTENER_H_
#define _DEOGLCANVASVIEWLISTENER_H_


/**
 * Canvas view listener.
 * \note Called from main thread during synchronization.
 */
class deoglCanvasViewListener{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create canvas view listener. */
	deoglCanvasViewListener();
	
	/** Clean up canvas view listener. */
	virtual ~deoglCanvasViewListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Canvas view has been destroyed. Owner has to drop weak reference to canvas view. */
	virtual void CanvasViewDestroyed();
	
	/** Canvas view requires sync. */
	virtual void CanvasViewRequiresSync();
	/*@}*/
};

#endif
