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
		meObjectTexture::Ref texture;
		decString oldskin;
		decString newskin;
	};
	
private:
	sTexture *pTextures;
	int pTextureCount;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUObjectTextureSetSkin> Ref;
	
	// constructor, destructor
	meUObjectTextureSetSkin(meObjectTexture *texture, const char *newskin);
	meUObjectTextureSetSkin(meObjectTextureList &textures, const char *newskin);
	
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
