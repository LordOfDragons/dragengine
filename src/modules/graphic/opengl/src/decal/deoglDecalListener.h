/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLDECALLISTENER_H_
#define _DEOGLDECALLISTENER_H_

#include <dragengine/deObject.h>

class deoglRDecal;


/**
 * Render decal listener.
 */
class deoglDecalListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create decal listener. */
	deoglDecalListener();
	
	/** Clean up decal listener. */
	virtual ~deoglDecalListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Decal has been destroyed. */
	virtual void DecalDestroyed( deoglRDecal &decal );
	
	/** Decal geometry changed. */
	virtual void GeometryChanged( deoglRDecal &decal );
	
	/** Texture changed. */
	virtual void TextureChanged( deoglRDecal &decal );
	
	/** TUC changed. */
	virtual void TUCChanged( deoglRDecal &decal );
	/*@}*/
};

#endif
