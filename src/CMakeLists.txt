configure_file(
    pxr/usd/pxr.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/pxr/usd/pxr.h
    @ONLY
)

add_library(usd
    pxr/usd/apiSchemaBase.cpp
    pxr/usd/attribute.cpp
    pxr/usd/attributeLimits.cpp
    pxr/usd/attributeQuery.cpp
    pxr/usd/clip.cpp
    pxr/usd/clipCache.cpp
    pxr/usd/clipSet.cpp
    pxr/usd/clipSetDefinition.cpp
    pxr/usd/clipsAPI.cpp
    pxr/usd/collectionAPI.cpp
    pxr/usd/collectionMembershipQuery.cpp
    pxr/usd/collectionPredicateLibrary.cpp
    pxr/usd/colorSpaceAPI.cpp
    pxr/usd/colorSpaceDefinitionAPI.cpp
    pxr/usd/common.cpp
    pxr/usd/debugCodes.cpp
    pxr/usd/editContext.cpp
    pxr/usd/editTarget.cpp
    pxr/usd/errors.cpp
    pxr/usd/flattenUtils.cpp
    pxr/usd/inherits.cpp
    pxr/usd/instanceCache.cpp
    pxr/usd/instanceKey.cpp
    pxr/usd/interpolation.cpp
    pxr/usd/interpolators.cpp
    pxr/usd/modelAPI.cpp
    pxr/usd/namespaceEditor.cpp
    pxr/usd/notice.cpp
    pxr/usd/object.cpp
    pxr/usd/payloads.cpp
    pxr/usd/prim.cpp
    pxr/usd/primCompositionQuery.cpp
    pxr/usd/primData.cpp
    pxr/usd/primDataHandle.cpp
    pxr/usd/primDefinition.cpp
    pxr/usd/primFlags.cpp
    pxr/usd/primRange.cpp
    pxr/usd/primTypeInfo.cpp
    pxr/usd/primTypeInfoCache.cpp
    pxr/usd/property.cpp
    pxr/usd/references.cpp
    pxr/usd/relationship.cpp
    pxr/usd/resolveInfo.cpp
    pxr/usd/resolveTarget.cpp
    pxr/usd/resolver.cpp
    pxr/usd/schemaBase.cpp
    pxr/usd/schemaRegistry.cpp
    pxr/usd/specializes.cpp
    pxr/usd/stage.cpp
    pxr/usd/stageCache.cpp
    pxr/usd/stageCacheContext.cpp
    pxr/usd/stageLoadRules.cpp
    pxr/usd/stagePopulationMask.cpp
    pxr/usd/timeCode.cpp
    pxr/usd/tokens.cpp
    pxr/usd/typed.cpp
    pxr/usd/valueUtils.cpp
    pxr/usd/variantSets.cpp
)

target_include_directories(usd
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Compile features can only raise the effective standard, so this can't be
# undercut by a toolchain file that pre-sets CMAKE_CXX_STANDARD lower.
target_compile_features(usd PUBLIC cxx_std_17)

target_link_libraries(usd
    PUBLIC
        pxr::arch
        pxr::gf
        pxr::kind
        pxr::pcp
        pxr::sdf
        pxr::ar
        pxr::plug
        pxr::tf
        pxr::trace
        pxr::ts
        pxr::vt
        pxr::work
        TBB::tbb
)

if(NOT WIN32 AND NOT SKBUILD)
    set_target_properties(usd PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

if(BUILD_PYTHON_BINDINGS)
    target_sources(usd
        PRIVATE
            pxr/usd/moduleDeps.cpp
            pxr/usd/pyConversions.cpp
            pxr/usd/pyEditContext.cpp
    )

    target_link_libraries(usd
        PUBLIC
            pxr::boost-python
    )

    add_subdirectory(python)
endif()

if (ENABLE_PRECOMPILED_HEADERS)
    target_precompile_headers(usd
        PRIVATE
            pxr/usd/pch.h
    )
endif()

if(MSVC)
    target_compile_options(usd PRIVATE /bigobj)
endif()

set_target_properties(usd
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        OUTPUT_NAME "PxrUsd"
)

target_compile_definitions(usd
    PRIVATE
        USD_EXPORTS=1
        MFB_PACKAGE_MODULE=Usd
        MFB_PACKAGE_NAME=usd
        MFB_ALT_PACKAGE_NAME=usd
)

if(SKBUILD)
    set(_lib_prefix "../../.libs/")
    set(_build_dir "../pluginfo/usd/resources")
else()
    set(_lib_prefix "../../")
    set(_build_dir "usd/usd/resources")
endif()

set(PLUG_INFO_LIBRARY_PATH "${_lib_prefix}$<TARGET_FILE_NAME:usd>")
configure_file(pxr/usd/plugInfo.json plugInfo.json @ONLY)

file(
    GENERATE
    OUTPUT "$<TARGET_FILE_DIR:usd>/usd/usd/resources/plugInfo_$<CONFIG>.json"
    INPUT "${CMAKE_CURRENT_BINARY_DIR}/plugInfo.json"
)

# The schema registry reads generatedSchema.usda next to plugInfo.json.
add_custom_command(TARGET usd POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/pxr/usd/generatedSchema.usda"
        "$<TARGET_FILE_DIR:usd>/usd/usd/resources/generatedSchema.usda"
)

install(
    FILES "$<TARGET_FILE_DIR:usd>/usd/usd/resources/plugInfo_$<CONFIG>.json"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/${_build_dir}"
    RENAME plugInfo.json
)

install(
    FILES pxr/usd/generatedSchema.usda
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/${_build_dir}"
)

install(
    FILES pxr/usd/schema.usda
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/${_build_dir}/usd"
)

target_sources(usd
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/pxr/usd/pxr.h
            pxr/usd/api.h
            pxr/usd/apiSchemaBase.h
            pxr/usd/attribute.h
            pxr/usd/attributeLimits.h
            pxr/usd/attributeQuery.h
            pxr/usd/clipsAPI.h
            pxr/usd/collectionAPI.h
            pxr/usd/collectionMembershipQuery.h
            pxr/usd/collectionPredicateLibrary.h
            pxr/usd/colorSpaceAPI.h
            pxr/usd/colorSpaceDefinitionAPI.h
            pxr/usd/common.h
            pxr/usd/debugCodes.h
            pxr/usd/editContext.h
            pxr/usd/editTarget.h
            pxr/usd/errors.h
            pxr/usd/flattenUtils.h
            pxr/usd/inherits.h
            pxr/usd/interpolation.h
            pxr/usd/modelAPI.h
            pxr/usd/namespaceEditor.h
            pxr/usd/notice.h
            pxr/usd/object.h
            pxr/usd/payloads.h
            pxr/usd/prim.h
            pxr/usd/primCompositionQuery.h
            pxr/usd/primData.h
            pxr/usd/primDataHandle.h
            pxr/usd/primDefinition.h
            pxr/usd/primFlags.h
            pxr/usd/primRange.h
            pxr/usd/primTypeInfo.h
            pxr/usd/property.h
            pxr/usd/references.h
            pxr/usd/relationship.h
            pxr/usd/resolveInfo.h
            pxr/usd/resolveTarget.h
            pxr/usd/resolver.h
            pxr/usd/schemaBase.h
            pxr/usd/schemaRegistry.h
            pxr/usd/specializes.h
            pxr/usd/stage.h
            pxr/usd/stageCache.h
            pxr/usd/stageCacheContext.h
            pxr/usd/stageLoadRules.h
            pxr/usd/stagePopulationMask.h
            pxr/usd/timeCode.h
            pxr/usd/tokens.h
            pxr/usd/typed.h
            pxr/usd/variantSets.h
)

if(BUILD_PYTHON_BINDINGS)
    target_sources(usd
        PUBLIC
            FILE_SET public_headers
            TYPE HEADERS
            BASE_DIRS
                ${CMAKE_CURRENT_SOURCE_DIR}
            FILES
                pxr/usd/pyConversions.h
                pxr/usd/pyEditContext.h
    )
endif()

install(
    TARGETS usd EXPORT ${PROJECT_NAME}
    FILE_SET public_headers
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT ${PROJECT_NAME}
    FILE pxr-usd-targets.cmake
    NAMESPACE pxr::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/pxr-usd
)
