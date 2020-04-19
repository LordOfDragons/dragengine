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

#ifndef _DEOGLSHADERASTHELPER_H_
#define _DEOGLSHADERASTHELPER_H_

// Helper macros to increase readability of code generation
/////////////////////////////////////////////////////////////

#define NODE_FREE( node ) \
	node->FreeReference();\
	node = NULL

#define NODE_FREE2( node, node2 ) \
	node->FreeReference();\
	node = NULL;\
	node2->FreeReference();\
	node2 = NULL

#define NODE_FREE3( node, node2, node3 ) \
	node->FreeReference();\
	node = NULL;\
	node2->FreeReference();\
	node2 = NULL;\
	node3->FreeReference();\
	node3 = NULL

#define LIST_ADD_NODE( list, node ) \
	list.Add( node );\
	node->FreeReference();\
	node = NULL

#define NEW_NODE_FUNCCALL( name, node1 ) \
	new deoglSASTNodeFunctionCall( name, node1 );\
	node1->FreeReference();\
	node1 = NULL

#define NEW_NODE_FUNCCALL2( name, node1, node2 ) \
	new deoglSASTNodeFunctionCall( name, node1, node2 );\
	node1->FreeReference();\
	node1 = NULL;\
	node2->FreeReference();\
	node2 = NULL

#define NEW_NODE_FUNCCALL3( name, node1, node2, node3 ) \
	new deoglSASTNodeFunctionCall( name, node1, node2, node3 );\
	node1->FreeReference();\
	node1 = NULL;\
	node2->FreeReference();\
	node2 = NULL;\
	node3->FreeReference();\
	node3 = NULL

#define NEW_NODE_FUNCCALL4( name, node1, node2, node3, node4 ) \
	new deoglSASTNodeFunctionCall( name, node1, node2, node3, node4 );\
	node1->FreeReference();\
	node1 = NULL;\
	node2->FreeReference();\
	node2 = NULL;\
	node3->FreeReference();\
	node3 = NULL;\
	node4->FreeReference();\
	node4 = NULL

#define NEW_NODE_OP( op, node1 ) \
	new deoglSASTNodeOperator( op, node1 );\
	node1->FreeReference();\
	node1 = NULL

#define NEW_NODE_OP2( node1, op, node2 ) \
	new deoglSASTNodeOperator( op, node1, node2 );\
	node1->FreeReference();\
	node1 = NULL;\
	node2->FreeReference();\
	node2 = NULL

#define NEW_NODE_DECLVAR( datatype, name, nodeinit )\
	new deoglSASTNodeDeclareVariable( name, datatype, nodeinit );\
	nodeinit->FreeReference();\
	nodeinit = NULL

#define NODE_CATCH_CLEANUP( node ) \
	if( node ) node->FreeReference()

#define NEW_NODE_IFELSE( nodeCondition ) \
	new deoglSASTNodeIfElse( nodeCondition );\
	nodeCondition->FreeReference();\
	nodeCondition = NULL;

#define IF_ADD_NODE( nodeIfElse, node ) \
	nodeIfElse->GetNodeIf()->GetNodeList().Add( node );\
	node->FreeReference();\
	node = NULL

#define ELSE_ADD_NODE( nodeIfElse, node ) \
	nodeIfElse->GetNodeElse()->GetNodeList().Add( node );\
	node->FreeReference();\
	node = NULL

#endif
