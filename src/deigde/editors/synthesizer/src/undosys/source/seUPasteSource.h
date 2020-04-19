/* 
 * Drag[en]gine IGDE Synthesizer Editor
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


#ifndef _SEUPASTESOURCE_H_
#define _SEUPASTESOURCE_H_

#include <deigde/undo/igdeUndo.h>
#include "../../synthesizer/source/seSourceList.h"
#include "../../synthesizer/link/seLinkList.h"
#include "../../synthesizer/controller/seControllerList.h"

class seSynthesizer;



/**
 * \brief Undo action paste source.
 */
class seUPasteSource : public igdeUndo{
private:
	seSynthesizer *pSynthesizer;
	seSourceList pSourceList;
	seLinkList pRemoveLinkList;
	seControllerList pRemoveControllerList;
	int pIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new undo object. */
	seUPasteSource( seSynthesizer *synthesizer, const seSourceList &sourceList, int index );
	
protected:
	/** \brief Clean up the undo object. */
	virtual ~seUPasteSource();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
