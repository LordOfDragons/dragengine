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

#ifndef _DESKINPROPERTYMAPPED_H_
#define _DESKINPROPERTYMAPPED_H_

#include "deSkinProperty.h"
#include "../../../common/curve/decCurveBezier.h"
#include "../../../common/math/decMath.h"


/**
 * \brief Skin Texture Property Mapped.
 *
 * Samples curves to produce value to use. For each color component one curve can be defined.
 * If all but the red component curve are empty the red component curve is used for all other
 * components. Different inputs can be used for the curves:
 * - Time: Use elapsed time.
 * - Bone Position X: Use named bone X position.
 * - Bone Position Y: Use named bone Y position.
 * - Bone Position Z: Use named bone Z position.
 * - Bone Rotation X: Use named bone X rotation in the range 0 to 1 (0 to 360 degrees).
 * - Bone Rotation Y: Use named bone Y rotation in the range 0 to 1 (0 to 360 degrees).
 * - Bone Rotation Z: Use named bone Z rotation in the range 0 to 1 (0 to 360 degrees).
 * - Bone Scale X: Use named bone X scale.
 * - Bone Scale Y: Use named bone Y scale.
 * - Bone Scale Z: Use named bone Z scale.
 * 
 * The input range maps the input value to the 0->1 range used to sample the curves.
 * This allows to easily modify play speed (for the elapsed time case) or bone state range
 * (for bone related types). For bone related times the bone name has to be set too.
 * By default input values are wrapped around inside input range. This is best used for
 * time based repeating animations. For bone related input it can be useful to enable
 * clamping to avoid values wrapping around.
 */
class DE_DLL_EXPORT deSkinPropertyMapped : public deSkinProperty{
private:
	int pComponents[4];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new named skin property color with the given type.
	 * \param type Type of the skin property obtained from adding a property
	 * name to the texture property map object held by the engine.
	 */
	deSkinPropertyMapped(const char *type);
	
	/** \brief Clean up skin property color. */
	virtual ~deSkinPropertyMapped();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Red component. */
	inline int GetRed() const{ return pComponents[0]; }
	
	/** \brief Set red component. */
	void SetRed(int mapped);
	
	/** \brief Green component. */
	inline int GetGreen() const{ return pComponents[1]; }
	
	/** \brief Set green component. */
	void SetGreen(int mapped);
	
	/** \brief Blue component. */
	inline int GetBlue() const{ return pComponents[2]; }
	
	/** \brief Set blue component. */
	void SetBlue(int mapped);
	
	/** \brief Alpha component. */
	inline int GetAlpha() const{ return pComponents[3]; }
	
	/** \brief Set alpha component. */
	void SetAlpha(int mapped);
	
	/** \brief Component. */
	int GetComponent(int component) const;
	
	/** \brief Set component. */
	void SetComponent(int component, int mapped);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void Visit(deSkinPropertyVisitor &visitor);
	/*@}*/
};

#endif
