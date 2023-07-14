#include <headers/engine/base/TexturePool.hpp>

std::unordered_map<uint32_t, TexturePool::TextureEntry*> TexturePool::pool = {};

uint32_t TexturePool::getHash(std::string str) {
    std::hash<std::string> hasher;
    return hasher(str);
}

uint32_t TexturePool::getHash(std::vector<std::string> strs) {
    std::string base;
    std::copy(strs[0].rbegin(), strs[0].rend(), std::back_inserter(base));
    for (int i = 1; i < strs.size(); i++) {
        base.append(strs[i]);
    }
    return getHash(base);
}

void TexturePool::addTextureIfNotPresent(std::string name, Texture* tex){
    uint32_t hash = getHash(name);
    addTextureIfNotPresent(hash, tex);
}

void TexturePool::addTextureIfNotPresent(std::vector<std::string> files, Texture* tex){
    uint32_t hash = getHash(files);
    addTextureIfNotPresent(hash, tex);
}

void TexturePool::addTextureIfNotPresent(uint32_t hash, Texture* tex) {
    try {
        TextureEntry* mapTex = pool.at(hash);
    } catch (const std::out_of_range& e) {
        pool[hash] = new TextureEntry{ tex, 1 };
    }
}

Texture* TexturePool::getTexture(std::string name) {
    try {
        return pool.at(getHash(name))->tex;
    } catch (const std::out_of_range& e) {
        return nullptr;
    }
}

Texture* TexturePool::getTexture(std::vector<std::string> files) {
    try {
        return pool.at(getHash(files))->tex;
    } catch (const std::out_of_range& e) {
        return nullptr;
    }
}

void TexturePool::removeTexture(uint32_t hash) {
    try {
        TextureEntry* texEntry = pool.at(hash);
        texEntry->referenceCount -= 1;
        if (texEntry->referenceCount <= 0) {
            pool.erase(hash);
            texEntry->tex->cleanup();
            delete texEntry->tex;
            delete texEntry;
        }
    } catch (const std::out_of_range& e) {
        return;
    }
}

void TexturePool::removeTexture(std::string name) {
    uint32_t hash = getHash(name);
    removeTexture(hash);
}

void TexturePool::removeTexture(std::vector<std::string> files) {
    uint32_t hash = getHash(files);
    removeTexture(hash);
}

void TexturePool::registerTextureUsage(uint32_t hash) {
    try {
        TextureEntry* texEntry = pool.at(hash);
        texEntry->referenceCount += 1;
    } catch (const std::out_of_range& e) {
        return;
    }
}

void TexturePool::registerTextureUsage(std::string name) {
    registerTextureUsage(getHash(name));
}

void TexturePool::registerTextureUsage(std::vector<std::string> files) {
    registerTextureUsage(getHash(files));
}