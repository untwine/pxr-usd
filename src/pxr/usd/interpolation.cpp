//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include <pxr/usd/pxr.h>
#include <pxr/usd/interpolation.h>

#include <pxr/tf/enum.h>
#include <pxr/tf/registryManager.h>

USD_NAMESPACE_OPEN_SCOPE


TF_REGISTRY_FUNCTION(TfEnum)
{
    TF_ADD_ENUM_NAME(UsdInterpolationTypeHeld, "Held");
    TF_ADD_ENUM_NAME(UsdInterpolationTypeLinear, "Linear");
}

USD_NAMESPACE_CLOSE_SCOPE

