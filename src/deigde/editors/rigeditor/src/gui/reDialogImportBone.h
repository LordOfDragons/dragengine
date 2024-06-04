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

#ifndef _REDIALOGIMPORTBONE_H_
#define _REDIALOGIMPORTBONE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/dialog/igdeDialog.h>

class reWindowMain;


/**
 * \brief Dialog for importing bones.
 */
class reDialogImportBone : public igdeDialog{
private:
	reWindowMain &pWindowMain;
	
	igdeTextFieldReference pEditPath;
	igdeButtonReference pBtnPath;
	igdeTextFieldReference pEditScaling;
	igdeCheckBoxReference pChkImportBoneProperties;
	igdeCheckBoxReference pChkImportShapes;
	igdeCheckBoxReference pChkImportConstraints;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	reDialogImportBone( reWindowMain &windowMain );
	
protected:
	/** \brief Clean up dialog. */
	virtual ~reDialogImportBone();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Path. */
	const decString &GetPath() const;
	
	/** \brief Set path. */
	void SetPath( const char *path );
	
	/** \brief Scaling. */
	float GetScaling() const;
	
	/** \brief Set scaling. */
	void SetScaling( float scaling );
	
	/** \brief Bone properties are imported. */
	bool GetImportBoneProperties() const;
	
	/** \brief Set if bone properties are imported. */
	void SetImportBoneProperties( bool import );
	
	/** \brief Shapes are imported. */
	bool GetImportShapes() const;
	
	/** \brief Set if shapes are imported. */
	void SetImportShapes( bool import );
	
	/** \brief Constraints are imported. */
	bool GetImportConstraints() const;
	
	/** \brief Set if constraints are imported. */
	void SetImportConstraints( bool import );
	
	
	
	/** \brief Accept dialog. */
	virtual bool Accept();
	/*@}*/
};

#endif
