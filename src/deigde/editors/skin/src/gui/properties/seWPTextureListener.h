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
public:
	typedef deTObjectReference<seWPTextureListener> Ref;
	
private:
	seWPTexture &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPTextureListener(seWPTexture &panel);
	
	/** \brief Clean up listener. */
	~seWPTextureListener() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Mapped have been added or removed. */
	void MappedStructureChanged(seSkin *skin) override;
	
	/** Mapped name changed. */
	void MappedNameChanged(seSkin *skin, seMapped *mapped) override;
	
	
	
	/** \brief Texture added or removed. */
	void TextureStructureChanged(seSkin *skin) override;
	
	/** \brief Texture changed. */
	void TextureChanged(seSkin *skin, seTexture *texture) override;
	
	/** \brief Texture name changed. */
	void TextureNameChanged(seSkin *skin, seTexture *texture) override;
	
	/** \brief Active texture changed. */
	void ActiveTextureChanged(seSkin *skin) override;
	
	
	
	/** \brief Property added or removed. */
	void PropertyStructureChanged (seSkin *skin, seTexture *texture) override;
	
	/** \brief Property changed. */
	void PropertyChanged(seSkin *skin, seTexture *texture, seProperty *property) override;
	
	/** \brief Active property changed. */
	void ActivePropertyChanged(seSkin *skin, seTexture *texture) override;
	
	/** \brief Property node changed. */
	void PropertyNodeChanged(seSkin *skin, seTexture *texture, seProperty *property, sePropertyNode *node) override;
	/*@}*/
};

#endif
