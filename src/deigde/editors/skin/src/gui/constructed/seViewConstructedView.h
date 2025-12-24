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

#ifndef _SEVIEWCONSTRUCTEDVIEW_H_
#define _SEVIEWCONSTRUCTEDVIEW_H_

#include "seViewConstructedViewListener.h"
#include "../../skin/seSkin.h"

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeMouseKeyListener.h>
#include <deigde/gui/event/igdeMouseDragListener.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasView.h>

class seProperty;
class sePropertyNode;
class sePropertyNodeGroup;
class seWindowMain;
class sePropertyNodeList;


/**
 * \brief Constructed property preview.
 */
class seViewConstructedView : public igdeViewRenderWindow{
public:
	typedef deTObjectReference<seViewConstructedView> Ref;
	
private:
	seWindowMain &pWindowMain;
	seViewConstructedViewListener::Ref pListener;
	
	igdeMouseKeyListener::Ref pKeyHandling;
	igdeMouseDragListener::Ref pDragNode;
	
	seSkin::Ref pSkin;
	
	igdeAction::Ref pActionConstructedFromImage;
	igdeAction::Ref pActionAddShape;
	igdeAction::Ref pActionAddImage;
	igdeAction::Ref pActionAddText;
	igdeAction::Ref pActionRemoveNode;
	igdeAction::Ref pActionCopyNode;
	igdeAction::Ref pActionCutNode;
	igdeAction::Ref pActionPasteNode;
	igdeAction::Ref pActionEnterGroup;
	igdeAction::Ref pActionExitGroup;
	igdeAction::Ref pActionGroupNodes;
	igdeAction::Ref pActionUngroupNodes;
	igdeAction::Ref pActionMoveNodeTop;
	igdeAction::Ref pActionMoveNodeUp;
	igdeAction::Ref pActionMoveNodeDown;
	igdeAction::Ref pActionMoveNodeBottom;
	igdeAction::Ref pActionSetMask;
	igdeAction::Ref pActionRemoveMask;
	igdeAction::Ref pActionSizeFromImage;
	
	
	
	int pZoom;
	float pZoomScale;
	decTexMatrix2 pZoomScaleMatrix;
	
	decPoint pOffset;
	decPoint pBorderSize;
	
	deCanvasView::Ref pCanvasContent;
	deCanvasPaint::Ref pCanvasContentBackground;
	
	deCanvasPaint::Ref pCanvasMarkerBorder;
	deCanvasImage::Ref pCanvasMarkerResizeTopLeft;
	deCanvasImage::Ref pCanvasMarkerResizeTop;
	deCanvasImage::Ref pCanvasMarkerResizeTopRight;
	deCanvasImage::Ref pCanvasMarkerResizeLeft;
	deCanvasImage::Ref pCanvasMarkerResizeRight;
	deCanvasImage::Ref pCanvasMarkerResizeBottomLeft;
	deCanvasImage::Ref pCanvasMarkerResizeBottomRight;
	deCanvasImage::Ref pCanvasMarkerResizeBottom;
	deCanvasImage::Ref pCanvasMarkerRotateTopLeft;
	deCanvasImage::Ref pCanvasMarkerRotateTopRight;
	deCanvasImage::Ref pCanvasMarkerRotateBottomLeft;
	deCanvasImage::Ref pCanvasMarkerRotateBottomRight;
	deCanvasImage::Ref pCanvasMarkerShearTop;
	deCanvasImage::Ref pCanvasMarkerShearBottom;
	deCanvasImage::Ref pCanvasMarkerShearLeft;
	deCanvasImage::Ref pCanvasMarkerShearRight;
	
	deCanvasPaint::Ref pCanvasGroupDarkeningTop;
	deCanvasPaint::Ref pCanvasGroupDarkeningBottom;
	deCanvasPaint::Ref pCanvasGroupDarkeningLeft;
	deCanvasPaint::Ref pCanvasGroupDarkeningRight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property view. */
	seViewConstructedView(seWindowMain &windowMain);
	
protected:
	/** \brief Clean up constructed property view. */
	virtual ~seViewConstructedView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Skin or \em nullptr. */
	inline const seSkin::Ref &GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or \em nullptr. */
	void SetSkin(seSkin *skin);
	
	
	
	/** \brief Zoom factor in percentage. */
	inline int GetZoom() const{ return pZoom; }
	
	/** \brief Set zoom factor in percentage. */
	void SetZoom(int zoom);
	
	/** \brief Offset in pixels. */
	inline const decPoint &GetOffset() const{ return pOffset; }
	
	/** \brief Set offset pixels. */
	void SetOffset(const decPoint &offset);
	
	/** \brief Size of content. */
	decPoint GetContentSize() const;
	
	/** \brief Zoom scale. */
	inline float GetZoomScale() const{ return pZoomScale; }
	
	/** \brief Zoom scale matrix. */
	inline const decTexMatrix2 &GetZoomScaleMatrix() const{ return pZoomScaleMatrix; }
	
	
	
	/** \brief Active property or \em nullptr. */
	seProperty *GetActiveProperty() const;
	
	/** \brief Active property node or \em nullptr. */
	sePropertyNode *GetActiveNode() const;
	
	/** \brief Active property node group or \em nullptr. */
	sePropertyNodeGroup *GetActiveNodeGroup() const;
	
	/** \brief Node at coordinates or \em nullptr. */
	sePropertyNode *NodeAtPosition(const decPoint &position) const;
	
	
	
	/** \brief Update constructed canvas. */
	void UpdateConstructedCanvas();
	
	/** \brief Recreate content canvas. */
	void RecreateContentCanvas();
	
	/** \brief Update content canvas parameters. */
	void UpdateContentCanvasParams();
	
	/** \brief Update markers. */
	void UpdateMarkers();
	
	/** \brief Update group darkening. */
	void UpdateGroupDarkening();
	
	
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	
	/** \brief Resize canvas to fit window size. */
	virtual void OnResize();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/** \brief Get selection boundaries. */
	void GetSelectionBoundary(const sePropertyNode::List &list, decVector2 &minBounds, decVector2 &maxBounds);
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionConstructedFromImage() const{ return pActionConstructedFromImage; }
	inline const igdeAction::Ref &GetActionAddShape() const{ return pActionAddShape; }
	inline const igdeAction::Ref &GetActionAddImage() const{ return pActionAddImage; }
	inline const igdeAction::Ref &GetActionAddText() const{ return pActionAddText; }
	inline const igdeAction::Ref &GetActionRemoveNode() const{ return pActionRemoveNode; }
	inline const igdeAction::Ref &GetActionCopyNode() const{ return pActionCopyNode; }
	inline const igdeAction::Ref &GetActionCutNode() const{ return pActionCutNode; }
	inline const igdeAction::Ref &GetActionPasteNode() const{ return pActionPasteNode; }
	inline const igdeAction::Ref &GetActionEnterGroup() const{ return pActionEnterGroup; }
	inline const igdeAction::Ref &GetActionExitGroup() const{ return pActionExitGroup; }
	inline const igdeAction::Ref &GetActionGroupNodes() const{ return pActionGroupNodes; }
	inline const igdeAction::Ref &GetActionUngroupNodes() const{ return pActionUngroupNodes; }
	inline const igdeAction::Ref &GetActionMoveNodeTop() const{ return pActionMoveNodeTop; }
	inline const igdeAction::Ref &GetActionMoveNodeUp() const{ return pActionMoveNodeUp; }
	inline const igdeAction::Ref &GetActionMoveNodeDown() const{ return pActionMoveNodeDown; }
	inline const igdeAction::Ref &GetActionMoveNodeBottom() const{ return pActionMoveNodeBottom; }
	inline const igdeAction::Ref &GetActionSetMask() const{ return pActionSetMask; }
	inline const igdeAction::Ref &GetActionRemoveMask() const{ return pActionRemoveMask; }
	inline const igdeAction::Ref &GetActionSizeFromImage() const{ return pActionSizeFromImage; }
	
	/** \brief Markers. */
	inline const deCanvasView::Ref &GetCanvasContent() const{ return pCanvasContent; }
	inline const deCanvasPaint::Ref &GetCanvasContentBackground() const{ return pCanvasContentBackground; }
	inline const deCanvasPaint::Ref &GetCanvasMarkerBorder() const{ return pCanvasMarkerBorder; }
	inline const deCanvasImage::Ref &GetCanvasMarkerResizeTopLeft() const{ return pCanvasMarkerResizeTopLeft; }
	inline const deCanvasImage::Ref &GetCanvasMarkerResizeTop() const{ return pCanvasMarkerResizeTop; }
	inline const deCanvasImage::Ref &GetCanvasMarkerResizeTopRight() const{ return pCanvasMarkerResizeTopRight; }
	inline const deCanvasImage::Ref &GetCanvasMarkerResizeLeft() const{ return pCanvasMarkerResizeLeft; }
	inline const deCanvasImage::Ref &GetCanvasMarkerResizeRight() const{ return pCanvasMarkerResizeRight; }
	inline const deCanvasImage::Ref &GetCanvasMarkerResizeBottomLeft() const{ return pCanvasMarkerResizeBottomLeft; }
	inline const deCanvasImage::Ref &GetCanvasMarkerResizeBottomRight() const{ return pCanvasMarkerResizeBottomRight; }
	inline const deCanvasImage::Ref &GetCanvasMarkerResizeBottom() const{ return pCanvasMarkerResizeBottom; }
	inline const deCanvasImage::Ref &GetCanvasMarkerRotateTopLeft() const{ return pCanvasMarkerRotateTopLeft; }
	inline const deCanvasImage::Ref &GetCanvasMarkerRotateTopRight() const{ return pCanvasMarkerRotateTopRight; }
	inline const deCanvasImage::Ref &GetCanvasMarkerRotateBottomLeft() const{ return pCanvasMarkerRotateBottomLeft; }
	inline const deCanvasImage::Ref &GetCanvasMarkerRotateBottomRight() const{ return pCanvasMarkerRotateBottomRight; }
	inline const deCanvasImage::Ref &GetCanvasMarkerShearTop() const{ return pCanvasMarkerShearTop; }
	inline const deCanvasImage::Ref &GetCanvasMarkerShearBottom() const{ return pCanvasMarkerShearBottom; }
	inline const deCanvasImage::Ref &GetCanvasMarkerShearLeft() const{ return pCanvasMarkerShearLeft; }
	inline const deCanvasImage::Ref &GetCanvasMarkerShearRight() const{ return pCanvasMarkerShearRight; }
	/*@}*/
	
	
	
private:
	void pCreateMarkerCanvas(deCanvasImage::Ref &canvas, const char *pathImage, float order) const;
	void pCreateDarkeningCanvas(deCanvasPaint::Ref &canvas, float order) const;
	
	void pRecreateContentCanvas(const sePropertyNodeGroup &nodeGroup, deCanvasView &canvasView);
	void pUpdateContentCanvasParams(const sePropertyNodeGroup &nodeGroup, deCanvasView &canvasView);
};

#endif
