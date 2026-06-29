#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// STL
#include <filesystem>
#include <unordered_map>
#include <string>
#include <memory>

#include "AssetRegistry.h"
#include "DefaultShaders.h"

template<typename T>
class ResourceManager
{
    public:
        ResourceManager(AssetRegistry* assetRegistry, std::vector<std::string> supportedFileTypes);
        virtual ~ResourceManager() = default;

        void deleteElement(const std::string& key);
        T getElement(const std::string& key);
        const T& getElementRef(const std::string& key);

        void refreshElements();
        virtual void registerElement(const std::filesystem::path& /*sourcePath*/){};
        virtual void registerElement(const std::string& /*key*/, T&& /*newElement*/){};

        std::vector<std::string> getKeys();
        const std::unordered_map<std::string, std::unique_ptr<T>>& getMap();

        std::string getCurrentActiveDirectory();
        void setCurrentActiveDirectory(std::string directory);

    protected:
        AssetRegistry*                                      m_assetRegistry;
        std::unordered_map<std::string, std::unique_ptr<T>> m_elements;
        std::filesystem::path                               m_activeDirectory;
        std::vector<std::string>                            m_supportedFileTypes; 
};

template<typename T>
void ResourceManager<T>::deleteElement(const std::string& key)
{
    m_elements.erase(key);
}

template<typename T>
ResourceManager<T>::ResourceManager(AssetRegistry* assetRegistry, std::vector<std::string> supportedFileTypes)
    : m_assetRegistry(assetRegistry),
      m_supportedFileTypes(supportedFileTypes)
{
    m_activeDirectory = m_assetRegistry->getDefaultAssetPath<T>();
}

template<typename T>
std::vector<std::string> ResourceManager<T>::getKeys()
{
    std::vector<std::string> keys;

    for (const auto& [key, element] : m_elements)
    {
        keys.push_back(key);
    }

    return keys;
}

template<typename T>
T ResourceManager<T>::getElement(const std::string& key)
{
    auto it = m_elements.find(key);

    if (it == m_elements.end() || !it->second)
    {
        throw std::runtime_error("ResourceManager::getElementRef:: Key not found: " + key);
    }

    return *it->second;
}

template<typename T>
const T& ResourceManager<T>::getElementRef(const std::string& key)
{
    auto it = m_elements.find(key);

    if (it == m_elements.end() || !it->second)
    {
        throw std::runtime_error("ResourceManager::getElementRef:: Key not found: " + key);
    }

    return *it->second;
}


template<typename T>
const std::unordered_map<std::string, std::unique_ptr<T>>& ResourceManager<T>::getMap()
{
    return m_elements;
}

template<typename T>
std::string ResourceManager<T>::getCurrentActiveDirectory()
{
    return m_activeDirectory.string();
}

template<typename T>
void ResourceManager<T>::setCurrentActiveDirectory(std::string directory)
{
    m_activeDirectory = std::filesystem::path(directory);
}

template<typename T>
void ResourceManager<T>::refreshElements()
{
    const auto& oldMap = getMap();

    for (const auto& path : m_assetRegistry->getAllFilesOfType(m_activeDirectory, m_supportedFileTypes))
    {
        std::string key;
        
        try
        {
            key = std::filesystem::canonical(path).string();
        }
        catch(const std::exception& e)
        {
            std::cout << "ERROR::ResourceManager::refreshElements:: " << e.what() << std::endl;
            continue;
        }
        
        if (!oldMap.contains(key))
        {
            registerElement(path);
        }
    }

    for (const auto& [key, element] : oldMap)
    {
        // Default shaders are hardcoded, not files.
        if (key == gfx::defaultShaderProgramKey ||
            key == gfx::defaultVertexShaderKey  ||
            key == gfx::defaultFragmentShaderKey
        )
        {
            continue;
        }

        // Keep old keys as long as the file still exists
        if (!std::filesystem::exists(key))
        {
            #ifdef ENABLE_DEBUG_MESSAGES
                std::cout << "INFO::ResourceManager::refreshElements::Deleting key because it no longer exists: " << key << std::endl;
            #endif

            deleteElement(key);
        };
    }
}

#endif
