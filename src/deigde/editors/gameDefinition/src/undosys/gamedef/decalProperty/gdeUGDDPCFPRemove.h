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


#ifndef _GDEGDDPCFPREMOVE_H_
#define _GDEGDDPCFPREMOVE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class gdeGameDefinition;
class gdeProperty;
class gdeFilePattern;



/**
 * \brief Undo action object class property remove custom file pattern.
 */
class gdeUGDDPCFPRemove : public igdeUndo{
private:
	gdeGameDefinition *pGameDefinition;
	gdeProperty *pProperty;
	gdeFilePattern *pFilePattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	gdeUGDDPCFPRemove( gdeGameDefinition *gamedef, gdeProperty *property, gdeFilePattern *filePattern );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~gdeUGDDPCFPRemove();
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
