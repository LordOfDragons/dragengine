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

#ifndef _AECLIPBOARDDATARULE_H_
#define _AECLIPBOARDDATARULE_H_

#include "../animator/rule/aeRuleList.h"

#include <deigde/clipboard/igdeClipboardData.h>



/**
 * Rule clipboard data.
 */
class aeClipboardDataRule : public igdeClipboardData{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<aeClipboardDataRule> Ref;
	
	/** Type name. */
	static const char * const TYPE_NAME;
	
	
	
private:
	aeRuleList pRules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new clipboard data. */
	aeClipboardDataRule( aeRule *rule );
	
	/** Create a new clipboard data. */
	aeClipboardDataRule( const aeRuleList &rules );
	
protected:
	/**
	 * Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~aeClipboardDataRule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Rules. */
	inline const aeRuleList &GetRules() const{ return pRules; }
	/*@}*/
};

#endif
