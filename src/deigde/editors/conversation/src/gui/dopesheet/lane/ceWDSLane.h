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

#ifndef _CEWDSLANE_H_
#define _CEWDSLANE_H_

#include "../../../conversation/strip/ceStripList.h"

#include <deigde/gui/event/igdeMouseKeyListenerReference.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/canvas/deCanvasViewReference.h>
#include <dragengine/resources/canvas/deCanvasPaintReference.h>
#include <dragengine/resources/canvas/deCanvasTextReference.h>

class ceWindowDopeSheet;
class ceUCAASpeakStripSetPause;
class ceUCAASpeakStripSetDuration;
class ceUCAASpeakStripsScale;
class ceStrip;
class decStringList;

class igdeMenuCascade;
class igdeUndo;


/**
 * \brief Dope sheet lane base class.
 */
class ceWDSLane : public deObject{
private:
	class cStrip : public deObject{
	public:
		deCanvasViewReference handlePause;
		deCanvasPaintReference handlePauseBg;
		deCanvasViewReference handleDuration;
		deCanvasPaintReference handleDurationBg;
		deCanvasPaintReference stripIdBg;
		deCanvasTextReference stripId;
		cStrip();
		
	protected:
		virtual ~cStrip();
	};
	
	
	ceWindowDopeSheet &pWindow;
	const int pIndex;
	decString pLabel;
	decString pDescription;
	
	decObjectList pStrips;
	decObjectList pTimeLines;
	
	deCanvasViewReference pCanvas;
	deCanvasViewReference pCanvasPanelSheet;
	deCanvasPaintReference pCanvasBar;
	deCanvasPaintReference pCanvasBarSelection;
	deCanvasViewReference pCanvasHandles;
	
	int pBarHeight;
	igdeMouseKeyListenerReference pMouseKeyListener;
	int pSelectionFrom;
	int pSelectionTo;
	
	const ceStripList pEmptyList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dope sheet lane. */
	ceWDSLane( ceWindowDopeSheet &dopeSheet, int index, const char *label, const char *description );
	
protected:
	/** \brief Clean up dope sheet lane. */
	virtual ~ceWDSLane();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Dope sheet. */
	inline ceWindowDopeSheet &GetWindow() const{ return pWindow; }
	
	/** \brief Index of lane in dope sheet. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Label. */
	inline const decString &GetLabel() const{ return pLabel; }
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Canvas. */
	inline deCanvasView *GetCanvas() const{ return pCanvas; }
	
	/** \brief Mouse listener. */
	inline igdeMouseKeyListener *GetMouseKeyListener() const{ return pMouseKeyListener; }
	
	
	
	/**
	 * \brief Retrieves the index of the strip entry matching the given time.
	 * 
	 * If not found returns -1. if position is past the left or right end returns -1.
	 */
	int GetStripAt( int x ) const;
	
	/**
	 * \brief Retrieves the index of insert strip .
	 * 
	 * Returns index between 0 and count.
	 */
	int GetInsertStripAt( int x ) const;
	
	/** \brief Retrieves the index of the strip pause point matching the given time or -1 if not found. */
	int GetStripPauseAt( int x ) const;
	
	/** \brief Retrieves the index of the strip duration point matching the given time or -1 if not found. */
	int GetStripDurationAt( int x ) const;
	
	/** \brief Retrieves the start position of a strip. */
	float GetStripStartFor( int index ) const;
	
	/** \brief Retrieves the pause position of a strip. */
	float GetStripPauseFor( int index ) const;
	
	/** \brief Maximum time of line. */
	float GetMaximumLineTime() const;
	
	/** \brief Strip selection from index or -1 if no selection. */
	inline int GetSelectionFrom() const{ return pSelectionFrom; }
	
	/** \brief Strip selection to index or -1 if no selection. */
	inline int GetSelectionTo() const{ return pSelectionTo; }
	
	/** \brief Set strip selection. */
	void SetSelection( int from, int to );
	
	
	
	/** \brief Dope sheet size changed. */
	void OnResize();
	
	/** \brief Action changed. */
	void OnActionChanged();
	
	/** \brief Current time changed. */
	void OnTimeChanged();
	
	
	
	/** \brief Create canvas. */
	void CreateCanvas();
	
	/** \brief Update canvas. */
	void UpdateCanvas();
	
	/** \brief Rebuild canvas. */
	void RebuildCanvas();
	
	/** \brief Edit strip. */
	void EditStrip( ceStrip *strip );
	
	
	
	/** \brief Context menu if appropriate. */
	virtual void OnContextMenu( igdeMenuCascade &menu, const decPoint &position );
	
	/** \brief Strip list. */
	virtual const ceStripList &GetStripList() const = 0;
	
	/** \brief Fill ID list. */
	virtual void FillIDList( decStringList &list ) = 0;
	
	/** \brief Create add strip undo action. */
	virtual igdeUndo *UndoStripAdd( ceStrip *strip, int index ) = 0;
	
	/** \brief Create remove strip undo action. */
	virtual igdeUndo *UndoStripRemove( ceStrip *strip ) = 0;
	
	/** \brief Create remove all strip undo action. */
	virtual igdeUndo *UndoStripRemoveAll() = 0;
	
	/** \brief Create replace strip undo action. */
	virtual igdeUndo *UndoStripReplace( ceStrip *strip, ceStrip *withStrip ) = 0;
	
	/** \brief Create move strip undo action. */
	virtual igdeUndo *UndoStripMove( ceStrip *strip, int toIndex ) = 0;
	
	/** \brief Create strip set pause undo action. */
	virtual ceUCAASpeakStripSetPause *UndoStripSetPause( ceStrip *strip, float pause ) = 0;
	
	/** \brief Create strip set duration undo action. */
	virtual ceUCAASpeakStripSetDuration *UndoStripSetDuration( ceStrip *strip, float duration ) = 0;
	
	/** \brief Create scale strips undo action. */
	virtual ceUCAASpeakStripsScale *UndoScaleStrips() = 0;
	
	
	
protected:
	inline const ceStripList &GetEmptyList() const{ return pEmptyList; }
	void FillIDListLookAt( decStringList &list );
	void CreateHandle( deCanvasViewReference &canvas, deCanvasPaintReference &canvasBg, const decPoint &size );
	/*@}*/
};

#endif
