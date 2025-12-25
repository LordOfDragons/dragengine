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

#ifndef _SKYELAYER_H_
#define _SKYELAYER_H_

#include "../controller/skyeControllerTarget.h"
#include "../body/skyeBody.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/sky/deSkyLayer.h>

class skyeSky;
class skyeProperty;

class igdeEnvironment;



/**
 * \brief Sky layer.
 */
class skyeLayer : public deObject{
private:
	igdeEnvironment &pEnvironment;
	
	skyeSky *pSky;
	
	decString pName;
	
	decVector pOffset;
	decVector pOrientation;
	decColor pColor;
	float pIntensity;
	float pTransparency;
	decString pPathSkin;
	deSkin::Ref pEngSkin;
	
	decVector pLightOrientation;
	decColor pLightColor;
	float pLightIntensity;
	float pAmbientIntensity;
	
	bool pMulBySkyLight;
	bool pMulBySkyColor;
	
	skyeBody::List pBodies;
	skyeBody::Ref pActiveBody;
	
	skyeControllerTarget pTargets[deSkyLayer::etAmbientIntensity + 1];
	deSkyLayer::eTargets pActiveTarget;
	
	bool pSelected;
	bool pActive;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<skyeLayer> Ref;

	/** \brief List type. */
	typedef decTObjectOrderedSet<skyeLayer> List;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create layer. */
	skyeLayer(igdeEnvironment &environment);
	
	/** \brief Clean up layer. */
	virtual ~skyeLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Parent sky or \em nullptr. */
	inline skyeSky *GetSky() const{ return pSky; }
	
	/** \brief Set parent sky or \em nullptr. */
	void SetSky(skyeSky *sky);
	
	/** \brief Offset. */
	inline const decVector &GetOffset() const{ return pOffset; }
	
	/** \brief Set offset. */
	void SetOffset(const decVector &offset);
	
	/** \brief Orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decVector &orientation);
	
	/** \brief Color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set blending color. */
	void SetColor(const decColor &color);
	
	/** \brief Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** \brief Set intensity. */
	void SetIntensity(float intensity);
	
	/** \brief Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** \brief Set transparency. */
	void SetTransparency(float transparency);
	
	/** \brief Skin path. */
	inline const decString &GetSkinPath() const{ return pPathSkin; }
	
	/** \brief Set skin path. */
	void SetSkinPath(const char *skinPath);
	
	/** \brief Engine skin or \em nullptr. */
	inline const deSkin::Ref &GetEngineSkin() const{ return pEngSkin; }
	
	/** \brief Light orientation. */
	inline const decVector &GetLightOrientation() const{ return pLightOrientation; }
	
	/** \brief Set light orientation. */
	void SetLightOrientation(const decVector &orientation);
	
	/** \brief Light color. */
	inline const decColor &GetLightColor() const{ return pLightColor; }
	
	/** \brief Set light color. */
	void SetLightColor(const decColor &color);
	
	/** \brief Light intensity. */
	inline float GetLightIntensity() const{ return pLightIntensity; }
	
	/** \brief Set light intensity. */
	void SetLightIntensity(float intensity);
	
	/** \brief Ambient light intensity. */
	inline float GetAmbientIntensity() const{ return pAmbientIntensity; }
	
	/** \brief Set ambient light intensity. */
	void SetAmbientIntensity(float intensity);
	
	/** \brief Layer intensity is multiplied by total sky light intensity. */
	inline bool GetMuliplyBySkyLight() const{ return pMulBySkyLight; }
	
	/** \brief Set if layer intensity is multiplied by total sky light intensity. */
	void SetMultiplyBySkyLight(bool multiply);
	
	/** \brief Layer color is multiplied by total sky light color. */
	inline bool GetMuliplyBySkyColor() const{ return pMulBySkyColor; }
	
	/** \brief Set if layer color is multiplied by total sky light color. */
	void SetMultiplyBySkyColor(bool multiply);
	
	/** \brief Layer is active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if layer is active one. */
	void SetActive(bool active);
	
	/** \brief Layer is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if layer is selected. */
	void SetSelected(bool selected);
	
	/** \brief Update relative resources after changing the base path. */
	void UpdateRelativeResources();
	/*@}*/
	
	
	
	/** \name Bodies */
	/*@{*/
	/** \brief Bodies. */
	const skyeBody::List &GetBodies() const{ return pBodies; }
	
	/** \brief Add body. */
	void AddBody(skyeBody *body);
	
	/** \brief Insert body. */
	void InsertBodyAt(skyeBody *body, int index);
	
	/** \brief Move body. */
	void MoveBodyTo(skyeBody *body, int index);
	
	/** \brief Removes body. */
	void RemoveBody(skyeBody *body);
	
	/** \brief Remove all bodies. */
	void RemoveAllBodies();
	
	/** \brief Active body or \em nullptr. */
	inline const skyeBody::Ref &GetActiveBody() const{ return pActiveBody; }
	
	/** \brief Set active body or \em nullptr. */
	void SetActiveBody(skyeBody *body);
	/*@}*/
	
	
	
	/** \name Targets */
	/*@{*/
	/** \brief Target. */
	const skyeControllerTarget &GetTarget(deSkyLayer::eTargets target) const;
	skyeControllerTarget &GetTarget(deSkyLayer::eTargets target);
	
	/** \brief Active target. */
	inline deSkyLayer::eTargets GetActiveTarget() const{ return pActiveTarget; }
	
	/** \brief Set active target. */
	void SetActiveTarget(deSkyLayer::eTargets target);
	
	/** \brief Notify listeners target changed. */
	void NotifyTargetChanged(deSkyLayer::eTargets target);
	/*@}*/
	
	
	
private:
	void pUpdateSkin();
};

#endif
