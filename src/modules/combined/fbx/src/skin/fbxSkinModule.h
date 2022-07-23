/* 
 * FBX Skin Module
 *
 * Copyskinht (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _FBXSKINMODULE_H_
#define _FBXSKINMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/skin/deBaseSkinModule.h>

class fbxScene;
class fbxNode;
class fbxProperty;
class fbxMaterial;
class fbxTexture;

class deSkinTexture;


/**
 * FBX Skin Module.
 */
class fbxSkinModule : public deBaseSkinModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	fbxSkinModule( deLoadableModule &loadableModule );
	
	/** Clean up module. */
	virtual ~fbxSkinModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Load skin. */
	virtual void LoadSkin( decBaseFileReader &reader, deSkin &skin );
	
	/** Save skin. */
	virtual void SaveSkin( decBaseFileWriter &writer, const deSkin &skin );
	/*@}*/
	
	
	
private:
	void pLoadSkin( deSkin &skin, fbxScene &scene );
	void pLoadMaterial( deSkin &skin, fbxScene &scene, const fbxMaterial &material );
	void pAddPropertyValue( deSkinTexture &texture, const char *name, float value );
	void pAddPropertyColor( deSkinTexture &texture, const char *name, const decColor &color );
	void pAddPropertyImage( deSkinTexture &texture, const char *name, const fbxTexture &fbxtex );
};

#endif
