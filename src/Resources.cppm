module;
#include <span>
#include <string>
#include "stb/stb_image.h"
export module Resources;

export std::span<const stbi_uc> viking_room_png() noexcept;

export const std::string & viking_room_obj();


