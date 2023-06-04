/* 
 * Drag[en]gine IGDE Rig Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
