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

#ifndef _DESCRIPTINGDRAGONSCRIPT_H_
#define _DESCRIPTINGDRAGONSCRIPT_H_

#include <libdscript/libdscript.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingModule.h>

#include "dedsLoadingScreen.h"
#include "dedsVRPlaceholder.h"
#include "dedsEngineException.h"


class deClassPhysicsSystem;
class deEngine;

class deClassAISystem;
class deClassAnimation;
class deClassAnimationBuilder;
class deClassAnimator;
class deClassAnimatorController;
class deClassAnimatorInstance;
class deClassAnimatorRule;
class deClassAnimatorSystem;
class deClassARAnimation;
class deClassARAnimationDifference;
class deClassARAnimationSelect;
class deClassARBoneTransformator;
class deClassARForeignState;
class deClassARGroup;
class deClassARInverseKinematic;
class deClassARLimit;
class deClassARMirror;
class deClassARStateManipulator;
class deClassARStateSnapshot;
class deClassARSubAnimator;
class deClassARTrackTo;
class deClassAudioSystem;
class deClassBillboard;
class deClassCachedVegetation;
class deClassCamera;
class deClassCanvas;
class deClassCanvasCanvasView;
class deClassCanvasImage;
class deClassCanvasPaint;
class deClassCanvasRenderWorld;
class deClassCanvasText;
class deClassCanvasVideoPlayer;
class deClassCanvasView;
class deClassCaptureCanvas;
class deClassCache;
class deClassCollider;
class deClassColliderVolume;
class deClassColliderComponent;
class deClassColliderRig;
class deClassColliderListener;
class deClassColliderBreakingListener;
class deClassColliderCollisionTest;
class deClassColliderConstraint;
class deClassCollisionFilter;
class deClassCollisionInfo;
class deClassCollisionTester;
class deClassColor;
class deClassColorMatrix;
class deClassComponent;
class deClassConnection;
class deClassConnectionListener;
class deClassCurve2D;
class deClassCurveBezier;
class deClassCurveBezier3D;
class deClassCurveDistanceMapping;
class deClassCrashRecoverySystem;
class deClassDebugDrawer;
class deClassDecal;
class deClassDVector;
class deClassDMatrix;
class deClassDMatrix4;
class deClassDynamicSkin;
class deClassEasyXML;
class deClassEasyXMLElement;
class deClassEffect;
class deClassEffectColorMatrix;
class deClassEffectDistortImage;
class deClassEffectFilterKernel;
class deClassEffectOverlayImage;
class deClassEngine;
class deClassEnvMapProbe;
class deClassFileReader;
class deClassFileSystem;
class deClassFileWriter;
class deClassFont;
class deClassFontBuilder;
class deClassForceField;
class deClassGame;
class deClassGraphicSystem;
class deClassHeightTerrain;
class deClassImage;
class deClassImagePixels;
class deClassEditableImage;
class deClassInputDevice;
class deClassInputDeviceAxis;
class deClassInputDeviceButton;
class deClassInputDeviceFeedback;
class deClassInputDeviceComponent;
class deClassInputEvent;
class deClassInputSystem;
class deClassLanguagePack;
class deClassLanguagePackBuilder;
class deClassLayerMask;
class deClassLight;
class deClassLocomotion;
class deClassLumimeter;
class deClassMath;
class deClassMatrix;
class deClassMatrix4;
class deClassMemoryFile;
class deClassMicrophone;
class deClassModel;
class deClassModelBuilder;
class deClassModuleParameter;
class deClassMutableID;
class deClassNavigationInfo;
class deClassNavigationSpace;
class deClassNavigationBlocker;
class deClassNavigator;
class deClassNavigatorPath;
class deClassNetworkMessage;
class deClassNetworkState;
class deClassNetworkStateListener;
class deClassNetworkSystem;
class deClassOcclusionMesh;
class deClassOcclusionMeshBuilder;
class deClassParticleEmitter;
class deClassParticleEmitterController;
class deClassParticleEmitterInstance;
class deClassParticleEmitterInstanceListener;
class deClassPoint;
class deClassPoint3;
class deClassPropField;
class deClassPropFieldListener;
class deClassQuaternion;
class deClassResourceListener;
class deClassRig;
class deClassRigBuilder;
class deClassRuntimeMeter;
class deClassSafeArray;
class deClassScriptSystem;
class deClassServer;
class deClassServerListener;
class deClassShapeList;
class deClassSkin;
class deClassSkinBuilder;
class deClassSky;
class deClassSkyBody;
class deClassSkyController;
class deClassSkyInstance;
class deClassSkyLayer;
class deClassSkyLink;
class deClassSkyTarget;
class deClassSmoothFloat;
class deClassSmoothDouble;
class deClassSmoothVector;
class deClassSmoothVector2;
class deClassSmoothDVector;
class deClassSound;
class deClassSoundLevelMeter;
class deClassSoundLevelMeterSpeaker;
class deClassSoundLevelMeterListener;
class deClassSpeaker;
class deClassStringID;
class deClassSynthesizer;
class deClassSynthesizerController;
class deClassSynthesizerEffect;
class deClassSynthesizerInstance;
class deClassSynthesizerSource;
class deClassSynthesizerSystem;
class deClassSEStretch;
class deClassSSSound;
class deClassSSWave;
class deClassSSChain;
class deClassSSSynthesizer;
class deClassSSGroup;
class deClassTexMatrix;
class deClassTexMatrix2;
class deClassTouchSensor;
class deClassTouchSensorListener;
class deClassUnicodeString;
class deClassUniqueID;
class deClassUTF8Decoder;
class deClassVector;
class deClassVector2;
class deClassVideo;
class deClassVideoPlayer;
class deClassVRSystem;
class deClassWorld;
class deClassService;
class deClassServiceListener;
class deClassServiceObject;

class deCollisionInfo;
class dedsColliderListenerClosest;
class dedsColliderListenerAdaptor;

class dedsResourceLoader;
class decPath;
class dePropertyList;
class deCollider;
class deErrorTracePoint;
class deErrorTraceValue;
//class dedsLockManager;



// scripting dragonscript class
class deScriptingDragonScript : public deBaseScriptingModule{
public:
	enum eKeyEventIdentifiers{
		eeiKeyPress,
		eeiKeyRelease,
		eeiKeyType,
		eeiMousePress,
		eeiMouseRelease,
		eeiMouseMove,
		eeiCount, // no value, just count of entries
	};
	
	enum eState{
		esStopped,
		esSkipOneFrame,
		esLoadBasicPackages,
		esLoadGame,
		esCreateGameObject,
		esInitGameObject,
		esReady,
		esRestartShutdown,
		esRestartInit
	};
	
	struct sModuleVersion{
		decString version;
		int major;
		int minor;
		int patch;
		
		sModuleVersion();
		void SetVersion( const char *version );
	};
	
private:
	sModuleVersion pCompatibleVersion;
	sModuleVersion pModuleVersion;
	eState pState;
	
	deClassAISystem *pClsAISys;
	deClassAnimation *pClsAnim;
	deClassAnimationBuilder *pClsAnimBuilder;
	deClassAnimator *pClsAr;
	deClassAnimatorInstance *pClsArI;
	deClassAnimatorController *pClsAnimatorCtrl;
	deClassAnimatorRule *pClsArR;
	deClassAnimatorSystem *pClsAnimatorSystem;
	deClassARAnimation *pClsARAnim;
	deClassARAnimationDifference *pClsARAnimDiff;
	deClassARAnimationSelect *pClsARAnimSelect;
	deClassARBoneTransformator *pClsARBoneTrans;
	deClassARForeignState *pClsARFSta;
	deClassARGroup *pClsARGroup;
	deClassARInverseKinematic *pClsARIK;
	deClassARLimit *pClsARLimit;
	deClassARMirror *pClsARMirror;
	deClassARStateManipulator *pClsARStaM;
	deClassARStateSnapshot *pClsARSnap;
	deClassARSubAnimator *pClsARSubA;
	deClassARTrackTo *pClsARTrack;
	deClassAudioSystem *pClsAudSys;
	deClassBillboard *pClsBillboard;
	deClassCachedVegetation *pClsCVeg;
	deClassCamera *pClsCam;
	deClassCanvas *pClsCanvas;
	deClassCanvasCanvasView *pClsCanvasCView;
	deClassCanvasImage *pClsCanvasImage;
	deClassCanvasPaint *pClsCanvasPaint;
	deClassCanvasRenderWorld *pClsCanvasRenW;
	deClassCanvasText *pClsCanvasText;
	deClassCanvasVideoPlayer *pClsCanvasVidP;
	deClassCanvasView *pClsCanvasView;
	deClassCaptureCanvas *pClsCapCan;
	deClassCache *pClsCache;
	deClassColliderListener *pClsCLL;
	deClassColliderBreakingListener *pClsCBL;
	deClassColliderConstraint *pClsCCon;
	deClassColliderCollisionTest *pClsCCT;
	deClassCollider *pClsCol;
	deClassColliderVolume *pClsColVol;
	deClassColliderComponent *pClsColComp;
	deClassColliderRig *pClsColRig;
	deClassCollisionFilter *pClsCF;
	deClassCollisionInfo *pClsCI;
	deClassCollisionTester *pClsCT;
	deClassColor *pClsClr;
	deClassColorMatrix *pClsClrMat;
	deClassComponent *pClsComp;
	deClassConnectionListener *pClsConL;
	deClassConnection *pClsCon;
	deClassCurve2D *pClsCurve2D;
	deClassCurveBezier *pClsCBe;
	deClassCurveBezier3D *pClsCBezier3D;
	deClassCurveDistanceMapping *pClsCDistMap;
	deClassCrashRecoverySystem *pClsCRSys;
	deClassDebugDrawer *pClsDD;
	deClassDecal *pClsDec;
	deClassDVector *pClsDVec;
	deClassDMatrix *pClsDMat;
	deClassDMatrix4 *pClsDMat4;
	deClassDynamicSkin *pClsDSkin;
	deClassEasyXMLElement *pClsXMLEl;
	deClassEasyXML *pClsXML;
	deClassEffect *pClsEff;
	deClassEffectColorMatrix *pClsEffClrMat;
	deClassEffectDistortImage *pClsEffDistImg;
	deClassEffectFilterKernel *pClsEffFilKer;
	deClassEffectOverlayImage *pClsEffOverImg;
	deClassEngine *pClsEngine;
	deClassEnvMapProbe *pClsEnvMapProbe;
	deClassFileReader *pClsFileReader;
	deClassFileWriter *pClsFileWriter;
	deClassFileSystem *pClsFileSys;
	deClassFont *pClsFnt;
	deClassFontBuilder *pClsFontBuilder;
	deClassForceField *pClsFF;
	deClassGame *pClsGame;
	deClassGraphicSystem *pClsGraSys;
	deClassHeightTerrain *pClsHT;
	deClassImage *pClsImg;
	deClassImagePixels *pClsImagePixels;
	deClassEditableImage *pClsEditableImage;
	deClassInputEvent *pClsInpEvent;
	deClassInputDevice *pClsInpDev;
	deClassInputDeviceAxis *pClsInpDevAxis;
	deClassInputDeviceButton *pClsInpDevBtn;
	deClassInputDeviceFeedback *pClsInpDevFb;
	deClassInputDeviceComponent *pClsInpDevComp;
	deClassInputSystem *pClsInpSys;
	deClassLanguagePack *pClsLP;
	deClassLanguagePackBuilder *pClsLangPackBuilder;
	deClassLayerMask *pClsLyM;
	deClassLight *pClsLig;
	deClassLocomotion *pClsLoco;
	deClassLumimeter *pClsLM;
	deClassMath *pClsMath;
	deClassMatrix *pClsMat;
	deClassMatrix4 *pClsMat4;
	deClassMemoryFile *pClsMemoryFile;
	deClassMicrophone *pClsMic;
	deClassModel *pClsMdl;
	deClassModelBuilder *pClsModelBuilder;
	deClassModuleParameter *pClsModPar;
	deClassMutableID *pClsMUID;
	deClassNavigationInfo *pClsNavInfo;
	deClassNavigationSpace *pClsNavSpace;
	deClassNavigationBlocker *pClsNavBlocker;
	deClassNavigator *pClsNavigator;
	deClassNavigatorPath *pClsNavigatorPath;
	deClassNetworkMessage *pClsNM;
	deClassNetworkStateListener *pClsNSL;
	deClassNetworkSystem *pClsNetSys;
	deClassOcclusionMesh *pClsOccM;
	deClassOcclusionMeshBuilder *pClsOccMBuilder;
	deClassNetworkState *pClsNS;
	deClassParticleEmitter *pClsPE;
	deClassParticleEmitterController *pClsPEC;
	deClassParticleEmitterInstance *pClsPEI;
	deClassParticleEmitterInstanceListener *pClsPEIL;
	deClassPhysicsSystem *pClsPhySys;
	deClassPoint3 *pClsPt3;
	deClassPoint *pClsPt;
	deClassPropField *pClsPF;
	deClassPropFieldListener *pClsPFL;
	deClassQuaternion *pClsQuat;
	deClassResourceListener *pClsRN;
	deClassRig *pClsRig;
	deClassRigBuilder *pClsRigBuilder;
	deClassRuntimeMeter *pClsRTM;
	deClassServerListener *pClsSvrL;
	deClassSafeArray *pClsSA;
	deClassScriptSystem *pClsScrSys;
	deClassShapeList *pClsShaList;
	deClassService *pClsService;
	deClassServiceListener *pClsServiceListener;
	deClassServiceObject *pClsServiceObject;
	deClassServer *pClsSvr;
	deClassSkin *pClsSkin;
	deClassSkinBuilder *pClsSkinBuilder;
	deClassSky *pClsSky;
	deClassSkyBody *pClsSkyBody;
	deClassSkyController *pClsSkyCtrl;
	deClassSkyInstance *pClsSkyInst;
	deClassSkyLayer *pClsSkyLayer;
	deClassSkyLink *pClsSkyLink;
	deClassSkyTarget *pClsSkyTarget;
	deClassSmoothFloat *pClsSmFlt;
	deClassSmoothDouble *pClsSmDbl;
	deClassSmoothVector *pClsSmVec;
	deClassSmoothVector2 *pClsSmVec2;
	deClassSmoothDVector *pClsSmDVec;
	deClassSound *pClsSnd;
	deClassSoundLevelMeter *pClsSoundLevelMeter;
	deClassSoundLevelMeterSpeaker *pClsSoundLevelMeterSpeaker;
	deClassSoundLevelMeterListener *pClsSoundLevelMeterListener;
	deClassSpeaker *pClsSpk;
	deClassStringID *pClsSID;
	deClassSynthesizer *pClsSyn;
	deClassSynthesizerController *pClsSynCtrl;
	deClassSynthesizerEffect *pClsSynEff;
	deClassSynthesizerInstance *pClsSynI;
	deClassSynthesizerSource *pClsSynS;
	deClassSynthesizerSystem *pClsSynthesizerSystem;
	deClassSEStretch *pClsSEStretch;
	deClassSSSound *pClsSSSound;
	deClassSSWave *pClsSSWave;
	deClassSSChain *pClsSSChain;
	deClassSSSynthesizer *pClsSSSyn;
	deClassSSGroup *pClsSSGroup;
	deClassTexMatrix *pClsTexMatrix;
	deClassTexMatrix2 *pClsTexMatrix2;
	deClassTouchSensor *pClsTS;
	deClassTouchSensorListener *pClsTSL;
	deClassUnicodeString *pClsUS;
	deClassUniqueID *pClsUID;
	deClassUTF8Decoder *pClsU8D;
	deClassVector *pClsVec;
	deClassVector2 *pClsVec2;
	deClassVideo *pClsVid;
	deClassVideoPlayer *pClsVP;
	deClassVRSystem *pClsVRSys;
	deClassWorld *pClsWorld;
	
	dedsLoadingScreen::Ref pLoadingScreen;
	dedsVRPlaceholder::Ref pVRPlaceholder;
	
	decString pInitScriptDirectory;
	decString pInitGameObject;
	
	dsClass *pClsResourceLoaderType;
	
	dsEngine *pScriptEngine;
	dsClass *pClsGameObj;
	
	dedsResourceLoader *pResourceLoader;
//	dedsLockManager *pLockManager;
	
	deCollisionInfo *pColInfo;
	dedsColliderListenerClosest *pColliderListenerClosest;
	dedsColliderListenerAdaptor *pColliderListenerAdaptor;
	
	decPointerList pDeleteValuesLaterList;
	
	// objects
	dsValue *pGameObj;
	decString pRestartInfo;
	bool pRestartRequested;
	
	deVFSContainer::Ref pVFSContainerHideScriptDirectory;

	// dpi awareness
	decPoint pDpiAccumMouseMoved;
	
public:
	// constructor, destructor
	deScriptingDragonScript( deLoadableModule &loadableModule );
	~deScriptingDragonScript();
	
	// runtime
	/**
	 * \brief Directory relative to the module share directory where shared module data is located.
	 * \returns \em NULL if no such directory is used.
	 */
	virtual const char *GetSharedDataDir() const;
	
	/**
	 * \brief VFS directory where the module share directory is shown at.
	 * \returns \em NULL if no such directory is used.
	 */
	virtual const char *GetVFSSharedDataDir() const;
	
	virtual bool Init( const char *scriptDirectory, const char *gameObject );
	
	virtual void ShutDown();

	// script packages
	void LoadPackage(const char *name, const char *directory);

	/** @name Management */
	/*@{*/
	/** Creates a peer for the given collider object. */
	virtual deBaseScriptingCollider *CreateCollider( deCollider *collider );
	/** Creates a peer for the given server object. */
	virtual deBaseScriptingServer *CreateServer( deServer *server );
	/** Creates a peer for the given connection object. */
	virtual deBaseScriptingConnection *CreateConnection( deConnection *connection );
	/** Creates a peer for the given network state object. */
	virtual deBaseScriptingNetworkState *CreateNetworkState( deNetworkState *state );
	/** Creates a peer for the given touch sensor object. */
	virtual deBaseScriptingTouchSensor *CreateTouchSensor( deTouchSensor *touchSensor );
	/** Creates a peer for the given prop field object. */
	virtual deBaseScriptingPropField *CreatePropField( dePropField *propField );
	/** Creates a peer for the given particle emitter instance object or NULL if not used. */
	virtual deBaseScriptingParticleEmitterInstance *CreateParticleEmitterInstance( deParticleEmitterInstance *instance );
	
	/** \brief Create deSoundLevelMeter peer. */
	virtual deBaseScriptingSoundLevelMeter *CreateSoundLevelMeter( deSoundLevelMeter *meter );
	
	/** \brief Create deSpeaker peer. */
	virtual deBaseScriptingSpeaker *CreateSpeaker( deSpeaker *speaker );
	
	/**
	 * \brief Create deService peer.
	 * \version 1.23
	 */
	virtual deBaseScriptingService *CreateService( deService *service );
	
	/**
	 * Initializes the game scripts. This usually involves creating the
	 * game objects and calling init functions on them.
	 */
	virtual bool InitGame();
	/**
	 * Exits the game scripts. This usually involves calling exit functions
	 * and freeing game objects.
	 */
	virtual bool ExitGame();
	/**
	 * Called at the beginning of each update cycle. This gives the game
	 * scripts the chance to do frame update related tasks that do not
	 * involve updating the screen. You can update skins or offscreen
	 * render targets but you should not update the screen itself.
	 * @return true if the call has been successfull or false otherwise
	 */
	virtual bool OnFrameUpdate();
	/**
	 * Called after the render window changed size.
	 */
	virtual bool OnResizeRenderWindow();
	/**
	 * Send the given event to game scripts.
	 * @return true if the call has been successfull or false otherwise
	 */
	virtual bool SendEvent( deInputEvent *event );
	
	/**
	 * \brief User requested window to be closed.
	 * 
	 * Default implementation calls deEngine.Quit().
	 */
	virtual void UserRequestQuit();
	
	/**
	 * \brief Called after the application received or lost focus.
	 * \return true if the call has been successfull or false otherwise
	 * \version 1.22
	 */
	virtual bool OnAppActivate();
	
	/** Requested compatible module version. */
	inline const sModuleVersion &GetModuleVersion() const{ return pModuleVersion; }
	
	/** Module version. */
	inline const sModuleVersion &GetCompatibleVersion() const{ return pCompatibleVersion; }
	
	/** VR Placeholder or nullptr. */
	inline const dedsVRPlaceholder::Ref &GetVRPlaceholder() const{ return pVRPlaceholder; }
	/*@}*/
	
	
	
public:
	// script management
	inline dsEngine *GetScriptEngine() const{ return pScriptEngine; }
	
	/** Retrieves the streaming version. */
	inline int GetStreamingVersion() const{ return 1; }
	
	/** Retrieves the resource loader. */
	inline dedsResourceLoader *GetResourceLoader() const{ return pResourceLoader; }
	
	/** Retrieves the shared collision info. */
	inline deCollisionInfo *GetCollisionInfo() const{ return pColInfo; }
	
	/** \brief Shared collider listener closest. */
	inline dedsColliderListenerClosest &GetColliderListenerClosest() const{ return *pColliderListenerClosest; }
	
	/** \brief Shared collider listener adaptor. */
	inline dedsColliderListenerAdaptor &GetColliderListenerAdaptor() const{ return *pColliderListenerAdaptor; }
	
	// classes
	inline deClassAISystem *GetClassAISystem() const{ return pClsAISys; }
	inline deClassAnimation *GetClassAnimation() const{ return pClsAnim; }
	inline deClassAnimationBuilder *GetClassAnimationBuilder() const{ return pClsAnimBuilder; }
	inline deClassAnimator *GetClassAnimator() const{ return pClsAr; }
	inline deClassAnimatorInstance *GetClassAnimatorInstance() const{ return pClsArI; }
	inline deClassAnimatorController *GetClassAnimatorController() const{ return pClsAnimatorCtrl; }
	inline deClassAnimatorRule *GetClassAnimatorRule() const{ return pClsArR; }
	inline deClassAnimatorSystem *GetClassAnimatorSystem() const{ return pClsAnimatorSystem; }
	inline deClassARAnimation *GetClassARAnimation() const{ return pClsARAnim; }
	inline deClassARAnimationDifference *GetClassARAnimationDifference() const{ return pClsARAnimDiff; }
	inline deClassARAnimationSelect *GetClassARAnimationSelect() const{ return pClsARAnimSelect; }
	inline deClassARBoneTransformator *GetClassARBoneTransformator() const{ return pClsARBoneTrans; }
	inline deClassARForeignState *GetClassARForeignState() const{ return pClsARFSta; }
	inline deClassARMirror *GetClassARMirror() const{ return pClsARMirror; }
	inline deClassARGroup *GetClassARGroup() const{ return pClsARGroup; }
	inline deClassARInverseKinematic *GetClassARInverseKinematic() const{ return pClsARIK; }
	inline deClassARLimit *GetClassARLimit() const{ return pClsARLimit; }
	inline deClassARStateManipulator *GetClassARStateManipulator() const{ return pClsARStaM; }
	inline deClassARStateSnapshot *GetClassARStateSnapshot() const{ return pClsARSnap; }
	inline deClassARSubAnimator *GetClassARSubAnimator() const{ return pClsARSubA; }
	inline deClassARTrackTo *GetClassARTrackTo() const{ return pClsARTrack; }
	inline deClassAudioSystem *GetClassAudioSystem() const{ return pClsAudSys; }
	inline deClassNetworkSystem *GetClassNetworkSystem() const{ return pClsNetSys; }
	inline deClassBillboard *GetClassBillboard() const{ return pClsBillboard; }
	inline deClassCachedVegetation *GetClassCachedVegetation() const{ return pClsCVeg; }
	inline deClassCamera *GetClassCamera() const{ return pClsCam; }
	inline deClassCanvas *GetClassCanvas() const{ return pClsCanvas; }
	inline deClassCanvasCanvasView *GetClassCanvasCanvasView() const{ return pClsCanvasCView; }
	inline deClassCanvasImage *GetClassCanvasImage() const{ return pClsCanvasImage; }
	inline deClassCanvasPaint *GetClassCanvasPaint() const{ return pClsCanvasPaint; }
	inline deClassCanvasRenderWorld *GetClassCanvasRenderWorld() const{ return pClsCanvasRenW; }
	inline deClassCanvasText *GetClassCanvasText() const{ return pClsCanvasText; }
	inline deClassCanvasVideoPlayer *GetClassCanvasVideoPlayer() const{ return pClsCanvasVidP; }
	inline deClassCanvasView *GetClassCanvasView() const{ return pClsCanvasView; }
	inline deClassCaptureCanvas *GetClassCaptureCanvas() const{ return pClsCapCan; }
	inline deClassCache *GetClassCache() const{ return pClsCache; }
	inline deClassCollider *GetClassCollider() const{ return pClsCol; }
	inline deClassColliderVolume *GetClassColliderVolume() const{ return pClsColVol; }
	inline deClassColliderComponent *GetClassColliderComponent() const{ return pClsColComp; }
	inline deClassColliderRig *GetClassColliderRig() const{ return pClsColRig; }
	inline deClassColliderListener *GetClassColliderListener() const{ return pClsCLL; }
	inline deClassColliderBreakingListener *GetClassColliderBreakingListener() const{ return pClsCBL; }
	inline deClassColliderConstraint *GetClassColliderConstraint() const{ return pClsCCon; }
	inline deClassColliderCollisionTest *GetClassColliderCollisionTest() const{ return pClsCCT; }
	inline deClassCollisionFilter *GetClassCollisionFilter() const{ return pClsCF; }
	inline deClassCollisionInfo *GetClassCollisionInfo() const{ return pClsCI; }
	inline deClassCollisionTester *GetClassCollisionTester() const{ return pClsCT; }
	inline deClassColor *GetClassColor() const{ return pClsClr; }
	inline deClassColorMatrix *GetClassColorMatrix() const{ return pClsClrMat; }
	inline deClassComponent *GetClassComponent() const{ return pClsComp; }
	inline deClassConnection *GetClassConnection() const{ return pClsCon; }
	inline deClassConnectionListener *GetClassConnectionListener() const{ return pClsConL; }
	inline deClassCrashRecoverySystem *GetClassCrashRecoverySystem() const{ return pClsCRSys; }
	inline deClassCurve2D *GetClassCurve2D() const{ return pClsCurve2D; }
	inline deClassCurveBezier *GetClassCurveBezier() const{ return pClsCBe; }
	inline deClassCurveBezier3D *GetClassCurveBezier3D() const{ return pClsCBezier3D; }
	inline deClassCurveDistanceMapping *GetClassCurveDistanceMapping() const{ return pClsCDistMap; }
	inline deClassDebugDrawer *GetClassDebugDrawer() const{ return pClsDD; }
	inline deClassDecal *GetClassDecal() const{ return pClsDec; }
	inline deClassDVector *GetClassDVector() const{ return pClsDVec; }
	inline deClassDMatrix *GetClassDMatrix() const{ return pClsDMat; }
	inline deClassDMatrix4 *GetClassDMatrix4() const{ return pClsDMat4; }
	inline deClassDynamicSkin *GetClassDynamicSkin() const{ return pClsDSkin; }
	inline deClassEasyXMLElement *GetClassEasyXMLElement() const{ return pClsXMLEl; }
	inline deClassEasyXML *GetClassEasyXML() const{ return pClsXML; }
	inline deClassEffect *GetClassEffect() const{ return pClsEff; }
	inline deClassEffectColorMatrix *GetClassEffectColorMatrix() const{ return pClsEffClrMat; }
	inline deClassEffectDistortImage *GetClassEffectDistortImage() const{ return pClsEffDistImg; }
	inline deClassEffectFilterKernel *GetClassEffectFilterKernel() const{ return pClsEffFilKer; }
	inline deClassEffectOverlayImage *GetClassEffectOverlayImage() const{ return pClsEffOverImg; }
	inline deClassEngine *GetClassEngine() const{ return pClsEngine; }
	inline deClassEnvMapProbe *GetClassEnvMapProbe() const{ return pClsEnvMapProbe; }
	inline deClassFileReader *GetClassFileReader() const{ return pClsFileReader; }
	inline deClassFileWriter *GetClassFileWriter() const{ return pClsFileWriter; }
	inline deClassFileSystem *GetClassFileSystem() const{ return pClsFileSys; }
	inline deClassFont *GetClassFont() const{ return pClsFnt; }
	inline deClassFontBuilder *GetClassFontBuilder() const{ return pClsFontBuilder; }
	inline deClassForceField *GetClassForceField() const{ return pClsFF; }
	inline deClassGame *GetClassGame() const{ return pClsGame; }
	inline deClassGraphicSystem *GetClassGraphicSystem() const{ return pClsGraSys; }
	inline deClassHeightTerrain *GetClassHeightTerrain() const{ return pClsHT; }
	inline deClassImage *GetClassImage() const{ return pClsImg; }
	inline deClassImagePixels *GetClassImagePixels() const{ return pClsImagePixels; }
	inline deClassEditableImage *GetClassEditableImage() const{ return pClsEditableImage; }
	inline deClassInputDevice *GetClassInputDevice() const{ return pClsInpDev; }
	inline deClassInputDeviceAxis *GetClassInputDeviceAxis() const{ return pClsInpDevAxis; }
	inline deClassInputDeviceButton *GetClassInputDeviceButton() const{ return pClsInpDevBtn; }
	inline deClassInputDeviceFeedback *GetClassInputDeviceFeedback() const{ return pClsInpDevFb; }
	inline deClassInputDeviceComponent *GetClassInputDeviceComponent() const{ return pClsInpDevComp; }
	inline deClassInputEvent *GetClassInputEvent() const{ return pClsInpEvent; }
	inline deClassInputSystem *GetClassInputSystem() const{ return pClsInpSys; }
	inline deClassLanguagePack *GetClassLanguagePack() const{ return pClsLP; }
	inline deClassLanguagePackBuilder *GetClassLanguagePackBuilder() const{ return pClsLangPackBuilder; }
	inline deClassLayerMask *GetClassLayerMask() const{ return pClsLyM; }
	inline deClassLight *GetClassLight() const{ return pClsLig; }
	inline deClassLocomotion *GetClassLocomotion() const{ return pClsLoco; }
	inline deClassLumimeter *GetClassLumimeter() const{ return pClsLM; }
	inline deClassMath *GetClassMath() const{ return pClsMath; }
	inline deClassMatrix *GetClassMatrix() const{ return pClsMat; }
	inline deClassMatrix4 *GetClassMatrix4() const{ return pClsMat4; }
	inline deClassMemoryFile *GetClassMemoryFile() const{ return pClsMemoryFile; }
	inline deClassMicrophone *GetClassMicrophone() const{ return pClsMic; }
	inline deClassModel *GetClassModel() const{ return pClsMdl; }
	inline deClassModelBuilder *GetClassModelBuilder() const{ return pClsModelBuilder; }
	inline deClassModuleParameter *GetClassModuleParameter() const{ return pClsModPar; }
	inline deClassMutableID *GetClassMutableID() const{ return pClsMUID; }
	inline deClassNavigationInfo *GetClassNavigationInfo() const{ return pClsNavInfo; }
	inline deClassNavigationBlocker *GetClassNavigationBlocker() const{ return pClsNavBlocker; }
	inline deClassNavigationSpace *GetClassNavigationSpace() const{ return pClsNavSpace; }
	inline deClassNavigator *GetClassNavigator() const{ return pClsNavigator; }
	inline deClassNavigatorPath *GetClassNavigatorPath() const{ return pClsNavigatorPath; }
	inline deClassNetworkMessage *GetClassNetworkMessage() const{ return pClsNM; }
	inline deClassNetworkState *GetClassNetworkState() const{ return pClsNS; }
	inline deClassNetworkStateListener *GetClassNetworkStateListener() const{ return pClsNSL; }
	inline deClassOcclusionMesh *GetClassOcclusionMesh() const{ return pClsOccM; }
	inline deClassOcclusionMeshBuilder *GetClassOcclusionMeshBuilder() const{ return pClsOccMBuilder; }
	inline deClassParticleEmitter *GetClassParticleEmitter() const{ return pClsPE; }
	inline deClassParticleEmitterController *GetClassParticleEmitterController() const{ return pClsPEC; }
	inline deClassParticleEmitterInstance *GetClassParticleEmitterInstance() const{ return pClsPEI; }
	inline deClassParticleEmitterInstanceListener *GetClassParticleEmitterInstanceListener() const{ return pClsPEIL; }
	inline deClassPhysicsSystem *GetClassPhysicsSystem() const{ return pClsPhySys; }
	inline deClassPoint3 *GetClassPoint3() const{ return pClsPt3; }
	inline deClassPoint *GetClassPoint() const{ return pClsPt; }
	inline deClassPropField *GetClassPropField() const{ return pClsPF; }
	inline deClassPropFieldListener *GetClassPropFieldListener() const{ return pClsPFL; }
	inline deClassQuaternion *GetClassQuaternion() const{ return pClsQuat; }
	inline deClassResourceListener *GetClassResourceListener() const{ return pClsRN; }
	inline deClassRig *GetClassRig() const{ return pClsRig; }
	inline deClassRigBuilder *GetClassRigBuilder() const{ return pClsRigBuilder; }
	inline deClassRuntimeMeter *GetClassRuntimeMeter() const{ return pClsRTM; }
	inline deClassSafeArray *GetClassSafeArray() const{ return pClsSA; }
	inline deClassScriptSystem *GetClassScriptSystem() const{ return pClsScrSys; }
	inline deClassServer *GetClassServer() const{ return pClsSvr; }
	inline deClassServerListener *GetClassServerListener() const{ return pClsSvrL; }
	inline deClassShapeList *GetClassShapeList() const{ return pClsShaList; }
	inline deClassService *GetClassService() const{ return pClsService; }
	inline deClassServiceListener *GetClassServiceListener() const{ return pClsServiceListener; }
	inline deClassServiceObject *GetClassServiceObject() const{ return pClsServiceObject; }
	inline deClassSkin *GetClassSkin() const{ return pClsSkin; }
	inline deClassSkinBuilder *GetClassSkinBuilder() const{ return pClsSkinBuilder; }
	inline deClassSky *GetClassSky() const{ return pClsSky; }
	inline deClassSkyBody *GetClassSkyBody() const{ return pClsSkyBody; }
	inline deClassSkyController *GetClassSkyController() const{ return pClsSkyCtrl; }
	inline deClassSkyInstance *GetClassSkyInstance() const{ return pClsSkyInst; }
	inline deClassSkyLayer *GetClassSkyLayer() const{ return pClsSkyLayer; }
	inline deClassSkyLink *GetClassSkyLink() const{ return pClsSkyLink; }
	inline deClassSkyTarget *GetClassSkyTarget() const{ return pClsSkyTarget; }
	inline deClassSmoothFloat *GetClassSmoothFloat() const{ return pClsSmFlt; }
	inline deClassSmoothDouble *GetClassSmoothDouble() const{ return pClsSmDbl; }
	inline deClassSmoothVector *GetClassSmoothVector() const{ return pClsSmVec; }
	inline deClassSmoothVector2 *GetClassSmoothVector2() const{ return pClsSmVec2; }
	inline deClassSmoothDVector *GetClassSmoothDVector() const{ return pClsSmDVec; }
	inline deClassSound *GetClassSound() const{ return pClsSnd; }
	inline deClassSoundLevelMeter *GetClassSoundLevelMeter() const{ return pClsSoundLevelMeter; }
	inline deClassSoundLevelMeterSpeaker *GetClassSoundLevelMeterSpeaker() const{ return pClsSoundLevelMeterSpeaker; }
	inline deClassSoundLevelMeterListener *GetClassSoundLevelMeterListener() const{ return pClsSoundLevelMeterListener; }
	inline deClassSpeaker *GetClassSpeaker() const{ return pClsSpk; }
	inline deClassStringID *GetClassStringID() const{ return pClsSID; }
	inline deClassSynthesizer *GetClassSynthesizer() const{ return pClsSyn; }
	inline deClassSynthesizerController *GetClassSynthesizerController() const{ return pClsSynCtrl; }
	inline deClassSynthesizerEffect *GetClassSynthesizerEffect() const{ return pClsSynEff; }
	inline deClassSynthesizerInstance *GetClassSynthesizerInstance() const{ return pClsSynI; }
	inline deClassSynthesizerSource *GetClassSynthesizerSource() const{ return pClsSynS; }
	inline deClassSynthesizerSystem *GetClassSynthesizerSystem() const{ return pClsSynthesizerSystem; }
	inline deClassSEStretch *GetClassSEStretch() const{ return pClsSEStretch; }
	inline deClassSSSound *GetClassSSSound() const{ return pClsSSSound; }
	inline deClassSSWave *GetClassSSWave() const{ return pClsSSWave; }
	inline deClassSSChain *GetClassSSChain() const{ return pClsSSChain; }
	inline deClassSSSynthesizer *GetClassSSSynthesizer() const{ return pClsSSSyn; }
	inline deClassSSGroup *GetClassSSGroup() const{ return pClsSSGroup; }
	inline deClassTexMatrix *GetClassTexMatrix() const{ return pClsTexMatrix; }
	inline deClassTexMatrix2 *GetClassTexMatrix2() const{ return pClsTexMatrix2; }
	inline deClassTouchSensor *GetClassTouchSensor() const{ return pClsTS; }
	inline deClassTouchSensorListener *GetClassTouchSensorListener() const{ return pClsTSL; }
	inline deClassUnicodeString *GetClassUnicodeString() const{ return pClsUS; }
	inline deClassUniqueID *GetClassUniqueID() const{ return pClsUID; }
	inline deClassUTF8Decoder *GetClassUTF8Decoder() const{ return pClsU8D; }
	inline deClassVector *GetClassVector() const{ return pClsVec; }
	inline deClassVector2 *GetClassVector2() const{ return pClsVec2; }
	inline deClassVideo *GetClassVideo() const{ return pClsVid; }
	inline deClassVideoPlayer *GetClassVideoPlayer() const{ return pClsVP; }
	inline deClassVRSystem *GetClassVRSystem() const{ return pClsVRSys; }
	inline deClassWorld *GetClassWorld() const{ return pClsWorld; }
	
	inline dsClass *GetClassResourceLoaderType() const{ return pClsResourceLoaderType; }
	
	/** @name Management */
	/*@{*/
	/** Retrieves the lock manager. */
//	inline dedsLockManager *GetLockManager() const{ return pLockManager; }
	/*@}*/
	
	// helper functions
	inline dsValue *GetGameObject() const{ return pGameObj; }
	const decVector &GetVector( dsRealObject *myself ) const;
	void PushVector( dsRunTime *rt, const decVector &v );
	const decQuaternion &GetQuaternion( dsRealObject *myself ) const;
	void PushQuaternion( dsRunTime *rt, const decQuaternion &q );
	const decMatrix &GetMatrix(dsRealObject *myself) const;
	void PushMatrix( dsRunTime *rt, const decMatrix &m );
	const decColor &GetColor(dsRealObject *myself) const;
	void PushColor( dsRunTime *rt, const decColor &c );
	const decPoint &GetPoint(dsRealObject *myself) const;
	void PushPoint( dsRunTime *rt, const decPoint &p );
	const decPoint3 &GetPoint3( dsRealObject *myself ) const;
	void PushPoint3( dsRunTime *rt, const decPoint3 &pt );
	
	inline const decString &GetRestartInfo() const{ return pRestartInfo; }
	void RequestRestart( const char *info );
	
	/** \brief Adds a value to delete later. */
	void AddValueDeleteLater( dsValue *value );
	/** \brief Delete all values registered to be deleted later. */
	void DeleteValuesDeleteLater();
	
	/** \brief Add dragonscript exception to the module trace. */
	void SetErrorTraceDS( const duException &exception );
	
	/** \brief Log dragonscript exception. */
	void LogExceptionDS( const duException &exception );
	/*@}*/
	
private:
	void pLoadBasicPackage();
	void pLoadGamePackage( const char *directory, const char *gameClass );
	void pAddScripts( dsPackage *package, deVirtualFileSystem &vfs, const char *pathDir );
	bool pIsFileType(const char *Filename, const char *Extension);
	int pGetConstantValue(dsClass *Class, const char *name) const;
	bool pCallFunction(const char *name);
	decString BuildFullName( const dsClass *theClass ) const;
	void pAddVFSContainerHideScriptDirectory();
	void pRemoveVFSContainerHideScriptDirectory();
	void pPreprocessEventDpiAware(deInputEvent &event);
	void pPreprocessMouseMoveDpiAware(deInputEvent &event);
	
public:
	void pAddExceptionTrace( deErrorTracePoint *tracePoint );
	void LogExceptionDSTrace();
	void pAddSendEventTrace( deErrorTracePoint &tracePoint, const deInputEvent &event );
	void pAddVectorTrace( deErrorTraceValue *traceValue, const decVector &vector, const char *name );
	void pAddColliderTrace( deErrorTracePoint *tracePoint, deCollider *collider, const char *name );
};

#endif
