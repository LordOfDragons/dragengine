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

#ifndef _CEWPASNIPPET_H_
#define _CEWPASNIPPET_H_

#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeButtonReference.h>

#include "ceWPAction.h"

class ceWPTopic;
class ceCASnippet;



/**
 * \brief Snippet Conversation Action property window.
 */
class ceWPASnippet : public ceWPAction{
private:
	igdeComboBoxFilterReference pCBFile;
	igdeComboBoxFilterReference pCBTopic;
	igdeButtonReference pBtnJumpToTopic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPASnippet( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPASnippet();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active action. */
	ceCASnippet *GetAction() const;
	
	/** \brief Update topic list. */
	void UpdateTopicList();
	
	/** \brief Update file list. */
	void UpdateFileList();
	
	/** \brief Update active action. */
	void UpdateAction();
	/*@}*/
};

#endif
