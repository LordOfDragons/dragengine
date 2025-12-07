/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	ceWPTTIMAIfElseIfCase(ceWindowMain &windowMain, ceConversation &conversation,
		ceCAIfElse &ifElse, ceCAIfElseCase *ifCase, int index);
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAIfElseIfCase();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief If-Else Case. */
	inline ceCAIfElseCase *GetIfCase() const{return pIfCase;}
	
	/** \brief Index. */
	inline int GetIndex() const{return pIndex;}
	
	/** \brief Set index. */
	void SetIndex(int index);
	
	/** \brief Parent if-else model. */
	ceWPTTIMAIfElse *GetModelIfElse() const;
	
	/** \brief Model with condition or \em NULL. */
	inline ceWPTTIMAIfElseIfCaseCondition *GetChildCondition() const{return pCondition;}
	
	/** \brief Model with actions or \em NULL. */
	inline ceWPTTIMAIfElseIfCaseActions *GetChildActions() const{return pActions;}
	
	/** \brief Update if-case. */
	void Update();
	
	/** \brief Update action lists if present. */
	void UpdateActionLists();
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu(igdeMenuCascade &contextMenu);
	/*@}*/
	
	
	
private:
	void pUpdateText();
};

#endif
