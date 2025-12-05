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

#ifndef _DEOGLSKINRENDERABLE_H_
#define _DEOGLSKINRENDERABLE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * Skin renderable usage information.
 */
class deoglSkinRenderable : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglSkinRenderable> Ref;


private:
	decString pName;
	bool pRequiresTexture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin renderable. */
	deoglSkinRenderable( const char *name );
	
protected:
	/** Cleans up the skin renderable. */
	virtual ~deoglSkinRenderable();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Renderable name. */
	inline const decString &GetName() const{ return pName; }
	
	/** One or more texture properties using this renderable require a texture. */
	inline bool GetRequiresTexture() const{ return pRequiresTexture; }
	
	/** Set if one or more texture properties using this renderable require a texture. */
	void SetRequiresTexture( bool requiresTexture );
	/*@}*/
};

#endif
