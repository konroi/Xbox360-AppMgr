#include "stdafx.h"
#include "AppManager.h"

#include "../hud/dashboard/DashboardHooks.h"
#include "../cheats/CheatEngine.h"
#include "../GTAScratch.h"
#include "MSPSpoof.h"

void OnDashboardLoad();

void AppManager::Events::OnTitleLoad(AppManager::C_AppInstance *pInstance) {
	AppMgrDbg("[AppMgr] Title Loaded -> %ws", pInstance->CachedDllNameW.cstr);

	pInstance->RegisterActiveTitle();
	CheatEngine::Dispose();

	if (pInstance->IsDefaultApp()) {
		if (AppMgrIsTitleRealDash(pInstance)) {
			dashboard::hooks::install(pInstance);
		}
		if (!AppMgr.HasDashLoaded) OnDashboardLoad();
		AppMgrDbg("[AppMgr] Default app has loaded.");
		return;
	}

	if (Teapot::AuthFlag != AUTHFLAG_ACTIVE) 
		return;
	
	CheatEngine::Init(pInstance);
}

void AppManager::Events::OnDllLoad(AppManager::C_AppInstance *pInstance) {
	AppMgrDbg("[AppMgr] Dll Loaded -> %ws", pInstance->CachedDllNameW.cstr);

	if(wcsstr(pInstance->CachedDllNameW.cstr, L"Guide.MP.Purchase") != NULL) {
		//MSPSpoof::Exec(pInstance);
	}
}
