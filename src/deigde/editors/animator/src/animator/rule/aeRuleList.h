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

#ifndef _AERULELIST_H_
#define _AERULELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class aeRule;



/**
 * Rule List.
 */
class aeRuleList{
private:
	decObjectOrderedSet pRules;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new rules list. */
	aeRuleList();
	/** Create a copy of a rule list. */
	aeRuleList( const aeRuleList &copy );
	/** Clean up the rule list. */
	~aeRuleList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the number of rules. */
	int GetCount() const;
	/** Retrieve the rule at the given position. */
	aeRule *GetAt( int index ) const;
	/** Retrieve the index of the given rule or -1 if not found. */
	int IndexOf( aeRule *rule ) const;
	/** Determine if a rule exists. */
	bool Has( aeRule *rule ) const;
	/** Add a rule. */
	void Add( aeRule *rule );
	/** Insert a new rule. */
	void Insert( aeRule *rule, int index );
	/** Move a rule to a new position. */
	void Move( aeRule *rule, int index );
	/** Remove a rule. */
	void Remove( aeRule *rule );
	/** Remove all rules. */
	void RemoveAll();
	
	/** Set the list from another list. */
	aeRuleList &operator=( const aeRuleList &list );
	/*@}*/
};

#endif
