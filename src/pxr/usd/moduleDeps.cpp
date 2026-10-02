// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
////////////////////////////////////////////////////////////////////////

#include <pxr/usd/pxr.h>
#include <pxr/tf/registryManager.h>
#include <pxr/tf/scriptModuleLoader.h>
#include <pxr/tf/token.h>

#include <vector>

USD_NAMESPACE_OPEN_SCOPE

TF_REGISTRY_FUNCTION(TfScriptModuleLoader) {
    // List of direct dependencies for this library.
    const std::vector<TfToken> reqs = {
        TfToken("arch"),
        TfToken("gf"),
        TfToken("kind"),
        TfToken("pcp"),
        TfToken("sdf"),
        TfToken("ar"),
        TfToken("plug"),
        TfToken("tf"),
        TfToken("trace"),
        TfToken("ts"),
        TfToken("vt"),
        TfToken("work"),
        TfToken("boost-python")
    };
    TfScriptModuleLoader::GetInstance().
        RegisterLibrary(TfToken("usd"), TfToken("pxr.Usd"), reqs);
}

USD_NAMESPACE_CLOSE_SCOPE
