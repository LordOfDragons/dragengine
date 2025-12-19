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

#ifndef _CEWDSLANEEYESLOOKAT_H_
#define _CEWDSLANEEYESLOOKAT_H_

#include "ceWDSLane.h"


/**
 * \brief Dope sheet eyes look-at lane.
 */
class ceWDSLaneEyesLookAt : public ceWDSLane{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceWDSLaneEyesLookAt> Ref;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dope sheet lane. */
	ceWDSLaneEyesLookAt(ceWindowDopeSheet &dopeSheet, int index,
		const char *label, const char *description);
	
protected:
	/** \brief Clean up dope sheet lane. */
	~ceWDSLaneEyesLookAt() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Context menu if appropriate. */
	virtual void OnContextMenu(igdeMenuCascade &menu, const decPoint &position);
	
	
	
	/** \brief Strip list. */
	const ceStrip::List &GetStripList() const override;
	
	/** \brief Fill ID list. */
	void FillIDList(decStringList &list) override;
	
	/** \brief Create add strip undo action. */
	igdeUndo::Ref UndoStripAdd(ceStrip *strip, int index) override;
	
	/** \brief Create remove strip undo action. */
	igdeUndo::Ref UndoStripRemove(ceStrip *strip) override;
	
	/** \brief Create remove all strip undo action. */
	igdeUndo::Ref UndoStripRemoveAll() override;
	
	/** \brief Create replace strip undo action. */
	igdeUndo::Ref UndoStripReplace(ceStrip *strip, ceStrip *withStrip) override;
	
	/** \brief Create move strip undo action. */
	igdeUndo::Ref UndoStripMove(ceStrip *strip, int toIndex) override;
	
	/** \brief Create strip set pause undo action. */
	ceUCAASpeakStripSetPause::Ref UndoStripSetPause(ceStrip *strip, float pause) override;
	
	/** \brief Create strip set duration undo action. */
	ceUCAASpeakStripSetDuration::Ref UndoStripSetDuration(ceStrip *strip, float duration) override;
	
	/** \brief Create scale strips undo action. */
	ceUCAASpeakStripsScale::Ref UndoScaleStrips() override;
	/*@}*/
};

#endif
