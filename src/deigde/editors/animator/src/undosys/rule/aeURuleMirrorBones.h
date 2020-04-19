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

#ifndef _AEUURULEMIRRORBONES_H_
#define _AEUURULEMIRRORBONES_H_

#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/string/decStringList.h>

class aeRule;



/**
 * \brief Undo action rule mirror bones.
 * 
 * Mirrors the following bone names:
 * - '.l' with '.r'
 * - '.L' with '.R'
 * - ' l' with ' r'
 * - ' L' with ' R'
 * - '.left' with '.right'
 * - ' left' with ' right'
 */
class aeURuleMirrorBones : public igdeUndo{
private:
	aeRule *pRule;
	decStringList pPatterns;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	aeURuleMirrorBones( aeRule *rule );
	
protected:
	/** \brief Clean up undo. */
	virtual ~aeURuleMirrorBones();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action affects any bones. */
	bool HasAnyEffect() const;
	
	
	
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
