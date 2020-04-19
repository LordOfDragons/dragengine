/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSASTNODEOPERATOR_H_
#define _DEOGLSASTNODEOPERATOR_H_

#include "deoglSASTNode.h"



/**
 * @brief Shader Abstract Syntax Tree Node Operator.
 */
class deoglSASTNodeOperator : public deoglSASTNode{
public:
	/** Operators. */
	enum eOperators{
		/** -A. */
		eoNegate,
		/** ! A. */
		eoNot,
		/** A = B. */
		eoAssign,
		/** A + B. */
		eoAdd,
		/** A - B. */
		eoSubtract,
		/** A * B. */
		eoMultiply,
		/** A / B. */
		eoDivide,
		/** A < B. */
		eoLessThan,
		/** A > B. */
		eoGreaterThan,
		/** A <= B. */
		eoLessThanOrEqual,
		/** A >= B. */
		eoGreaterThanOrEqual,
		/** A += B. */
		eoAssignAdd,
		/** A -= B. */
		eoAssignSubtract,
		/** A *= B. */
		eoAssignMultiply,
		/** A /= B. */
		eoAssignDivide,
		
		EO_COUNT
	};
	
private:
	int pOperator;
	deoglSASTNode *pNodeOperand1;
	deoglSASTNode *pNodeOperand2;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstract syntax tree node. */
	deoglSASTNodeOperator();
	deoglSASTNodeOperator( int op );
	deoglSASTNodeOperator( int op, deoglSASTNode *nodeOperand1 );
	deoglSASTNodeOperator( int op, deoglSASTNode *nodeOperand1, deoglSASTNode *nodeOperand2 );
	/** Cleans up the shader abstract syntax tree node. */
	virtual ~deoglSASTNodeOperator();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the operator. */
	inline int GetOperator() const{ return pOperator; }
	/** Sets the operator. */
	void SetOperator( int op );
	/** Retrieves the first operand node or NULL if not used. */
	inline deoglSASTNode *GetNodeOperand1() const{ return pNodeOperand1; }
	/** Sets the first operand node or NULL if not used. */
	void SetNodeOperand1( deoglSASTNode *node );
	/** Retrieves the second operand node or NULL if not used. */
	inline deoglSASTNode *GetNodeOperand2() const{ return pNodeOperand2; }
	/** Sets the second operand node or NULL if not used. */
	void SetNodeOperand2( deoglSASTNode *node );
	
	/** Generate source code. */
	virtual void GenerateSourceCode( deoglSASTGenerateCode &gencode );
	/*@}*/
};

#endif
