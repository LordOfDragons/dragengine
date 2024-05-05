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

#ifndef _DEBPTSCONTACTRESULTCALLBACK_H_
#define _DEBPTSCONTACTRESULTCALLBACK_H_

#include "../coldet/debpContactResultCallback.h"

class debpTouchSensor;


/**
 * \brief Modification of \ref debpContactResultCallback for use with touch sensor shapes.
 * 
 * \ref debpContactResultCallback provides near phase collision filtering between the
 * moving collider and the colliders overlapping the touch sensor. For use with touch
 * sensor shapes though the collision filtering has to include the touch sensor too.
 * To work properly this class has to provide these collision filtering behaviors:
 * 
 * - Moving collider versus hit collider
 * - Hit collider versus parent touch sensor
 * 
 * The moving collider is not supposed to be tested against the touch sensor. Only the
 * hit collider has to be tested. This supports the situation where the touch sensor
 * filters out a collider the moving collider can hit. This can be for example due to
 * an ignore collider in the touch sensor. The near phase has to do the filtering since
 * the broad phase can not handle this well.
 */
class debpTSContactResultCallback : public debpContactResultCallback{
private:
	const debpTouchSensor &pTouchSensor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates result callback. */
	debpTSContactResultCallback( deCollisionInfo *colinfo, const debpTouchSensor &touchSensor );
	/*@}*/
	
	
	
	/** \name Bullet */
	/*@{*/
	/** \brief Collision is possible. */
	virtual bool needsCollision( btBroadphaseProxy *proxy0 ) const;
	/*@}*/
};

#endif
