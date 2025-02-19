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

std::map<fs::path, writer_status_t(*)(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude)> fs_tree;

writer_status_t writer_file_0(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/platforms/default.ini",writer_file_0);} } static init;static constexpr const char* name  = "default.ini";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_0.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X7B,0X1,0X0,0X0,0X78,0X9C,0XBD,0X8E,0XB1,0XE,0XC2,0X30,0XC,0X44,0XF7,0X7E,0X85,0X25,0X6,0XF,0XAD,0X27,0XE6,0X7E,0X3,0X1F,0X50,0X55,0X28,0X35,0X21,0X58,0X84,0X24,0X4A,0X1C,0XBE,0X9F,0XB4,0X9D,0X98,0X2B,0X75,0XBA,0XD3,0XE9,0XEE,0XE9,0XA6,0XA5,0X8A,0X57,0X92,0X0,0X31,0XA9,0XC4,0X50,0XE6,0X8E,0XEF,0X26,0XBB,0X2,0X23,0X4C,0X48,0X1F,0X93,0XF9,0X35,0X6,0XA3,0XF2,0XB5,0X38,0X0,0XD2,0XED,0XBA,0XC9,0XD3,0X6B,0XDC,0X4D,0X2D,0X96,0XBC,0X84,0XB7,0XCD,0X94,0X7C,0X75,0X12,0X70,0X25,0XAC,0XC9,0X41,0XCC,0X0,0X33,0X5C,0X90,0X8A,0XB6,0X15,0X6F,0XA5,0XDD,0XB6,0XDA,0XE2,0X98,0XB1,0XEB,0X38,0XA5,0XE3,0X57,0X1B,0XE3,0X84,0XB3,0XFF,0X49,0XD1,0X7,0XF7,0XFD,0XF,0X30,0X46,0X78,0XF5,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
return WRITER_STATUS_OK;
}
writer_status_t writer_dir_1(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={}, const std::string& _name = {}){
static constexpr const char* name  = "platforms";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
fs::create_directories(file);
if(exclude.find(file)==exclude.end())writer_file_0(file, env, exclude);
return WRITER_STATUS_OK;
}
writer_status_t writer_file_1(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/README.md",writer_file_1);} } static init;static constexpr const char* name  = "README.md";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_1.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
out.close();
return WRITER_STATUS_OK;
}
writer_status_t writer_file_2(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/meson.build",writer_file_2);} } static init;static constexpr const char* name  = "meson.build";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_2.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X0,0X0,0X0,0X0,0X78,0X9C,0X3,0X0,0X0,0X0,0X0,0X1,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
} auto proj_name = env.child("project").attribute("name").as_string("undefined"); {constexpr const uint8_t tmp[] = {0X10,0X0,0X0,0X0,0X78,0X9C,0X1,0X10,0X0,0XEF,0XFF,0XA,0XA,0X70,0X72,0X6F,0X6A,0X65,0X63,0X74,0X28,0XA,0X20,0X20,0X20,0X20,0X27,0X25,0X3A,0X3,0XE5,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE(proj_name){constexpr const uint8_t tmp[] = {0X26,0X1,0X0,0X0,0X78,0X9C,0XA5,0X8F,0X41,0XE,0X83,0X20,0X10,0X45,0XF7,0X9C,0X82,0X1D,0X1A,0XAB,0XD1,0X76,0X67,0X62,0XAF,0XD1,0X85,0X69,0X8,0XC2,0X68,0X48,0X70,0X20,0X2,0X4D,0XDB,0XD3,0X17,0XB4,0X49,0XF,0XD0,0XD9,0XCD,0XBC,0XF9,0X7F,0XFE,0XB0,0X13,0XA1,0XA9,0X46,0X26,0X9D,0X63,0XF7,0XA3,0X79,0XC0,0XE6,0XB5,0XC5,0X9E,0XB2,0XB6,0XE9,0X9A,0X96,0X1D,0XD3,0X15,0XBC,0X45,0XFE,0X63,0XD7,0X81,0X76,0X4D,0XF7,0X85,0XA,0X66,0X11,0X4D,0XE0,0XD6,0X85,0X44,0X7D,0X7F,0X18,0X72,0X1F,0XD4,0X20,0XAB,0XEA,0X7C,0XC9,0XD6,0X25,0X21,0X42,0X29,0XBE,0X18,0X3B,0X9,0XC3,0XC5,0XB6,0XC4,0X15,0X30,0XF8,0X62,0X64,0XF5,0XD,0X6D,0X1D,0X31,0X7A,0X50,0XF5,0X1C,0X51,0X66,0X8F,0XA4,0XA0,0X46,0XE0,0X12,0XC5,0X2,0XE9,0X5A,0X8E,0X57,0XFE,0XA3,0X4F,0X6A,0XB2,0X6A,0XD4,0X6F,0XAE,0XC0,0XD1,0X21,0X25,0X76,0X80,0XA,0X50,0XBE,0XA,0XB6,0XCF,0XF3,0X2,0X91,0X46,0X27,0X6,0X4F,0X90,0X31,0X88,0XC9,0X40,0XB1,0X7F,0XC7,0X3E,0X48,0XC5,0X5C,0X40,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE(proj_name){constexpr const uint8_t tmp[] = {0X97,0X0,0X0,0X0,0X78,0X9C,0X3D,0X8E,0X41,0XA,0XC3,0X20,0X14,0X44,0XF7,0X9E,0XC2,0X9D,0X2D,0X48,0XBB,0XCF,0X55,0X42,0X11,0XD1,0X4F,0X3B,0XF4,0XFB,0X2B,0X46,0X69,0XC8,0XE9,0X63,0X88,0X64,0X56,0XC3,0X9B,0X61,0X18,0X63,0X95,0XEE,0X9A,0X4D,0XF2,0X90,0X47,0XC8,0XD9,0XBC,0X4E,0X12,0X29,0X93,0X44,0X92,0X0,0X5A,0X26,0X3D,0X27,0X8,0X36,0XD7,0XA1,0XCD,0XED,0X8D,0X35,0XF1,0XE1,0X47,0X95,0X21,0X5F,0XF7,0X47,0XFD,0XF4,0XDE,0X40,0X90,0XC0,0X2D,0X92,0X8B,0X28,0X14,0XEA,0XAF,0X9C,0X23,0X57,0XB8,0X54,0XCF,0X3C,0XE9,0X5A,0X1A,0X59,0X75,0X57,0X6A,0X10,0X97,0XBC,0XDC,0XFA,0X13,0X79,0XEE,0XEC,0XDE,0X2F,0XF1,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE(proj_name){constexpr const uint8_t tmp[] = {0X2C,0X0,0X0,0X0,0X78,0X9C,0XD3,0X33,0X54,0XD7,0XE4,0XE2,0XCA,0XCC,0X2B,0X2E,0X49,0XCC,0XC9,0X89,0XCF,0X48,0X4D,0X4C,0X49,0X2D,0X2A,0XD6,0XE0,0X52,0X0,0X82,0XE8,0X58,0X1D,0X30,0X5D,0X5C,0X9A,0X94,0X92,0X59,0X64,0XA5,0XA0,0XE,0X0,0X15,0X45,0XC,0X20,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE(proj_name){constexpr const uint8_t tmp[] = {0X4,0X0,0X0,0X0,0X78,0X9C,0X1,0X4,0X0,0XFB,0XFF,0X27,0X2C,0XA,0X29,0X1,0X61,0X0,0X87,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
return WRITER_STATUS_OK;
}
writer_status_t writer_file_3(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/.gitignore",writer_file_3);} } static init;static constexpr const char* name  = ".gitignore";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_3.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X56,0X0,0X0,0X0,0X78,0X9C,0X55,0X88,0X3B,0XE,0X80,0X20,0XC,0X40,0XF7,0XDE,0X82,0XD5,0X81,0X9E,0XA9,0X94,0XAA,0X55,0X12,0X1B,0XA,0XF1,0XFA,0X30,0XC2,0XF4,0X3E,0X98,0XBA,0X96,0X8C,0X80,0X59,0XBD,0X4D,0X44,0X26,0XBE,0X65,0X8A,0XF7,0X64,0XF5,0X7B,0X84,0X9B,0XE3,0X1,0X61,0XEF,0XF8,0X57,0X32,0X8,0XEB,0X33,0XE2,0X97,0X2E,0X39,0XB5,0X88,0XE3,0X0,0XD5,0XBD,0X1D,0XC6,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
return WRITER_STATUS_OK;
}
writer_status_t writer_file_4(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/build.sh",writer_file_4);} } static init;static constexpr const char* name  = "build.sh";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_4.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X69,0X0,0X0,0X0,0X78,0X9C,0X25,0XCC,0X31,0XE,0X80,0X20,0XC,0X46,0XE1,0XDD,0X53,0XD4,0X38,0X13,0X4E,0XE0,0XA4,0XF7,0X30,0XA0,0XBF,0X4A,0X52,0XC1,0XD0,0X76,0XF0,0XF6,0X1A,0X5C,0XDF,0X97,0XBC,0XA1,0XF7,0X31,0X65,0X1F,0X83,0X9C,0XDD,0X5,0X29,0X99,0X4,0X6A,0X37,0XB9,0X39,0X2E,0XAC,0X65,0XD4,0X6A,0X20,0X72,0X2E,0X5A,0XE2,0X4D,0X9F,0X1B,0X63,0X5,0X23,0X8,0XBE,0X56,0XB1,0X96,0XBC,0XA7,0XC3,0X2A,0XA8,0XB9,0XEF,0XC4,0XB6,0X42,0XFF,0X27,0X65,0XD1,0XC0,0X4C,0X6E,0X6A,0XF8,0X2,0X90,0X3C,0X25,0XB,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
return WRITER_STATUS_OK;
}
writer_status_t writer_file_5(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/LICENCE.md",writer_file_5);} } static init;static constexpr const char* name  = "LICENCE.md";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_5.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
out.close();
return WRITER_STATUS_OK;
}
writer_status_t writer_file_6(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/subprojects/pugixml.wrap",writer_file_6);} } static init;static constexpr const char* name  = "pugixml.wrap";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_6.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X42,0X2,0X0,0X0,0X78,0X9C,0X75,0X52,0X59,0X6E,0X83,0X30,0X10,0XFD,0XE7,0X14,0XBD,0X40,0XF0,0X82,0X97,0X50,0XA9,0X27,0X89,0XAA,0X68,0X6C,0XF,0X60,0XD5,0X60,0X64,0X96,0XA4,0X39,0X7D,0X21,0X21,0X5D,0XA2,0XF4,0XCF,0XD2,0XBC,0X5D,0X3E,0X9C,0X12,0XF4,0XBB,0XCA,0X7,0X7C,0XCF,0X9C,0X4F,0X68,0XC7,0X98,0X3E,0X5F,0XDE,0X5E,0XFA,0XA9,0XF6,0XE7,0X36,0XEC,0X58,0XCE,0X44,0X36,0XC4,0X29,0X59,0X3C,0X4E,0X29,0X2C,0X97,0X66,0X1C,0XFB,0XE1,0X95,0X90,0XDA,0X8F,0XCD,0X64,0X72,0X1B,0X5B,0X72,0XC1,0XE9,0X4C,0X36,0X6,0X81,0X64,0X1B,0X3F,0X23,0X99,0X57,0X6A,0X3E,0X42,0XCA,0XEB,0XCB,0X5D,0X61,0XF5,0XE9,0XA0,0XC5,0X7,0X83,0X7,0X54,0X3,0X43,0XB3,0X20,0X14,0XA3,0X55,0XB9,0X2F,0XB4,0X14,0X5C,0X18,0XA9,0X98,0X58,0X5E,0XA0,0XAA,0X42,0X80,0X28,0X19,0X38,0XE3,0X1C,0X0,0X5A,0XAA,0X5,0X96,0X54,0X8,0XA9,0XB5,0X2B,0X95,0X64,0X8A,0XA2,0X65,0XB4,0X70,0X1C,0XB3,0X1E,0X46,0XDB,0X3C,0XF1,0X3C,0XAE,0X9E,0X3B,0X76,0XBC,0XDE,0XF3,0X8B,0XEF,0X37,0XE4,0XDF,0X7E,0XEB,0X30,0XCE,0XE4,0X2D,0XE,0XB1,0X33,0X93,0XF,0XEE,0X5A,0X75,0XE6,0XF7,0XA2,0X9B,0XA,0XA9,0X71,0XBC,0X29,0X6D,0X2A,0X5B,0X7A,0X5E,0X58,0X4,0X63,0X9C,0X36,0X56,0X2F,0XB1,0XAD,0X55,0X96,0XA3,0XAA,0X14,0X97,0X50,0X1A,0XBA,0X17,0X14,0XA8,0XA6,0XC8,0X38,0X97,0X6E,0X2F,0X5D,0X61,0X2D,0X2F,0X2D,0XD7,0X66,0X1,0X52,0X59,0XB2,0X42,0X7E,0X4F,0X6,0X21,0X18,0XB0,0X1F,0XFF,0XAF,0XFF,0X93,0X70,0XCB,0X4C,0X12,0X6,0X84,0X1,0X7,0XE2,0XE2,0XA9,0XB,0X11,0XDC,0X63,0XE6,0X67,0XE3,0XDF,0XB8,0XC7,0X19,0XD3,0XE0,0X63,0XB7,0X58,0XDD,0XB0,0X59,0X76,0XE8,0X53,0X9C,0XBD,0X5B,0X3E,0XC8,0X46,0XFB,0XB5,0XA4,0XC3,0XFE,0XB,0X31,0XC0,0XC1,0X3F,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
return WRITER_STATUS_OK;
}
writer_status_t writer_file_7(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/){
struct init_t{ init_t(){fs_tree.emplace("./template/subprojects/miniz.wrap",writer_file_7);} } static init;static constexpr const char* name  = "miniz.wrap";
fs::path file = dir / name;
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_7.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X79,0X0,0X0,0X0,0X78,0X9C,0X1D,0X8B,0X41,0XA,0XC2,0X40,0XC,0X45,0XF7,0X39,0X45,0X2F,0XA0,0X83,0XCB,0X11,0X5C,0X8,0XA,0XDE,0XA1,0X14,0XD1,0X4E,0X68,0X2,0X9D,0XC9,0X90,0X49,0X2B,0X78,0XFA,0XA6,0X5D,0XFD,0XFF,0X1F,0XFF,0XF5,0X3F,0XFD,0XD4,0XD3,0XC4,0X36,0XC0,0XA2,0X73,0X77,0XEB,0XC8,0XAC,0XB6,0X6B,0X8,0X8E,0X68,0XF9,0X9E,0X47,0XC9,0X41,0X79,0XA4,0X9,0XE7,0X18,0X63,0XC8,0X5C,0XF8,0XF,0X8A,0X2B,0X37,0X96,0XE2,0XF7,0XD7,0XF3,0XFE,0X80,0X84,0XD5,0XC8,0XC7,0X5,0X32,0X1A,0X49,0XF2,0X9A,0XB1,0X49,0X81,0XBE,0XAA,0XAC,0X9C,0X70,0X80,0X43,0XDC,0XF9,0X9E,0X6F,0X17,0X36,0XF1,0X25,0X29,0XAE,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}out.close();
return WRITER_STATUS_OK;
}
writer_status_t writer_dir_2(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={}, const std::string& _name = {}){
static constexpr const char* name  = "subprojects";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
fs::create_directories(file);
if(exclude.find(file)==exclude.end())writer_file_6(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_7(file, env, exclude);
return WRITER_STATUS_OK;
}
writer_status_t writer_dir_0(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={}, const std::string& _name = {}){
static constexpr const char* name  = "";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
fs::create_directories(file);
if(exclude.find(file)==exclude.end())writer_dir_1(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_1(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_2(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_3(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_4(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_file_5(file, env, exclude);
if(exclude.find(file)==exclude.end())writer_dir_2(file, env, exclude);
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
