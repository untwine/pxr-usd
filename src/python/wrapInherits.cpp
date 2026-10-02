//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include <pxr/usd/pxr.h>
#include <pxr/usd/inherits.h>
#include <pxr/tf/pyResultConversions.h>

#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/operators.hpp>

using std::string;

USD_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

void wrapUsdInherits()
{
    class_<UsdInherits>("Inherits", no_init)
        .def("AddInherit", &UsdInherits::AddInherit,
             (arg("primPath"),
              arg("position")=UsdListPositionBackOfPrependList))
        .def("RemoveInherit", &UsdInherits::RemoveInherit, arg("primPath"))
        .def("ClearInherits", &UsdInherits::ClearInherits)
        .def("SetInherits", &UsdInherits::SetInherits)
        .def("GetAllDirectInherits", &UsdInherits::GetAllDirectInherits,
             return_value_policy<TfPySequenceToList>())
        .def("GetPrim", (UsdPrim (UsdInherits::*)()) &UsdInherits::GetPrim)
        .def(!self)
        ;
}
