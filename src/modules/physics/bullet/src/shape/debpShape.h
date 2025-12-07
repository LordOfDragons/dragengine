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

#ifndef _DEBPSHAPE_H_
#define _DEBPSHAPE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class decShape;
class debpDCollisionVolume;
class debpDCollisionBox;
class dePhysicsBullet;



/**
 * \brief Bullet Shape.
 *
 * Bullet counterpart of an engine shape. The bullet shape contains the original engine
 * shape and the collision volume used for collision testing. The collision volume can
 * be updated using a transformation matrix. A type is held to easily identify the
 * shape without requiring a visitor.
 */
class debpShape : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<debpShape> Ref;
	
	
	/** \brief Shape types. */
	enum eShapeTypes{
		estSphere,
		estBox,
		estCylinder,
		estCapsule
	};
	
	
	
private:
	int pType;
	decShape *pShape;
	debpDCollisionVolume *pCollisionVolume;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shape. */
	debpShape(int type, decShape *shape);
	
protected:
	/** \brief Clean up shape. */
	virtual ~debpShape();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Shape type. */
	inline int GetType() const{return pType;}
	
	/** \brief Engine shape. */
	inline decShape *GetShape() const{return pShape;}
	
	/** \brief Collision volume. */
	inline debpDCollisionVolume *GetCollisionVolume() const{return pCollisionVolume;}
	
	/** \brief Update collision volume using a transformation matrix. */
	virtual void UpdateWithMatrix(const decDMatrix &transformation, const decDVector &scale) = 0;
	
	/** \brief Print out on console debugging information about shape. */
	virtual void PrintDebug(dePhysicsBullet &module) = 0;
	/*@}*/
	
	
	
protected:
	void SetCollisionVolume(debpDCollisionVolume *collisionVolume);
	static float UniformScale(const decDMatrix &matrix);
};

#endif
