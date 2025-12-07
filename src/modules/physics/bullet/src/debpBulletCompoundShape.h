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

#ifndef _DEBPBULLETCOMPOUNDSHAPE_H_
#define _DEBPBULLETCOMPOUNDSHAPE_H_

#include "debpBulletShape.h"

#include <dragengine/common/collection/decObjectList.h>

class btCompoundShape;



/**
 * \brief Wraps a bullet compound collision shape.
 * 
 * All child shapes are also stored in addition to the root shape.
 */
class debpBulletCompoundShape : public debpBulletShape {
private:
	decObjectList pChildShapes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create bullet shape wrapper taking ownership of bullet shape. */
	debpBulletCompoundShape(btCompoundShape *shape);
	
	/** \brief Clean up bullet shape wrapper deleting wrapped bullet shape. */
	virtual ~debpBulletCompoundShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bullet shape. */
	inline btCompoundShape *GetCompoundShape() const{return (btCompoundShape*)GetShape();}
	
	/** \brief Add child shape to clean up once the parent shape is finished. */
	void AddChildShape(debpBulletShape *shape);
	/*@}*/
};

#endif
