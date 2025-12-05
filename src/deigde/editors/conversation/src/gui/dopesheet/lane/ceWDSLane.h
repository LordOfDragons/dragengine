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

#ifndef _CEWDSLANE_H_
#define _CEWDSLANE_H_

#include "../../../conversation/strip/ceStripList.h"

#include <deigde/gui/event/igdeMouseKeyListener.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasText.h>

class ceWindowDopeSheet;
class ceUCAASpeakStripSetPause;
class ceUCAASpeakStripSetDuration;
class ceUCAASpeakStripsScale;
class ceStrip;
class decStringList;

class igdeMenuCascade;
class igdeUndo;


/**
 * Dope sheet lane base class.
 */
class ceWDSLane : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceWDSLane> Ref;


private:
	class cStrip : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<cStrip> Ref;


	public:
		deCanvasView::Ref handlePause;
		deCanvasPaint::Ref handlePauseBg;
		deCanvasView::Ref handleDuration;
		deCanvasPaint::Ref handleDurationBg;
		deCanvasPaint::Ref stripIdBg;
		deCanvasText::Ref stripId;
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
	
	deCanvasView::Ref pCanvas;
	deCanvasView::Ref pCanvasPanelSheet;
	deCanvasPaint::Ref pCanvasBar;
	deCanvasPaint::Ref pCanvasBarSelection;
	deCanvasView::Ref pCanvasHandles;
	
	int pBarHeight;
	igdeMouseKeyListener::Ref pMouseKeyListener;
	int pSelectionFrom;
	int pSelectionTo;
	
	const ceStripList pEmptyList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dope sheet lane. */
	ceWDSLane( ceWindowDopeSheet &dopeSheet, int index, const char *label, const char *description );
	
protected:
	/** Clean up dope sheet lane. */
	virtual ~ceWDSLane();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Dope sheet. */
	inline ceWindowDopeSheet &GetWindow() const{ return pWindow; }
	
	/** Index of lane in dope sheet. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Label. */
	inline const decString &GetLabel() const{ return pLabel; }
	
	/** Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** Canvas. */
	inline deCanvasView *GetCanvas() const{ return pCanvas; }
	
	/** Mouse listener. */
	inline igdeMouseKeyListener *GetMouseKeyListener() const{ return pMouseKeyListener; }
	
	
	
	/**
	 * Retrieves the index of the strip entry matching the given time.
	 * 
	 * If not found returns -1. if position is past the left or right end returns -1.
	 */
	int GetStripAt( int x ) const;
	
	/**
	 * Retrieves the index of insert strip .
	 * 
	 * Returns index between 0 and count.
	 */
	int GetInsertStripAt( int x ) const;
	
	/** Retrieves the index of the strip pause point matching the given time or -1 if not found. */
	int GetStripPauseAt( int x ) const;
	
	/** Retrieves the index of the strip duration point matching the given time or -1 if not found. */
	int GetStripDurationAt( int x ) const;
	
	/** Retrieves the start position of a strip. */
	float GetStripStartFor( int index ) const;
	
	/** Retrieves the pause position of a strip. */
	float GetStripPauseFor( int index ) const;
	
	/** Maximum time of line. */
	float GetMaximumLineTime() const;
	
	/** Strip selection from index or -1 if no selection. */
	inline int GetSelectionFrom() const{ return pSelectionFrom; }
	
	/** Strip selection to index or -1 if no selection. */
	inline int GetSelectionTo() const{ return pSelectionTo; }
	
	/** Set strip selection. */
	void SetSelection( int from, int to );
	
	
	
	/** Dope sheet size changed. */
	void OnResize();
	
	/** Action changed. */
	void OnActionChanged();
	
	/** Current time changed. */
	void OnTimeChanged();
	
	
	
	/** Create canvas. */
	void CreateCanvas();
	
	/** Update canvas. */
	void UpdateCanvas();
	
	/** Rebuild canvas. */
	void RebuildCanvas();
	
	/** Edit strip. */
	void EditStrip( ceStrip *strip );
	
	
	
	/** Context menu if appropriate. */
	virtual void OnContextMenu( igdeMenuCascade &menu, const decPoint &position );
	
	/** Strip list. */
	virtual const ceStripList &GetStripList() const = 0;
	
	/** Fill ID list. */
	virtual void FillIDList( decStringList &list ) = 0;
	
	/** Default duration. */
	virtual float DefaultDuration( const decString &id );
	
	/** Create add strip undo action. */
	virtual igdeUndo *UndoStripAdd( ceStrip *strip, int index ) = 0;
	
	/** Create remove strip undo action. */
	virtual igdeUndo *UndoStripRemove( ceStrip *strip ) = 0;
	
	/** Create remove all strip undo action. */
	virtual igdeUndo *UndoStripRemoveAll() = 0;
	
	/** Create replace strip undo action. */
	virtual igdeUndo *UndoStripReplace( ceStrip *strip, ceStrip *withStrip ) = 0;
	
	/** Create move strip undo action. */
	virtual igdeUndo *UndoStripMove( ceStrip *strip, int toIndex ) = 0;
	
	/** Create strip set pause undo action. */
	virtual ceUCAASpeakStripSetPause *UndoStripSetPause( ceStrip *strip, float pause ) = 0;
	
	/** Create strip set duration undo action. */
	virtual ceUCAASpeakStripSetDuration *UndoStripSetDuration( ceStrip *strip, float duration ) = 0;
	
	/** Create scale strips undo action. */
	virtual ceUCAASpeakStripsScale *UndoScaleStrips() = 0;
	
	
	
protected:
	inline const ceStripList &GetEmptyList() const{ return pEmptyList; }
	void FillIDListLookAt( decStringList &list );
	void CreateHandle( deCanvasView::Ref &canvas, deCanvasPaint::Ref &canvasBg, const decPoint &size );
	/*@}*/
};

#endif
