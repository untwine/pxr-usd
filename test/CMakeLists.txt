if (WIN32)
    # Patch CMake's 'add_test' to extend the DLL search path on Windows. Use the
    # target name associated with the test, if it is also a CMake target.

    macro(add_test)
        _add_test(${ARGV})

        cmake_parse_arguments("" "" "NAME" "" ${ARGV})
        if (TARGET ${_NAME})
            set(DLL_DIRS $<TARGET_RUNTIME_DLL_DIRS:${_NAME}>)
        else()
            set(DLL_DIRS $<TARGET_RUNTIME_DLL_DIRS:pyUsd>)
        endif()
        set(DLL_DIRS
            ${DLL_DIRS}
            "${onetbb_LIB_DIRS_RELEASE}"
            "${onetbb_BIN_DIRS_RELEASE}"
        )
        set_tests_properties(${_NAME}
            PROPERTIES ENVIRONMENT
            "PATH=$<JOIN:$<SHELL_PATH:${DLL_DIRS}>,\;>")
    endmacro()
endif()

add_subdirectory(plugins)

# Test scripts are laid out per configuration, mirroring upstream's installed
# 'tests' directory.
set(_testDir "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")

set(PATH_SEP $<IF:$<BOOL:${WIN32}>,$<SEMICOLON>,:>)

set(_plugins
    "$<TARGET_FILE_DIR:usd>/usd/usd/resources/plugInfo_$<CONFIG>.json"
    "$<TARGET_FILE_DIR:pxr::sdf>/usd/sdf/resources/plugInfo.json"
    "$<TARGET_FILE_DIR:pxr::ar>/usd/ar/resources/plugInfo.json"
    "${CMAKE_CURRENT_SOURCE_DIR}/plugins/TestUsdUIHintsMetadata/plugInfo.json"
)

set(_pythonPath
    "$<TARGET_FILE_DIR:pxr::pyTf>/../.."
    "$<TARGET_FILE_DIR:pxr::pyGf>/../.."
    "$<TARGET_FILE_DIR:pxr::pyTrace>/../.."
    "$<TARGET_FILE_DIR:pxr::pyTs>/../.."
    "$<TARGET_FILE_DIR:pxr::pyVt>/../.."
    "$<TARGET_FILE_DIR:pxr::pyWork>/../.."
    "$<TARGET_FILE_DIR:pxr::pyPlug>/../.."
    "$<TARGET_FILE_DIR:pxr::pyAr>/../.."
    "$<TARGET_FILE_DIR:pxr::pyKind>/../.."
    "$<TARGET_FILE_DIR:pxr::pySdf>/../.."
    "$<TARGET_FILE_DIR:pxr::pyPcp>/../.."
    "$<TARGET_FILE_DIR:pyUsd>/../.."
)

list(JOIN _plugins "${PATH_SEP}" _plugins)
list(JOIN _pythonPath "${PATH_SEP}" _pythonPath)

set(testWrapper
    "${Python_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/testWrapper.py"
    "--env-var=PXR_PLUGINPATH_NAME=${_plugins}"
)

add_executable(testUsdAttributeArrayEditsCpp testUsdAttributeArrayEdits.cpp)
target_link_libraries(testUsdAttributeArrayEditsCpp PUBLIC pxr::tf usd pxr::vt)
add_test(NAME testUsdAttributeArrayEditsCpp
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdAttributeArrayEditsCpp>"
)

add_executable(testUsdAttributeBlockingCpp testUsdAttributeBlocking.cpp)
target_link_libraries(testUsdAttributeBlockingCpp PUBLIC pxr::tf usd)
add_test(NAME testUsdAttributeBlockingCpp
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdAttributeBlockingCpp>"
)

add_executable(testUsdAttributeInterpolationCpp testUsdAttributeInterpolationCpp.cpp)
target_link_libraries(testUsdAttributeInterpolationCpp PUBLIC usd)
add_test(NAME testUsdAttributeInterpolationCpp
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdAttributeInterpolationCpp>"
)

add_executable(testUsdAttributeLimitsCpp testUsdAttributeLimits.cpp)
target_link_libraries(testUsdAttributeLimitsCpp PUBLIC pxr::sdf pxr::tf usd)
add_test(NAME testUsdAttributeLimitsCpp
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdAttributeLimitsCpp>"
)

add_executable(testUsdCreateAttributeCpp testUsdCreateAttributeCpp.cpp)
target_link_libraries(testUsdCreateAttributeCpp PUBLIC usd)
add_test(NAME testUsdCreateAttributeCpp
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdCreateAttributeCpp>"
)

add_executable(testUsdHardToReach testUsdHardToReach.cpp)
target_link_libraries(testUsdHardToReach PUBLIC usd)
add_test(NAME testUsdHardToReach
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdHardToReach>"
)

add_executable(testUsdMetadataCpp testUsdMetadata.cpp)
target_link_libraries(testUsdMetadataCpp PUBLIC usd)
add_test(NAME testUsdMetadataCpp
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdMetadataCpp>"
)

# Test-specific plugins are only supported with shared library builds.
if (BUILD_SHARED_LIBS)
    add_executable(testUsdResolverChanged testUsdResolverChanged.cpp)
    target_link_libraries(testUsdResolverChanged PUBLIC pxr::arch pxr::plug pxr::sdf pxr::tf usd)
    target_include_directories(testUsdResolverChanged PRIVATE
        $<TARGET_PROPERTY:TestUsdResolverChangedResolver,INTERFACE_INCLUDE_DIRECTORIES>
    )
    add_test(NAME testUsdResolverChanged
        COMMAND ${testWrapper}
            "$<TARGET_FILE:testUsdResolverChanged>"
            --testenv-dir=testUsdResolverChanged
            "--env-var=USD_RESOLVER_CHANGED_PLUGIN=$<TARGET_FILE_DIR:TestUsdResolverChangedResolver>/plugInfo_$<CONFIG>.json"
    )
endif()

add_executable(testUsdInstancingCpp testUsdInstancingCpp.cpp)
target_link_libraries(testUsdInstancingCpp PUBLIC pxr::tf usd pxr::sdf)
add_test(NAME testUsdInstancingCpp
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdInstancingCpp>"
        --testenv-dir=testUsdInstancingCpp.testenv
        --env-var=USD_ASSIGN_PROTOTYPES_DETERMINISTICALLY=1
)

add_executable(testUsdPrimGetDescendants testUsdPrimGetDescendants.cpp)
target_link_libraries(testUsdPrimGetDescendants PUBLIC pxr::tf usd)
add_test(NAME testUsdPrimGetDescendants
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdPrimGetDescendants>"
        --testenv-dir=testUsdPrimGetDescendants.testenv
)

add_executable(testUsdStageNotification testUsdStageNotification.cpp)
target_link_libraries(testUsdStageNotification PUBLIC usd)
add_test(NAME testUsdStageNotification
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdStageNotification>"
)

add_executable(testUsdStageNoPython testUsdStageNoPython.cpp)
target_link_libraries(testUsdStageNoPython PUBLIC usd)
add_test(NAME testUsdStageNoPython
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdStageNoPython>"
        --testenv-dir=testUsdStageNoPython.testenv
)

add_executable(testUsdTemplatedIO testUsdTemplatedIO.cpp)
target_link_libraries(testUsdTemplatedIO PUBLIC usd)
add_test(NAME testUsdTemplatedIO
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdTemplatedIO>"
)

add_executable(testUsdStageThreading testUsdStageThreading.cpp)
target_link_libraries(testUsdStageThreading PUBLIC pxr::arch pxr::tf pxr::work pxr::sdf usd)
add_test(NAME testUsdStageThreading
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdStageThreading>"
        --testenv-dir=testUsdStageThreading
)

add_executable(testUsdThreadedAuthoring testUsdThreadedAuthoring.cpp)
target_link_libraries(testUsdThreadedAuthoring PUBLIC pxr::arch pxr::tf pxr::work pxr::sdf usd TBB::tbb)
add_test(NAME testUsdThreadedAuthoring
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdThreadedAuthoring>"
)

add_executable(testUsdTimeCodeStream testUsdTimeCodeStream.cpp)
target_link_libraries(testUsdTimeCodeStream PUBLIC pxr::tf usd)
add_test(NAME testUsdTimeCodeStream
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdTimeCodeStream>"
)

add_executable(testUsdSchemaBase testUsdSchemaBase.cpp)
target_link_libraries(testUsdSchemaBase PUBLIC usd)
add_test(NAME testUsdSchemaBase
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdSchemaBase>"
)

add_executable(testUsdSchemaRegistryCpp testUsdSchemaRegistryCpp.cpp)
target_link_libraries(testUsdSchemaRegistryCpp PUBLIC usd)
add_test(NAME testUsdSchemaRegistryCpp
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdSchemaRegistryCpp>"
        --testenv-dir=testUsdSchemaRegistry
)

add_executable(testUsdResolveTarget testUsdResolveTarget.cpp)
target_link_libraries(testUsdResolveTarget PUBLIC pxr::pcp pxr::sdf pxr::tf usd pxr::vt)
add_test(NAME testUsdResolveTarget
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdResolveTarget>"
        --testenv-dir=testUsdResolveTarget.testenv
)

add_executable(testUsdTimeValueAuthoringCpp testUsdTimeValueAuthoring.cpp)
target_link_libraries(testUsdTimeValueAuthoringCpp PUBLIC pxr::arch pxr::plug pxr::sdf pxr::tf usd)
add_test(NAME testUsdTimeValueAuthoringCpp
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdTimeValueAuthoringCpp>"
        --testenv-dir=testUsdTimeValueAuthoring
)

add_executable(testUsdUsdzBugGHSA01 testUsdUsdzBugGHSA01.cpp)
target_link_libraries(testUsdUsdzBugGHSA01 PUBLIC pxr::ar pxr::arch pxr::tf pxr::sdf usd)
add_test(NAME testUsdUsdzBugGHSA01
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdUsdzBugGHSA01>"
        --testenv-dir=testUsdUsdzBugGHSA01.testenv
)

add_executable(testUsdUsdcBugGHSA02 testUsdUsdcBugGHSA02.cpp)
target_link_libraries(testUsdUsdcBugGHSA02 PUBLIC pxr::ar pxr::arch pxr::tf pxr::sdf usd)
add_test(NAME testUsdUsdcBugGHSA02
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdUsdcBugGHSA02>"
        --testenv-dir=testUsdUsdcBugGHSA02.testenv
)

if(PXR_PREFER_SAFETY_OVER_SPEED)
    target_compile_definitions(testUsdUsdzBugGHSA01 PRIVATE PXR_PREFER_SAFETY_OVER_SPEED)
    target_compile_definitions(testUsdUsdcBugGHSA02 PRIVATE PXR_PREFER_SAFETY_OVER_SPEED)
endif()

add_executable(testUsdSplinesCpp testUsdSplinesCpp.cpp)
target_link_libraries(testUsdSplinesCpp PUBLIC pxr::tf pxr::ts pxr::sdf usd)
add_test(NAME testUsdSplinesCpp
    COMMAND ${testWrapper}
        "$<TARGET_FILE:testUsdSplinesCpp>"
)

if(BUILD_PYTHON_BINDINGS)
    set(_scripts
        testUsdAppliedAPISchemas.py
        testUsdAttributeBlocking.py
        testUsdAttributeConnections.py
        testUsdAttributeLimits.py
        testUsdAttributeAssetPathVariableExpressions.py
        testUsdAttributeQuery.py
        testUsdBugs.py
        testUsdBug119633.py
        testUsdBug141491.py
        testUsdChangeProcessing.py
        testUsdClasses.py
        testUsdCollectionAPI.py
        testUsdCratePayloadConversionFromVersion07.py
        testUsdCrateRelocates.py
        testUsdCrateSpecific.py
        testUsdCreateProperties.py
        testUsdDataFormats.py
        testUsdExternalAssetDependencies.py
        testUsdErrors.py
        testUsdFallbackPrimTypes.py
        testUsdFileFormats.py
        testUsdFlatten.py
        testUsdFlatten2.py
        testUsdFlattenLayerStack.py
        testUsdFlattenProperties.py
        testUsdInherits.py
        testUsdInstanceProxy.py
        testUsdInstancing.py
        testUsdLoadUnloadDeepNestedInstancing.py
        testUsdMetadata.py
        testUsdModel.py
        testUsdNamespaceEditor.py
        testUsdNamespaceEditorDependentEditsBase.py
        testUsdNamespaceEditorDependentEditsBasicClassArcs.py
        testUsdNamespaceEditorDependentEditsBasicReferencesAndPayloads.py
        testUsdNamespaceEditorDependentEditsBasicRelocates.py
        testUsdNamespaceEditorDependentEditsBasicSublayers.py
        testUsdNamespaceEditorDependentEditsBasicVariants.py
        testUsdNamespaceEditorDependentEditsPathExpressionFixup.py
        testUsdNamespaceEditorDependentEditsProperties.py
        testUsdNamespaceEditorDependentEditsSiblingNodeSpecConflicts.py
        testUsdNamespaceEditorDependentEditsTargetPathFixup.py
        testUsdNamespaceEditorPathExpressionFixup.py
        testUsdNamespaceEditorProperties.py
        testUsdNamespaceEditorTargetPathFixup.py
        testUsdNotices.py
        testUsdObjectsChangedNotices.py
        testUsdObjectsChangedNoticesSublayerOps.py
        testUsdOpaqueAttributes.py
        testUsdPathExpressionAttrs.py
        testUsdPayloads.py
        testUsdPrimCompositionQuery.py
        testUsdPrimFlagsPredicate.py
        testUsdPrimRange.py
        testUsdPrims.py
        testUsdReadOutOfBounds.py
        testUsdReferences.py
        testUsdRelationships.py
        testUsdResolveTargetPy.py
        testUsdSchemaBasePy.py
        testUsdSchemaRegistry.py
        testUsdSchemaRegistryThreadedInit.py
        testUsdSchemaVersioning.py
        testUsdSpecializes.py
        testUsdSplines.py
        testUsdStage.py
        testUsdStageCache.py
        testUsdStageDefaultResolver.py
        testUsdStageLoadUnload.py
        testUsdStagePopulationMasks.py
        testUsdTimeCode.py
        testUsdTimeCodeRepr.py
        testUsdTimeOffsets.py
        testUsdTimeSamples.py
        testUsdTimeValueAuthoring.py
        testUsdUsdzFileFormat.py
        testUsdValueClips.py
        testUsdVariantEditing.py
        testUsdVariantFallbacks.py
        testUsdVariants.py
    )
    foreach(_script ${_scripts})
        file(GENERATE OUTPUT "${_testDir}/${_script}" INPUT "${_script}")
    endforeach()

    list(APPEND testWrapper
        "--env-var=PYTHONPATH=${_pythonPath}"
    )

    add_test(NAME testUsdAppliedAPISchemas
        COMMAND ${testWrapper}
            "${_testDir}/testUsdAppliedAPISchemas.py"
            --testenv-dir=testUsdAppliedAPISchemas
    )

    add_test(NAME testUsdAppliedAPISchemas_AutoApplyDisabled
        COMMAND ${testWrapper}
            "${_testDir}/testUsdAppliedAPISchemas.py"
            --testenv-dir=testUsdAppliedAPISchemas
            --env-var=USD_DISABLE_AUTO_APPLY_API_SCHEMAS=1
    )

    add_test(NAME testUsdAttributeBlocking
        COMMAND ${testWrapper}
            "${_testDir}/testUsdAttributeBlocking.py"
    )

    add_test(NAME testUsdAttributeConnections
        COMMAND ${testWrapper}
            "${_testDir}/testUsdAttributeConnections.py"
    )

    add_test(NAME testUsdAttributeAssetPathVariableExpressions
        COMMAND ${testWrapper}
            "${_testDir}/testUsdAttributeAssetPathVariableExpressions.py"
    )

    add_test(NAME testUsdAttributeQuery
        COMMAND ${testWrapper}
            "${_testDir}/testUsdAttributeQuery.py"
    )

    add_test(NAME testUsdErrors
        COMMAND ${testWrapper}
            "${_testDir}/testUsdErrors.py"
            --env-var=TF_FATAL_THROW=0
    )

    add_test(NAME testUsdValueClips
        COMMAND ${testWrapper}
            "${_testDir}/testUsdValueClips.py"
            --testenv-dir=testUsdValueClips
    )

    add_test(NAME testUsdVariantEditing
        COMMAND ${testWrapper}
            "${_testDir}/testUsdVariantEditing.py"
            --testenv-dir=testUsdVariantEditing.testenv
    )

    add_test(NAME testUsdVariantFallbacks
        COMMAND ${testWrapper}
            "${_testDir}/testUsdVariantFallbacks.py"
            --testenv-dir=testUsdVariantFallbacks.testenv
    )

    add_test(NAME testUsdVariants
        COMMAND ${testWrapper}
            "${_testDir}/testUsdVariants.py"
            --testenv-dir=testUsdVariants
    )

    add_test(NAME testUsdBugs
        COMMAND ${testWrapper}
            "${_testDir}/testUsdBugs.py"
    )

    add_test(NAME testUsdBug119633
        COMMAND ${testWrapper}
            "${_testDir}/testUsdBug119633.py"
            --testenv-dir=testUsdBug119633.testenv
    )

    add_test(NAME testUsdBug141491
        COMMAND ${testWrapper}
            "${_testDir}/testUsdBug141491.py"
            --testenv-dir=testUsdBug141491.testenv
    )

    add_test(NAME testUsdChangeProcessing
        COMMAND ${testWrapper}
            "${_testDir}/testUsdChangeProcessing.py"
    )

    add_test(NAME testUsdCollectionAPI
        COMMAND ${testWrapper}
            "${_testDir}/testUsdCollectionAPI.py"
            --testenv-dir=testUsdCollectionAPI
    )

    add_test(NAME testUsdClasses
        COMMAND ${testWrapper}
            "${_testDir}/testUsdClasses.py"
    )

    add_test(NAME testUsdCreateProperties
        COMMAND ${testWrapper}
            "${_testDir}/testUsdCreateProperties.py"
    )

    add_test(NAME testUsdExternalAssetDependencies
        COMMAND ${testWrapper}
            "${_testDir}/testUsdExternalAssetDependencies.py"
            --testenv-dir=testUsdExternalAssetDependencies.testenv
            "--env-var=USD_EXTERNAL_ASSETS_PLUGIN=$<TARGET_FILE_DIR:TestUsdProceduralExternalAssetsFileFormatPlugin>/plugInfo_$<CONFIG>.json"
    )

    add_test(NAME testUsdFallbackPrimTypes
        COMMAND ${testWrapper}
            "${_testDir}/testUsdFallbackPrimTypes.py"
            --testenv-dir=testUsdFallbackPrimTypes
    )

    add_test(NAME testUsdInstancing
        COMMAND ${testWrapper}
            "${_testDir}/testUsdInstancing.py"
            --testenv-dir=testUsdInstancing
            --env-var=USD_ASSIGN_PROTOTYPES_DETERMINISTICALLY=1
    )

    add_test(NAME testUsdInstanceProxy
        COMMAND ${testWrapper}
            "${_testDir}/testUsdInstanceProxy.py"
            --testenv-dir=testUsdInstanceProxy
    )

    add_test(NAME testUsdDataFormats
        COMMAND ${testWrapper}
            "${_testDir}/testUsdDataFormats.py"
    )

    add_test(NAME testUsdLoadUnloadDeepNestedInstancing
        COMMAND ${testWrapper}
            "${_testDir}/testUsdLoadUnloadDeepNestedInstancing.py"
            --testenv-dir=testUsdLoadUnloadDeepNestedInstancing.testenv
    )

    add_test(NAME testUsdTimeSamples
        COMMAND ${testWrapper}
            "${_testDir}/testUsdTimeSamples.py"
    )

    add_test(NAME testUsdReferences
        COMMAND ${testWrapper}
            "${_testDir}/testUsdReferences.py"
    )

    add_test(NAME testUsdPrimCompositionQuery
        COMMAND ${testWrapper}
            "${_testDir}/testUsdPrimCompositionQuery.py"
            --testenv-dir=testUsdPrimCompositionQuery.testenv
    )

    add_test(NAME testUsdPrims
        COMMAND ${testWrapper}
            "${_testDir}/testUsdPrims.py"
            --testenv-dir=testUsdPrims.testenv
    )

    add_test(NAME testUsdFileFormats
        COMMAND ${testWrapper}
            "${_testDir}/testUsdFileFormats.py"
            --testenv-dir=testUsdFileFormats
    )

    add_test(NAME testUsdFileFormats_asset
        COMMAND ${testWrapper}
            "${_testDir}/testUsdFileFormats.py"
            --testenv-dir=testUsdFileFormats
            --env-var=USDC_USE_ASSET=1
    )

    add_test(NAME testUsdFileFormats_pread
        COMMAND ${testWrapper}
            "${_testDir}/testUsdFileFormats.py"
            --testenv-dir=testUsdFileFormats
            --env-var=USDC_USE_PREAD=1
    )

    add_test(NAME testUsdStage
        COMMAND ${testWrapper}
            "${_testDir}/testUsdStage.py"
    )

    add_test(NAME testUsdStageCache
        COMMAND ${testWrapper}
            "${_testDir}/testUsdStageCache.py"
    )

    add_test(NAME testUsdStagePopulationMasks
        COMMAND ${testWrapper}
            "${_testDir}/testUsdStagePopulationMasks.py"
    )

    add_test(NAME testUsdTimeCodeRepr
        COMMAND ${testWrapper}
            "${_testDir}/testUsdTimeCodeRepr.py"
    )

    add_test(NAME testUsdTimeCode
        COMMAND ${testWrapper}
            "${_testDir}/testUsdTimeCode.py"
    )

    add_test(NAME testUsdTimeOffsets
        COMMAND ${testWrapper}
            "${_testDir}/testUsdTimeOffsets.py"
    )

    add_test(NAME testUsdPrimRange
        COMMAND ${testWrapper}
            "${_testDir}/testUsdPrimRange.py"
            --testenv-dir=testUsdPrimRange
    )

    add_test(NAME testUsdStageDefaultResolver
        COMMAND ${testWrapper}
            "${_testDir}/testUsdStageDefaultResolver.py"
    )

    add_test(NAME testUsdStageLoadUnload
        COMMAND ${testWrapper}
            "${_testDir}/testUsdStageLoadUnload.py"
    )

    add_test(NAME testUsdPayloads
        COMMAND ${testWrapper}
            "${_testDir}/testUsdPayloads.py"
    )

    add_test(NAME testUsdPathExpressionAttrs
        COMMAND ${testWrapper}
            "${_testDir}/testUsdPathExpressionAttrs.py"
    )

    add_test(NAME testUsdCratePayloadConversionFromVersion07
        COMMAND ${testWrapper}
            "${_testDir}/testUsdCratePayloadConversionFromVersion07.py"
            --testenv-dir=testUsdCratePayloadConversionFromVersion07.testenv
    )

    add_test(NAME testUsdCrateRelocates
        COMMAND ${testWrapper}
            "${_testDir}/testUsdCrateRelocates.py"
            --testenv-dir=testUsdCrateRelocates.testenv
    )

    add_test(NAME testUsdCrateSpecific
        COMMAND ${testWrapper}
            "${_testDir}/testUsdCrateSpecific.py"
    )

    add_test(NAME testUsdInherits
        COMMAND ${testWrapper}
            "${_testDir}/testUsdInherits.py"
    )

    add_test(NAME testUsdModel
        COMMAND ${testWrapper}
            "${_testDir}/testUsdModel.py"
    )

    add_test(NAME testUsdResolveTargetPy
        COMMAND ${testWrapper}
            "${_testDir}/testUsdResolveTargetPy.py"
            --testenv-dir=testUsdResolveTarget.testenv
    )

    add_test(NAME testUsdSpecializes
        COMMAND ${testWrapper}
            "${_testDir}/testUsdSpecializes.py"
    )

    add_test(NAME testUsdSplines
        COMMAND ${testWrapper}
            "${_testDir}/testUsdSplines.py"
    )

    add_test(NAME testUsdMetadata
        COMMAND ${testWrapper}
            "${_testDir}/testUsdMetadata.py"
            --testenv-dir=testUsdMetadata
    )

    add_test(NAME testUsdFlatten
        COMMAND ${testWrapper}
            "${_testDir}/testUsdFlatten.py"
            --testenv-dir=testUsdFlatten
    )

    add_test(NAME testUsdFlatten2
        COMMAND ${testWrapper}
            "${_testDir}/testUsdFlatten2.py BasicInstancingAndVariants/root.usda"
            --testenv-dir=testUsdFlatten2
            --diff-stdout=flat_BasicInstancingAndVariants.usda
            --env-var=USD_ASSIGN_PROTOTYPES_DETERMINISTICALLY=1
    )

    add_test(NAME testUsdFlatten3
        COMMAND ${testWrapper}
            "${_testDir}/testUsdFlatten2.py BasicInstancing/root.usda"
            --testenv-dir=testUsdFlatten2
            --diff-stdout=flat_BasicInstancing.usda
            --env-var=USD_ASSIGN_PROTOTYPES_DETERMINISTICALLY=1
    )

    add_test(NAME testUsdFlatten4
        COMMAND ${testWrapper}
            "${_testDir}/testUsdFlatten2.py BasicInstancingAndNestedInstances/root.usda"
            --testenv-dir=testUsdFlatten2
            --diff-stdout=flat_BasicInstancingAndNestedInstances.usda
            --env-var=USD_ASSIGN_PROTOTYPES_DETERMINISTICALLY=1
    )

    add_test(NAME testUsdFlattenLayerStack
        COMMAND ${testWrapper}
            "${_testDir}/testUsdFlattenLayerStack.py"
            --testenv-dir=testUsdFlattenLayerStack
    )

    add_test(NAME testUsdNotices
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNotices.py"
            --testenv-dir=testUsdNotices.testenv
    )

    add_test(NAME testUsdObjectsChangedNotices
        COMMAND ${testWrapper}
            "${_testDir}/testUsdObjectsChangedNotices.py"
    )

    add_test(NAME testUsdObjectsChangedNoticesSublayerOps
        COMMAND ${testWrapper}
            "${_testDir}/testUsdObjectsChangedNoticesSublayerOps.py"
            --testenv-dir=testUsdObjectsChangedNoticesSublayerOps
    )

    add_test(NAME testUsdNamespaceEditor
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditor.py"
            --testenv-dir=testUsdNamespaceEditor
    )

    add_test(NAME testUsdNamespaceEditorDependentEditsBasicClassArcs
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorDependentEditsBasicClassArcs.py"
    )

    add_test(NAME testUsdNamespaceEditorDependentEditsBasicReferencesAndPayloads
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorDependentEditsBasicReferencesAndPayloads.py"
    )

    add_test(NAME testUsdNamespaceEditorDependentEditsBasicRelocates
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorDependentEditsBasicRelocates.py"
    )

    add_test(NAME testUsdNamespaceEditorDependentEditsBasicSublayers
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorDependentEditsBasicSublayers.py"
    )

    add_test(NAME testUsdNamespaceEditorDependentEditsBasicVariants
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorDependentEditsBasicVariants.py"
    )

    add_test(NAME testUsdNamespaceEditorDependentEditsPathExpressionFixup
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorDependentEditsPathExpressionFixup.py"
    )

    add_test(NAME testUsdNamespaceEditorDependentEditsProperties
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorDependentEditsProperties.py"
    )

    add_test(NAME testUsdNamespaceEditorDependentEditsSiblingNodeSpecConflicts
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorDependentEditsSiblingNodeSpecConflicts.py"
    )

    add_test(NAME testUsdNamespaceEditorDependentEditsTargetPathFixup
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorDependentEditsTargetPathFixup.py"
    )

    add_test(NAME testUsdNamespaceEditorPathExpressionFixup
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorPathExpressionFixup.py"
            --testenv-dir=testUsdNamespaceEditorPathExpressionFixup
    )

    add_test(NAME testUsdNamespaceEditorProperties
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorProperties.py"
            --testenv-dir=testUsdNamespaceEditorProperties
    )

    add_test(NAME testUsdNamespaceEditorTargetPathFixup
        COMMAND ${testWrapper}
            "${_testDir}/testUsdNamespaceEditorTargetPathFixup.py"
            --testenv-dir=testUsdNamespaceEditorTargetPathFixup
    )

    add_test(NAME testUsdReadOutOfBounds
        COMMAND ${testWrapper}
            "${_testDir}/testUsdReadOutOfBounds.py"
            --testenv-dir=testUsdReadOutOfBounds
    )

    add_test(NAME testUsdRelationships
        COMMAND ${testWrapper}
            "${_testDir}/testUsdRelationships.py"
    )

    add_test(NAME testUsdSchemaBasePy
        COMMAND ${testWrapper}
            "${_testDir}/testUsdSchemaBasePy.py"
    )

    add_test(NAME testUsdSchemaRegistry
        COMMAND ${testWrapper}
            "${_testDir}/testUsdSchemaRegistry.py"
            --testenv-dir=testUsdSchemaRegistry
    )

    add_test(NAME testUsdSchemaRegistryThreadedInit
        COMMAND ${testWrapper}
            "${_testDir}/testUsdSchemaRegistryThreadedInit.py"
    )

    add_test(NAME testUsdSchemaVersioning
        COMMAND ${testWrapper}
            "${_testDir}/testUsdSchemaVersioning.py"
            --testenv-dir=testUsdSchemaVersioning
    )

    add_test(NAME testUsdTimeValueAuthoring
        COMMAND ${testWrapper}
            "${_testDir}/testUsdTimeValueAuthoring.py"
            --testenv-dir=testUsdTimeValueAuthoring
    )

    add_test(NAME testUsdUsdzFileFormat
        COMMAND ${testWrapper}
            "${_testDir}/testUsdUsdzFileFormat.py"
            --testenv-dir=testUsdUsdzFileFormat
    )

    add_test(NAME testUsdUsdzFileFormat_asset
        COMMAND ${testWrapper}
            "${_testDir}/testUsdUsdzFileFormat.py"
            --testenv-dir=testUsdUsdzFileFormat
            --env-var=USDC_USE_ASSET=1
    )

    add_test(NAME testUsdUsdzFileFormat_pread
        COMMAND ${testWrapper}
            "${_testDir}/testUsdUsdzFileFormat.py"
            --testenv-dir=testUsdUsdzFileFormat
            --env-var=USDC_USE_PREAD=1
    )

    add_test(NAME testUsdOpaqueAttributes
        COMMAND ${testWrapper}
            "${_testDir}/testUsdOpaqueAttributes.py"
    )
endif()
