/* 
 * Drag[en]gine IGDE Animator Editor
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
