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

#ifndef _CEWPASNIPPET_H_
#define _CEWPASNIPPET_H_

#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeButton.h>

#include "ceWPAction.h"

class ceWPTopic;
class ceCASnippet;


/**
 * \brief Snippet Conversation Action property window.
 */
class ceWPASnippet : public ceWPAction{
private:
	igdeComboBoxFilter::Ref pCBFile, pCBTopic;
	igdeCheckBox::Ref pChkCreateSideLane;
	igdeButton::Ref pBtnJumpToTopic;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPASnippet(ceWPTopic &parentPanel);
	
protected:
	/** \brief Clean up panel. */
	~ceWPASnippet() override;
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
