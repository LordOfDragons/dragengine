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

#ifndef _MEWINDOWVEGETATION_H_
#define _MEWINDOWVEGETATION_H_

#include <deigde/gui/nodeview/igdeNVBoard.h>

class meWindowMain;
class meWindowVegetationListener;
class meHTVRule;
class meWVNode;

class meHTVegetationLayer;
class meWorld;



/**
 * \brief Vegetation Window.
 */
class meWindowVegetation : public igdeNVBoard{
private:
	meWindowMain &pWindowMain;
	meWindowVegetationListener *pListener;
	
	meWorld *pWorld;
	decPoint pSector;
	meHTVegetationLayer *pVLayer;
	
	float pUnitsToPixel;
	float pPixelToUnits;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	meWindowVegetation(meWindowMain &windowMain);
	
protected:
	/** \brief Clean up view. */
	virtual ~meWindowVegetation();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline meWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld(meWorld *world);
	
	/** \brief Sector coordinates. */
	inline const decPoint &GetSector() const{ return pSector; }
	
	/** \brief Set sector coordinates. */
	void SetSector(const decPoint &sector);
	
	/** \brief Vegetation layer. */
	inline meHTVegetationLayer *GetVLayer() const{ return pVLayer; }
	
	/** \brief Set vegetation layer. */
	void SetVLayer(meHTVegetationLayer *vlayer);
	
	/** \brief Active vegetation layer rule. */
	meHTVRule *GetVRule() const;
	
	
	
	/** \brief Update nodes from vegetation layer. */
	void UpdateNodesFromVLayer();
	
	/** \brief Update links from vegetation layer. */
	void UpdateLinksFromVLayer();
	
	/** \brief Select node matching active rule. */
	void SelectNodeOfActiveRule();
	
	/** \brief Units to pixel factor. */
	inline float GetUnitsToPixel() const{ return pUnitsToPixel; }
	
	/** \brief Pixel to units factor. */
	inline float GetPixelToUnits() const{ return pPixelToUnits; }
	
	/** \brief Node with rule. */
	meWVNode *GetNodeWithRule(meHTVRule *rule);
	
	/** \brief Update node with rule. */
	void UpdateNodeWithRule(meHTVRule *rule);
	
	/** \brief Node with rule moved. */
	void NodeWithRuleMoved(meHTVRule *rule);
	/*@}*/
};

#endif
