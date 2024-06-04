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

#ifndef _CEWDSLANEGESTURE_H_
#define _CEWDSLANEGESTURE_H_

#include "ceWDSLane.h"


/**
 * \brief Dope sheet gestures lane.
 */
class ceWDSLaneGesture : public ceWDSLane{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dope sheet lane. */
	ceWDSLaneGesture( ceWindowDopeSheet &dopeSheet, int index,
		const char *label, const char *description );
	
protected:
	/** \brief Clean up dope sheet lane. */
	virtual ~ceWDSLaneGesture();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Strip list. */
	virtual const ceStripList &GetStripList() const;
	
	/** \brief Fill ID list. */
	virtual void FillIDList( decStringList &list );
	
	/** Default duration. */
	virtual float DefaultDuration( const decString &id );
	
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
