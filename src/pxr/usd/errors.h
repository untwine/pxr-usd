//
// Copyright 2021 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_USD_USD_ERRORS_H
#define PXR_USD_USD_ERRORS_H

/// \file usd/errors.h

#include <pxr/usd/pxr.h>
#include <pxr/usd/api.h>
#include <pxr/tf/exception.h>

USD_NAMESPACE_OPEN_SCOPE

/// \class UsdExpiredPrimAccessError
///
/// Usd throws this exception when code attempts to access an invalid
/// (i.e. expired or null) prim.
class UsdExpiredPrimAccessError : public TfBaseException
{
public:
    using TfBaseException::TfBaseException;
    USD_API virtual ~UsdExpiredPrimAccessError() override;
};

USD_NAMESPACE_CLOSE_SCOPE

#endif
