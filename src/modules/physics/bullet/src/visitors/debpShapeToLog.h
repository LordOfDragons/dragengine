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

#ifndef _DEBPSHAPETOLOG_H_
#define _DEBPSHAPETOLOG_H_

#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class debpDCollisionVolume;
class debpDCollisionSphere;
class debpDCollisionBox;
class debpDCollisionCylinder;
class debpDCollisionCapsule;


/** Shape to log string visitor. */
class debpShapeToLog : public decShapeVisitor{
private:
	decString pLog;
	
	
	
public:
	/** \name Constructor, Destructor */
	/*@{*/
	/** \brief Create visitor. */
	debpShapeToLog();
	
	/** \brief Clean up visitor. */
	virtual ~debpShapeToLog() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline const decString &GetLog() const{ return pLog; }
	
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit sphere shape. */
	virtual void VisitShapeSphere( decShapeSphere &sphere ) override;
	
	/** \brief Visit box shape. */
	virtual void VisitShapeBox( decShapeBox &box ) override;
	
	/** \brief Visit cylinder shape. */
	virtual void VisitShapeCylinder( decShapeCylinder &cylinder ) override;
	
	/** \brief Visit capsule shape. */
	virtual void VisitShapeCapsule( decShapeCapsule &capsule ) override;
	
	/** \brief Visit hull shape. */
	virtual void VisitShapeHull( decShapeHull &hull ) override;
	/*@}*/
};

#endif
