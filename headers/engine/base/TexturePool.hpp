#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_BASE_TEXTUREPOOL_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_BASE_TEXTUREPOOL_HPP_

#include "includes.hpp"
#include "Texture.hpp"

class TexturePool {
    private:
        struct TextureEntry {
            Texture* tex;
            uint32_t referenceCount;
        };

        static std::unordered_map<uint32_t, TextureEntry*> pool;
        static uint32_t getHash(std::string str);
        static uint32_t getHash(std::vector<std::string> strs);
        static void addTextureIfNotPresent(uint32_t hash, Texture* tex);
        static void removeTexture(uint32_t hash);
        static void registerTextureUsage(uint32_t hash);

       public:
        static void addTextureIfNotPresent(std::string name, Texture* tex);
        static void addTextureIfNotPresent(std::vector<std::string> files, Texture* tex);
        static void registerTextureUsage(std::string name);
        static void registerTextureUsage(std::vector<std::string> files);
        static Texture* getTexture(std::string name);
        static Texture* getTexture(std::vector<std::string> files);
        static void removeTexture(std::string name);
        static void removeTexture(std::vector<std::string> files);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_BASE_TEXTUREPOOL_HPP_