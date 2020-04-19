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

#ifndef _LPEVIEWLANGPACKLISTENER_H_
#define _LPEVIEWLANGPACKLISTENER_H_

#include "../langpack/lpeLangPackListener.h"

class lpeViewLangPack;



/**
 * \brief Language pack view listener.
 */
class lpeViewLangPackListener : public lpeLangPackListener{
private:
	lpeViewLangPack &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	lpeViewLangPackListener( lpeViewLangPack &window );
	
	/** \brief Clean up listener. */
	~lpeViewLangPackListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Language pack parameters changed. */
	virtual void LangPackChanged( lpeLangPack *langpack );
	
	
	
	/** \brief Entries have been added or removed. */
	virtual void EntryStructureChanged ( lpeLangPack *langpack );
	
	/** \brief An entry changed. */
	virtual void EntryChanged( lpeLangPack *langpack, lpeLangPackEntry *entry );
	
	/** \brief An entry name changed. */
	virtual void EntryNameChanged( lpeLangPack *langpack, lpeLangPackEntry *entry );
	
	/** \brief Selection of one or mote entries changed. */
	virtual void EntrySelectionChanged( lpeLangPack *langpack );
	
	/** \brief Active entry changed. */
	virtual void ActiveEntryChanged( lpeLangPack *langpack );
	/*@}*/
};

#endif
