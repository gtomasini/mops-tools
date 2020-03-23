#include "daa_file.h"
#include <filesystem>

namespace daa
{
namespace io
{
    bool file::is_file(const std::string & a_filename) {
		return std::experimental::filesystem::exists(a_filename);
    }
}
}