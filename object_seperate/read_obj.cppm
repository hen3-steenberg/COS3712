module;
#include "glm/glm.hpp"
#include "rapidobj/rapidobj.hpp"
#include <filesystem>
#include <flat_map>
#include <tuple>
#include <vector>
export module read_obj;
export import triangle;

export std::vector<std::pair<rapidobj::Material, std::vector<triangle>>>
read_obj_file(std::filesystem::path const& path)
{
    std::vector<std::pair<rapidobj::Material, std::vector<triangle>>> new_objects{};
    auto res = rapidobj::ParseFile(path);
    if (!res.error) {

        for (auto mat : res.materials) {
            new_objects.emplace_back(mat, std::vector<triangle>{});
        }

        for (auto const& shape : res.shapes) {
            for (size_t triangle_idx = 0; triangle_idx < shape.mesh.material_ids.size(); ++triangle_idx) {
                size_t vertex_idx = triangle_idx * 3;
                int material_idx = shape.mesh.material_ids[triangle_idx];

                auto idx1 = shape.mesh.indices[vertex_idx];
                auto idx2 = shape.mesh.indices[vertex_idx + 1];
                auto idx3 = shape.mesh.indices[vertex_idx + 2];
                new_objects[material_idx].second.emplace_back(triangle{
                    .vertices = { getPosition(res, idx1), getPosition(res, idx2), getPosition(res, idx3) },
                    .normals = { getNormal(res, idx1), getNormal(res, idx2), getNormal(res, idx3) },
                    .texcoords = { getTexCoord(res, idx1), getTexCoord(res, idx2), getTexCoord(res, idx3) } });
            }
        }
    }
    return new_objects;
}