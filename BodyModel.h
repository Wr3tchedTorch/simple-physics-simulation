#pragma once
#include <SFML/Graphics/Color.hpp>
#include <id.h>
#include <cstdint>
#include <type_traits>
#include <string.h>
#include <cstring>
#include <string>

struct BodyModel
{
    std::string m_Type = "box";

	b2BodyId  m_Id;
	sf::Color m_Color;

    float m_MaterialDamageMultiplier = 1.0f;
	float m_Health    = 100.0f;
    float m_MaxHealth = 100.0f;
};

struct b2BodyIdHash {
    std::size_t operator()(const b2BodyId& id) const {
        uint64_t key;
        static_assert(sizeof(b2BodyId) == sizeof(uint64_t));
        std::memcpy(&key, &id, sizeof(key));
        return std::hash<uint64_t>{}(key);
    }
};

struct b2BodyIdEqual {
    bool operator()(const b2BodyId& lhs, const b2BodyId& rhs) const {
        uint64_t l, r;
        std::memcpy(&l, &lhs, sizeof(l));
        std::memcpy(&r, &rhs, sizeof(r));
        return l == r;
    }
};