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

#ifndef _CEWPTTIMAIFELSEIFCASE_H_
#define _CEWPTTIMAIFELSEIFCASE_H_

#include "../../ceWPTTreeItemModel.h"

class ceCAIfElse;
class ceWPTTIMAIfElse;
class ceWPTTIMAIfElseIfCase;
class ceWPTTIMAIfElseIfCaseCondition;
class ceWPTTIMAIfElseIfCaseActions;


/**
 * \brief Action if-else action.
 */
class ceWPTTIMAIfElseIfCase : public ceWPTTreeItemModel{
private:
	ceCAIfElseCase *pIfCase;
	int pIndex;
	ceWPTTIMAIfElseIfCaseCondition *pCondition;
	ceWPTTIMAIfElseIfCaseActions *pActions;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAIfElseIfCase( ceWindowMain &windowMain, ceConversation &conversation,
		ceCAIfElse &ifElse, ceCAIfElseCase *ifCase, int index );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAIfElseIfCase();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief If-Else Case. */
	inline ceCAIfElseCase *GetIfCase() const{ return pIfCase; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex( int index );
	
	/** \brief Parent if-else model. */
	ceWPTTIMAIfElse *GetModelIfElse() const;
	
	/** \brief Model with condition or \em NULL. */
	inline ceWPTTIMAIfElseIfCaseCondition *GetChildCondition() const{ return pCondition; }
	
	/** \brief Model with actions or \em NULL. */
	inline ceWPTTIMAIfElseIfCaseActions *GetChildActions() const{ return pActions; }
	
	/** \brief Update if-case. */
	void Update();
	
	/** \brief Update action lists if present. */
	void UpdateActionLists();
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	/*@}*/
	
	
	
private:
	void pUpdateText();
};

#endif
