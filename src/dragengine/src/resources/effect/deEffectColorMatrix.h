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

#ifndef _DEEFFECTCOLORMATRIX_H_
#define _DEEFFECTCOLORMATRIX_H_

#include "deEffect.h"
#include "../../common/math/decMath.h"


/**
 * \brief Effect Color Matrix Class.
 *
 * Color Matrix effects are post processing effects that manipulate
 * the final image after rendering. Every pixel is transformed by
 * a given color matrix. This transformation is a linear transformation.
 * This effect is best used as the last effect in an effect chain since
 * most graphic modules can carry out such a transform as part of the
 * final image rendering in hardware.
 */
class DE_DLL_EXPORT deEffectColorMatrix : public deEffect{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deEffectColorMatrix> Ref;
	
	
private:
	decColorMatrix pColorMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new color matrix effect. */
	deEffectColorMatrix(deEffectManager *manager);
	
protected:
	/**
	 * \brief Clean up color matrix effect.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deEffectColorMatrix() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Color matrix. */
	inline const decColorMatrix &GetColorMatrix() const{ return pColorMatrix; }
	
	/** \brief Set color matrix. */
	void SetColorMatrix(const decColorMatrix &colorMatrix);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	void Visit(deEffectVisitor &visitor) override;
	/*@}*/
};

#endif
