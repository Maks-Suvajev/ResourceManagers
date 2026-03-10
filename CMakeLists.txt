add_library(ResourceManagersLib INTERFACE)

add_subdirectory(Material)
add_subdirectory(Mesh)
add_subdirectory(Texture)
add_subdirectory(Shader)


target_include_directories(ResourceManagersLib 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_libraries(ResourceManagersLib INTERFACE
    MaterialLib
    MeshLib
    TextureLib
    ShaderLib
)

