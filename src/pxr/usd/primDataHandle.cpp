//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include <pxr/usd/pxr.h>
#include <pxr/usd/primDataHandle.h>
#include <pxr/usd/primData.h>

USD_NAMESPACE_OPEN_SCOPE


// defined in primData.cpp.
std::string Usd_DescribePrimData(const Usd_PrimData *p);

std::string
Usd_PrimDataHandle::GetDescription(SdfPath const &proxyPrimPath) const
{
    return Usd_DescribePrimData(_p.get(), proxyPrimPath);
}


USD_NAMESPACE_CLOSE_SCOPE

