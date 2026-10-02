//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include <pxr/usd/pxr.h>
#include <pxr/usd/interpolation.h>
#include <pxr/tf/pyEnum.h>

USD_NAMESPACE_USING_DIRECTIVE

void
wrapUsdInterpolationType()
{
    TfPyWrapEnum<UsdInterpolationType>();
}
