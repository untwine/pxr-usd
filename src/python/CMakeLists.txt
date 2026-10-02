add_library(pyUsd SHARED
    module.cpp
    wrapAPISchemaBase.cpp
    wrapAttribute.cpp
    wrapAttributeLimits.cpp
    wrapAttributeQuery.cpp
    wrapClipsAPI.cpp
    wrapCollectionAPI.cpp
    wrapCollectionMembershipQuery.cpp
    wrapColorSpaceAPI.cpp
    wrapColorSpaceDefinitionAPI.cpp
    wrapCommon.cpp
    wrapEditContext.cpp
    wrapEditTarget.cpp
    wrapFlattenUtils.cpp
    wrapInherits.cpp
    wrapInterpolation.cpp
    wrapModelAPI.cpp
    wrapNamespaceEditor.cpp
    wrapNotice.cpp
    wrapObject.cpp
    wrapPayloads.cpp
    wrapPrim.cpp
    wrapPrimCompositionQuery.cpp
    wrapPrimDefinition.cpp
    wrapPrimFlags.cpp
    wrapPrimRange.cpp
    wrapPrimTypeInfo.cpp
    wrapProperty.cpp
    wrapReferences.cpp
    wrapRelationship.cpp
    wrapResolveInfo.cpp
    wrapResolveTarget.cpp
    wrapSchemaBase.cpp
    wrapSchemaRegistry.cpp
    wrapSpecializes.cpp
    wrapStage.cpp
    wrapStageCache.cpp
    wrapStageCacheContext.cpp
    wrapStageLoadRules.cpp
    wrapStagePopulationMask.cpp
    wrapTimeCode.cpp
    wrapTokens.cpp
    wrapTyped.cpp
    wrapUtils.cpp
    wrapVariantSets.cpp
    wrapVersion.cpp
)

set_target_properties(pyUsd
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME _usd
        RUNTIME_OUTPUT_DIRECTORY "$<CONFIG>/pxr/Usd"
        LIBRARY_OUTPUT_DIRECTORY "$<CONFIG>/pxr/Usd"
        ARCHIVE_OUTPUT_DIRECTORY "$<CONFIG>/pxr/Usd"
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(pyUsd
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(pyUsd
    PUBLIC
        usd
        pxr::boost-python
)

if(NOT WIN32)
    set(_rpath_prefix "$ORIGIN")
    if(APPLE)
        set(_rpath_prefix "@loader_path")
    endif()

    if(SKBUILD)
        set(_pyUsd_rpath "${_rpath_prefix}/../.libs")
    else()
        file(RELATIVE_PATH _pyUsd_libdir_rel
            "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_PYTHON_LIBDIR}/pxr/Usd"
            "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
        )
        set(_pyUsd_rpath "${_rpath_prefix}/${_pyUsd_libdir_rel}")
    endif()

    set_target_properties(pyUsd PROPERTIES
        INSTALL_RPATH "${_pyUsd_rpath}"
        INSTALL_RPATH_USE_LINK_PATH TRUE
    )
endif()

if(WIN32)
    set_target_properties(pyUsd PROPERTIES SUFFIX ".pyd")
elseif(APPLE)
    set_target_properties(pyUsd PROPERTIES SUFFIX ".so")
endif()

if(MSVC)
    target_compile_options(pyUsd PRIVATE /bigobj)
endif()

target_compile_definitions(
    pyUsd
    PRIVATE
        BOOST_BIND_GLOBAL_PLACEHOLDERS
        BOOST_PYTHON_NO_PY_SIGNATURES
        MFB_PACKAGE_MODULE=Usd
        MFB_PACKAGE_NAME=usd
        MFB_ALT_PACKAGE_NAME=usd
)

file(
    GENERATE
    OUTPUT "$<TARGET_FILE_DIR:pyUsd>/__init__.py"
    INPUT "__init__.py"
)

install(
    TARGETS pyUsd EXPORT ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PYTHON_LIBDIR}/pxr/Usd
    RUNTIME DESTINATION ${CMAKE_INSTALL_PYTHON_LIBDIR}/pxr/Usd
)
install(
    FILES __init__.py
    DESTINATION ${CMAKE_INSTALL_PYTHON_LIBDIR}/pxr/Usd
)
