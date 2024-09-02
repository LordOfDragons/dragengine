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

#ifndef _DECSHAPE_H_
#define _DECSHAPE_H_

#include "../math/decMath.h"

class decShapeVisitor;


/**
 * \brief Analytical Shape.
 * 
 * Defines an analytical shape. Exposes a visitor to determine the
 * type of shape. Subclass to define a new shape. The shape class
 * provides the basic parameters of a shape being the position
 * and the orientation.
 */
class DE_DLL_EXPORT decShape{
private:
	decVector pPosition;
	decQuaternion pOrientation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shape with position (0,0,0) and orientation (0,0,0,1). */
	decShape();
	
	/** \brief Create shape with the given position and orientation (0,0,0,1). */
	decShape( const decVector &position );
	
	/** \brief Create shape with the given position and orientation. */
	decShape( const decVector &position, const decQuaternion &orientation );
	
	/** \brief Clean up shape. */
	virtual ~decShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	
	
	/** \brief Create copy of shape. */
	virtual decShape *Copy() const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void Visit( decShapeVisitor &visitor );
	/*@}*/
};


#endif
