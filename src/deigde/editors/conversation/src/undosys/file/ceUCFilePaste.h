/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEUCFILEPASTE_H_
#define _CEUCFILEPASTE_H_

#include "../../conversation/file/ceConversationFileList.h"

#include <deigde/undo/igdeUndo.h>

class ceConversation;
class ceConversationFile;



/**
 * \brief Undo action paste conversation file.
 */
class ceUCFilePaste : public igdeUndo{
private:
	ceConversation *pConversation;
	ceConversationFileList pFiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	ceUCFilePaste( ceConversation *conversation, const ceConversationFileList &files );
	
	/** \brief Clean up undo action. */
	virtual ~ceUCFilePaste();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief List of files. */
	inline const ceConversationFileList &GetFiles() const{ return pFiles; }
	
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
