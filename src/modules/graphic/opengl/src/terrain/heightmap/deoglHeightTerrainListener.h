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

#ifndef _DEOGLHEIGHTTERRAINLISTENER_H_
#define _DEOGLHEIGHTTERRAINLISTENER_H_

#include <dragengine/deObject.h>

class deoglRHeightTerrain;


/**
 * Render height terrain listener.
 */
class deoglHeightTerrainListener : public deObject{
public:
	typedef deTObjectReference<deoglHeightTerrainListener> Ref;
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create height terrain listener. */
	deoglHeightTerrainListener();
	
	/** Clean up height terrain listener. */
	virtual ~deoglHeightTerrainListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Height terrain has been destroyed. */
	virtual void HeightTerrainDestroyed( deoglRHeightTerrain &heightTerrain );
	
	/** Sectors changed. */
	virtual void SectorsChanged( deoglRHeightTerrain &heightTerrain );
	/*@}*/
};

#endif
