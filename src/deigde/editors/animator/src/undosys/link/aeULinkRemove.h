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

// include only once
#ifndef _AEULINKREMOVE_H_
#define _AEULINKREMOVE_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class aeLink;
class aeRule;
class aeAnimator;
class aeControllerTarget;



/**
 * @brief Undo Link Remove.
 *
 * Removes a link.
 */
class aeULinkRemove : public igdeUndo{
private:
	struct sTarget{
		aeRule *rule;
		aeControllerTarget *target;
	};
	
	aeAnimator *pAnimator;
	aeLink *pLink;
	
	sTarget *pTargets;
	int pTargetCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	aeULinkRemove( aeLink *link );
protected:
	/** \brief Clean up undo. */
	virtual ~aeULinkRemove();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
	void pAddTarget( int targetCount, aeRule *rule, aeControllerTarget *target );
};

// end of include only once
#endif
