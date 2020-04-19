/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEWDSLANEHEADLOOKAT_H_
#define _CEWDSLANEHEADLOOKAT_H_

#include "ceWDSLane.h"


/**
 * \brief Dope sheet head look-at lane.
 */
class ceWDSLaneHeadLookAt : public ceWDSLane{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dope sheet lane. */
	ceWDSLaneHeadLookAt( ceWindowDopeSheet &dopeSheet, int index,
		const char *label, const char *description );
	
protected:
	/** \brief Clean up dope sheet lane. */
	virtual ~ceWDSLaneHeadLookAt();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Context menu if appropriate. */
	virtual void OnContextMenu( igdeMenuCascade &menu, const decPoint &position );
	
	
	
	/** \brief Strip list. */
	virtual const ceStripList &GetStripList() const;
	
	/** \brief Fill ID list. */
	virtual void FillIDList( decStringList &list );
	
	/** \brief Create add strip undo action. */
	virtual igdeUndo *UndoStripAdd( ceStrip *strip, int index );
	
	/** \brief Create remove strip undo action. */
	virtual igdeUndo *UndoStripRemove( ceStrip *strip );
	
	/** \brief Create remove all strip undo action. */
	virtual igdeUndo *UndoStripRemoveAll();
	
	/** \brief Create replace strip undo action. */
	virtual igdeUndo *UndoStripReplace( ceStrip *strip, ceStrip *withStrip );
	
	/** \brief Create move strip undo action. */
	virtual igdeUndo *UndoStripMove( ceStrip *strip, int toIndex );
	
	/** \brief Create strip set pause undo action. */
	virtual ceUCAASpeakStripSetPause *UndoStripSetPause( ceStrip *strip, float pause );
	
	/** \brief Create strip set duration undo action. */
	virtual ceUCAASpeakStripSetDuration *UndoStripSetDuration( ceStrip *strip, float duration );
	
	/** \brief Create scale strips undo action. */
	virtual ceUCAASpeakStripsScale *UndoScaleStrips();
	/*@}*/
};

#endif
