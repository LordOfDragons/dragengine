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
#ifndef _AEURULEIKSETSOLVERBONE_H_
#define _AEURULEIKSETSOLVERBONE_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class aeRuleInverseKinematic;



/**
 * Undo set rule inverse kinematic solver bone name.
 *
 * Undo to set the bone name of a inverse kinematic rule.
 */
class aeURuleIKSetSolverBone : public igdeUndo{
private:
	aeRuleInverseKinematic *pRule;
	
	decString pOldName;
	decString pNewName;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	aeURuleIKSetSolverBone( aeRuleInverseKinematic *rule, const char *newName );
protected:
	/** Clean up undo. */
	virtual ~aeURuleIKSetSolverBone();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	/** Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
