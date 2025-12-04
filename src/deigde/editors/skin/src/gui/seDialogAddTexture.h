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

#ifndef _SEDIALOGADDTEXTURE_H_
#define _SEDIALOGADDTEXTURE_H_

#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/dialog/igdeDialog.h>

class seWindowMain;



/**
 * \brief Add Texture Dialog.
 */
class seDialogAddTexture : public igdeDialog{
private:
	seWindowMain &pWindowMain;
	igdeListBox::Ref pListModelTextureNames;
	igdeTextField::Ref pEditTextureName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	seDialogAddTexture( seWindowMain &windowMain );
	
protected:
	/** \brief Clean up dialog. */
	virtual ~seDialogAddTexture();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Texture name. */
	const decString &GetTextureName() const;
	
	/** \brief Set texture name. */
	void SetTextureName( const char *name );
	/*@}*/
	
	
	
protected:
	void pUpdateModelTextureList();
};

#endif
