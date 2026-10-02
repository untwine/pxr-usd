add_library(TestUsdResolverChangedResolver SHARED plugin.cpp)
target_link_libraries(TestUsdResolverChangedResolver PUBLIC usd pxr::ar pxr::plug pxr::tf)
file(
    GENERATE
    OUTPUT "$<TARGET_FILE_DIR:TestUsdResolverChangedResolver>/plugInfo_$<CONFIG>.json"
    INPUT plugInfo.json
)

target_include_directories(TestUsdResolverChangedResolver
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
