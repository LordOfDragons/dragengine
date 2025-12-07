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

#ifndef _LPEWPLANGPACK_H_
#define _LPEWPLANGPACK_H_

#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class lpeLangPack;
class lpeWindowProperties;
class lpeWPLangPackListener;



/**
 * \brief Language pack properties panel.
 */
class lpeWPLangPack : public igdeContainerScroll{
private:
	lpeWindowProperties &pWindowProperties;
	lpeWPLangPackListener::Ref pListener;
	
	lpeLangPack::Ref pLangPack;
	
	igdeTextField::Ref pEditIdentifier;
	igdeTextField::Ref pEditName;
	igdeTextArea::Ref pEditDescription;
	igdeTextField::Ref pEditMissingText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	lpeWPLangPack(lpeWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up panel. */
	virtual ~lpeWPLangPack();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline lpeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Language pack or NULL. */
	inline const lpeLangPack::Ref &GetLangPack() const{ return pLangPack; }
	
	/** \brief Set language pack or NULL. */
	void SetLangPack(lpeLangPack *langpack);
	
	/** \brief Update language pack. */
	void UpdateLangPack();
	/*@}*/
};

#endif
