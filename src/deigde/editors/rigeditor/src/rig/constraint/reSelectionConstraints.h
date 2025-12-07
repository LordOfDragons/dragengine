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
#ifndef _RESELECTIONCONSTRAINTS_H_
#define _RESELECTIONCONSTRAINTS_H_

// predefinitions
class reRig;
class reRigConstraint;
class reRigConstraintList;
class deColliderVolume;



/**
 * @brief Rig Constraint Selection.
 * Stores a list of selected rig constraints as well as the active rig constraint.
 */
class reSelectionConstraints{
private:
	reRig *pRig;
	
	reRigConstraint **pConstraints;
	int pConstraintCount, pConstraintSize;
	reRigConstraint *pActiveConstraint;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig constraint selection. */
	reSelectionConstraints(reRig *rig);
	/** Cleans up the rig constraint selection. */
	~reSelectionConstraints();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent rig. */
	inline reRig *GetParentRig() const{return pRig;}
	
	/** Retrieves the number of selected constraints. */
	inline int GetConstraintCount() const{return pConstraintCount;}
	/** Retrieves the constraint at the given index. */
	reRigConstraint *GetConstraintAt(int index) const;
	/** Determines if the given constraint exists. */
	bool HasConstraint(reRigConstraint *constraint) const;
	/** Retrieves the index of the given constraint or -1 if not found. */
	int IndexOfConstraint(reRigConstraint *constraint) const;
	/** Retrieves the index of the constraint with the given collider or -1 if not found. */
	int IndexOfConstraintWith(deColliderVolume *collider) const;
	/** Adds a constraint if not existing already. */
	void AddConstraint(reRigConstraint *constraint);
	/** Removes a constraint if existing. */
	void RemoveConstraint(reRigConstraint *constraint);
	/** Removes all constraints. */
	void RemoveAllConstraints();
	
	/** Retrieves the active constraint or NULL. */
	inline reRigConstraint *GetActiveConstraint() const{return pActiveConstraint;}
	/** Determines if an active constraint exists. */
	bool HasActiveConstraint() const;
	/** Sets the acitve constraint or NULL. */
	void SetActiveConstraint(reRigConstraint *constraint);
	/** Activates the next constraint in the list. */
	//void ActivateNextConstraint();
	
	/** Removes all constraints and sets the active constraint to NULL. */
	void Reset();
	
	/** Adds all selected and visible constraints to the given list. */
	void AddVisibleConstraintsTo(reRigConstraintList &list) const;
	/*@}*/
};

// end of include only once
#endif
