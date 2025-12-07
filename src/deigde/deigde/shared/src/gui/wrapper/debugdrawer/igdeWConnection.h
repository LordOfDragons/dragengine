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

#ifndef _IGDEWCONNECTION_H_
#define _IGDEWCONNECTION_H_

#include "igdeWDebugDrawerShape.h"



/**
 * \brief Connection Wrapper.
 * 
 * Draws a connection between two points using a debug drawer shapes. The connection
 * is represented as a line with optional arrow heads on both ends. The arrows and the
 * line are each a separate shape. This way the geometry of the connection line can be
 * altered without changing the shape faces just by changing position, orientation and
 * scaling of the individual shapes.
 */
class DE_DLL_EXPORT igdeWConnection{
private:
	igdeWDebugDrawerShape pDDSConnection;
	igdeWDebugDrawerShape pDDSArrowAnchor;
	igdeWDebugDrawerShape pDDSArrowTarget;
	
	decVector pAnchorPosition;
	decVector pTargetPosition;
	decColor pColor;
	
	bool pArrowOnAnchor;
	bool pArrowOnTarget;
	float pArrowSize;
	float pArrowLength;
	float pArrowRadius;
	
	bool pVisible;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create wrapper. */
	igdeWConnection();
	
	/** \brief Clean up wrapper. */
	~igdeWConnection();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set parent debug drawer or NULL. */
	void SetParentDebugDrawer(deDebugDrawer *debugDrawer);
	
	/** \brief Anchor position. */
	inline const decVector &GetAnchorPosition() const{return pAnchorPosition;}
	
	/** \brief Set anchor position. */
	void SetAnchorPosition(const decVector &position);
	
	/** \brief Target position. */
	inline const decVector &GetTargetPosition() const{return pTargetPosition;}
	
	/** \brief Set target position. */
	void SetTargetPosition(const decVector &position);
	
	/** \brief Set anchor and target position at the same time. */
	void SetPositions(const decVector &anchorPosition, const decVector &targetPosition);
	
	/** \brief Connection color. */
	inline const decColor &GetColor() const{return pColor;}
	
	/** \brief Set connection color. */
	void SetColor(const decColor &color);
	
	/** \brief Determines if an arrow head is drawn on the anchor end. */
	inline bool GetArrowOnAnchor() const{return pArrowOnAnchor;}
	
	/** \brief Sets if an arrow head is drawn on the anchor end. */
	void SetArrowOnAnchor(bool arrowOnAnchor);
	
	/** \brief Determines if an arrow head is drawn on the target end. */
	inline bool GetArrowOnTarget() const{return pArrowOnTarget;}
	
	/** \brief Sets if an arrow head is drawn on the target end. */
	void SetArrowOnTarget(bool arrowOnTarget);
	
	/** \brief Size of the arrow heads. */
	inline float GetArrowSize() const{return pArrowSize;}
	
	/** \brief Set size of the arrow heads. */
	void SetArrowSize(float size);
	
	/** \brief Determines if the debug drawer shape is visible. */
	inline bool GetVisible() const{return pVisible;}
	
	/** \brief Sets if the debug drawer shape is visible. */
	void SetVisible(bool visible);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pRebuildArrow();
	void pUpdateGeometries();
};

#endif
