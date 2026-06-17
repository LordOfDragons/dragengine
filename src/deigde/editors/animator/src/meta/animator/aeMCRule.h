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

#ifndef _AEMCRULE_H_
#define _AEMCRULE_H_

#include <deigde/meta/igdeMetaContext.h>

class aeWindowMain;
class aeRule;


/**
 * Rule meta context.
 */
class aeMCRule : public igdeMetaContext{
public:
	using Ref = deTObjectReference<aeMCRule>;
	
private:
	aeWindowMain &pWindowMain;
	aeRule *pRule;
	deTObjectReference<aeRule> pGuardRule;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule meta context. */
	explicit aeMCRule(aeWindowMain &windowMain, aeRule *rule);
	
protected:
	/**
	 * Clean up object.
	 */
	virtual ~aeMCRule() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Window main. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Rule. */
	inline aeRule *GetRule() const{ return pRule; }
	
	/** Rule reference. */
	aeRule &GetRuleRef() const;
	
	/** Capture context. */
	Ref Capture() const;
	
	/** Undo system or nullptr to apply actions immediately. */
	igdeUndoSystem *GetUndoSystem() const override;
	
	/** \brief Clipboard or nullptr if not supported. */
	igdeClipboard *GetClipboard() const override;
	/*@}*/
};

#endif
