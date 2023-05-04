/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	aeULinkRemove( aeLink *link );
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
	void pAddTargets( int targetCount, aeLink *link, aeRule *rule );
	void pAddTarget( int targetCount, aeRule *rule, aeControllerTarget *target );
};

#endif
