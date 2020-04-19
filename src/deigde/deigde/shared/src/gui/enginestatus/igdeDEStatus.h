/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEDESTATUS_H_
#define _IGDEDESTATUS_H_

#include "../igdeTextAreaReference.h"
#include "../igdeIconListBoxReference.h"
#include "../igdeButtonReference.h"
#include "../layout/igdeContainerFlow.h"

class igdeDialogEngine;
class deBaseSystem;


/**
 * Panel showing the status of the game engine. Can also start and stop
 * the engine.
 */
class igdeDEStatus : public igdeContainerFlow{
private:
	igdeDialogEngine &pDialogEngine;
	
	igdeTextAreaReference pTextStatus;
	igdeIconListBoxReference pListSystems;
	igdeButtonReference pBtnStart;
	igdeButtonReference pBtnStop;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeDEStatus( igdeDialogEngine &windowEngine );
	
	
	
protected:
	/** \brief Cleans up panel. */
	virtual ~igdeDEStatus();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	void UpdateStatus();
	
	/** \brief Start game engine. */
	void StartEngine();
	
	/** \brief Stop game engine. */
	void StopEngine();
	/*@}*/
	
	
	
private:
	void pAddSystem( deBaseSystem *system );
};

#endif
