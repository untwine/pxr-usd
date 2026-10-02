//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include <pxr/usd/pxr.h>
#include <pxr/usd/common.h>
#include <pxr/usd/object.h>
#include <pxr/usd/stage.h>
#include <pxr/usd/stageCache.h>
#include <pxr/tf/pyEnum.h>

#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/enum.hpp>


USD_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

void wrapUsdCommon()
{
    def("Describe", (std::string (*)(const UsdObject &)) UsdDescribe);
    def("Describe", (std::string (*)(const UsdStageWeakPtr &)) UsdDescribe);
    def("Describe", (std::string (*)(const UsdStageCache &)) UsdDescribe);

    TfPyWrapEnum<UsdListPosition>();
    TfPyWrapEnum<UsdLoadPolicy>();
    enum_<UsdSchemaKind>("SchemaKind")
        .value("Invalid", UsdSchemaKind::Invalid)
        .value("AbstractBase", UsdSchemaKind::AbstractBase)
        .value("AbstractTyped", UsdSchemaKind::AbstractTyped)
        .value("ConcreteTyped", UsdSchemaKind::ConcreteTyped)
        .value("NonAppliedAPI", UsdSchemaKind::NonAppliedAPI)
        .value("SingleApplyAPI", UsdSchemaKind::SingleApplyAPI)
        .value("MultipleApplyAPI", UsdSchemaKind::MultipleApplyAPI)
    ;
}
