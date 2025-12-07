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

#ifndef _DECSHAPEBOX_H_
#define _DECSHAPEBOX_H_

#include "decShape.h"


/**
 * \brief Analytic Box Shape.
 *
 * Defines an analytic box shape. Box shapes are defined by a position,
 * an orientation, the half length of their extends and tapering. The
 * tapering is multiplied with the x and z half extends at the top of
 * the box.
 */
class DE_DLL_EXPORT decShapeBox : public decShape{
private:
	decVector pHalfExtends;
	decVector2 pTapering;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create box shape. */
	decShapeBox(const decVector &halfExtends);
	
	/** \brief Create box shape. */
	decShapeBox(const decVector &halfExtends, const decVector &position);
	
	/** \brief Create box shape. */
	decShapeBox(const decVector &halfExtends, const decVector &position,
		const decQuaternion &orientation);
	
	/** \brief Create tapered box shape. */
	decShapeBox(const decVector &halfExtends, const decVector2 &tapering);
	
	/** \brief Create tapered box shape. */
	decShapeBox(const decVector &halfExtends, const decVector2 &tapering,
		const decVector &position);
	
	/** \brief Create tapered box shape. */
	decShapeBox(const decVector &halfExtends, const decVector2 &tapering,
		const decVector &position, const decQuaternion &orientation);
	
	/** \brief Clean up box shape. */
	~decShapeBox() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Half extends. */
	inline const decVector &GetHalfExtends() const{ return pHalfExtends; }
	
	/** \brief Set half extends. */
	void SetHalfExtends(const decVector &halfExtends);
	
	/** \brief Tapering. */
	inline const decVector2 &GetTapering() const{ return pTapering; }
	
	/** \brief Set tapering. */
	void SetTapering(const decVector2 &tapering);
	
	
	
	/** \brief Create copy of shape. */
	decShape *Copy() const override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	void Visit(decShapeVisitor &visitor) override;
	/*@}*/
};

#endif
