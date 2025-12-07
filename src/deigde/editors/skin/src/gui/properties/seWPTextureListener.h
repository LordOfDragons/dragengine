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

#ifndef _SEWPTEXTURELISTENER_H_
#define _SEWPTEXTURELISTENER_H_

#include "../../skin/seSkinListener.h"

class seWPTexture;



/**
 * \brief Texture panel listener.
 */
class seWPTextureListener : public seSkinListener{
private:
	seWPTexture &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPTextureListener(seWPTexture &panel);
	
	/** \brief Clean up listener. */
	virtual ~seWPTextureListener();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Mapped have been added or removed. */
	virtual void MappedStructureChanged(seSkin *skin);
	
	/** Mapped name changed. */
	virtual void MappedNameChanged(seSkin *skin, seMapped *mapped);
	
	
	
	/** \brief Texture added or removed. */
	virtual void TextureStructureChanged(seSkin *skin);
	
	/** \brief Texture changed. */
	virtual void TextureChanged(seSkin *skin, seTexture *texture);
	
	/** \brief Texture name changed. */
	virtual void TextureNameChanged(seSkin *skin, seTexture *texture);
	
	/** \brief Active texture changed. */
	virtual void ActiveTextureChanged(seSkin *skin);
	
	
	
	/** \brief Property added or removed. */
	virtual void PropertyStructureChanged (seSkin *skin, seTexture *texture);
	
	/** \brief Property changed. */
	virtual void PropertyChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** \brief Active property changed. */
	virtual void ActivePropertyChanged(seSkin *skin, seTexture *texture);
	
	/** \brief Property node changed. */
	virtual void PropertyNodeChanged(seSkin *skin, seTexture *texture, seProperty *property, sePropertyNode *node);
	/*@}*/
};

#endif
