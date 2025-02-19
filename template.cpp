
//Automatically generated, if you want to include changes, please do that in the main function
//The rest should not be touched.

#include <iostream>
#include <filesystem>
#include <fstream>
#include <fstream>
#include <set>
#include <map>
#include <vector>

#include <pugixml.hpp>

#ifdef TE4_COMPRESS
#include <miniz.h>
// Decompresses the input data
static std::vector<uint8_t>
inflate(std::basic_string_view<uint8_t> compressedData) {
auto size = ((uint32_t *)compressedData.data())[0];

std::vector<uint8_t> decompressedData(size);
mz_ulong decompressedSize = decompressedData.size();

if (mz_uncompress(decompressedData.data(), &decompressedSize,
                    compressedData.data() + 4,
                    compressedData.size() - 4) != MZ_OK) {
    throw std::runtime_error("Decompression failed");
}

// Resize to actual decompressed size
decompressedData.resize(decompressedSize);
return decompressedData;
}
#endif

namespace fs = std::filesystem;

bool no_override=true;

std::string escape_c_str(const std::string &input) {
  std::string escaped;
  for (char c : input) {
    switch (c) {
    case '\"':
      escaped += "\\\"";
      break;
    case '\\':
      escaped += "\\\\";
      break;
    case '\n':
      escaped += "\\n";
      break;
    case '\t':
      escaped += "\\t";
      break;
    default:
      escaped += c;
      break;
    }
  }
  return escaped;
}

std::string escape_meson_str(const std::string &input) {
  std::string escaped;
  for (char c : input) {
    switch (c) {
    case '\"':
      escaped += "\\\"";
      break;
    case '\\':
      escaped += "\\\\";
      break;
    case '\n':
      escaped += "\\n";
      break;
    case '\t':
      escaped += "\\t";
      break;
    default:
      escaped += c;
      break;
    }
  }
  return escaped;
}

std::string escape_xml_str(const std::string &input) {
  std::string escaped;
  for (char c : input) {
    switch (c) {
    case '&':
      escaped += "&amp;";
      break;
    case '<':
      escaped += "&lt;";
      break;
    case '>':
      escaped += "&gt;";
      break;
    case '\"':
      escaped += "&quot;";
      break;
    case '\'':
      escaped += "&apos;";
      break;
    default:
      escaped += c;
      break;
    }
  }
  return escaped;
}

#define WRITE(x) {std::string tmp = x; out.write(tmp.data(),tmp.size());}
#define WRITE_C(x) {std::string tmp = escape_c_str(x); out.write(tmp.data(),tmp.size());}
#define WRITE_MESON(x) {std::string tmp = escape_meson_str(x); out.write(tmp.data(),tmp.size());}
#define WRITE_XML(x) {std::string tmp = escape_xml_str(x); out.write(tmp.data(),tmp.size());}

typedef enum {
    WRITER_STATUS_ERROR,
    WRITER_STATUS_OK,
    WRITER_STATUS_SKIP,
} writer_status_t;

typedef pugi::xml_node env_t;

std::map<fs::path, writer_status_t(*)(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude)> fs_tree;

writer_status_t writer_file_0(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/build.sh",writer_file_0);} } static init;static constexpr int perms = 509;static constexpr const char* name  = "build.sh";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_0.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X23,0X21,0X2F,0X62,0X69,0X6E,0X2F,0X62,0X61,0X73,0X68,0XA,0X6D,0X65,0X73,0X6F,0X6E,0X20,0X73,0X65,0X74,0X75,0X70,0X20,0X2D,0X44,0X62,0X5F,0X6C,0X74,0X6F,0X3D,0X74,0X72,0X75,0X65,0X20,0X20,0X2D,0X2D,0X62,0X75,0X69,0X6C,0X64,0X74,0X79,0X70,0X65,0X3D,0X72,0X65,0X6C,0X65,0X61,0X73,0X65,0X20,0X2D,0X2D,0X72,0X65,0X63,0X6F,0X6E,0X66,0X69,0X67,0X75,0X72,0X65,0X20,0X62,0X75,0X69,0X6C,0X64,0X2F,0XA,0X73,0X75,0X64,0X6F,0X20,0X6D,0X65,0X73,0X6F,0X6E,0X20,0X69,0X6E,0X73,0X74,0X61,0X6C,0X6C,0X20,0X2D,0X43,0X20,0X62,0X75,0X69,0X6C,0X64,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_file_1(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/LICENCE.md",writer_file_1);} } static init;static constexpr int perms = 436;static constexpr const char* name  = "LICENCE.md";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_1.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}
auto proj_licence = env.child("project").child("licences");
auto proj_author = env.child("project").attribute("author").as_string("Unknown Author");
if(!proj_licence){{constexpr const uint8_t tmp[] = {0XA,0XA,0X41,0X6C,0X6C,0X20,0X72,0X69,0X67,0X68,0X74,0X73,0X20,0X72,0X65,0X73,0X65,0X72,0X76,0X65,0X64,0X20,0X62,0X79,0X20,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE(proj_author){constexpr const uint8_t tmp[] = {0XA,0XA,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}
}
else for (pugi::xml_node child: proj_licence.children())
    if (child.type() == pugi::node_pcdata)
        WRITE(child.value());out.close();
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_file_2(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/.gitignore",writer_file_2);} } static init;static constexpr int perms = 436;static constexpr const char* name  = ".gitignore";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_2.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X2F,0X62,0X75,0X69,0X6C,0X64,0X2F,0XA,0X2F,0X64,0X69,0X73,0X74,0X2F,0XA,0X2F,0X2E,0X63,0X61,0X63,0X68,0X65,0X2F,0XA,0X2F,0X73,0X75,0X62,0X70,0X72,0X6F,0X6A,0X65,0X63,0X74,0X73,0X2F,0X2A,0XA,0X21,0X2F,0X73,0X75,0X62,0X70,0X72,0X6F,0X6A,0X65,0X63,0X74,0X73,0X2F,0X2A,0X2E,0X77,0X72,0X61,0X70,0XA,0X21,0X73,0X75,0X62,0X70,0X72,0X6F,0X6A,0X65,0X63,0X74,0X73,0X2F,0X70,0X61,0X63,0X6B,0X61,0X67,0X65,0X66,0X69,0X6C,0X65,0X73,0X2F,0XA,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_file_3(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/meson.build",writer_file_3);} } static init;static constexpr int perms = 436;static constexpr const char* name  = "meson.build";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_3.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
} auto proj_name = env.child("project").attribute("name").as_string("undefined"); {constexpr const uint8_t tmp[] = {0XA,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
} auto proj_version = env.child("project").attribute("version").as_string("0.1.0"); {constexpr const uint8_t tmp[] = {0XA,0X70,0X72,0X6F,0X6A,0X65,0X63,0X74,0X28,0XA,0X20,0X20,0X20,0X20,0X27,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_MESON(proj_name){constexpr const uint8_t tmp[] = {0X27,0X2C,0XA,0X20,0X20,0X20,0X20,0X5B,0X27,0X63,0X70,0X70,0X27,0X5D,0X2C,0XA,0X20,0X20,0X20,0X20,0X76,0X65,0X72,0X73,0X69,0X6F,0X6E,0X3A,0X20,0X27,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_MESON(proj_version){constexpr const uint8_t tmp[] = {0X27,0X2C,0XA,0X20,0X20,0X20,0X20,0X6D,0X65,0X73,0X6F,0X6E,0X5F,0X76,0X65,0X72,0X73,0X69,0X6F,0X6E,0X3A,0X20,0X27,0X3E,0X3D,0X20,0X31,0X2E,0X31,0X27,0X2C,0XA,0X20,0X20,0X20,0X20,0X64,0X65,0X66,0X61,0X75,0X6C,0X74,0X5F,0X6F,0X70,0X74,0X69,0X6F,0X6E,0X73,0X3A,0X20,0X5B,0X27,0X63,0X70,0X70,0X5F,0X73,0X74,0X64,0X3D,0X63,0X2B,0X2B,0X32,0X33,0X27,0X5D,0X2C,0XA,0X29,0XA,0XA,0X61,0X64,0X64,0X5F,0X67,0X6C,0X6F,0X62,0X61,0X6C,0X5F,0X61,0X72,0X67,0X75,0X6D,0X65,0X6E,0X74,0X73,0X28,0X5B,0X27,0X2D,0X57,0X6E,0X6F,0X2D,0X75,0X6E,0X75,0X73,0X65,0X64,0X2D,0X66,0X75,0X6E,0X63,0X74,0X69,0X6F,0X6E,0X27,0X5D,0X2C,0X20,0X6C,0X61,0X6E,0X67,0X75,0X61,0X67,0X65,0X3A,0X20,0X27,0X63,0X70,0X70,0X27,0X29,0XA,0X61,0X64,0X64,0X5F,0X67,0X6C,0X6F,0X62,0X61,0X6C,0X5F,0X61,0X72,0X67,0X75,0X6D,0X65,0X6E,0X74,0X73,0X28,0X5B,0X27,0X2D,0X57,0X6E,0X6F,0X2D,0X75,0X6E,0X75,0X73,0X65,0X64,0X2D,0X66,0X75,0X6E,0X63,0X74,0X69,0X6F,0X6E,0X27,0X5D,0X2C,0X20,0X6C,0X61,0X6E,0X67,0X75,0X61,0X67,0X65,0X3A,0X20,0X27,0X63,0X27,0X29,0XA,0XA,0X6D,0X69,0X6E,0X69,0X7A,0X5F,0X64,0X65,0X70,0X20,0X3D,0X20,0X64,0X65,0X70,0X65,0X6E,0X64,0X65,0X6E,0X63,0X79,0X28,0X27,0X6D,0X69,0X6E,0X69,0X7A,0X27,0X29,0XA,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X5F,0X64,0X65,0X70,0X20,0X3D,0X20,0X64,0X65,0X70,0X65,0X6E,0X64,0X65,0X6E,0X63,0X79,0X28,0X27,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X27,0X29,0XA,0XA,0X69,0X66,0X20,0X67,0X65,0X74,0X5F,0X6F,0X70,0X74,0X69,0X6F,0X6E,0X28,0X27,0X63,0X6F,0X6D,0X70,0X72,0X65,0X73,0X73,0X27,0X29,0XA,0X20,0X20,0X20,0X20,0X6F,0X70,0X74,0X73,0X20,0X3D,0X20,0X5B,0X27,0X2D,0X44,0X54,0X45,0X34,0X5F,0X43,0X4F,0X4D,0X50,0X52,0X45,0X53,0X53,0X27,0X2C,0X27,0X2D,0X44,0X54,0X45,0X34,0X5F,0X49,0X4E,0X54,0X45,0X52,0X4E,0X41,0X4C,0X27,0X5D,0XA,0X65,0X6C,0X73,0X65,0X20,0XA,0X20,0X20,0X20,0X20,0X6F,0X70,0X74,0X73,0X3D,0X5B,0X27,0X2D,0X44,0X54,0X45,0X34,0X5F,0X49,0X4E,0X54,0X45,0X52,0X4E,0X41,0X4C,0X27,0X5D,0XA,0X65,0X6E,0X64,0X69,0X66,0XA,0XA,0X63,0X6C,0X69,0X20,0X3D,0X20,0X65,0X78,0X65,0X63,0X75,0X74,0X61,0X62,0X6C,0X65,0X28,0XA,0X20,0X20,0X20,0X20,0X27,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_MESON(proj_name){constexpr const uint8_t tmp[] = {0X27,0X2C,0XA,0X20,0X20,0X20,0X20,0X5B,0X27,0X6D,0X61,0X69,0X6E,0X2E,0X63,0X70,0X70,0X27,0X5D,0X2C,0XA,0X20,0X20,0X20,0X20,0X64,0X65,0X70,0X65,0X6E,0X64,0X65,0X6E,0X63,0X69,0X65,0X73,0X3A,0X20,0X5B,0X6D,0X69,0X6E,0X69,0X7A,0X5F,0X64,0X65,0X70,0X2C,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X5F,0X64,0X65,0X70,0X5D,0X2C,0XA,0X20,0X20,0X20,0X20,0X6C,0X69,0X6E,0X6B,0X5F,0X77,0X69,0X74,0X68,0X3A,0X20,0X5B,0X5D,0X2C,0XA,0X20,0X20,0X20,0X20,0X69,0X6E,0X63,0X6C,0X75,0X64,0X65,0X5F,0X64,0X69,0X72,0X65,0X63,0X74,0X6F,0X72,0X69,0X65,0X73,0X3A,0X20,0X5B,0X5D,0X2C,0XA,0X20,0X20,0X20,0X20,0X69,0X6E,0X73,0X74,0X61,0X6C,0X6C,0X3A,0X20,0X74,0X72,0X75,0X65,0X2C,0XA,0X29,0XA,0XA,0X23,0X69,0X6E,0X73,0X74,0X61,0X6C,0X6C,0X5F,0X6D,0X61,0X6E,0X28,0X27,0X6D,0X61,0X6E,0X2F,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_MESON(proj_name){constexpr const uint8_t tmp[] = {0X2E,0X31,0X27,0X29,0XA,0XA,0X69,0X6E,0X73,0X74,0X61,0X6C,0X6C,0X5F,0X68,0X65,0X61,0X64,0X65,0X72,0X73,0X28,0XA,0X20,0X20,0X20,0X20,0X5B,0X5D,0X2C,0XA,0X20,0X20,0X20,0X20,0X73,0X75,0X62,0X64,0X69,0X72,0X3A,0X20,0X27,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_MESON(proj_name){constexpr const uint8_t tmp[] = {0X27,0X2C,0XA,0X29,0XA,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_file_4(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/README.md",writer_file_4);} } static init;static constexpr int perms = 436;static constexpr const char* name  = "README.md";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_4.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}
auto proj_description = env.child("project").child("description");
for (pugi::xml_node child: proj_description.children()){
    if (child.type() == pugi::node_pcdata)
        WRITE(child.value());
}out.close();
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_file_5(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/subprojects/pugixml.wrap",writer_file_5);} } static init;static constexpr int perms = 436;static constexpr const char* name  = "pugixml.wrap";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_5.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X5B,0X77,0X72,0X61,0X70,0X2D,0X66,0X69,0X6C,0X65,0X5D,0XA,0X64,0X69,0X72,0X65,0X63,0X74,0X6F,0X72,0X79,0X20,0X3D,0X20,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X2D,0X31,0X2E,0X31,0X34,0XA,0X73,0X6F,0X75,0X72,0X63,0X65,0X5F,0X75,0X72,0X6C,0X20,0X3D,0X20,0X68,0X74,0X74,0X70,0X73,0X3A,0X2F,0X2F,0X67,0X69,0X74,0X68,0X75,0X62,0X2E,0X63,0X6F,0X6D,0X2F,0X7A,0X65,0X75,0X78,0X2F,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X2F,0X61,0X72,0X63,0X68,0X69,0X76,0X65,0X2F,0X76,0X31,0X2E,0X31,0X34,0X2E,0X74,0X61,0X72,0X2E,0X67,0X7A,0XA,0X73,0X6F,0X75,0X72,0X63,0X65,0X5F,0X66,0X69,0X6C,0X65,0X6E,0X61,0X6D,0X65,0X20,0X3D,0X20,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X2D,0X31,0X2E,0X31,0X34,0X2E,0X74,0X61,0X72,0X2E,0X67,0X7A,0XA,0X73,0X6F,0X75,0X72,0X63,0X65,0X5F,0X68,0X61,0X73,0X68,0X20,0X3D,0X20,0X36,0X31,0X30,0X66,0X39,0X38,0X33,0X37,0X35,0X34,0X32,0X34,0X62,0X35,0X36,0X31,0X34,0X37,0X35,0X34,0X61,0X36,0X66,0X33,0X34,0X61,0X34,0X39,0X31,0X61,0X64,0X62,0X64,0X64,0X61,0X61,0X65,0X63,0X30,0X37,0X34,0X65,0X39,0X30,0X34,0X34,0X35,0X37,0X37,0X64,0X39,0X36,0X35,0X31,0X36,0X30,0X65,0X63,0X31,0X30,0X33,0X64,0X32,0X65,0XA,0X70,0X61,0X74,0X63,0X68,0X5F,0X66,0X69,0X6C,0X65,0X6E,0X61,0X6D,0X65,0X20,0X3D,0X20,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X5F,0X31,0X2E,0X31,0X34,0X2D,0X31,0X5F,0X70,0X61,0X74,0X63,0X68,0X2E,0X7A,0X69,0X70,0XA,0X70,0X61,0X74,0X63,0X68,0X5F,0X75,0X72,0X6C,0X20,0X3D,0X20,0X68,0X74,0X74,0X70,0X73,0X3A,0X2F,0X2F,0X77,0X72,0X61,0X70,0X64,0X62,0X2E,0X6D,0X65,0X73,0X6F,0X6E,0X62,0X75,0X69,0X6C,0X64,0X2E,0X63,0X6F,0X6D,0X2F,0X76,0X32,0X2F,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X5F,0X31,0X2E,0X31,0X34,0X2D,0X31,0X2F,0X67,0X65,0X74,0X5F,0X70,0X61,0X74,0X63,0X68,0XA,0X70,0X61,0X74,0X63,0X68,0X5F,0X68,0X61,0X73,0X68,0X20,0X3D,0X20,0X32,0X33,0X63,0X65,0X61,0X62,0X62,0X64,0X37,0X62,0X63,0X37,0X34,0X61,0X36,0X63,0X63,0X36,0X63,0X32,0X65,0X36,0X66,0X36,0X32,0X35,0X61,0X39,0X62,0X30,0X38,0X34,0X30,0X61,0X30,0X37,0X30,0X65,0X31,0X32,0X32,0X35,0X64,0X38,0X35,0X64,0X33,0X63,0X63,0X32,0X39,0X63,0X32,0X37,0X62,0X36,0X63,0X63,0X30,0X35,0X39,0X31,0X33,0X35,0XA,0X73,0X6F,0X75,0X72,0X63,0X65,0X5F,0X66,0X61,0X6C,0X6C,0X62,0X61,0X63,0X6B,0X5F,0X75,0X72,0X6C,0X20,0X3D,0X20,0X68,0X74,0X74,0X70,0X73,0X3A,0X2F,0X2F,0X67,0X69,0X74,0X68,0X75,0X62,0X2E,0X63,0X6F,0X6D,0X2F,0X6D,0X65,0X73,0X6F,0X6E,0X62,0X75,0X69,0X6C,0X64,0X2F,0X77,0X72,0X61,0X70,0X64,0X62,0X2F,0X72,0X65,0X6C,0X65,0X61,0X73,0X65,0X73,0X2F,0X64,0X6F,0X77,0X6E,0X6C,0X6F,0X61,0X64,0X2F,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X5F,0X31,0X2E,0X31,0X34,0X2D,0X31,0X2F,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X2D,0X31,0X2E,0X31,0X34,0X2E,0X74,0X61,0X72,0X2E,0X67,0X7A,0XA,0X77,0X72,0X61,0X70,0X64,0X62,0X5F,0X76,0X65,0X72,0X73,0X69,0X6F,0X6E,0X20,0X3D,0X20,0X31,0X2E,0X31,0X34,0X2D,0X31,0XA,0XA,0X5B,0X70,0X72,0X6F,0X76,0X69,0X64,0X65,0X5D,0XA,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X20,0X3D,0X20,0X70,0X75,0X67,0X69,0X78,0X6D,0X6C,0X5F,0X64,0X65,0X70,0XA,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_file_6(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/subprojects/miniz.wrap",writer_file_6);} } static init;static constexpr int perms = 436;static constexpr const char* name  = "miniz.wrap";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_6.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X5B,0X77,0X72,0X61,0X70,0X2D,0X67,0X69,0X74,0X5D,0XA,0X75,0X72,0X6C,0X20,0X3D,0X20,0X68,0X74,0X74,0X70,0X73,0X3A,0X2F,0X2F,0X67,0X69,0X74,0X68,0X75,0X62,0X2E,0X63,0X6F,0X6D,0X2F,0X72,0X69,0X63,0X68,0X67,0X65,0X6C,0X39,0X39,0X39,0X2F,0X6D,0X69,0X6E,0X69,0X7A,0XA,0X72,0X65,0X76,0X69,0X73,0X69,0X6F,0X6E,0X20,0X3D,0X20,0X48,0X45,0X41,0X44,0XA,0X64,0X65,0X70,0X74,0X68,0X20,0X3D,0X20,0X31,0XA,0X6D,0X65,0X74,0X68,0X6F,0X64,0X20,0X3D,0X20,0X6D,0X65,0X73,0X6F,0X6E,0XA,0X5B,0X70,0X72,0X6F,0X76,0X69,0X64,0X65,0X5D,0XA,0X6D,0X69,0X6E,0X69,0X7A,0X20,0X3D,0X20,0X6D,0X69,0X6E,0X69,0X7A,0X5F,0X64,0X65,0X70,0XA,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_dir_1(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={}, const std::string& _name = {}){
static constexpr int perms = 509;static constexpr const char* name  = "subprojects";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
fs::create_directories(file);
if(exclude.find(file)==exclude.end())writer_file_5(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_6(file, env, exclude);
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_file_7(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/platforms/default.ini",writer_file_7);} } static init;static constexpr int perms = 436;static constexpr const char* name  = "default.ini";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_7.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X5B,0X62,0X75,0X69,0X6C,0X74,0X2D,0X69,0X6E,0X20,0X6F,0X70,0X74,0X69,0X6F,0X6E,0X73,0X5D,0XA,0X63,0X5F,0X61,0X72,0X67,0X73,0X20,0X3D,0X20,0X5B,0X27,0X2D,0X6D,0X61,0X72,0X63,0X68,0X3D,0X6E,0X61,0X74,0X69,0X76,0X65,0X27,0X2C,0X20,0X27,0X2D,0X4F,0X33,0X27,0X2C,0X20,0X27,0X2D,0X66,0X6C,0X74,0X6F,0X27,0X2C,0X20,0X27,0X2D,0X66,0X75,0X73,0X65,0X2D,0X6C,0X69,0X6E,0X6B,0X65,0X72,0X2D,0X70,0X6C,0X75,0X67,0X69,0X6E,0X27,0X5D,0XA,0X63,0X5F,0X6C,0X69,0X6E,0X6B,0X5F,0X61,0X72,0X67,0X73,0X20,0X3D,0X20,0X5B,0X27,0X2D,0X6D,0X61,0X72,0X63,0X68,0X3D,0X6E,0X61,0X74,0X69,0X76,0X65,0X27,0X2C,0X20,0X27,0X2D,0X4F,0X33,0X27,0X2C,0X20,0X27,0X2D,0X66,0X6C,0X74,0X6F,0X27,0X2C,0X20,0X27,0X2D,0X66,0X75,0X73,0X65,0X2D,0X6C,0X69,0X6E,0X6B,0X65,0X72,0X2D,0X70,0X6C,0X75,0X67,0X69,0X6E,0X27,0X2C,0X20,0X5D,0X20,0X23,0X27,0X2D,0X73,0X74,0X61,0X74,0X69,0X63,0X27,0X2C,0X20,0X27,0X2D,0X73,0X74,0X61,0X74,0X69,0X63,0X2D,0X6C,0X69,0X62,0X67,0X63,0X63,0X27,0XA,0XA,0X63,0X70,0X70,0X5F,0X61,0X72,0X67,0X73,0X20,0X3D,0X20,0X5B,0X27,0X2D,0X6D,0X61,0X72,0X63,0X68,0X3D,0X6E,0X61,0X74,0X69,0X76,0X65,0X27,0X2C,0X20,0X27,0X2D,0X4F,0X33,0X27,0X2C,0X20,0X27,0X2D,0X66,0X6C,0X74,0X6F,0X27,0X2C,0X20,0X27,0X2D,0X66,0X75,0X73,0X65,0X2D,0X6C,0X69,0X6E,0X6B,0X65,0X72,0X2D,0X70,0X6C,0X75,0X67,0X69,0X6E,0X27,0X5D,0XA,0X63,0X70,0X70,0X5F,0X6C,0X69,0X6E,0X6B,0X5F,0X61,0X72,0X67,0X73,0X20,0X3D,0X20,0X5B,0X27,0X2D,0X6D,0X61,0X72,0X63,0X68,0X3D,0X6E,0X61,0X74,0X69,0X76,0X65,0X27,0X2C,0X20,0X27,0X2D,0X4F,0X33,0X27,0X2C,0X20,0X27,0X2D,0X66,0X6C,0X74,0X6F,0X27,0X2C,0X20,0X27,0X2D,0X66,0X75,0X73,0X65,0X2D,0X6C,0X69,0X6E,0X6B,0X65,0X72,0X2D,0X70,0X6C,0X75,0X67,0X69,0X6E,0X27,0X2C,0X20,0X5D,0X20,0X23,0X27,0X2D,0X73,0X74,0X61,0X74,0X69,0X63,0X27,0X2C,0X20,0X27,0X2D,0X73,0X74,0X61,0X74,0X69,0X63,0X2D,0X6C,0X69,0X62,0X67,0X63,0X63,0X27,0X2C,0X20,0X27,0X2D,0X73,0X74,0X61,0X74,0X69,0X63,0X2D,0X6C,0X69,0X62,0X73,0X74,0X64,0X63,0X2B,0X2B,0X27,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_dir_2(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={}, const std::string& _name = {}){
static constexpr int perms = 509;static constexpr const char* name  = "platforms";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
fs::create_directories(file);
if(exclude.find(file)==exclude.end())writer_file_7(file, env, exclude);
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_dir_0(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={}, const std::string& _name = {}){
static constexpr int perms = 509;static constexpr const char* name  = "";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
fs::create_directories(file);
if(exclude.find(file)==exclude.end())writer_file_0(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_1(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_2(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_3(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_4(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_dir_1(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_dir_2(file, env, exclude);
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}

#ifndef TE4_INTERNAL
int main(int argc, const char* argv[]){
    if(argc<3)exit(1);
    pugi::xml_document doc;
    doc.load_file((fs::path(getenv("PWD"))/argv[2]).c_str());

    //Default entry point. Change by adding exclusions and further calls if you need to change its behaviour
    writer_dir_0(argv[1],doc.root());
    return 0;
}
#endif
