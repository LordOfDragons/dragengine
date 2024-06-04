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
