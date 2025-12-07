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

#ifndef _DEOALCOMPONENTTEXTURE_H_
#define _DEOALCOMPONENTTEXTURE_H_

class deoalComponent;
class deoalAComponentTexture;
class deoalSkin;

class deComponentTexture;



/**
 * \brief Component texture.
 */
class deoalComponentTexture{
private:
	deoalComponent &pComponent;
	const int pIndex;
	deoalAComponentTexture::Ref pATexture;
	
	deoalSkin *pSkin;
	
	bool pDirtyTexture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component texture. */
	deoalComponentTexture(deoalComponent &component, int index);
	
	/** \brief Clean up component texture. */
	~deoalComponentTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Component. */
	inline deoalComponent &GetComponent() const{ return pComponent; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Audio component texture. */
	inline const deoalAComponentTexture::Ref &GetATexture() const{ return pATexture; }
	
	/** \brief Synchronize. */
	void Synchronize();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Texture changed. */
	void TextureChanged(const deComponentTexture &texture);
	
	/** \brief Mark texture dirty. */
	void MarkDirty();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
