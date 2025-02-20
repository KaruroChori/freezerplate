#ifndef TE4_INTERNAL
#define TE4_COMPRESS
#endif
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

std::map<fs::path, writer_status_t(*)(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude, const std::string& _name)> fs_tree;

writer_status_t writer_file_0(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 509;static constexpr const char* name  = "build.sh";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_0.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X6A,0X0,0X0,0X0,0X78,0X9C,0X25,0XCC,0X31,0XE,0X80,0X20,0XC,0X46,0XE1,0XDD,0X53,0XD4,0X38,0X13,0X4E,0XE0,0XA4,0XF7,0X30,0XA0,0XBF,0X4A,0X52,0XC1,0XD0,0X76,0XF0,0XF6,0X1A,0X59,0XDF,0X97,0XBC,0XA1,0XF7,0X31,0X65,0X1F,0X83,0X9C,0XDD,0X5,0X29,0X99,0X4,0X6A,0X37,0XB9,0X39,0X2E,0XAC,0X65,0XD4,0X6A,0X20,0X72,0X2E,0X5A,0XE2,0X4D,0X9F,0X1B,0X63,0X5,0X23,0X8,0XBE,0X56,0XB1,0X96,0XBC,0XA7,0XC3,0X2A,0XE8,0X77,0XDF,0X89,0X6D,0X85,0XDA,0X27,0X65,0XD1,0XC0,0X4C,0X6E,0X6A,0XF8,0X2,0XB5,0XAB,0X25,0X6F,};
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
writer_status_t writer_file_1(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = "LICENCE.md";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_1.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X0,0X0,0X0,0X0,0X78,0X9C,0X3,0X0,0X0,0X0,0X0,0X1,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}
static std::map<std::string, std::string> common_licences = {
    // Open-source licenses
    {"MIT", "https://opensource.org/licenses/MIT"},
    {"Apache-2.0", "https://www.apache.org/licenses/LICENSE-2.0"},
    {"BSD-3-Clause", "https://opensource.org/licenses/BSD-3-Clause"},
    {"BSD-2-Clause", "https://opensource.org/licenses/BSD-2-Clause"},
    {"Unlicense", "https://unlicense.org/"},
    {"MPL-2.0", "https://www.mozilla.org/en-US/MPL/2.0/"},
    {"GPL-3.0", "https://www.gnu.org/licenses/gpl-3.0.en.html"},
    {"GPL-2.0", "https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html"},
    {"GPL-1.0", "https://www.gnu.org/licenses/old-licenses/gpl-1.0.txt"},
    {"LGPL-3.0", "https://www.gnu.org/licenses/lgpl-3.0.en.html"},
    {"LGPL-2.1", "https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html"},
    {"AGPL-3.0", "https://www.gnu.org/licenses/agpl-3.0.en.html"},

    // Creative Commons licenses
    {"CC0-1.0", "https://creativecommons.org/publicdomain/zero/1.0/"},
    {"CC-BY-4.0", "https://creativecommons.org/licenses/by/4.0/"},
    {"CC-BY-SA-4.0", "https://creativecommons.org/licenses/by-sa/4.0/"},
    {"CC-BY-ND-4.0", "https://creativecommons.org/licenses/by-nd/4.0/"},
    {"CC-BY-NC-4.0", "https://creativecommons.org/licenses/by-nc/4.0/"},
    {"CC-BY-NC-SA-4.0", "https://creativecommons.org/licenses/by-nc-sa/4.0/"},
    {"CC-BY-NC-ND-4.0", "https://creativecommons.org/licenses/by-nc-nd/4.0/"},
    {"CC-Zero-v1.0", "https://creativecommons.org/publicdomain/zero/1.0/"},
};

auto proj_licence = env.child("project").child("licences");
auto proj_author = env.child("project").attribute("author").as_string("Unknown Author");
if(!proj_licence){{constexpr const uint8_t tmp[] = {0X19,0X0,0X0,0X0,0X78,0X9C,0X1,0X19,0X0,0XE6,0XFF,0XA,0XA,0X41,0X6C,0X6C,0X20,0X72,0X69,0X67,0X68,0X74,0X73,0X20,0X72,0X65,0X73,0X65,0X72,0X76,0X65,0X64,0X20,0X62,0X79,0X20,0X67,0XF1,0X8,0X7A,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE(proj_author){constexpr const uint8_t tmp[] = {0X2,0X0,0X0,0X0,0X78,0X9C,0X1,0X2,0X0,0XFD,0XFF,0XA,0XA,0X0,0X20,0X0,0X15,};
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
writer_status_t writer_file_2(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = ".gitignore";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_2.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X57,0X0,0X0,0X0,0X78,0X9C,0X55,0X88,0X3B,0XE,0X80,0X20,0XC,0X40,0XF7,0XDE,0X82,0XD5,0X81,0X9E,0XA9,0X94,0XAA,0X55,0X12,0X1B,0XA,0XF1,0XFA,0X30,0XC2,0XF4,0X3E,0X98,0XBA,0X96,0X8C,0X80,0X59,0XBD,0X4D,0X44,0X26,0XBE,0X65,0X8A,0XF7,0X64,0XF5,0X7B,0X84,0X9B,0XE3,0X1,0X61,0XEF,0XF8,0X57,0X32,0X8,0XEB,0X33,0XE2,0X97,0X2E,0X39,0XB5,0X88,0X23,0XC,0XF3,0X8D,0X1D,0XD0,};
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
writer_status_t writer_file_3(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = "meson.build";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_3.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X0,0X0,0X0,0X0,0X78,0X9C,0X3,0X0,0X0,0X0,0X0,0X1,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}
auto proj_name = env.child("project").attribute("name").as_string("undefined");
auto proj_version = env.child("project").attribute("version").as_string("0.1.0");
{constexpr const uint8_t tmp[] = {0XE,0X0,0X0,0X0,0X78,0X9C,0X1,0XE,0X0,0XF1,0XFF,0X70,0X72,0X6F,0X6A,0X65,0X63,0X74,0X28,0XA,0X20,0X20,0X20,0X20,0X27,0X24,0X2,0X3,0XD1,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_MESON(proj_name){constexpr const uint8_t tmp[] = {0X1E,0X0,0X0,0X0,0X78,0X9C,0X53,0XD7,0XE1,0X52,0X0,0X82,0X68,0XF5,0XE4,0X82,0X2,0XF5,0X58,0X8,0XA7,0X2C,0XB5,0XA8,0X38,0X33,0X3F,0XCF,0X4A,0X41,0X1D,0X0,0X65,0X7,0X7,0X64,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_MESON(proj_version){constexpr const uint8_t tmp[] = {0X98,0X1,0X0,0X0,0X78,0X9C,0XA5,0X90,0X41,0X4B,0X3,0X31,0X10,0X85,0XEF,0XF9,0X15,0XB9,0XCD,0X96,0XEE,0XA,0X55,0X4F,0X85,0X15,0X44,0XF7,0X20,0X68,0X95,0XB6,0XE0,0XA1,0X94,0X90,0X26,0XB3,0X21,0X90,0X4D,0XC2,0X66,0X23,0XD5,0X5F,0XEF,0X6C,0X6D,0XA5,0X87,0XDE,0X9A,0XE3,0X7B,0XEF,0X7B,0X99,0X19,0X28,0X19,0XA7,0XD7,0X61,0XA,0X5E,0X7C,0X61,0X9F,0X6C,0XF0,0X73,0XE,0XF,0X35,0X9F,0XDD,0XCC,0XE0,0XCF,0XD4,0XD8,0XCA,0XEC,0X6,0X11,0XE2,0X40,0X6E,0X9A,0XF3,0XD,0XA8,0X18,0X45,0X1A,0X74,0XAD,0XA6,0XD3,0XDB,0X3B,0XD8,0X96,0X6C,0XC2,0X98,0XD4,0X5A,0X18,0X17,0X76,0XD2,0X9,0XD9,0X9B,0XDC,0XA1,0X1F,0X52,0XB1,0X81,0XEA,0XD3,0X87,0X2A,0XFB,0X9C,0X50,0X57,0X6D,0XF6,0X6A,0XEC,0X20,0X82,0X3B,0XE9,0X4D,0X96,0X6,0XE9,0X37,0X6A,0X83,0XC9,0X35,0X3C,0XD1,0XAC,0XB3,0XDE,0XFE,0X8,0X8D,0X91,0XD7,0X34,0X71,0X44,0XAF,0XD1,0XAB,0XEF,0X2,0XE,0X3A,0X5,0X62,0X36,0X76,0XDF,0XB9,0XB,0X89,0XA3,0X33,0X96,0XD8,0X96,0X1B,0X3C,0X6D,0X5A,0X80,0XA,0X5D,0XEC,0X31,0X25,0XB2,0XC6,0X43,0X90,0X9C,0X88,0XA5,0X99,0X9E,0XD7,0XCD,0XBD,0X78,0X7A,0X7F,0XFB,0X58,0X36,0XAB,0X15,0X94,0X47,0XE1,0X65,0XB1,0X6E,0X96,0X8B,0XC7,0X57,0XD8,0X32,0X74,0X9,0XF9,0X3F,0X54,0X9F,0X90,0XF3,0X84,0XD7,0XB6,0X65,0X4C,0X39,0X4B,0X95,0XB8,0X47,0X95,0X7,0XB9,0X73,0X58,0X1C,0X20,0XF8,0X5,0XF0,0XEC,0X81,0XD5,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_MESON(proj_name){constexpr const uint8_t tmp[] = {0X98,0X0,0X0,0X0,0X78,0X9C,0X3D,0X8E,0X41,0XA,0XC3,0X20,0X14,0X44,0XF7,0X9E,0X42,0XE8,0XC2,0X16,0XA4,0XDD,0XE7,0X2A,0XA1,0X88,0XE8,0XA7,0X1D,0XFA,0XFD,0X15,0XA3,0XB4,0XE4,0XF4,0X31,0X44,0X32,0XAB,0XE1,0XCD,0X30,0X8C,0XB1,0X4A,0X77,0XCD,0X26,0X79,0XC8,0X3D,0XE4,0X6C,0X9E,0X7,0X89,0X94,0X49,0X22,0X49,0X0,0X2D,0X93,0X9E,0X13,0X4,0XAB,0XEB,0XD0,0XE6,0XF6,0XC2,0X3F,0XF1,0XEE,0X47,0X95,0X21,0X1F,0XF7,0X43,0X7D,0XF7,0XDE,0X40,0X90,0XC0,0X2D,0X92,0X8B,0X28,0X14,0XEA,0XB7,0X1C,0X23,0X67,0XB8,0X54,0XCF,0X3C,0XE9,0X5A,0X1A,0X59,0X75,0X53,0XEA,0X32,0X90,0X4B,0X5E,0XAE,0XFD,0X8A,0X3C,0X36,0X19,0X8,0X30,0X14,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_MESON(proj_name){constexpr const uint8_t tmp[] = {0X2C,0X0,0X0,0X0,0X78,0X9C,0XD3,0X33,0X54,0XD7,0XE4,0XE2,0XCA,0XCC,0X2B,0X2E,0X49,0XCC,0XC9,0X89,0XCF,0X48,0X4D,0X4C,0X49,0X2D,0X2A,0XD6,0XE0,0X52,0X0,0X82,0XE8,0X58,0X1D,0X30,0X5D,0X5C,0X9A,0X94,0X92,0X59,0X64,0XA5,0XA0,0XE,0X0,0X15,0X45,0XC,0X20,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_MESON(proj_name){constexpr const uint8_t tmp[] = {0X5,0X0,0X0,0X0,0X78,0X9C,0X1,0X5,0X0,0XFA,0XFF,0X27,0X2C,0XA,0X29,0XA,0X1,0XF2,0X0,0X91,};
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
writer_status_t writer_file_4(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = "README.md";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_4.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X2,0X0,0X0,0X0,0X78,0X9C,0X1,0X2,0X0,0XFD,0XFF,0X23,0X20,0X0,0X68,0X0,0X44,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_XML(env.child("project").attribute("name").as_string("undefined")){constexpr const uint8_t tmp[] = {0X2C,0X0,0X0,0X0,0X78,0X9C,0X1,0X2C,0X0,0XD3,0XFF,0XA,0XA,0X43,0X75,0X73,0X74,0X6F,0X6D,0X20,0X74,0X65,0X6D,0X70,0X6C,0X61,0X74,0X65,0X20,0X62,0X75,0X69,0X6C,0X64,0X65,0X72,0X2E,0XA,0XA,0X23,0X23,0X20,0X44,0X65,0X73,0X63,0X72,0X69,0X70,0X74,0X69,0X6F,0X6E,0XA,0XA,0X47,0XCD,0XE,0X53,};
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
}{constexpr const uint8_t tmp[] = {0X2F,0X0,0X0,0X0,0X78,0X9C,0XE3,0XE2,0X52,0X56,0X56,0XF0,0XC8,0X2F,0X57,0X28,0XC9,0X57,0X48,0XCE,0XCF,0X2D,0XC8,0XCC,0X49,0XE5,0X42,0X12,0X2A,0X2D,0X86,0X70,0X7D,0X32,0X93,0X53,0XF3,0X92,0X53,0XB9,0X0,0X2E,0X1A,0XD,0X4,};
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
writer_status_t writer_file_5(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = "pugixml.wrap";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_5.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X43,0X2,0X0,0X0,0X78,0X9C,0X75,0X52,0X59,0X6E,0X83,0X30,0X10,0XFD,0XE7,0X14,0XBD,0X40,0XF0,0X82,0X97,0X50,0XA9,0X27,0X89,0XAA,0X68,0X6C,0XF,0X60,0XD5,0X60,0X64,0X96,0XA4,0X39,0X7D,0X21,0X21,0X5D,0XA2,0XF4,0XCF,0XD2,0XBC,0X5D,0X3E,0X9C,0X12,0XF4,0XBB,0XCA,0X7,0X7C,0XCF,0X9C,0X4F,0X68,0XC7,0X98,0X3E,0X5F,0XDE,0X5E,0XFA,0XA9,0XF6,0XE7,0X36,0XEC,0X58,0XCE,0X44,0X36,0XC4,0X29,0X59,0X3C,0X4E,0X29,0X2C,0X97,0X66,0X1C,0XFB,0XE1,0X95,0X90,0XDA,0X8F,0XCD,0X64,0X72,0X1B,0X5B,0X72,0XC1,0XE9,0X4C,0X36,0X6,0X81,0X64,0X1B,0X3F,0X23,0X99,0X57,0X6A,0X3E,0X42,0XCA,0XEB,0XCB,0X5D,0X61,0XF5,0XE9,0XA0,0XC5,0X7,0X83,0X7,0X54,0X3,0X43,0XB3,0X20,0X14,0XA3,0X55,0XB9,0X2F,0XB4,0X14,0X5C,0X18,0XA9,0X98,0X58,0X5E,0XA0,0XAA,0X42,0X80,0X28,0X19,0X38,0XE3,0X1C,0X0,0X5A,0XAA,0X5,0X96,0X54,0X8,0XA9,0XB5,0X2B,0X95,0X64,0X8A,0XA2,0X65,0XB4,0X70,0X1C,0XB3,0X1E,0X46,0XDB,0X3C,0XF1,0X3C,0XAE,0X9E,0X3B,0X76,0XBC,0XDE,0XF3,0X8B,0XEF,0X37,0XE4,0XDF,0X7E,0XEB,0X30,0XCE,0XE4,0X2D,0XE,0XB1,0X33,0X93,0XF,0XEE,0X5A,0X75,0XE6,0XF7,0XA2,0X9B,0XA,0XA9,0X71,0XBC,0X29,0X6D,0X2A,0X5B,0X7A,0X5E,0X58,0X4,0X63,0X9C,0X36,0X56,0X2F,0XB1,0XAD,0X55,0X96,0XA3,0XAA,0X14,0X97,0X50,0X1A,0XBA,0X17,0X14,0XA8,0XA6,0XC8,0X38,0X97,0X6E,0X2F,0X5D,0X61,0X2D,0X2F,0X2D,0XD7,0X66,0X1,0X52,0X59,0XB2,0X42,0X7E,0X4F,0X6,0X21,0X18,0XB0,0X1F,0XFF,0XAF,0XFF,0X93,0X70,0XCB,0X4C,0X12,0X6,0X84,0X1,0X7,0XE2,0XE2,0XA9,0XB,0X11,0XDC,0X63,0XE6,0X67,0XE3,0XDF,0XB8,0XC7,0X19,0XD3,0XE0,0X63,0XB7,0X58,0XDD,0XB0,0X59,0X76,0XE8,0X53,0X9C,0XBD,0X5B,0X3E,0XC8,0X46,0XFB,0XB5,0XA4,0XC3,0X3E,0XFB,0X2,0XF3,0X9,0XC1,0X49,};
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
writer_status_t writer_file_6(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = "miniz.wrap";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_6.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X7A,0X0,0X0,0X0,0X78,0X9C,0X1D,0X8B,0X41,0XA,0XC2,0X40,0XC,0X45,0XF7,0X39,0X45,0X2F,0XA0,0X83,0XCB,0X11,0X5C,0X8,0XA,0XDE,0XA1,0X14,0XD1,0X4E,0X68,0X2,0X9D,0XC9,0X90,0X49,0X2B,0X78,0XFA,0XA6,0X5D,0XFD,0XFF,0X1F,0XFF,0XF5,0X3F,0XFD,0XD4,0XD3,0XC4,0X36,0XC0,0XA2,0X73,0X77,0XEB,0XC8,0XAC,0XB6,0X6B,0X8,0X8E,0X68,0XF9,0X9E,0X47,0XC9,0X41,0X79,0XA4,0X9,0XE7,0X18,0X63,0XC8,0X5C,0XF8,0XF,0X8A,0X2B,0X37,0X96,0XE2,0XF7,0XD7,0XF3,0XFE,0X80,0X84,0XD5,0XC8,0XC7,0X5,0X32,0X1A,0X49,0XF2,0X9A,0XB1,0X49,0X81,0XBE,0XAA,0XAC,0X9C,0X70,0X80,0X43,0XDC,0XF9,0X9E,0X6F,0X17,0X60,0X3,0X1A,0XEC,0X29,0XB8,};
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
writer_status_t writer_file_7(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = "meson.options";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_7.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X30,0X0,0X0,0X0,0X78,0X9C,0X1,0X30,0X0,0XCF,0XFF,0X6F,0X70,0X74,0X69,0X6F,0X6E,0X28,0X27,0X63,0X6F,0X6D,0X70,0X72,0X65,0X73,0X73,0X27,0X2C,0X20,0X74,0X79,0X70,0X65,0X3A,0X20,0X27,0X62,0X6F,0X6F,0X6C,0X65,0X61,0X6E,0X27,0X2C,0X20,0X76,0X61,0X6C,0X75,0X65,0X3A,0X20,0X74,0X72,0X75,0X65,0X29,0XA5,0X60,0X10,0XBE,};
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
writer_status_t writer_file_8(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = "default.ini";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_8.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X7C,0X1,0X0,0X0,0X78,0X9C,0XBD,0X8E,0XB1,0XE,0XC2,0X30,0XC,0X44,0XF7,0X7E,0X85,0X25,0X6,0XF,0XAD,0X27,0XE6,0X7E,0X3,0X1F,0X50,0X55,0X28,0X35,0X21,0X58,0X84,0X24,0X4A,0X1C,0XBE,0X9F,0XB4,0X9D,0X98,0X2B,0X75,0XBA,0XD3,0XE9,0XEE,0XE9,0XA6,0XA5,0X8A,0X57,0X92,0X0,0X31,0XA9,0XC4,0X50,0XE6,0X8E,0XEF,0X26,0XBB,0X2,0X23,0X4C,0X48,0X1F,0X93,0XF9,0X35,0X6,0XA3,0XF2,0XB5,0X38,0X0,0XD2,0XED,0XBA,0XC9,0XD3,0X6B,0XDC,0X4D,0X2D,0X96,0XBC,0X84,0XB7,0XCD,0X94,0X7C,0X75,0X12,0X70,0X25,0XAC,0XC9,0X41,0XCC,0X0,0X33,0X5C,0X90,0X8A,0XB6,0X15,0X6F,0XA5,0XDD,0XB6,0XDA,0XE2,0X98,0XB1,0XEB,0X38,0XA5,0XE3,0X57,0X1B,0XE3,0X84,0XB3,0XFF,0X49,0XD1,0X7,0XF7,0X3D,0XFE,0X0,0XA9,0X62,0X79,0X1C,};
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
if(exclude.find(file)==exclude.end())writer_file_8(file, env, exclude);
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
if(exclude.find(file)==exclude.end())writer_file_7(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_dir_2(file, env, exclude);
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}

#ifndef TE4_INTERNAL
int main(int argc, const char* argv[]){
    if(argc<3)exit(1);
    pugi::xml_document doc;
    doc.load_file(/*(fs::path(getenv("PWD"))/argv[2]).c_str()*/argv[2]);
fs_tree.emplace("build.sh",writer_file_0);fs_tree.emplace("LICENCE.md",writer_file_1);fs_tree.emplace(".gitignore",writer_file_2);fs_tree.emplace("meson.build",writer_file_3);fs_tree.emplace("README.md",writer_file_4);fs_tree.emplace("subprojects/pugixml.wrap",writer_file_5);fs_tree.emplace("subprojects/miniz.wrap",writer_file_6);fs_tree.emplace("subprojects",writer_dir_1);fs_tree.emplace("meson.options",writer_file_7);fs_tree.emplace("platforms/default.ini",writer_file_8);fs_tree.emplace("platforms",writer_dir_2);fs_tree.emplace(".",writer_dir_0);
    //Default entry point. Change by adding exclusions and further calls if you need to change its behaviour
    #if !__has_include("impl.frag.cpp")
        writer_dir_0(argv[1], doc.root());

    #else
        #include "impl.frag.cpp"
    #endif

    return 0;
}
#endif
