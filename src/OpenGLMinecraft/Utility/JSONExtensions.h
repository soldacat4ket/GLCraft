#pragma once

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace nlohmann
{
    // define how glm types should be read and written to JSON
    template <typename T, glm::qualifier Q>
    struct adl_serializer<glm::vec<2, T, Q>>
    {
        static void to_json(json& j, const glm::vec<2, T, Q>& v)
        {
            j = {v.x, v.y};
        }

        static void from_json(const json& j, glm::vec<2, T, Q>& v)
        {
            v.x = j.at(0).get<T>();
            v.y = j.at(1).get<T>();
        }
    };

    template <typename T, glm::qualifier Q>
    struct adl_serializer<glm::vec<3, T, Q>>
    {
        static void to_json(json& j, const glm::vec<3, T, Q>& v)
        {
            j = {v.x, v.y, v.z};
        }

        static void from_json(const json& j, glm::vec<3, T, Q>& v)
        {
            v.x = j.at(0).get<T>();
            v.y = j.at(1).get<T>();
            v.z = j.at(2).get<T>();
        }
    };

    template <typename T, glm::qualifier Q>
    struct adl_serializer<glm::vec<4, T, Q>>
    {
        static void to_json(json& j, const glm::vec<4, T, Q>& v)
        {
            j = {v.x, v.y, v.z, v.w};
        }

        static void from_json(const json& j, glm::vec<4, T, Q>& v)
        {
            v.x = j.at(0).get<T>();
            v.y = j.at(1).get<T>();
            v.z = j.at(2).get<T>();
            v.w = j.at(3).get<T>();
        }
    };
}