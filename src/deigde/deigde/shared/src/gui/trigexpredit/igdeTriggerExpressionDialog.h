/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDETRIGGEREXPRESSIONDIALOG_H_
#define _IGDETRIGGEREXPRESSIONDIALOG_H_

#include "../igdeWidgetReference.h"
#include "../dialog/igdeDialog.h"


class igdeTriggerExpressionEditor;
class igdeTriggerExpressionParser;
class igdeTriggerTargetList;


/**
 * \brief Trigger Expression Dialog.
 */
class DE_DLL_EXPORT igdeTriggerExpressionDialog : public igdeDialog{
private:
	igdeWidgetReference pEditor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeTriggerExpressionDialog( igdeEnvironment &environment, const igdeTriggerTargetList &targetList,
		igdeTriggerExpressionParser &parser, const char *title = "Edit Trigger Expression" );
	
	
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeTriggerExpressionDialog();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Expression. */
	const decString &GetExpression() const;
	
	/** \brief Set expression. */
	void SetExpression( const char *expression );
	/*@}*/
};

#endif
