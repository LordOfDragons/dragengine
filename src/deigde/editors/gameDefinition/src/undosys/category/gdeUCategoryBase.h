/* 
 * Drag[en]gine IGDE Game Definition Editor
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


#ifndef _GDEUCATEGORYBASE_H_
#define _GDEUCATEGORYBASE_H_

#include <deigde/undo/igdeUndo.h>

class gdeGameDefinition;
class gdeCategory;



/**
 * \brief Undo action category base class.
 */
class gdeUCategoryBase : public igdeUndo{
public:
	/** \brief Category type. */
	enum eCategoryType{
		ectObjectClass,
		ectParticleEmitter,
		ectSkin,
		ectSky
	};
	
	
	
private:
	gdeGameDefinition *pGameDefinition;
	eCategoryType pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	gdeUCategoryBase( gdeGameDefinition *gameDefintiion, eCategoryType type );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~gdeUCategoryBase();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Type. */
	inline eCategoryType GetType() const{ return pType; }
	
	/** \brief Notify game definition. */
	void Notify();
	/*@}*/
};

#endif
