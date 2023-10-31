/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#ifndef _LPELANGPACKLISTENER_H_
#define _LPELANGPACKLISTENER_H_

#include <dragengine/deObject.h>

class lpeLangPack;
class lpeLangPackEntry;



/**
 * Language pack listener.
 */
class lpeLangPackListener : public deObject{
public:
	typedef deTObjectReference<lpeLangPackListener> Ref;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new langpack listener. */
	lpeLangPackListener();
	
protected:
	/** Cleans up the listener. */
	virtual ~lpeLangPackListener() override;
	/*@}*/
	
	
	
	
public:
	/** \name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged( lpeLangPack *langpack );
	
	/** Undo changed. */
	virtual void UndoChanged( lpeLangPack *langpack );
	
	/** Language pack parameters changed. */
	virtual void LangPackChanged( lpeLangPack *langpack );
	
	/** Entries have been added or removed. */
	virtual void EntryStructureChanged ( lpeLangPack *langpack );
	
	/** An entry changed. */
	virtual void EntryChanged( lpeLangPack *langpack, lpeLangPackEntry *entry );
	
	/** An entry name changed. */
	virtual void EntryNameChanged( lpeLangPack *langpack, lpeLangPackEntry *entry );
	
	/** Selection of one or mote entries changed. */
	virtual void EntrySelectionChanged( lpeLangPack *langpack );
	
	/** Active entry changed. */
	virtual void ActiveEntryChanged( lpeLangPack *langpack );
	/*@}*/
};

#endif
