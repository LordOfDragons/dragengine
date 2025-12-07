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

#ifndef _IGDENATIVEFOXNVSOCKET_H_
#define _IGDENATIVEFOXNVSOCKET_H_

#include "../foxtoolkit.h"


class igdeGuiTheme;


/**
 * \brief FOX toolkit Native NodeView Socket.
 */
class igdeNativeFoxNVSocket : public FXFrame{
	FXDECLARE(igdeNativeFoxNVSocket)
protected:
	   igdeNativeFoxNVSocket();
	
private:
	bool pChecked;
	int pRadius;
	FXColor pColorFrame;
	FXColor pColorFillChecked;
	FXColor pColorFillUnchecked;
	FXBitmap *pWindowShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxNVSocket(FXComposite *parent, FXObject *target, int selector,
		int flags, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxNVSocket();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();
	virtual FXbool canFocus() const;
	
	
	/** \brief Checked. */
	inline bool GetChecked() const{return pChecked;}
	
	/** \brief Set checked. */
	void SetChecked(bool checked);
	
	/** \brief Frame color. */
	inline FXColor GetColorFrame() const{return pColorFrame;}
	
	/** \brief Set frame color. */
	void SetColorFrame(FXColor color);
	
	/** \brief Checked fill color. */
	inline FXColor GetColorFillChecked() const{return pColorFillChecked;}
	
	/** \brief Set checked fill color. */
	void SetColorFillChecked(FXColor color);
	
	/** \brief Unchecked fill color. */
	inline FXColor GetColorFillUnchecked() const{return pColorFillUnchecked;}
	
	/** \brief Set unchecked fill color. */
	void SetColorFillUnchecked(FXColor color);
	
	
	
	/** \brief Center position. */
	decPoint GetCenter() const;
	
	/** \brief Left border position. */
	decPoint GetBorderLeft() const;
	
	/** \brief Right border position. */
	decPoint GetBorderRight() const;
	
	/** \brief Is point inside socket. */
	bool IsInsideSocket(const decPoint &point) const;
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onResize(FXObject*, FXSelector, void*);
	long onPaint(FXObject*, FXSelector, void*);
	/*@}*/
	
	
	
protected:
	void pUpdateWindowShape();
};

#endif
