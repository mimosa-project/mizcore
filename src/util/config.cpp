#include <filesystem>

#include "config.hpp"

namespace fs = std::filesystem;

const fs::path&
mizcore::VctPath()
{
    static fs::path mml_vct_path =
      fs::path(__FILE__).parent_path().parent_path() / "config" / "mml.vct";
    return mml_vct_path;
}
