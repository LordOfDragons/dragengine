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

#ifndef _SEVIEWCONSTRUCTEDVIEW_H_
#define _SEVIEWCONSTRUCTEDVIEW_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeMouseKeyListenerReference.h>
#include <deigde/gui/event/igdeMouseDragListenerReference.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvasImageReference.h>
#include <dragengine/resources/canvas/deCanvasPaintReference.h>
#include <dragengine/resources/canvas/deCanvasViewReference.h>

class seProperty;
class sePropertyNode;
class sePropertyNodeGroup;
class seSkin;
class seViewConstructedViewListener;
class seWindowMain;
class sePropertyNodeList;


/**
 * \brief Constructed property preview.
 */
class seViewConstructedView : public igdeViewRenderWindow{
private:
	seWindowMain &pWindowMain;
	seViewConstructedViewListener *pListener;
	
	igdeMouseKeyListenerReference pKeyHandling;
	igdeMouseDragListenerReference pDragNode;
	
	seSkin *pSkin;
	
	igdeActionReference pActionConstructedFromImage;
	igdeActionReference pActionAddShape;
	igdeActionReference pActionAddImage;
	igdeActionReference pActionAddText;
	igdeActionReference pActionRemoveNode;
	igdeActionReference pActionCopyNode;
	igdeActionReference pActionCutNode;
	igdeActionReference pActionPasteNode;
	igdeActionReference pActionEnterGroup;
	igdeActionReference pActionExitGroup;
	igdeActionReference pActionGroupNodes;
	igdeActionReference pActionUngroupNodes;
	igdeActionReference pActionMoveNodeTop;
	igdeActionReference pActionMoveNodeUp;
	igdeActionReference pActionMoveNodeDown;
	igdeActionReference pActionMoveNodeBottom;
	igdeActionReference pActionSetMask;
	igdeActionReference pActionRemoveMask;
	igdeActionReference pActionSizeFromImage;
	
	
	
	int pZoom;
	float pZoomScale;
	decTexMatrix2 pZoomScaleMatrix;
	
	decPoint pOffset;
	decPoint pBorderSize;
	
	deCanvasViewReference pCanvasContent;
	deCanvasPaintReference pCanvasContentBackground;
	
	deCanvasPaintReference pCanvasMarkerBorder;
	deCanvasImageReference pCanvasMarkerResizeTopLeft;
	deCanvasImageReference pCanvasMarkerResizeTop;
	deCanvasImageReference pCanvasMarkerResizeTopRight;
	deCanvasImageReference pCanvasMarkerResizeLeft;
	deCanvasImageReference pCanvasMarkerResizeRight;
	deCanvasImageReference pCanvasMarkerResizeBottomLeft;
	deCanvasImageReference pCanvasMarkerResizeBottomRight;
	deCanvasImageReference pCanvasMarkerResizeBottom;
	deCanvasImageReference pCanvasMarkerRotateTopLeft;
	deCanvasImageReference pCanvasMarkerRotateTopRight;
	deCanvasImageReference pCanvasMarkerRotateBottomLeft;
	deCanvasImageReference pCanvasMarkerRotateBottomRight;
	deCanvasImageReference pCanvasMarkerShearTop;
	deCanvasImageReference pCanvasMarkerShearBottom;
	deCanvasImageReference pCanvasMarkerShearLeft;
	deCanvasImageReference pCanvasMarkerShearRight;
	
	deCanvasPaintReference pCanvasGroupDarkeningTop;
	deCanvasPaintReference pCanvasGroupDarkeningBottom;
	deCanvasPaintReference pCanvasGroupDarkeningLeft;
	deCanvasPaintReference pCanvasGroupDarkeningRight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property view. */
	seViewConstructedView( seWindowMain &windowMain );
	
protected:
	/** \brief Clean up constructed property view. */
	virtual ~seViewConstructedView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Skin or \em NULL. */
	inline seSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or \em NULL. */
	void SetSkin( seSkin *skin );
	
	
	
	/** \brief Zoom factor in percentage. */
	inline int GetZoom() const{ return pZoom; }
	
	/** \brief Set zoom factor in percentage. */
	void SetZoom( int zoom );
	
	/** \brief Offset in pixels. */
	inline const decPoint &GetOffset() const{ return pOffset; }
	
	/** \brief Set offset pixels. */
	void SetOffset( const decPoint &offset );
	
	/** \brief Size of content. */
	decPoint GetContentSize() const;
	
	/** \brief Zoom scale. */
	inline float GetZoomScale() const{ return pZoomScale; }
	
	/** \brief Zoom scale matrix. */
	inline const decTexMatrix2 &GetZoomScaleMatrix() const{ return pZoomScaleMatrix; }
	
	
	
	/** \brief Active property or \em NULL. */
	seProperty *GetActiveProperty() const;
	
	/** \brief Active property node or \em NULL. */
	sePropertyNode *GetActiveNode() const;
	
	/** \brief Active property node group or \em NULL. */
	sePropertyNodeGroup *GetActiveNodeGroup() const;
	
	/** \brief Node at coordinates or \em NULL. */
	sePropertyNode *NodeAtPosition( const decPoint &position ) const;
	
	
	
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
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Get selection boundaries. */
	void GetSelectionBoundary( const sePropertyNodeList &list, decVector2 &minBounds, decVector2 &maxBounds );
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionConstructedFromImage() const{ return pActionConstructedFromImage; }
	inline igdeAction *GetActionAddShape() const{ return pActionAddShape; }
	inline igdeAction *GetActionAddImage() const{ return pActionAddImage; }
	inline igdeAction *GetActionAddText() const{ return pActionAddText; }
	inline igdeAction *GetActionRemoveNode() const{ return pActionRemoveNode; }
	inline igdeAction *GetActionCopyNode() const{ return pActionCopyNode; }
	inline igdeAction *GetActionCutNode() const{ return pActionCutNode; }
	inline igdeAction *GetActionPasteNode() const{ return pActionPasteNode; }
	inline igdeAction *GetActionEnterGroup() const{ return pActionEnterGroup; }
	inline igdeAction *GetActionExitGroup() const{ return pActionExitGroup; }
	inline igdeAction *GetActionGroupNodes() const{ return pActionGroupNodes; }
	inline igdeAction *GetActionUngroupNodes() const{ return pActionUngroupNodes; }
	inline igdeAction *GetActionMoveNodeTop() const{ return pActionMoveNodeTop; }
	inline igdeAction *GetActionMoveNodeUp() const{ return pActionMoveNodeUp; }
	inline igdeAction *GetActionMoveNodeDown() const{ return pActionMoveNodeDown; }
	inline igdeAction *GetActionMoveNodeBottom() const{ return pActionMoveNodeBottom; }
	inline igdeAction *GetActionSetMask() const{ return pActionSetMask; }
	inline igdeAction *GetActionRemoveMask() const{ return pActionRemoveMask; }
	inline igdeAction *GetActionSizeFromImage() const{ return pActionSizeFromImage; }
	
	/** \brief Markers. */
	inline deCanvasView *GetCanvasContent() const{ return pCanvasContent; }
	inline deCanvasPaint *GetCanvasContentBackground() const{ return pCanvasContentBackground; }
	inline deCanvasPaint *GetCanvasMarkerBorder() const{ return pCanvasMarkerBorder; }
	inline deCanvasImage *GetCanvasMarkerResizeTopLeft() const{ return pCanvasMarkerResizeTopLeft; }
	inline deCanvasImage *GetCanvasMarkerResizeTop() const{ return pCanvasMarkerResizeTop; }
	inline deCanvasImage *GetCanvasMarkerResizeTopRight() const{ return pCanvasMarkerResizeTopRight; }
	inline deCanvasImage *GetCanvasMarkerResizeLeft() const{ return pCanvasMarkerResizeLeft; }
	inline deCanvasImage *GetCanvasMarkerResizeRight() const{ return pCanvasMarkerResizeRight; }
	inline deCanvasImage *GetCanvasMarkerResizeBottomLeft() const{ return pCanvasMarkerResizeBottomLeft; }
	inline deCanvasImage *GetCanvasMarkerResizeBottomRight() const{ return pCanvasMarkerResizeBottomRight; }
	inline deCanvasImage *GetCanvasMarkerResizeBottom() const{ return pCanvasMarkerResizeBottom; }
	inline deCanvasImage *GetCanvasMarkerRotateTopLeft() const{ return pCanvasMarkerRotateTopLeft; }
	inline deCanvasImage *GetCanvasMarkerRotateTopRight() const{ return pCanvasMarkerRotateTopRight; }
	inline deCanvasImage *GetCanvasMarkerRotateBottomLeft() const{ return pCanvasMarkerRotateBottomLeft; }
	inline deCanvasImage *GetCanvasMarkerRotateBottomRight() const{ return pCanvasMarkerRotateBottomRight; }
	inline deCanvasImage *GetCanvasMarkerShearTop() const{ return pCanvasMarkerShearTop; }
	inline deCanvasImage *GetCanvasMarkerShearBottom() const{ return pCanvasMarkerShearBottom; }
	inline deCanvasImage *GetCanvasMarkerShearLeft() const{ return pCanvasMarkerShearLeft; }
	inline deCanvasImage *GetCanvasMarkerShearRight() const{ return pCanvasMarkerShearRight; }
	/*@}*/
	
	
	
private:
	void pCreateMarkerCanvas( deCanvasImageReference &canvas, const char *pathImage, float order ) const;
	void pCreateDarkeningCanvas( deCanvasPaintReference &canvas, float order ) const;
	
	void pRecreateContentCanvas( const sePropertyNodeGroup &nodeGroup, deCanvasView &canvasView );
	void pUpdateContentCanvasParams( const sePropertyNodeGroup &nodeGroup, deCanvasView &canvasView );
};

#endif
