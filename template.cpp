#ifndef TE4_INTERNAL
#define TE4_COMPRESS
#endif
//Automatically generated.
//If you want to include changes, please do that in the extra files `head.frag.cpp` and `body.frag.cpp`.
//This unit should not be touched.

#include <iostream>
#include <filesystem>
#include <fstream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <optional>
#include <charconv>
#include <unistd.h>

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

class TIME {
public:
    // Static variables to store the computed values
    inline static std::string currentYear;
    inline static std::string currentDate;
    inline static std::string currentTime;
    inline static std::string timestamp;
    
public:
    // Static member functions to get the current year, date, time, and timestamp
    inline static std::string YEAR() { return currentYear; }
    inline static std::string DATE() { return currentDate; }
    inline static std::string CLOCK() { return currentTime; }
    inline static std::string TIMESTAMP() { return timestamp; }

public:
    // Static function to initialize the static variables
    static void initialize() {
        std::time_t now = std::time(nullptr);
        std::tm* utcTime = std::gmtime(&now);

        // Get current year
        currentYear = std::to_string(1900 + utcTime->tm_year);

        // Format current date (YYYY-MM-DD)
        char dateBuffer[11];
        std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", utcTime);
        currentDate = dateBuffer;

        // Format current time (HH:MM:SS)
        char timeBuffer[9];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", utcTime);
        currentTime = timeBuffer;

        // Get timestamp
        timestamp = std::to_string(now);
    }
};

struct xml_path_node {
  const char *path;
  size_t str_len;
  pugi::xml_node operator()(pugi::xml_node ref, pugi::xml_node def = {}) {
    char str[str_len + 1];
    memcpy(str, path, str_len + 1);
    str[str_len] = 0;

    size_t idx = 0;

    // Recurse over **/ blocks
    for (;;) {
      size_t close = idx;
      for (; close < str_len && str[close] != '/' && str[close] != '~'; close++)
        ;
      char oldc = str[close];
      str[close] = 0;
      if (idx != close)
        ref = ref.child(str + idx); // Avoid the prefix /
      if (close == str_len)
        return ref; // If the end of string was met earlier
      else if (oldc == '~') {
        str[close] = oldc;
        idx = close;
        break;
      } else {
        idx = close + 1;
      }
    }

    return def;
  }
};

struct xml_path_attr {
  const char *path;
  size_t str_len;
  std::optional<std::string> operator()(pugi::xml_node ref,
                                        const char *def = {}) {
    char str[str_len + 1];
    memcpy(str, path, str_len + 1);
    str[str_len] = 0;

    size_t idx = 0;

    // Recurse over **/ blocks
    for (;;) {
      size_t close = idx;
      for (; close < str_len && str[close] != '/' && str[close] != '~'; close++)
        ;
      char oldc = str[close];
      str[close] = 0;
      if (idx != close)
        ref = ref.child(str + idx); // Avoid the prefix /
      if (close == str_len) {
        if (def != nullptr)
          return def;
        else
          return {};
      } else if (oldc == '~') {
        str[close] = oldc;
        idx = close;
        break;
      } else {
        idx = close + 1;
      }
    }
    // Process the terminal attributes and special properties name & text
    if (str[idx] == '~') {
      if (strcmp(str + idx + 1, "!txt") == 0) {
        std::string txt;
        for (auto child : ref.children()) {
          if (child.type() == pugi::node_pcdata ||
              child.type() == pugi::node_cdata)
            txt += child.text().as_string();
        }
        return txt;
      } else if (strcmp(str + idx + 1, "!tag") == 0)
        return ref.name();
      else {
        auto t = ref.attribute(str + idx + 1).as_string(def);
        if (t == nullptr)
          return {};
        else
          return t;
      }
    }

    return {};
  }

  template <typename T> T as(pugi::xml_node ref, T def = {}) {
    auto tmp = this->operator()(ref);
    if (tmp.has_value()) {
      T ret = def;
      std::from_chars<T>(tmp->c_str(), tmp->c_str() + tmp->size(), ret);
      return ret;
    } else
      return def;
  }
};

xml_path_node operator""_node(const char *str, size_t l) { return {str, l}; }
xml_path_attr operator""_attr(const char *str, size_t l) { return {str, l}; }

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

#if __has_include("head.frag.cpp")
  #include "head.frag.cpp"
#endif
writer_status_t writer_file_0(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 509;static constexpr const char* name  = "build.sh";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_0.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X81,0X0,0X0,0X0,0X78,0X9C,0X3D,0XCC,0X41,0XE,0XC2,0X30,0XC,0X44,0XD1,0X7D,0X4F,0X61,0XC4,0X3A,0XCA,0X9,0XBA,0X82,0X7B,0XA0,0XA4,0X1D,0XC0,0X92,0X1B,0X47,0XB1,0XBD,0XE0,0XF6,0X54,0X14,0X75,0XFB,0XDF,0X68,0XAE,0X97,0X5C,0XB9,0XE5,0X5A,0XEC,0X3D,0X6D,0X30,0X6D,0X64,0XF0,0XE8,0X94,0XEE,0XF5,0X21,0XAE,0XB3,0X8F,0X0,0X51,0X4A,0X35,0X58,0X56,0XFF,0X74,0XCC,0X3,0X82,0X62,0XD8,0XDB,0XC0,0XA2,0XED,0XC9,0XAF,0X18,0XA0,0X9F,0XE7,0XFF,0XC5,0XA2,0X5B,0X67,0XD9,0X27,0XB7,0XA3,0X4F,0X16,0XAB,0XD2,0X61,0XDC,0XCC,0X8B,0XC8,0X69,0X5F,0X6D,0X87,0X2D,0X64,};
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

auto proj_licence = env.child("project").child("licence");
auto proj_author = env.child("project").attribute("author").as_string("Unknown Author");
if(!proj_licence){{constexpr const uint8_t tmp[] = {0X18,0X0,0X0,0X0,0X78,0X9C,0X1,0X18,0X0,0XE7,0XFF,0XA,0X41,0X6C,0X6C,0X20,0X72,0X69,0X67,0X68,0X74,0X73,0X20,0X72,0X65,0X73,0X65,0X72,0X76,0X65,0X64,0X20,0X62,0X79,0X20,0X66,0XF6,0X8,0X70,};
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
/*else if(proj_licence.attribute("mnemonic").as_string(nullptr)!=nullptr){
    
}*/else WRITE("/project/licence~!txt"_attr(env).value_or("All rights reserved"))out.close();
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
}auto proj_name = env.child("project").attribute("name").as_string("undefined");{constexpr const uint8_t tmp[] = {0X0,0X0,0X0,0X0,0X78,0X9C,0X3,0X0,0X0,0X0,0X0,0X1,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}auto proj_version = env.child("project").attribute("version").as_string("0.1.0");{constexpr const uint8_t tmp[] = {0XE,0X0,0X0,0X0,0X78,0X9C,0X1,0XE,0X0,0XF1,0XFF,0X70,0X72,0X6F,0X6A,0X65,0X63,0X74,0X28,0XA,0X20,0X20,0X20,0X20,0X27,0X24,0X2,0X3,0XD1,};
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
}WRITE_XML("/project~name"_attr(env).value_or("undefined")){constexpr const uint8_t tmp[] = {0X67,0X0,0X0,0X0,0X78,0X9C,0X4D,0XCA,0X31,0XE,0X80,0X20,0XC,0X0,0XC0,0X9D,0X57,0X74,0XD4,0X45,0X76,0X57,0X46,0X9F,0X60,0X1C,0X80,0X54,0X21,0X81,0X94,0X94,0X36,0X46,0X5F,0XAF,0X71,0X72,0XBE,0X33,0XC6,0X69,0X17,0XAA,0XD0,0X3C,0XFB,0X8A,0XC2,0X39,0X82,0X60,0X6D,0XC5,0XB,0X42,0XD0,0X5C,0X4,0XCE,0X2C,0X9,0XD6,0X9D,0X11,0X6F,0XE4,0XF,0XB6,0X21,0X89,0XB4,0X3E,0X5B,0X7B,0XBC,0XA6,0X61,0X8A,0X54,0XED,0XE2,0X59,0X99,0X5C,0X22,0XCE,0XF6,0X9F,0X47,0X8,0X17,0X3C,0XBF,0X21,0X26,0X48,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_XML("/project~author"_attr(env).value_or("anonymous")){constexpr const uint8_t tmp[] = {0X3,0X0,0X0,0X0,0X78,0X9C,0X1,0X3,0X0,0XFC,0XFF,0X2E,0XA,0XA,0X0,0XAB,0X0,0X43,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}
auto repo = "/project~repo"_attr(env);
if(repo.has_value()){
{constexpr const uint8_t tmp[] = {0X1F,0X0,0X0,0X0,0X78,0X9C,0X1,0X1F,0X0,0XE0,0XFF,0XA,0X59,0X6F,0X75,0X20,0X63,0X61,0X6E,0X20,0X66,0X69,0X6E,0X64,0X20,0X75,0X73,0X20,0X69,0X6E,0X20,0X5B,0X6F,0X75,0X72,0X20,0X72,0X65,0X70,0X6F,0X5D,0X28,0XA3,0XE7,0XA,0X86,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_XML(repo.value()){constexpr const uint8_t tmp[] = {0X4,0X0,0X0,0X0,0X78,0X9C,0X1,0X4,0X0,0XFB,0XFF,0X29,0X2E,0XA,0XA,0X1,0X50,0X0,0X6C,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}
}
{constexpr const uint8_t tmp[] = {0X11,0X0,0X0,0X0,0X78,0X9C,0X1,0X11,0X0,0XEE,0XFF,0XA,0X23,0X23,0X20,0X44,0X65,0X73,0X63,0X72,0X69,0X70,0X74,0X69,0X6F,0X6E,0XA,0XA,0X29,0XFA,0X5,0X9,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}
auto description = "/project/description~!txt"_attr(env);
WRITE_XML(description.value_or("No description provided"))
{constexpr const uint8_t tmp[] = {0X4,0X2,0X0,0X0,0X78,0X9C,0X55,0X91,0X31,0X4F,0XC3,0X30,0X10,0X85,0X77,0XFF,0X8A,0X93,0X58,0XA0,0X2D,0X89,0X4,0X5B,0XB7,0X8A,0X5,0X6,0XC4,0X0,0XB,0XAA,0X90,0XEA,0XA6,0X97,0XE4,0X54,0XC7,0X17,0XCE,0X76,0X22,0XF8,0XF5,0X9C,0X13,0X85,0XC2,0X64,0XFB,0XFC,0XFC,0XEE,0X7B,0X3E,0X73,0X75,0X5,0X8F,0X3C,0X42,0X64,0XA8,0XB8,0XEB,0XC9,0XA1,0X31,0XEF,0X9C,0X60,0X24,0XE7,0XC0,0X23,0X9E,0XB6,0XC6,0XDC,0XC2,0XE,0X4,0X2B,0XF4,0X11,0X6,0X94,0X40,0XEC,0X81,0X6B,0XD8,0X77,0X18,0XD8,0X7F,0X5C,0XB7,0X31,0XF6,0X61,0X5B,0X96,0XD3,0XF1,0X98,0XC8,0X9D,0XA,0X75,0X2A,0X6F,0XA,0X78,0X4E,0XEC,0XCB,0X57,0XDB,0X25,0XB1,0X34,0X1B,0X5A,0X17,0X18,0X46,0X96,0XF3,0X6,0X8E,0X29,0X2,0X45,0XF5,0XFD,0X4C,0X24,0X18,0XA0,0X6A,0XAD,0X6F,0X74,0X25,0XF,0XB1,0X45,0X38,0XDA,0XD0,0X42,0XA8,0X84,0XFA,0X8,0X35,0X8B,0XCA,0XD5,0X98,0X7C,0X53,0X28,0XCD,0X53,0XD,0X5F,0X19,0XD1,0X2A,0XD0,0X85,0X7B,0X92,0XAD,0X56,0XA3,0XD,0XDD,0XFD,0XDD,0X6A,0X35,0X4B,0X96,0X14,0X93,0XA7,0XB3,0X11,0XC3,0XBF,0XC,0XB9,0XBA,0XFF,0XA6,0X46,0X6D,0XD8,0X29,0X1,0XFD,0XC9,0XA3,0X65,0XA7,0X48,0X5,0X4B,0X93,0XC3,0XBC,0XA8,0X56,0X96,0X5E,0X12,0XC0,0XA,0XC2,0X40,0XF6,0XE8,0X70,0XCE,0XF2,0XDB,0XAE,0XB5,0X3,0X66,0XAC,0X5E,0X78,0XA0,0X13,0XE6,0XB,0X1,0X1E,0XBD,0XBE,0XF5,0X35,0X35,0XFA,0X19,0X31,0X77,0XAF,0XD5,0X26,0X4C,0XCC,0X6A,0XDC,0XE5,0X58,0X3B,0X8,0XA9,0XEF,0X59,0XA2,0XE2,0X3E,0XAC,0XD7,0X17,0XA6,0XD,0X38,0X3A,0X23,0X74,0X7C,0X42,0X51,0X9B,0X4C,0X5,0XFA,0XAE,0XA9,0XAA,0X25,0X4B,0X28,0X8C,0X79,0X6B,0XD1,0X83,0X24,0XAF,0X3,0X3B,0X1C,0XE,0XF9,0XFF,0X4C,0X51,0XCE,0XF3,0XD0,0XAD,0X96,0X8C,0XB9,0XCC,0X3A,0X5,0X9D,0XF3,0X22,0XFB,0X1,0X87,0X37,0XB1,0XBF,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_XML("/project~name"_attr(env).value_or("undefined")){constexpr const uint8_t tmp[] = {0X28,0X0,0X0,0X0,0X78,0X9C,0X1,0X28,0X0,0XD7,0XFF,0X20,0X64,0X65,0X73,0X74,0X2D,0X64,0X69,0X72,0X20,0X5B,0X63,0X6F,0X6E,0X66,0X69,0X67,0X2E,0X78,0X6D,0X6C,0X5D,0XA,0X60,0X60,0X60,0XA,0XA,0X23,0X23,0X20,0X4C,0X69,0X63,0X65,0X6E,0X63,0X65,0XA,0XA,0X12,0X43,0XC,0X75,};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
}WRITE_XML("/project/licence~mnemonic"_attr(env).value_or("Unkown")){constexpr const uint8_t tmp[] = {0X24,0X0,0X0,0X0,0X78,0X9C,0X1,0X24,0X0,0XDB,0XFF,0X2C,0X20,0X63,0X68,0X65,0X63,0X6B,0X20,0X5B,0X2E,0X2F,0X4C,0X49,0X43,0X45,0X4E,0X43,0X45,0X2E,0X6D,0X64,0X5D,0X20,0X66,0X6F,0X72,0X20,0X64,0X65,0X74,0X61,0X69,0X6C,0X73,0X2E,0XA,0XC6,0XA3,0XB,0X17,};
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
if(exclude.find("subprojects/pugixml.wrap")==exclude.end())writer_file_5(file, env, exclude);
if(exclude.find("subprojects/miniz.wrap")==exclude.end())writer_file_6(file, env, exclude);
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
static constexpr int perms = 436;static constexpr const char* name  = "page.1";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_8.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
out.close();
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_dir_2(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={}, const std::string& _name = {}){
static constexpr int perms = 509;static constexpr const char* name  = "man";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
fs::create_directories(file);
if(exclude.find("man/page.1")==exclude.end())writer_file_8(file, env, exclude);
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_file_9(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = "default.ini";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_9.\n";return WRITER_STATUS_SKIP;}
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
writer_status_t writer_file_10(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = "zig-wasm32.ini";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_10.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
{constexpr const uint8_t tmp[] = {0X71,0X2,0X0,0X0,0X78,0X9C,0X9D,0X90,0XBB,0X6E,0XC3,0X30,0XC,0X45,0X77,0X7F,0X85,0X80,0XE,0X1A,0X12,0X25,0X40,0X3B,0X7B,0XEB,0X5F,0X4,0X85,0XC1,0XC8,0XB4,0X43,0X54,0X2F,0X88,0X34,0X5A,0XF7,0XEB,0X4B,0XDB,0X1D,0X9C,0X8E,0XDE,0XA8,0X73,0X75,0XF9,0XB8,0X2F,0X11,0X39,0X27,0XC3,0X28,0X53,0X31,0XCE,0X55,0XF4,0X39,0XD,0X34,0X4E,0X15,0XCD,0X7D,0XA2,0XD0,0X5F,0X15,0XAE,0X85,0XCC,0X5,0XDB,0X8A,0X1,0X81,0X51,0X99,0XAF,0X99,0XD9,0XD,0X14,0XD0,0X5C,0XAE,0X25,0X80,0XC,0XB9,0X46,0XBE,0XFE,0XD0,0XE8,0XBE,0X80,0XE3,0XDB,0XEB,0X85,0X12,0X19,0XF7,0XDE,0XE3,0X0,0X53,0X90,0X2E,0XD0,0XBD,0X42,0X9D,0X5B,0X16,0X10,0XF2,0X4D,0X73,0XBB,0X53,0X82,0X4A,0XC8,0X1F,0X8D,0X37,0XAD,0XB9,0X59,0X75,0XDA,0XB3,0XB1,0XDE,0X5B,0X25,0XA5,0X3C,0XB1,0XD3,0X49,0X21,0XD4,0X3D,0X83,0XAA,0XA8,0X42,0XD2,0XBE,0X7B,0XBC,0X11,0X95,0XFE,0XF1,0XD,0XF6,0X21,0X48,0XCE,0X61,0X2F,0XFC,0X21,0X15,0X59,0X2A,0X3D,0X8D,0X5D,0X81,0XA,0XBA,0XAB,0X6,0X20,0X8E,0X92,0XC9,0X45,0X28,0XA7,0X65,0XE7,0XE,0XEA,0XC8,0XEB,0X6F,0XE7,0X44,0X6B,0X94,0X76,0XBB,0X7B,0X39,0X9F,0XEC,0XD9,0XBA,0X21,0X65,0X87,0XDF,0X1E,0X37,0XCB,0X72,0X95,0X86,0X90,0X3E,0X8F,0X18,0X4B,0X39,0XE8,0X3A,0X38,0XB0,0XB9,0X3D,0X32,0X4B,0X17,0XC1,0X3F,0X28,0XA1,0X46,0X33,0XB3,0X60,0XD4,0X1E,0X16,0X23,0X7B,0X4D,0X45,0X30,0X59,0X1D,0X30,0X75,0X3,0X44,0XA,0XF3,0XA2,0X6C,0X3D,0X57,0XBA,0X7F,0X62,0XEA,0X9,0XD2,0X42,0X2,0X89,0X4,0XB4,0XBF,0XD4,0XB4,0XCE,0XFD,};
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
writer_status_t writer_file_11(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={} /*not used*/, const std::string& _name = {} /*not used*/){
static constexpr int perms = 436;static constexpr const char* name  = "cosmopolitan.ini";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
if(fs::exists(file) && no_override){std::cerr<<"File "<<file<<" already there. It will not be overwritten by rule writer_file_11.\n";return WRITER_STATUS_SKIP;}
std::ofstream out(file, std::ios::binary);
out.close();
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_dir_3(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={}, const std::string& _name = {}){
static constexpr int perms = 509;static constexpr const char* name  = "platforms";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
fs::create_directories(file);
if(exclude.find("platforms/default.ini")==exclude.end())writer_file_9(file, env, exclude);
if(exclude.find("platforms/zig-wasm32.ini")==exclude.end())writer_file_10(file, env, exclude);
if(exclude.find("platforms/cosmopolitan.ini")==exclude.end())writer_file_11(file, env, exclude);
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}
writer_status_t writer_dir_0(const fs::path& dir, const env_t& env, const std::set<fs::path>& exclude={}, const std::string& _name = {}){
static constexpr int perms = 509;static constexpr const char* name  = "";
fs::path file = dir / (_name.length()!=0?_name.c_str():name);
fs::create_directories(file);
if(exclude.find("build.sh")==exclude.end())writer_file_0(file, env, exclude);
if(exclude.find("LICENCE.md")==exclude.end())writer_file_1(file, env, exclude);
if(exclude.find(".gitignore")==exclude.end())writer_file_2(file, env, exclude);
if(exclude.find("meson.build")==exclude.end())writer_file_3(file, env, exclude);
if(exclude.find("README.md")==exclude.end())writer_file_4(file, env, exclude);
if(exclude.find("subprojects")==exclude.end())writer_dir_1(file, env, exclude);
if(exclude.find("meson.options")==exclude.end())writer_file_7(file, env, exclude);
if(exclude.find("man")==exclude.end())writer_dir_2(file, env, exclude);
if(exclude.find("platforms")==exclude.end())writer_dir_3(file, env, exclude);
fs::permissions(file,fs::perms(perms), fs::perm_options::replace);
return WRITER_STATUS_OK;
}

void g_inits(){
  TIME::initialize();
fs_tree.emplace("build.sh",writer_file_0);fs_tree.emplace("LICENCE.md",writer_file_1);fs_tree.emplace(".gitignore",writer_file_2);fs_tree.emplace("meson.build",writer_file_3);fs_tree.emplace("README.md",writer_file_4);fs_tree.emplace("subprojects/pugixml.wrap",writer_file_5);fs_tree.emplace("subprojects/miniz.wrap",writer_file_6);fs_tree.emplace("subprojects",writer_dir_1);fs_tree.emplace("meson.options",writer_file_7);fs_tree.emplace("man/page.1",writer_file_8);fs_tree.emplace("man",writer_dir_2);fs_tree.emplace("platforms/default.ini",writer_file_9);fs_tree.emplace("platforms/zig-wasm32.ini",writer_file_10);fs_tree.emplace("platforms/cosmopolitan.ini",writer_file_11);fs_tree.emplace("platforms",writer_dir_3);fs_tree.emplace(".",writer_dir_0);
}

#ifndef TE4_INTERNAL

void printHelp(const char* arg0) {
  std::cout << "Usage: "<<arg0<<" [options] source "
               "dest [env-file]\n"
            << "Options:\n"
            << "  -f, --force        Enable the override flag. Existing files "
               "will be overwritten.\n"
            << "  -h, --help         Display this help message and exit.\n"
            << "\n"
            << "Arguments:\n"
            << "  destination  Path to the directory where the "
               "template is expanded.\n"
            << "  env-file     Name of the environment file in XML "
               "format. If omitted, content will be read from stdin.\n"
            << "More info using man pages\n";
}

int main(int argc, char* argv[]){
    g_inits();

    std::string dest_folder;
    std::string src_file;
    bool no_bundle = false;

    pugi::xml_document doc = {};

    int opt;
    while ((opt = getopt(argc, argv, "fh")) != -1) {
        switch (opt) {
        case 'f':
        no_override = false;
        break;
        case 'h':
        case '?':
        printHelp(argv[0]);
        return 1;
        }
    }

    // Check positional arguments
    if (optind < argc) {
        dest_folder = argv[optind++];
    } else {
        std::cerr << "Error: Destination folder is required." << std::endl;
        return 1;
    }

    if (optind < argc) {
        src_file = argv[optind++];
        doc.load_file(src_file.c_str());
    } else {
        std::cout << "Enter file content (Ctrl+D to end):" << std::endl;
        std::ostringstream buffer;
        buffer << std::cin.rdbuf(); // Read all input until EOF
        doc.load_string(buffer.str().c_str());
    }

fs_tree.emplace("build.sh",writer_file_0);fs_tree.emplace("LICENCE.md",writer_file_1);fs_tree.emplace(".gitignore",writer_file_2);fs_tree.emplace("meson.build",writer_file_3);fs_tree.emplace("README.md",writer_file_4);fs_tree.emplace("subprojects/pugixml.wrap",writer_file_5);fs_tree.emplace("subprojects/miniz.wrap",writer_file_6);fs_tree.emplace("subprojects",writer_dir_1);fs_tree.emplace("meson.options",writer_file_7);fs_tree.emplace("man/page.1",writer_file_8);fs_tree.emplace("man",writer_dir_2);fs_tree.emplace("platforms/default.ini",writer_file_9);fs_tree.emplace("platforms/zig-wasm32.ini",writer_file_10);fs_tree.emplace("platforms/cosmopolitan.ini",writer_file_11);fs_tree.emplace("platforms",writer_dir_3);fs_tree.emplace(".",writer_dir_0);
    //Default entry point. Change by adding exclusions and further calls if you need to change its behaviour
    #if !__has_include("body.frag.cpp")
        writer_dir_0(dest_folder, doc.root());

    #else
        #include "body.frag.cpp"
    #endif

    return 0;
}
#endif
