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

#ifndef _MEUOBJECTTEXTURESETSKIN_H_
#define _MEUOBJECTTEXTURESETSKIN_H_

#include "../../../../world/object/texture/meObjectTexture.h"

#include <deigde/undo/igdeUndo.h>


/**
 * Undo action for setting object layer projection axis.
 */
class meUObjectTextureSetSkin : public igdeUndo{
public:
	typedef deTObjectReference<meUObjectTextureSetSkin> Ref;
	
	
private:
	class cTexture : public deObject{
	public:
		typedef deTObjectReference<cTexture> Ref;
		typedef decTObjectOrderedSet<cTexture> List;
		
		meObjectTexture::Ref texture;
		decString oldskin, newskin;
		
		cTexture() = default;
	};
	
private:
	cTexture::List pTextures;
	
public:
	// constructor, destructor
	meUObjectTextureSetSkin(meObjectTexture *texture, const char *newskin);
	meUObjectTextureSetSkin(meObjectTexture::List &textures, const char *newskin);
	
protected:
	~meUObjectTextureSetSkin();
	
public:
	// management
	void Undo();
	void Redo();
};
#endif
