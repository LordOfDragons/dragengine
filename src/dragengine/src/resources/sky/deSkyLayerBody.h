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

#ifndef _DESKYLAYERBODY_H_
#define _DESKYLAYERBODY_H_

#include "../../common/math/decMath.h"
#include "../skin/deSkin.h"


/**
 * \brief Sky Bodies Layer Body.
 *
 * Defines a body in a sky bodies layer. For positioning the body the
 * sky is considered to be a sphere of unit radius. The position of
 * the body is then defined using a quaternion rotation. At identity
 * rotation the body is located at the equator at the 0° meridian
 * which is the same as the body located 1 unit along the Z-Axis.
 * The size of the body is defined using two angles measured in
 * radians. The angles indicate the entire width and height of the
 * body not half the width and height. This angle is comparable to
 * the astronomic angle under which you see a body in the sky. The
 * body is drawn using the given image which can use transparency.
 * Furthermore a color including transparency can be defined which
 * is multiplied component wise with the image pixels.
 */
class DE_DLL_EXPORT deSkyLayerBody{
private:
	decQuaternion pOrientation;
	decVector2 pSize;
	decColor pColor;
	deSkin::Ref pSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new sky bodies layer body. */
	deSkyLayerBody();
	
	/** \brief Clean up sky bodies layer body. */
	~deSkyLayerBody();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{return pOrientation;}
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Size in radians. */
	inline const decVector2 &GetSize() const{return pSize;}
	
	/** \brief Set size in radians. */
	void SetSize(const decVector2 &size);
	
	/** \brief Blending color. */
	inline const decColor &GetColor() const{return pColor;}
	
	/** \brief Set blending color. */
	void SetColor(const decColor &color);
	
	/** \brief Skin or NULL if not set. */
	inline deSkin *GetSkin() const{return pSkin;}
	
	/** \brief Set skin or NULL if not set. */
	void SetSkin(deSkin *skin);
	/*@}*/
};

#endif
