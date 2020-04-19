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

#ifndef _SEUPROPERTYNODESRESIZE_H_
#define _SEUPROPERTYNODESRESIZE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>

class sePropertyNode;
class sePropertyNodeList;
class seUPropertyNodeData;



/**
 * \brief Undo action property resize nodes.
 */
class seUPropertyNodesResize : public igdeUndo{
private:
	decObjectList pNodes;
	
	decVector2 pPivot;
	decVector2 pOrigin;
	decVector2 pDistance;
	float pRotation;
	float pShearing;
	decTexMatrix2 pTransform;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	seUPropertyNodesResize( const sePropertyNodeList &nodes, const decVector2 &pivot,
		const decVector2 &origin, float rotation, float shearing );
	
protected:
	/** \brief Clean up undo. */
	virtual ~seUPropertyNodesResize();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set distance. */
	void SetDistance( const decVector2 &distance );
	
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pAddNodes( sePropertyNode *node );
};

#endif
