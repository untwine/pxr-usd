add_library(TestUsdProceduralExternalAssetsFileFormatPlugin SHARED plugin.cpp)
target_link_libraries(TestUsdProceduralExternalAssetsFileFormatPlugin PUBLIC usd pxr::sdf pxr::tf)
file(
    GENERATE
    OUTPUT "$<TARGET_FILE_DIR:TestUsdProceduralExternalAssetsFileFormatPlugin>/plugInfo_$<CONFIG>.json"
    INPUT plugInfo.json
)
