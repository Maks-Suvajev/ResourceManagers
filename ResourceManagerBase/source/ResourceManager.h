#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// STL
#include <filesystem>
#include <unordered_map>
#include <string>
#include <memory>

#include "AssetRegistry.h"

template<typename T>
class ResourceManager
{
    public:
        ResourceManager(AssetRegistry* assetRegistry, std::vector<std::string> supportedFileTypes);
        virtual ~ResourceManager() = default;

        void deleteElement(const std::string& key);
        void refreshElements();
        virtual void registerElement(const std::filesystem::path& sourcePath){};

        virtual void registerElement(const std::string& key, T&& newElement){};


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
            std::cout << "ERROR::TextureManager::refreshTextures:: " << e.what() << std::endl;
            continue;
        }
        
        if (!oldMap.contains(key))
        {
            registerElement(path);
        }
    }

    for (const auto& [key, element] : oldMap)
    {
        // Keep old keys as long as the file still exists
        if (!std::filesystem::exists(key))
        {
            #ifdef ENABLE_DEBUG_MESSAGES
                std::cout << "INFO::TextureManager::refreshTextures::Deleting key because it no longer exists: " << key << std::endl;
            #endif

            deleteElement(key);
        };
    }
}


#endif