add_library(ResourceManagerBaseLib INTERFACE)

target_include_directories(ResourceManagerBaseLib 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_libraries(ResourceManagerBaseLib 
    INTERFACE
        AssetsLib
    
) 
