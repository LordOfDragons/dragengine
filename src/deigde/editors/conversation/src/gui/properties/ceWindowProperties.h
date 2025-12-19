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

#ifndef _CEWINDOWPROPERTIES_H_
#define _CEWINDOWPROPERTIES_H_

#include "ceWPTopic.h"
#include "ceWPConversation.h"
#include "ceWPView.h"
#include "ceWPUndoHistory.h"

#include <deigde/gui/igdeTabBook.h>

class ceWindowMain;
class ceConversation;


/**
 * \brief Properties Panel.
 */
class ceWindowProperties : public igdeTabBook{
public:
	typedef deTObjectReference<ceWindowProperties> Ref;
	
	
private:
	ceWindowMain &pWindowMain;
	
	ceWPConversation::Ref pPanelConversation;
	ceWPTopic::Ref pPanelTopic;
	ceWPView::Ref pPanelView;
	ceWPUndoHistory::Ref pPanelUndoHistory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWindowProperties(ceWindowMain &windowMain);
	
protected:
	/** \brief Clean up window. */
	virtual ~ceWindowProperties();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline ceWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Topic panel. */
	inline const ceWPTopic::Ref &GetPanelTopic() const{ return pPanelTopic; }
	
	/** \brief Set conversation. */
	void SetConversation(ceConversation *conversation);
	
	/** \brief Conversation path changed. */
	void OnConversationPathChanged();
	/*@}*/
};

#endif
