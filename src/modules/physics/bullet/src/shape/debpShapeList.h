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

#ifndef _DEBPSHAPELIST_H_
#define _DEBPSHAPELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>

class debpShape;



/**
 * \brief Bullet Shape List.
 */
class debpShapeList{
private:
	decObjectOrderedSet pShapes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shape list. */
	debpShapeList();
	
	/** \brief Clean up shape list. */
	~debpShapeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of shapes. */
	int GetShapeCount() const;
	
	/** \brief Shape at index. */
	debpShape *GetShapeAt(int index) const;
	
	/** \brief Add shape. */
	void AddShape(debpShape *shape);
	
	/** \brief Remove all shapes. */
	void RemoveAllShapes();
	
	/** \brief Update collision volumes of all shapes using a transformation matrix. */
	void UpdateWithMatrix(const decDMatrix &transformation);
	
	/** \brief Update collision volumes of all shapes using a transformation matrix. */
	void UpdateWithMatrix(const decDMatrix &transformation, const decDVector &scale);
	/*@}*/
};

#endif
