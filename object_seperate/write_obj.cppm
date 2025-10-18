module;
#include "rapidobj/rapidobj.hpp"

#include <filesystem>
#include <fstream>
#include <span>
export module write_obj;
export import triangle;

export void
write_file(std::filesystem::path const& path, std::span<const triangle> faces, rapidobj::Material const& material)
{

    std::filesystem::path mtl_path = path;
    mtl_path.replace_extension(".mtl");

    {
        std::ofstream file{ path };

        file << "# Object seperation tool\n";
        file << "# Each processed object has one material (may not logically be a single object)\n";
        file << "mtllib " << mtl_path.filename() << "\n";
        file << "o " << path.stem() << "\n";

        for (auto const& face : faces) {
            for (auto const& vertex : face.vertices) {
                file << "v " << std::fixed << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
            }
        }

        file << std::endl;

        for (auto const& face : faces) {
            for (auto const& texcoord : face.texcoords) {
                file << "vt " << std::fixed << texcoord.x << " " << texcoord.y << "\n";
            }
        }

        file << std::endl;

        for (auto const& face : faces) {
            for (auto const& normal : face.normals) {
                file << "vn " << std::fixed << normal.x << " " << normal.y << " " << normal.z << "\n";
            }
        }

        file << std::endl;

        file << "usemtl " << material.name << "\n";

        for (size_t face_idx = 0; face_idx < faces.size(); ++face_idx) {
            file << "f";
            for (size_t idx = 0; idx < 3; ++idx) {
                size_t current_idx = 3 * face_idx + idx;
                file << " " << face_idx << "/" << face_idx << "/" << face_idx;
            }
            file << "\n";
        }
    }

    {
        std::ofstream mtl_file{ mtl_path };

        mtl_file << "# Object seperation tool\n";
        mtl_file << "# Each processed object has one material (may not logically be a single object)\n";

        mtl_file << "newmtl " << material.name << "\n";
        mtl_file << "Ns " << std::fixed << material.shininess << "\n";
        mtl_file << "Ka " << std::fixed << material.ambient[0] << " " << material.ambient[1] << " "
                 << material.ambient[2] << "\n";
        mtl_file << "Ks " << std::fixed << material.specular[0] << " " << material.specular[1] << " "
                 << material.specular[2] << "\n";
        mtl_file << "Ke " << std::fixed << material.emission[0] << " " << material.emission[1] << " "
                 << material.emission[2] << "\n";
        mtl_file << "Ni " << std::fixed << material.ior << "\n";
        mtl_file << "d " << std::fixed << material.dissolve << "\n";
        mtl_file << "illum " << material.illum << "\n";
        mtl_file << "map_Kd " << material.diffuse_texname << "\n";
        mtl_file << "map_Bump -bm " << std::fixed << material.bump_texopt.bump_multiplier << " "
                 << material.bump_texname << "\n";
    }
}