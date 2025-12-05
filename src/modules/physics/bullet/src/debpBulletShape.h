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

#ifndef _DEBPBULLETSHAPE_H_
#define _DEBPBULLETSHAPE_H_

#include <dragengine/deObject.h>

class btCollisionShape;



/**
 * \brief Wraps a bullet collision shape.
 * 
 * Bullet does not manage the lifetime of create collision shapes. This is
 * done to allow using collision shapes on multiple collision objects. To
 * avoid memory leaking the created collision shapes have to be wrapped
 * into debpBulletShape which is reference counted to properly release the
 * bullet shapes once nobody uses them anymore.
 */
class debpBulletShape : public deObject {
private:
	btCollisionShape *pShape;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<debpBulletShape> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create bullet shape wrapper taking ownership of bullet shape. */
	debpBulletShape( btCollisionShape *shape );
	
	/** \brief Clean up bullet shape wrapper deleting wrapped bullet shape. */
	virtual ~debpBulletShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bullet shape. */
	inline btCollisionShape *GetShape() const{ return pShape; }
	/*@}*/
};

#endif
