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

#ifndef _IGDENATIVEFOXNVSLOT_H_
#define _IGDENATIVEFOXNVSLOT_H_

#include "../foxtoolkit.h"
#include "../../../resources/igdeFont.h"


class igdeNVSlot;
class igdeGuiTheme;
class igdeNativeFoxNVSocket;


/**
 * \brief FOX toolkit Native NodeView Slot.
 */
class igdeNativeFoxNVSlot : public FXHorizontalFrame{
	FXDECLARE(igdeNativeFoxNVSlot)
protected:
	   igdeNativeFoxNVSlot();
	
public:
	enum eFoxIDs{
		ID_SELF = FXHorizontalFrame::ID_LAST,
		ID_SOCKET,
		ID_LABEL,
		ID_LAST
	};
	
private:
	igdeNVSlot *pOwner;
	igdeFont::Ref pFont;
	igdeNativeFoxNVSocket *pSocket;
	FXLabel *pLabel;
	
	FXDragType pDragTypeSocket;
	bool pIsDnd;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxNVSlot(igdeNVSlot &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxNVSlot();
	
	/** \brief Create native widget. */
	static igdeNativeFoxNVSlot* CreateNativeWidget(igdeNVSlot &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXbool canFocus() const;
	
	
	/** \brief Owner. */
	inline igdeNVSlot &GetOwner() const{return *pOwner;}
	
	
	
	/** \brief Update text from widget. */
	void UpdateText();
	
	/** \brief Update description from widget. */
	void UpdateDescription();
	
	/** \brief Update enabled from widget. */
	void UpdateEnabled();
	
	/** \brief Update color from widget. */
	void UpdateColor();
	
	/** \brief Update linked state from widget. */
	void UpdateLinkedState();
	
	
	
	/** \brief Center of slot in local window coordinates. */
	decPoint GetCenter() const;
	
	/** \brief Center of slot in owner node coordinates.. */
	decPoint GetCenterNode() const;
	
	/** \brief Center of slot in owner board coordinates.. */
	decPoint GetCenterBoard() const;
	
	/** \brief Connector position of slot in local window coordinates. */
	decPoint GetConnector() const;
	
	/** \brief Connector position of slot in owner node coordinates.. */
	decPoint GetConnectorNode() const;
	
	/** \brief Connector position of slot in owner board coordinates.. */
	decPoint GetConnectorBoard() const;
	
	
	
	static int NVSlotFlags(const igdeNVSlot &owner);
	static igdeFont *NVSlotFont(const igdeNVSlot &owner, const igdeGuiTheme &guitheme);
	static int NVSlotPadLeft(const igdeGuiTheme &guitheme);
	static int NVSlotPadRight(const igdeGuiTheme &guitheme);
	static int NVSlotPadTop(const igdeGuiTheme &guitheme);
	static int NVSlotPadBottom(const igdeGuiTheme &guitheme);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onPaint(FXObject*, FXSelector, void*);
	long onChildLayoutFlags(FXObject*, FXSelector, void*);
	
	long onSocketCommand(FXObject*, FXSelector, void*);
	long onSocketLeftMouseDown(FXObject*, FXSelector, void*);
	long onSocketMouseMoved(FXObject*, FXSelector, void*);
	long onSocketLeftMouseUp(FXObject*, FXSelector, void*);
	long onSocketDndEnter(FXObject*, FXSelector, void*);
	long onSocketDndLeave(FXObject*, FXSelector, void*);
	long onSocketDndDrop(FXObject*, FXSelector, void*);
	long onSocketDndMotion(FXObject*, FXSelector, void*);
	/*@}*/
};

typedef igdeNativeFoxNVSlot igdeNativeNVSlot;

#endif
