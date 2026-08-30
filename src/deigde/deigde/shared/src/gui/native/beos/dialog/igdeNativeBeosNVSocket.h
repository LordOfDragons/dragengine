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

#ifndef _IGDENATIVEBEOSNVSOCKET_H_
#define _IGDENATIVEBEOSNVSOCKET_H_

#include <interface/View.h>
#include <support/SupportDefs.h>
#include <dragengine/common/string/decString.h>

/**
 * \brief Node editor connection point (socket) visualization.
 */
class igdeNativeBeosNVSocket : public BView{
public:
	/** \brief Socket type. */
	enum eSocketType {
		estInput,
		estOutput
	};
	
private:
	decString pLabel;
	eSocketType pType;
	bool pConnected;
	rgb_color pColor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create socket. */
	igdeNativeBeosNVSocket(BRect frame, const char *label, eSocketType type);
	
	/** \brief Clean up socket. */
	~igdeNativeBeosNVSocket() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Get label. */
	inline const char *GetLabel() const{ return pLabel.GetString(); }
	
	/** \brief Set label. */
	void SetLabel(const char *label);
	
	/** \brief Get type. */
	inline eSocketType GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType(eSocketType type);
	
	/** \brief Get connected state. */
	inline bool IsConnected() const{ return pConnected; }
	
	/** \brief Set connected state. */
	void SetConnected(bool connected);
	
	/** \brief Get color. */
	inline rgb_color GetColor() const{ return pColor; }
	
	/** \brief Set color. */
	void SetColor(rgb_color color);
	
	/** \brief Get socket center point for connection lines. */
	BPoint GetSocketCenter() const;
	
	/** \brief BView overrides */
	void Draw(BRect updateRect) override;
	void MouseDown(BPoint where) override;
	void MouseUp(BPoint where) override;
	/*@}*/
};

#endif
