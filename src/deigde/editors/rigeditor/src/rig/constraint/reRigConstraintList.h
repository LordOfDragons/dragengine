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

// include only once
#ifndef _RERIGCONSTRAINTLIST_H_
#define _RERIGCONSTRAINTLIST_H_

// predefinitions
class reRigConstraint;



/**
 * @brief Rig Constraint List.
 * List of constraints.
 */
class reRigConstraintList{
private:
	reRigConstraint **pConstraints;
	int pConstraintCount;
	int pConstraintSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig constraint list. */
	reRigConstraintList();
	/** Cleans up the rig constraint list. */
	~reRigConstraintList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of constraints. */
	inline int GetConstraintCount() const{ return pConstraintCount; }
	/** Retrieves the constraint at the given index. */
	reRigConstraint *GetConstraintAt(int index) const;
	/** Retrieves the index of the constraint or -1 if not found. */
	int IndexOfConstraint(reRigConstraint::Ref constraint) const;
	/** Determines if the constraint exists. */
	bool HasConstraint(reRigConstraint::Ref constraint) const;
	/** Adds a new constraint. */
	void AddConstraint(reRigConstraint::Ref constraint);
	/** Removes the given constraint. */
	void RemoveConstraint(reRigConstraint::Ref constraint);
	/** Removes all constraints. */
	void RemoveAllConstraints();
	/*@}*/
};

// end of include only once
#endif
