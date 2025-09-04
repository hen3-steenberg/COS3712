module;
#include "glm/glm.hpp"
#include "rapidobj/rapidobj.hpp"
#include <array>
export module triangle;

export struct triangle {
    std::array<glm::vec3, 3> vertices;
    std::array<glm::vec3, 3> normals;
    std::array<glm::vec2, 3> texcoords;
};

export glm::vec3 getPosition(rapidobj::Result const& object, rapidobj::Index idx) {
    return glm::vec3
    {
        object.attributes.positions[3 * idx.position_index],
        object.attributes.positions[3 * idx.position_index + 1],
        object.attributes.positions[3 * idx.position_index + 2],
    };
}

export glm::vec3 getNormal(rapidobj::Result const& object, rapidobj::Index idx) {
    return glm::vec3
    {
        object.attributes.normals[3 * idx.normal_index],
        object.attributes.normals[3 * idx.normal_index + 1],
        object.attributes.normals[3 * idx.normal_index + 2],
    };
}

export glm::vec2 getTexCoord(rapidobj::Result const& object, rapidobj::Index idx) {
    return glm::vec2{
        object.attributes.texcoords[2 * idx.texcoord_index],
        object.attributes.texcoords[2 * idx.texcoord_index + 1],
        };
}