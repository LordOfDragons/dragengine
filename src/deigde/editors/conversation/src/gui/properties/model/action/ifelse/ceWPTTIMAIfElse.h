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

#ifndef _CEWPTTIMAIFELSE_H_
#define _CEWPTTIMAIFELSE_H_

#include "../ceWPTTIMAction.h"
#include "../../../../../conversation/action/ceCAIfElse.h"

class ceWPTTIMAIfElseIfCase;
class ceWPTTIMAIfElseElse;


/**
 * \brief Action if-else.
 */
class ceWPTTIMAIfElse : public ceWPTTIMAction{
private:
	ceWPTTIMAIfElseElse *pElse;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAIfElse(ceWindowMain &windowMain,
		ceConversation &conversation, ceCAIfElse *action);
	
protected:
	/** \brief Clean up tree item model. */
	~ceWPTTIMAIfElse() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceCAIfElse *GetActionIfElse() const{ return (ceCAIfElse*)GetAction(); }
	
	/** \brief Model with if-case or \em NULL. */
	ceWPTTIMAIfElseIfCase *GetIfCaseChild(ceCAIfElseCase *ifCase) const;
	
	/** \brief Model with else-case or \em NULL. */
	inline ceWPTTIMAIfElseElse *GetElseChild() const{ return pElse; }
	
	/** \brief Update action. */
	void Update() override;
	
	/** \brief Update action lists if present. */
	void UpdateActionLists() override;
	
	/** \brief Expanded state changed. */
	void OnExpandedChanged() override;
	
	/** \brief User requests context menu for selected item. */
	void OnContextMenu(igdeMenuCascade &contextMenu) override;
	/*@}*/
	
	
	
private:
	void pUpdateIfCases();
	void pUpdateElseCase();
};

#endif
