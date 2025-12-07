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

// include only once
#ifndef _RERIGTEXTURE_H_
#define _RERIGTEXTURE_H_

// includes
#include <dragengine/common/string/decString.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/skin/deSkin.h>

// predefinitions
class deEngine;
class reRig;



/**
 * @brief Rig Texture.
 * Defines a texture for use with the loaded component.
 */
class reRigTexture{
private:
	deEngine *pEngine;
	
	decString pName;
	decString pSkinPath;
	deSkin::Ref pEngSkin;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig texture. */
	reRigTexture(deEngine *engine, const char *name);
	/** Cleans up the rig texture. */
	~reRigTexture();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** Retrieves the name of the texture. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the skin file path. */
	inline const decString &GetSkinPath() const{ return pSkinPath; }
	/** Sets the skin file path. */
	void SetSkinPath(const char *skinPath);
	/** Loads the skin. */
	void LoadSkin();
	/** Retrieves the engine skin or NULL if not found. */
	inline const deSkin::Ref &GetEngineSkin() const{ return pEngSkin; }
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
