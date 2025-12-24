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

#ifndef _SEWINDOWPROPERTIES_H_
#define _SEWINDOWPROPERTIES_H_

#include "seWPMapped.h"
#include "seWPTexture.h"
#include "seWPNode.h"
#include "dynamicskin/seWPDynamicSkin.h"
#include "seWPView.h"
#include "seWPUndoHistory.h"

#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/properties/igdeWPUndoHistory.h>

class seSkin;
class seWindowMain;



/**
 * \brief Properties panel.
 */
class seWindowProperties : public igdeTabBook{
public:
	typedef deTObjectReference<seWindowProperties> Ref;
	
private:
	seWindowMain &pWindowMain;
	
	seWPMapped::Ref pPanelMapped;
	seWPTexture::Ref pPanelTexture;
	seWPNode::Ref pPanelNode;
	seWPDynamicSkin::Ref pPanelDynamicSkin;
	seWPView::Ref pPanelView;
	seWPUndoHistory::Ref pPanelUndoHistory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWindowProperties(seWindowMain &windowMain);
	
protected:
	/** \brief Cleanup window. */
	virtual ~seWindowProperties();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Set skin. */
	void SetSkin(seSkin *skin);
	
	/** \brief Skin path changed. */
	void OnSkinPathChanged();
	/*@}*/
};

#endif
