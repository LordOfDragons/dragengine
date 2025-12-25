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

#ifndef _SKYEULINKREMOVE_H_
#define _SKYEULINKREMOVE_H_

#include "../../sky/skyeSky.h"
#include "../../sky/layer/skyeLayer.h"
#include "../../sky/link/skyeLink.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/resources/sky/deSkyLayer.h>


/**
 * \brief Undo action remove link.
 */
class skyeULinkRemove : public igdeUndo{
public:
	typedef deTObjectReference<skyeULinkRemove> Ref;
	
	
private:
	class cTarget : public deObject{
	public:
		typedef deTObjectReference<cTarget> Ref;
		typedef decTObjectOrderedSet<cTarget> List;
		
		skyeLayer::Ref layer;
		deSkyLayer::eTargets target;
		
		cTarget() = default;
	};
	
	skyeSky::Ref pSky;
	skyeLink::Ref pLink;
	
	cTarget::List pTargets;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	skyeULinkRemove(skyeLink *link);
	
protected:
	/** \brief Clean up undo action. */
	virtual ~skyeULinkRemove();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
