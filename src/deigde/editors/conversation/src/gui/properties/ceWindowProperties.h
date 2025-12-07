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

#include <deigde/gui/igdeTabBook.h>

class ceWindowMain;
class ceConversation;
class ceWPTopic;
class ceWPConversation;
class ceWPTopic;
class ceWPView;
class ceWPUndoHistory;


/**
 * \brief Properties Panel.
 */
class ceWindowProperties : public igdeTabBook{
private:
	ceWindowMain &pWindowMain;
	
	ceWPConversation *pPanelConversation;
	ceWPTopic *pPanelTopic;
	ceWPView *pPanelView;
	ceWPUndoHistory *pPanelUndoHistory;
	
	
	
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
	ceWPTopic &GetPanelTopic() const;
	
	/** \brief Set conversation. */
	void SetConversation(ceConversation *conversation);
	
	/** \brief Conversation path changed. */
	void OnConversationPathChanged();
	/*@}*/
};

#endif
