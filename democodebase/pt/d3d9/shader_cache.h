#pragma once
#ifndef PT_D3D9_SHADER_CACHE_H
#define PT_D3D9_SHADER_CACHE_H

// TODO:
// Implement shader cache. Hashed HLSL is the key.

#include "shader_convenience.h"
//#include "constant_buffer_fwd.h"
//#include "pt/com_ptr.h"
#include <unordered_map>

namespace pt { namespace d3d9 {

struct shader_cache_key
{
    std::string hlsl;
    std::string function_name;
    std::string profile;

    bool operator==(const shader_cache_key& other) const
    {
        return profile == other.profile
            && function_name == other.function_name
            && hlsl == other.hlsl;
    }
};

class shader_cache_hash
{
public:
    std::size_t operator()(shader_cache_key const& s) const
    {
        std::size_t h1 = std::hash<std::string>()(s.hlsl);
        std::size_t h2 = std::hash<std::string>()(s.function_name);
        std::size_t h3 = std::hash<std::string>()(s.profile);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

class shader_cache
{
public:
    compiled_shader const& find(shader_cache_key const& key);
    void insert(shader_cache_key const& key);

private: // data members
    std::unordered_map<shader_cache_key,compiled_shader,shader_cache_hash> m_map;
};

}} // namespace pt::d3d9

#endif
