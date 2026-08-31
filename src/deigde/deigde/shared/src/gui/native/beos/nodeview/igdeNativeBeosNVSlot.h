/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDENATIBESOSNVSLOT_H_
#define _IGDENATIBESOSNVSLOT_H_

#include <interface/View.h>
#include "../../../nodeview/igdeNVSlot.h"
#include "../../../resources/igdeFont.h"

class igdeNVSlot;
class igdeGuiTheme;
class igdeNativeBeosNVSocket;
class igdeNativeBeosNVBoard;


/**
 * \brief BeOS native NodeView slot widget.
 */
class igdeNativeBeosNVSlot : public BView, public igdeNVSlot::cNativeNVSlot{
private:
	igdeNVSlot *pOwner;
	igdeFont::Ref pFont;
	igdeNativeBeosNVSocket *pSocket;
	bool pIsDnd;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeosNVSlot(igdeNVSlot &owner, BView *parent, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeBeosNVSlot() override;
	
	/** \brief Create native widget. */
	static igdeNativeBeosNVSlot* CreateNativeWidget(igdeNVSlot &owner);
	
	/** \brief Post create native widget. */
	void PostCreateNativeWidget() override;
	
	/** \brief Destroy native widget. */
	void DestroyNativeWidget() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Owner. */
	inline igdeNVSlot &GetOwner() const{ return *pOwner; }
	
	/** \brief Socket widget. */
	inline igdeNativeBeosNVSocket *GetSocket() const{ return pSocket; }
	
	/** \brief Get owner NVBoard native widget or nullptr. */
	igdeNativeBeosNVBoard *GetNativeBoard() const;
	
	
	
	void UpdateText() override;
	void UpdateDescription() override;
	void UpdateEnabled() override;
	void UpdateColor() override;
	void UpdateLinkedState() override;
	
	decPoint GetCenter() const override;
	decPoint GetCenterNode() const override;
	decPoint GetCenterBoard() const override;
	decPoint GetConnector() const override;
	decPoint GetConnectorNode() const override;
	decPoint GetConnectorBoard() const override;
	
	
	
	static igdeFont *NVSlotFont(const igdeNVSlot &owner, const igdeGuiTheme &guitheme);
	/*@}*/
	
	
	
	/** \name BView overrides */
	/*@{*/
	void Draw(BRect updateRect) override;
	void MouseDown(BPoint where) override;
	void MouseMoved(BPoint where, uint32 code, const BMessage *message) override;
	void MouseUp(BPoint where) override;
	BSize PreferredSize() override;
	/*@}*/
};

typedef igdeNativeBeosNVSlot igdeNativeNVSlot;

#endif
