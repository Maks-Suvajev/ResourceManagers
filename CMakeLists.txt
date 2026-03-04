add_library(ResourceManagersLib INTERFACE)

add_subdirectory(Material)
add_subdirectory(Mesh)
add_subdirectory(Texture)
add_subdirectory(Shader)


target_link_libraries(ResourceManagersLib INTERFACE
    MaterialLib
    MeshLib
    TextureLib
    ShaderLib
)

