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

#ifndef _AEULINKREMOVE_H_
#define _AEULINKREMOVE_H_

#include <deigde/undo/igdeUndo.h>

#include "../../animator/aeAnimator.h"
#include "../../animator/link/aeLink.h"
#include "../../animator/rule/aeRule.h"

class aeControllerTarget;



/**
 * Undo Link Remove.
 */
class aeULinkRemove : public igdeUndo{
public:
	typedef deTObjectReference<aeULinkRemove> Ref;
	
	
private:
	struct sTarget{
		aeRule::Ref rule;
		aeControllerTarget *target;
	};
	
	aeAnimator::Ref pAnimator;
	aeLink::Ref pLink;
	
	sTarget *pTargets;
	int pTargetCount;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<aeULinkRemove> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	aeULinkRemove(aeLink *link);
protected:
	/** Clean up undo. */
	virtual ~aeULinkRemove();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	/** Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
	void pAddTargets(int targetCount, aeLink *link, aeRule *rule);
	void pAddTarget(int targetCount, aeRule *rule, aeControllerTarget *target);
};

#endif
