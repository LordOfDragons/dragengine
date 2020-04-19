/* 
 * Drag[en]gine IGDE World Editor
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

// include only once
#ifndef _MEUOBJECTTEXTURESETSKIN_H_
#define _MEUOBJECTTEXTURESETSKIN_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class meObjectTexture;
class meObjectTextureList;



/**
 * Undo action for setting object layer projection axis.
 */
class meUObjectTextureSetSkin : public igdeUndo{
private:
	struct sTexture{
		meObjectTexture *texture;
		decString oldskin;
		decString newskin;
	};
	
private:
	sTexture *pTextures;
	int pTextureCount;
	
public:
	// constructor, destructor
	meUObjectTextureSetSkin( meObjectTexture *texture, const char *newskin );
	meUObjectTextureSetSkin( meObjectTextureList &textures, const char *newskin );
	
protected:
	~meUObjectTextureSetSkin();
	
public:
	// management
	void Undo();
	void Redo();
	
private:
	void pCleanUp();
};

// end of include only once
#endif
