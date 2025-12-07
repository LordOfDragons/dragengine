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

#ifndef _DECOMPONENTTEXTURE_H_
#define _DECOMPONENTTEXTURE_H_

#include "../../common/math/decMath.h"

class deSkin;
class deImage;
class deDynamicSkin;


/**
 * \brief Component Texture.
 * Texture definition for a component. For simple components a single skin is
 * assigned from which the contained textures are mapped to the model textures.
 * For more control the individual textures can be assigned different skins.
 * This is useful for components used as static terrain geometry and for editing
 * applications as this way individual textures can be assigned at run-time.
 * If the skin is NULL the matching texture of the skin assigned to the component
 * itself is used. Otherwise the first texture in the assigned skin is used to
 * replace the texture.
 */
class DE_DLL_EXPORT deComponentTexture{
private:
	deSkin *pSkin;
	int pTexture;
	decTexMatrix2 pTransform;
	deDynamicSkin *pDynamicSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new texture. */
	deComponentTexture();
	
	/** \brief Clean up texture. */
	~deComponentTexture();
	/*@}*/
	
	
	
	/** \name Projection */
	/*@{*/
	/** \brief Skin or NULL if not set. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or NULL if not set. */
	void SetSkin(deSkin *skin);
	
	/** \brief Texture number. */
	inline int GetTexture() const{ return pTexture; }
	
	/** \brief Set texture number. */
	void SetTexture(int texture);
	
	/** \brief Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set texture coordinate transformation matrix. */
	void SetTransform(const decTexMatrix2 &matrix);
	
	/** \brief Dynamic skin or NULL if not set. */
	inline deDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or NULL if not set. */
	void SetDynamicSkin(deDynamicSkin *dynamicSkin);
	/*@}*/
};

#endif
