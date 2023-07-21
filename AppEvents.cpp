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
		if (!AppMgr.HasDashLoaded) Native::CreateThreadExQ(OnDashboardLoad);
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

void OnDashboardLoad() {
	Native::Sleep(Native::DecVal(0x8589EB42) /*1000*/);
	AppMgr.HasDashLoaded = true;

	while (!Teapot::Initialized && !Teapot::InitError) Native::Sleep(0);

	switch (Teapot::AuthFlag) {
		case AUTHFLAG_EXPIRED: {
			debug("[INFO] UI_Expired Has Allocated");
			UI_ExpiredMsg(); 
			break;
		}
		case AUTHFLAG_NOEXIST: {
			if (DLaunch.Vars.bDefaultPathSet) {
				debug("Wait 6 seconds ran!");
				Native::Sleep(6000);
			}
			debug("[INFO] UI_REGISTER Has Allocated");
			REGISTER_EMAIL(L"");
			break;
		}
	}
}