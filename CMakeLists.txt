add_library(ResourceManagersLib INTERFACE)

add_subdirectory(ResourceManagerBase)
add_subdirectory(Material)
add_subdirectory(Scene-Model)
add_subdirectory(Texture)
add_subdirectory(Shader)

target_link_libraries(ResourceManagersLib INTERFACE
    ResourceManagerBaseLib
    MaterialLib
    SceneModelLib
    TextureLib
    ShaderLib
)

