/* Generated File. Do not edit */
#include <dragengine/systems/deModuleSystem.h>

extern deTObjectReference<deInternalModule> deadRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> dearRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deaiRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deanimRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deanRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deoalRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deCRRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> decrbRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deFontRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deftRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> degnRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deoglRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> depngRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> depng3DRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deTgaRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deJpegRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deIesRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deWebpRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deWebp3DRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> dewiRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> delpRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> demdlRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> denbRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deoccmRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> depbRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> dermRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> dedsRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> desmRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deoggRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> desynRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> dethRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deapngRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> dewebmRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> fbxModelRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> fbxRigRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> fbxAnimRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> fbxSkinRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> denvrRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deoxrRegisterInternalModule(deModuleSystem*);
extern deTObjectReference<deInternalModule> deModioRegisterInternalModule(deModuleSystem*);

static const deModuleSystem::FPRegisterInternalModule vInternalModuleFunctionsPriority[]{
	deadRegisterInternalModule,
	nullptr};

static const deModuleSystem::FPRegisterInternalModule vInternalModuleFunctions[]{
	dearRegisterInternalModule,
	deaiRegisterInternalModule,
	deanimRegisterInternalModule,
	deanRegisterInternalModule,
	deoalRegisterInternalModule,
	deCRRegisterInternalModule,
	decrbRegisterInternalModule,
	deFontRegisterInternalModule,
	deftRegisterInternalModule,
	degnRegisterInternalModule,
	deoglRegisterInternalModule,
	depngRegisterInternalModule,
	depng3DRegisterInternalModule,
	deTgaRegisterInternalModule,
	deJpegRegisterInternalModule,
	deIesRegisterInternalModule,
	deWebpRegisterInternalModule,
	deWebp3DRegisterInternalModule,
	dewiRegisterInternalModule,
	delpRegisterInternalModule,
	demdlRegisterInternalModule,
	denbRegisterInternalModule,
	deoccmRegisterInternalModule,
	depbRegisterInternalModule,
	dermRegisterInternalModule,
	dedsRegisterInternalModule,
	desmRegisterInternalModule,
	deoggRegisterInternalModule,
	desynRegisterInternalModule,
	dethRegisterInternalModule,
	deapngRegisterInternalModule,
	dewebmRegisterInternalModule,
	fbxModelRegisterInternalModule,
	fbxRigRegisterInternalModule,
	fbxAnimRegisterInternalModule,
	fbxSkinRegisterInternalModule,
	denvrRegisterInternalModule,
	deoxrRegisterInternalModule,
	deModioRegisterInternalModule,
	nullptr};

extern "C" {
MOD_ENTRY_POINT_ATTR const deModuleSystem::FPRegisterInternalModule* DEInternalModuleFunctionsPriority(){
	return (deModuleSystem::FPRegisterInternalModule*)&vInternalModuleFunctionsPriority;
}
MOD_ENTRY_POINT_ATTR const deModuleSystem::FPRegisterInternalModule* DEInternalModuleFunctions(){
	return (deModuleSystem::FPRegisterInternalModule*)&vInternalModuleFunctions;
}
}
