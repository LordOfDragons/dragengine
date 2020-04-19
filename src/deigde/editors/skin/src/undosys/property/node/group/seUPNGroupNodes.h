/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEUPNGROUPNODES_H_
#define _SEUPNGROUPNODES_H_

#include "../../../../skin/property/node/sePropertyNodeList.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class sePropertyNodeGroup;



/**
 * \brief Undo action group property nodes.
 */
class seUPNGroupNodes : public igdeUndo{
private:
	struct sNode{
		sePropertyNode *node;
		decPoint3 position;
		int index;
	};
	
	sePropertyNodeGroup *pParentGroup;
	sePropertyNodeGroup *pNodeGroup;
	int pNodeCount;
	sNode *pNodes;
	decVector pOffset;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	seUPNGroupNodes( const sePropertyNodeList &nodes );
	
protected:
	/** \brief Clean up undo. */
	virtual ~seUPNGroupNodes();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
