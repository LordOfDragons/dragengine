/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
