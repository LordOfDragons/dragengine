/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeMetaPropertyPathWidget.h"
#include "../igdeMetaPropertyString.h"
#include "../undo/igdeMetaPropertyPathUndo.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../gui/igdeCommonDialogs.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../gui/browse/igdeDialogBrowserParticleEmitter.h"
#include "../../../gui/browse/igdeDialogBrowserSky.h"
#include "../../../gui/browse/igdeDialogBrowserSkin.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"
#include "../../../loadsave/igdeLoadAnimator.h"
#include "../../../loadsave/igdeLoadSaveNavSpace.h"
#include "../../../loadsave/igdeLoadParticleEmitter.h"
#include "../../../loadsave/igdeLoadSky.h"
#include "../../../loadsave/igdeLoadSynthesizer.h"
#include "../../../loadsave/igdeLoadSaveCamera.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/localization/deLanguagePackManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceManager.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshManager.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyManager.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerManager.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>


#define LOGSOURCE "IGDE"


namespace {

class cListenerHelper{
	igdeMetaPropertyPathWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyPathWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyPathWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyPath &GetPropertyPath() const{ return pWidget.GetPropertyPath(); }
	inline bool IsValid() const{ return GetPropertyPath().IsValid(GetContext()); }
	
	void OnValueChanged(const decString &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyPath();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
		
		const auto &value = property.GetPropertyValue(context);
		if(value != newValue && pWidget.GetEditPath()){
			pWidget.RunWithPreventUpdate([&]{
				pWidget.GetEditPath()->SetPath(value);
			});
		}
	}
};


class cListener : public igdeEditPathListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyPathWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
		pHelper.OnValueChanged(editPath->GetPath());
	}
};


class cActionBrowseGameDef : public igdeAction{
	igdeMetaPropertyPathWidget &pWidget;
	
public:
	cActionBrowseGameDef(igdeMetaPropertyPathWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.MetaPropertyPath.Action.BrowseGameDef",
		environment.GetStockIcon(igdeEnvironment::esiSearch),
		"@Igde.MetaPropertyPath.Action.BrowseGameDef.ToolTip"),
	pWidget(widget){}
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyPath();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto owner = pWidget.GetButtonContextMenu()->GetParentWindow();
		const auto &oldPath = property.GetPropertyValue(context);
		auto newPath = oldPath;
		
		switch(property.GetResourceType()){
		case igdeEnvironment::efpltSkin:
			if(!igdeDialogBrowserSkin::SelectSkin(owner, newPath)){
				return;
			}
			break;
			
		case igdeEnvironment::efpltSky:
			if(!igdeDialogBrowserSky::SelectSky(owner, newPath)){
				return;
			}
			break;
			
		case igdeEnvironment::efpltParticleEmitter:
			if(!igdeDialogBrowserParticleEmitter::SelectParticleEmitter(owner, newPath)){
				return;
			}
			break;
			
		default:
			return;
		}
		
		if(newPath != oldPath){
			property.ChangePropertyValue(context, newPath, property.RealUndoInfo(context, *this));
		}
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyPathWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyPathWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyPath();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyString::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyPathWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pHelper(widget){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		if(!pHelper.IsValid()){
			return;
		}
		
		const auto clipboard = pHelper.GetContext()->GetClipboard();
		if(!clipboard){
			return;
		}
		
		auto clip = clipboard->GetWithTypeName(pHelper.GetPropertyPath().GetClipboardDataTypeName())
			.DynamicCast<igdeMetaPropertyString::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pHelper.GetPropertyPath().GetClipboardDataTypeName()));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyPathWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged({}, GetText());
	}
};


class cActionShowFileInfo : public igdeAction{
	igdeMetaPropertyPathWidget &pWidget;
	
public:
	cActionShowFileInfo(igdeMetaPropertyPathWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.MetaPropertyPath.Action.ShowFileInfo", nullptr,
		"@Igde.MetaPropertyPath.Action.ShowFileInfo.ToolTip"),
	pWidget(widget){}
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyPath();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto dict = pWidget.FileContentInformation();
		if(dict.IsEmpty()){
			return;
		}
		
		igdeCommonDialogs::ShowInformation(*pWidget.GetButtonContextMenu()->GetParentWindow(),
			"@Igde.MetaPropertyPath.Action.ShowFileInfo", dict);
	}
};


class cActionPreset : public igdeAction{
	cListenerHelper pHelper;
	igdeMetaPropertyPath::Preset::Ref pPreset;
	
public:
	cActionPreset(igdeMetaPropertyPathWidget &widget,
		const igdeMetaPropertyPath::Preset::Ref &preset) :
	igdeAction(preset->GetName(), preset->GetIcon(), preset->GetDescription()),
	pHelper(widget),
	pPreset(preset){
	}
	
	~cActionPreset() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pPreset->GetValue(), GetText());
	}
};

}


// Class igdeMetaPropertyPathWidget::PropertyListener
///////////////////////////////////////////////////////

igdeMetaPropertyPathWidget::PropertyListener::PropertyListener(igdeMetaPropertyPathWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyPathWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyPathWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyPath*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertyPathWidget::PropertyListener::OnBasePathChanged(
igdeMetaPropertyPath*, const igdeMetaContext::Ref& context){
	if(pWidget.GetContext() == context){
		pWidget.UpdateBasePath();
	}
}


// Class igdeMetaPropertyPathWidget
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPathWidget::igdeMetaPropertyPathWidget(igdeMetaPropertyPath &property) :
igdeMetaPropertyWidget(property),
pPropertyPath(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyPath.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyPathWidget::~igdeMetaPropertyPathWidget(){
	Drop();
	pPropertyPath.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyPathWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pEditPath)
	
	auto &helper = builder.GetHelper();
	pListener = deTObjectReference<cListener>::New(*this);
	
	if(pPropertyPath.GetCustomPatternList().IsNotEmpty()){
		helper.EditPath(pPropertyPath.GetDescription(),
			pPropertyPath.GetCustomPatternList(), pEditPath, pListener);
		
	}else{
		helper.EditPath(pPropertyPath.GetDescription(),
			pPropertyPath.GetResourceType(), pEditPath, pListener);
	}
	pEditPath->SetEnabled(false);
	WrapEditWidget(builder, noLabel, pEditPath);
	
	UpdateMatchable();
	
	UpdateBasePath();
	SetContext(builder.GetContext());
}

void igdeMetaPropertyPathWidget::Drop(){
	if(pEditPath && pListener){
		pEditPath->RemoveListener(pListener);
	}
	
	pListener.Clear();
	pEditPath.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyPathWidget::Update(){
	if(!pEditPath){
		return;
	}
	
	const bool valid = pPropertyPath.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pEditPath->SetPath(valid ? pPropertyPath.GetPropertyValue(GetContext()) : decString());
		pEditPath->SetEnabled(valid);
	});
	
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertyPathWidget::UpdateBasePath(){
	if(pEditPath){
		RunWithPreventUpdate([&]{
			pEditPath->SetBasePath(pPropertyPath.IsValid(GetContext())
				? pPropertyPath.GetPropertyBasePath(GetContext()) : decString());
		});
	}
}

void igdeMetaPropertyPathWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	switch(pPropertyPath.GetResourceType()){
	case igdeEnvironment::efpltSkin:
	case igdeEnvironment::efpltSky:
	case igdeEnvironment::efpltParticleEmitter:
		helper.MenuCommand(menu, deTObjectReference<cActionBrowseGameDef>::New(*this, context, env));
		helper.MenuSeparator(menu);
		break;
		
	default:
		break;
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, env));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, env));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionShowFileInfo>::New(*this, context, env));
	helper.MenuSeparator(menu);
	
	const auto presets = pPropertyPath.GetPropertyPresets(context);
	if(presets.IsNotEmpty()){
		auto submenu = igdeMenuCascade::Ref::New(env, "@Igde.MetaProperty.Action.Presets");
		presets.Visit([&](const igdeMetaPropertyPath::Preset::Ref &preset){
			helper.MenuCommand(submenu, deTObjectReference<cActionPreset>::New(*this, preset));
		});
		menu.AddChild(submenu);
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

bool igdeMetaPropertyPathWidget::IsPropertyValid() const{
	return pPropertyPath.IsValid(GetContext());
}

decStringDictionary igdeMetaPropertyPathWidget::FileContentInformation() const{
	if(!pEditPath || !pEditPath->GetParentWindow()){
		return {};
	}
	
	auto &context = GetContext();
	if(!pPropertyPath.IsValid(context)){
		return {};
	}
	
	auto &environment = context->GetEnvironment();
	auto engineController = environment.GetEngineController();
	if(!engineController || !engineController->GetEngine()){
		return {};
	}
	
	auto &engine = *engineController->GetEngine();
	decStringDictionary dict;
	
	try{
		const auto path = pEditPath->GetAbsolutePath();
		const auto vfsPath = decPath::CreatePathUnix(path);
		const auto &vfs = engine.GetVirtualFileSystem();
		const auto size = vfs->GetFileSize(vfsPath);
		dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.File.Path", path);
		dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.File.Name", vfsPath.GetLastComponent());
		dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.File.Size", decString::Formatted("{} ({})",
			igdeUIHelper::FormatSize1024(size), igdeUIHelper::FormatSizeTousand(size)));
		
		pFileInfo(environment, context, engine, path, dict);
		
	}catch(const deException &e){
		environment.GetLogger()->LogException(LOGSOURCE, e);
		igdeCommonDialogs::Error(*pEditPath->GetParentWindow(),
			"@Igde.MetaPropertyPath.Action.ShowFileInfo", e.GetDescription());
	}
	return dict;
}


// Protected Functions
////////////////////////

void igdeMetaPropertyPathWidget::OnContextChanged(){
	Update();
	UpdateBasePath();
}

void igdeMetaPropertyPathWidget::pFileInfo(igdeEnvironment &environment,
const igdeMetaContext::Ref &context, deEngine &engine, const decString &path,
decStringDictionary &dict) const{
	switch(pPropertyPath.GetResourceType()){
	case igdeEnvironment::efpltAnimation:
		pFileInfoAnimation(engine, path, dict);
		break;
		
	case igdeEnvironment::efpltAnimator:
		pFileInfoAnimator(environment, engine, path, dict);
		break;
		
	case igdeEnvironment::efpltFont:
		pFileInfoFont(engine, path, dict);
		break;
		
	case igdeEnvironment::efpltImage:
		pFileInfoImage(environment, engine, path, dict);
		break;
		
	case igdeEnvironment::efpltLanguagePack:
		pFileInfoLanguagePack(engine, path, dict);
		break;
		
	case igdeEnvironment::efpltModel:
		pFileInfoModel(engine, path, dict);
		break;
		
	case igdeEnvironment::efpltNavigationSpace:
		pFileInfoNavigationSpace(environment, engine, path, dict);
		break;
		
	case igdeEnvironment::efpltOcclusionMesh:
		pFileInfoOcclusionMesh(engine, path, dict);
		break;
		
	case igdeEnvironment::efpltParticleEmitter:
		pFileInfoParticleEmitter(environment, engine, path, dict);
		break;
		
	case igdeEnvironment::efpltRig:
		pFileInfoRig(engine, path, dict);
		break;
		
	case igdeEnvironment::efpltSkin:
		pFileInfoSkin(engine, path, dict);
		break;
		
	case igdeEnvironment::efpltSky:
		pFileInfoSky(environment, engine, path, dict);
		break;
		
	case igdeEnvironment::efpltSynthesizer:
		pFileInfoSynthesizer(environment, engine, path, dict);
		break;
		
	case igdeEnvironment::efpltSound:
		pFileInfoSound(engine, path, dict);
		break;
		
	case igdeEnvironment::efpltVideo:
		pFileInfoVideo(environment, engine, path, dict);
		break;
		
	case igdeEnvironment::efpltSpeechAnimation:
		pFileInfoSpeechAnimation(environment, engine, path, dict);
		break;
		
	case igdeEnvironment::efpltCamera:
		pFileInfoCamera(environment, engine, path, dict);
		break;
		
	case igdeEnvironment::efpltWorld:
		pFileInfoWorld(engine, path, dict);
		break;
		
	case igdeEnvironment::efpltAll:
		break;
	}
	
	pPropertyPath.FileContentInformation(context, path, dict);
}

void igdeMetaPropertyPathWidget::pFileInfoAnimation(deEngine &engine,
const decString &path, decStringDictionary &dict) const{
	const auto animation = engine.GetAnimationManager()->LoadAnimation(path, "/");
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Animation.Moves",
		decString::Formatted("{}", animation->GetMoveCount()));
}

void igdeMetaPropertyPathWidget::pFileInfoAnimator(igdeEnvironment &environment,
deEngine &engine, const decString &path, decStringDictionary &dict) const{
	auto animator = engine.GetAnimatorManager()->CreateAnimator();
	igdeLoadAnimator(environment, environment.GetLogger(), LOGSOURCE).Load(path, animator,
		engine.GetVirtualFileSystem()->OpenFileForReading(decPath::CreatePathUnix(path)));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Animator.Bones",
		decString::Formatted("{}", animator->GetListBones().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Animator.Vps",
		decString::Formatted("{}", animator->GetListVertexPositionSets().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Animator.Controllers",
		decString::Formatted("{}", animator->GetControllers().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Animator.Links",
		decString::Formatted("{}", animator->GetLinks().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Animator.Rules",
		decString::Formatted("{}", animator->GetRules().GetCount()));
}

void igdeMetaPropertyPathWidget::pFileInfoFont(deEngine &engine,
const decString &path, decStringDictionary &dict) const{
	const auto font = engine.GetFontManager()->LoadFont(path, "/");
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Font.LineHeight",
		decString::Formatted("{}", font->GetLineHeight()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Font.FixedSizes",
		decString::Formatted("{}", font->GetFixedSizes().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Font.Glyphs",
		decString::Formatted("{}", font->GetGlyphCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Font.IsColor",
		decString::Formatted("{}", font->GetIsColorFont()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Font.IsScalable",
		decString::Formatted("{}", font->GetScalable()));
}

void igdeMetaPropertyPathWidget::pFileInfoImage(igdeEnvironment &environment, deEngine &engine,
const decString &path, decStringDictionary &dict) const{
	const auto image = engine.GetImageManager()->LoadImage(path, "/");
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Image.Width",
		decString::Formatted("{}", image->GetWidth()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Image.Height",
		decString::Formatted("{}", image->GetHeight()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Image.Depth",
		decString::Formatted("{}", image->GetDepth()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Image.BitsPerPixel",
		decString::Formatted("{}", image->GetBitCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Image.Components",
		environment.GetTranslationManager().Translate(decString::Formatted(
			"Igde.MetaPropertyPath.FileInfo.Value.Image.Components{}",
			image->GetComponentCount())).ToUTF8());
}

void igdeMetaPropertyPathWidget::pFileInfoLanguagePack(deEngine &engine,
const decString &path, decStringDictionary &dict) const{
	const auto langPack = engine.GetLanguagePackManager()->LoadLanguagePack(path, "/");
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.LangPack.Name", langPack->GetName().ToUTF8());
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.LangPack.Description",
		langPack->GetDescription().ToUTF8());
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.LangPack.Entries",
		decString::Formatted("{}", langPack->GetEntryCount()));
}

void igdeMetaPropertyPathWidget::pFileInfoModel(deEngine &engine,
const decString &path, decStringDictionary &dict) const{
	const auto model = engine.GetModelManager()->LoadModel(path, "/");
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Model.Lods",
		decString::Formatted("{}", model->GetLODs().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Model.Textures",
		decString::Formatted("{}", model->GetTextures().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Model.Bones",
		decString::Formatted("{}", model->GetBones().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Model.Vps",
		decString::Formatted("{}", model->GetVertexPositionSets().GetCount()));
	
	if(model->GetLODCount() > 0){
		const auto &lod = model->GetLODs().First();
		dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Model.Vertices",
			decString::Formatted("{}", lod->GetVertices().GetCount()));
		dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Model.Faces",
			decString::Formatted("{}", lod->GetFaces().GetCount()));
		dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Model.Weights",
			decString::Formatted("{}", lod->GetWeights().GetCount()));
	}
}

void igdeMetaPropertyPathWidget::pFileInfoNavigationSpace(igdeEnvironment &environment,
deEngine &engine, const decString &path, decStringDictionary &dict) const{
	auto &tm = environment.GetTranslationManager();
	auto navspace = engine.GetNavigationSpaceManager()->CreateNavigationSpace();
	igdeLoadSaveNavSpace(&environment, LOGSOURCE).Load(navspace,
		engine.GetVirtualFileSystem()->OpenFileForReading(decPath::CreatePathUnix(path)));
	switch(navspace->GetType()){
	case deNavigationSpace::estGrid:
		dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.NavSpace.Type",
			tm.Translate("@Igde.MetaPropertyPath.FileInfo.Value.NavSpace.TypeGrid").ToUTF8());
		break;
	case deNavigationSpace::estMesh:
		dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.NavSpace.Type",
			tm.Translate("@Igde.MetaPropertyPath.FileInfo.Value.NavSpace.TypeMesh").ToUTF8());
		break;
	case deNavigationSpace::estVolume:
		dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.NavSpace.Type",
			tm.Translate("@Igde.MetaPropertyPath.FileInfo.Value.NavSpace.TypeVolume").ToUTF8());
		break;
	}
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.NavSpace.Vertices",
		decString::Formatted("{}", navspace->GetVertices().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.NavSpace.Edges",
		decString::Formatted("{}", navspace->GetEdges().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.NavSpace.Corners",
		decString::Formatted("{}", navspace->GetCorners().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.NavSpace.Faces",
		decString::Formatted("{}", navspace->GetFaces().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.NavSpace.Walls",
		decString::Formatted("{}", navspace->GetWalls().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.NavSpace.Rooms",
		decString::Formatted("{}", navspace->GetRooms().GetCount()));
}

void igdeMetaPropertyPathWidget::pFileInfoOcclusionMesh(deEngine &engine,
const decString &path, decStringDictionary &dict) const{
	const auto occmesh = engine.GetOcclusionMeshManager()->LoadOcclusionMesh(path, "/");
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.OcclusionMesh.Vertices",
		decString::Formatted("{}", occmesh->GetVertices().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.OcclusionMesh.Faces",
		decString::Formatted("{}", occmesh->GetFaces().GetCount()));
}

void igdeMetaPropertyPathWidget::pFileInfoParticleEmitter(igdeEnvironment &environment,
deEngine &engine, const decString &path, decStringDictionary &dict) const{
	auto emitter = engine.GetParticleEmitterManager()->CreateParticleEmitter();
	igdeLoadParticleEmitter(environment, environment.GetLogger(), LOGSOURCE).Load(path, emitter,
		engine.GetVirtualFileSystem()->OpenFileForReading(decPath::CreatePathUnix(path)));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.ParticleEmitter.Controllers",
		decString::Formatted("{}", emitter->GetControllers().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.ParticleEmitter.Types",
		decString::Formatted("{}", emitter->GetTypes().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.ParticleEmitter.Burst",
		decString::Formatted("{}", emitter->GetEmitBurst()));
}

void igdeMetaPropertyPathWidget::pFileInfoRig(deEngine &engine,
const decString &path, decStringDictionary &dict) const{
	const auto rig = engine.GetRigManager()->LoadRig(path, "/");
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Rig.Bones",
		decString::Formatted("{}", rig->GetBones().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Rig.Shapes",
		decString::Formatted("{}", rig->GetShapes().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Rig.ModelCollision",
		decString::Formatted("{}", rig->GetModelCollision()));
}

void igdeMetaPropertyPathWidget::pFileInfoSkin(deEngine &engine,
const decString &path, decStringDictionary &dict) const{
	const auto skin = engine.GetSkinManager()->LoadSkin(path, "/");
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Skin.Textures",
		decString::Formatted("{}", skin->GetTextures().GetCount()));
}

void igdeMetaPropertyPathWidget::pFileInfoSky(igdeEnvironment &environment, deEngine &engine,
const decString &path, decStringDictionary &dict) const{
	const auto sky = engine.GetSkyManager()->CreateSky();
	igdeLoadSky(environment, environment.GetLogger(), LOGSOURCE).Load(path, sky,
		engine.GetVirtualFileSystem()->OpenFileForReading(decPath::CreatePathUnix(path)));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sky.Controllers",
		decString::Formatted("{}", sky->GetControllers().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sky.Links",
		decString::Formatted("{}", sky->GetLinks().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sky.Layers",
		decString::Formatted("{}", sky->GetLayers().GetCount()));
}

void igdeMetaPropertyPathWidget::pFileInfoSynthesizer(igdeEnvironment &environment,
deEngine &engine, const decString &path, decStringDictionary &dict) const{
	const auto synthesizer = engine.GetSynthesizerManager()->CreateSynthesizer();
	igdeLoadSynthesizer(environment, environment.GetLogger(), LOGSOURCE).Load(path, synthesizer,
		engine.GetVirtualFileSystem()->OpenFileForReading(decPath::CreatePathUnix(path)));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Synthesizer.Channels",
		decString::Formatted("{}", synthesizer->GetChannelCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Synthesizer.SampleRate",
		decString::Formatted("{}", synthesizer->GetSampleRate()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Synthesizer.BytesPerSample",
		decString::Formatted("{}", synthesizer->GetBytesPerSample()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Synthesizer.Samples",
		decString::Formatted("{}", synthesizer->GetSampleCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Synthesizer.Controllers",
		decString::Formatted("{}", synthesizer->GetControllers().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Synthesizer.Links",
		decString::Formatted("{}", synthesizer->GetLinks().GetCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Synthesizer.Sources",
		decString::Formatted("{}", synthesizer->GetSources().GetCount()));
}

void igdeMetaPropertyPathWidget::pFileInfoSound(deEngine &engine,
const decString &path, decStringDictionary &dict) const{
	const auto sound = engine.GetSoundManager()->LoadSound(path, "/", false);
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sound.Channels",
		decString::Formatted("{}", sound->GetChannelCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sound.SampleRate",
		decString::Formatted("{}", sound->GetSampleRate()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sound.BytesPerSample",
		decString::Formatted("{}", sound->GetBytesPerSample()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sound.Samples",
		decString::Formatted("{}", sound->GetSampleCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sound.Playtime",
		decString::Formatted("{}", sound->GetPlayTime()));
}

void igdeMetaPropertyPathWidget::pFileInfoVideo(igdeEnvironment &environment,
deEngine &engine, const decString &path, decStringDictionary &dict) const{
	const auto video = engine.GetVideoManager()->LoadVideo(path, "/", false);
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Image.Width",
		decString::Formatted("{}", video->GetWidth()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Image.Height",
		decString::Formatted("{}", video->GetHeight()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Image.BitsPerPixel",
		decString::Formatted("{}", video->GetBitCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Image.Components",
		environment.GetTranslationManager().Translate(decString::Formatted(
			"Igde.MetaPropertyPath.FileInfo.Value.Image.Components{}",
			video->GetComponentCount())).ToUTF8());
	
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sound.Channels",
		decString::Formatted("{}", video->GetChannelCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sound.SampleRate",
		decString::Formatted("{}", video->GetSampleRate()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sound.BytesPerSample",
		decString::Formatted("{}", video->GetBytesPerSample()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sound.Samples",
		decString::Formatted("{}", video->GetSampleCount()));
	
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Video.Framerate",
		decString::Formatted("{}", video->GetFrameRate()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Video.Frames",
		decString::Formatted("{}", video->GetFrameCount()));
	dict.SetAt("@Igde.MetaPropertyPath.FileInfo.Key.Sound.Playtime",
		decString::Formatted("{}", video->GetPlayTime()));
}

void igdeMetaPropertyPathWidget::pFileInfoSpeechAnimation(igdeEnvironment &environment,
deEngine &engine, const decString &path, decStringDictionary &dict) const{
	// no shared loader for this yet
}

void igdeMetaPropertyPathWidget::pFileInfoCamera(igdeEnvironment &environment,
deEngine &engine, const decString &path, decStringDictionary &dict) const{
}

void igdeMetaPropertyPathWidget::pFileInfoWorld(deEngine &engine,
const decString &path, decStringDictionary &dict) const{
}
