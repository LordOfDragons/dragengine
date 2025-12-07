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

#ifndef _DEEFFECVERLAYIMAGE_H_
#define _DEEFFECVERLAYIMAGE_H_

#include "deEffect.h"
#include "../image/deImage.h"
#include "../../common/math/decMath.h"


/**
 * \brief Effect overlay image.
 * 
 * The overlay image effect simply renders a predefined image over the existing
 * render output. The image can occupy any part of the existing image using the
 * texture coordinates. The image has to contain transparency to work best although
 * you can specify an additional transparency yourself. Furthermore additional
 * effects can be applied to this image ( effect chaining ) before applying it.
 */
class DE_DLL_EXPORT deEffectOverlayImage : public deEffect{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deEffectOverlayImage> Ref;
	
	
private:
	deImage::Ref pImage;
	decVector2 pTexCoords[4];
	float pTransparency;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create effect overlay image. */
	deEffectOverlayImage(deEffectManager *manager);
	
protected:
	/**
	 * \brief Clean up effect overlay image.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deEffectOverlayImage();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Image or NULL if not set. */
	inline deImage *GetImage() const{return pImage;}
	
	/** \brief Set image or NULL if not set. */
	void SetImage(deImage *image);
	
	/**
	 * \brief Texture coordinates for corner.
	 * \throw deeOutOfBoundary \em corner is less than 0 or greater than 3.
	 */
	const decVector2 &GetTextureCoordinatesFor(int corner) const;
	
	/**
	 * \brief Set texture coordinates for corner.
	 * \throw deeOutOfBoundary \em corner is less than 0 or greater than 3.
	 */
	void SetTextureCoordinatesFor(int corner, const decVector2 &textureCoordinates);
	
	/** \brief Transparency. */
	inline float GetTransparency() const{return pTransparency;}
	
	/** \brief Set transparency. */
	void SetTransparency(float transparency);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit(deEffectVisitor &visitor);
	/*@}*/
};

#endif
