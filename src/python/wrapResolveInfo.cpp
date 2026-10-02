//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include <pxr/usd/pxr.h>
#include <pxr/usd/resolveInfo.h>
#include <pxr/tf/pyEnum.h>

#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/return_internal_reference.hpp>

using std::string;

USD_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

void wrapUsdResolveInfo()
{
    class_<UsdResolveInfo>("ResolveInfo")
        .def("GetSource", &UsdResolveInfo::GetSource)
        .def("HasAuthoredValueOpinion",
             &UsdResolveInfo::HasAuthoredValueOpinion)
        .def("HasAuthoredValue",
             &UsdResolveInfo::HasAuthoredValue)
        .def("GetNode", &UsdResolveInfo::GetNode)
        .def("ValueIsBlocked", &UsdResolveInfo::ValueIsBlocked)
        .def("ValueSourceMightBeTimeVarying",
             &UsdResolveInfo::ValueSourceMightBeTimeVarying)
        .def("HasNextWeakerInfo",
             &UsdResolveInfo::HasNextWeakerInfo)
        .def("GetNextWeakerInfo",
             &UsdResolveInfo::GetNextWeakerInfo,
             return_internal_reference<>())
        ;

    TfPyWrapEnum<UsdResolveInfoSource>();
}
