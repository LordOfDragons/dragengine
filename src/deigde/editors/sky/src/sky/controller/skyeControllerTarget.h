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

#ifndef _SKYECONTROLLERTARGET_H_
#define _SKYECONTROLLERTARGET_H_

#include "../link/skyeLink.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>

class deSkyControllerTarget;



/**
 * \brief Links for controller target.
 */
class skyeControllerTarget : public deObject{
public:
	typedef deTObjectReference<skyeControllerTarget> Ref;
	typedef decTObjectOrderedSet<skyeControllerTarget> List;
	
	
private:
	skyeLink::List pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller target. */
	skyeControllerTarget();
	
	/** \brief Create copy of controller target. */
	skyeControllerTarget(const skyeControllerTarget &copy);
	
protected:
	/** \brief Clean up controller target. */
	~skyeControllerTarget();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Links. */
	const skyeLink::List &GetLinks() const{ return pLinks; }
	
	/** \brief Add link. */
	void AddLink(skyeLink *link);
	
	/** \brief Remove link. */
	void RemoveLink(skyeLink *link);
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another controller target to this controller target. */
	skyeControllerTarget &operator=(const skyeControllerTarget &copy);
	/*@}*/
};

#endif
