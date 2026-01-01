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

#ifndef _MELSXMLHEIGHTTERRAIN_H_
#define _MELSXMLHEIGHTTERRAIN_H_

#include "meLSHeightTerrain.h"

class meHeightTerrain;
class meHeightTerrainNavSpace;
class meHeightTerrainNavSpaceType;
class meHeightTerrainPFLayer;
class meHeightTerrainTexture;
class meHeightTerrainSector;
class meHTVegetationLayer;
class meHTVRLink;
class meHTVRule;
class meHTVRuleClosestProp;
class meHTVRuleClosestVegetation;
class meHTVRuleCombine;
class meHTVRuleComponents;
class meHTVRuleConstant;
class meHTVRuleCurve;
class meHTVRuleGeometry;
class meHTVRuleMapping;
class meHTVRuleMath;
class meHTVRuleMultiMath;
class meHTVRulePropCount;
class meHTVRuleRandom;
class meHTVRuleResult;
class meHTVRuleVectorMath;
class meHTVVariation;
class meLoadSaveSystem;

class decXmlElementTag;
class decXmlWriter;



/**
 * @brief Load/Save XML Height Terrain.
 *
 * Load/Save XML Height Terrain.
 */
class meLSXMLHeightTerrain : public meLSHeightTerrain{
public:
	typedef deTObjectReference<meLSXMLHeightTerrain> Ref;
	
private:
	meLoadSaveSystem *pLSSys;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	explicit meLSXMLHeightTerrain(meLoadSaveSystem *lssys);
	
protected:
	/** Cleans up the object. */
	~meLSXMLHeightTerrain() override;
	
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Loads a height terrain. */
	void LoadFromFile(meHeightTerrain &heightTerrain, decBaseFileReader &file) override;
	/** Saves the height terrain. */
	void SaveToFile(meHeightTerrain &heightTerrain, decBaseFileWriter &file) override;
	/*@}*/
	
private:
	void pSaveHeightTerrain(decXmlWriter &writer, meHeightTerrain &heightTerrain);
	void pSaveSector(decXmlWriter &writer, meHeightTerrainSector &sector);
	void pSaveTexture(decXmlWriter &writer, const meHeightTerrainTexture &texture);
	void pSaveNavSpace(decXmlWriter &writer, const meHeightTerrainNavSpace &navspace);
	void pSaveNavSpaceType(decXmlWriter &writer, const meHeightTerrainNavSpaceType &navtype);
	void pSaveVLayer(decXmlWriter &writer, const meHTVegetationLayer &vlayer);
	void pSaveVLayerVariation(decXmlWriter &writer, const meHTVVariation &variation);
	void pSaveVLayerRule(decXmlWriter &writer, const meHTVRule &rule);
	void pSaveVLayerRuleClosestProp(decXmlWriter &writer, const meHTVRuleClosestProp &rule);
	void pSaveVLayerRuleClosestVegetation(decXmlWriter &writer, const meHTVRuleClosestVegetation &rule);
	void pSaveVLayerRuleCombine(decXmlWriter &writer, const meHTVRuleCombine &rule);
	void pSaveVLayerRuleComponents(decXmlWriter &writer, const meHTVRuleComponents &rule);
	void pSaveVLayerRuleCurve(decXmlWriter &writer, const meHTVRuleCurve &rule);
	void pSaveVLayerRuleGeometry(decXmlWriter &writer, const meHTVRuleGeometry &rule);
	void pSaveVLayerRuleMapping(decXmlWriter &writer, const meHTVRuleMapping &rule);
	void pSaveVLayerRuleMath(decXmlWriter &writer, const meHTVRuleMath &rule);
	void pSaveVLayerRuleMultiMath(decXmlWriter &writer, const meHTVRuleMultiMath &rule);
	void pSaveVLayerRuleRandom(decXmlWriter &writer, const meHTVRuleRandom &rule);
	void pSaveVLayerRuleResult(decXmlWriter &writer, const meHTVRuleResult &rule);
	void pSaveVLayerRuleVectorMath(decXmlWriter &writer, const meHTVRuleVectorMath &rule);
	void pSaveVLayerRuleConstant(decXmlWriter &writer, const meHTVRuleConstant &rule);
	void pSaveVLayerRulePropCount(decXmlWriter &writer, const meHTVRulePropCount &rule);
	void pSaveVLayerLink(decXmlWriter &writer, const meHTVegetationLayer &vlayer, const meHTVRLink &link);
	
	void pLoadHeightTerrain(decXmlElementTag &root, meHeightTerrain &heightTerrain);
	void pLoadSector(decXmlElementTag &root, meHeightTerrainSector &sector);
	void pLoadTexture(decXmlElementTag &root, meHeightTerrainSector &sector);
	void pLoadNavSpace(decXmlElementTag &root, meHeightTerrainSector &sector);
	void pLoadNavSpaceType(decXmlElementTag &root, meHeightTerrainNavSpace &navspace);
	void pLoadVLayer(decXmlElementTag &root, meHeightTerrain &heightTerrain);
	void pLoadVLayerVariation(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRules(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	bool pLoadVLayerRule(decXmlElementTag &root, meHTVRule &rule);
	void pLoadVLayerRuleClosestProp(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleClosestVegetation(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleCombine(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleComponents(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleCurve(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleGeometry(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleMapping(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleMath(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleMultiMath(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleRandom(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleResult(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleVectorMath(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRuleConstant(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerRulePropCount(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	void pLoadVLayerLink(decXmlElementTag &root, meHTVegetationLayer &vlayer);
	
	const char *pSlotInputNameForNumber(int ruleType, int slot) const;
	const char *pSlotOutputNameForNumber(int ruleType, int slot) const;
	int pSlotInputNumberForName(int ruleType, const char *name) const;
	int pSlotOutputNumberForName(int ruleType, const char *name) const;
};

// end of include only once
#endif
