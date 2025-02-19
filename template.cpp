#ifndef TE4_INTERNAL
#define TE4_COMPRESS
#endif
// Automatically generated, if you want to include changes, please do that in
// the main function The rest should not be touched.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
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

#define WRITE(x)                                                               \
  {                                                                            \
    std::string tmp = x;                                                       \
    out.write(tmp.data(), tmp.size());                                         \
  }

namespace fs = std::filesystem;

typedef enum {
  WRITER_STATUS_ERROR,
  WRITER_STATUS_OK,
  WRITER_STATUS_SKIP,
} writer_status_t;

typedef pugi::xml_node env_t;

writer_status_t
writer_file_0(const fs::path &dir, const env_t &env,
              const std::set<fs::path> &exclude = {} /*not used*/) {
  static constexpr const char *name = "LICENCE.md";
  fs::path file = dir / name;
  if (fs::exists(file)) {
    std::cerr << "File " << file
              << " already there. It will not be overwritten by rule "
                 "writer_file_0.\n";
    return WRITER_STATUS_SKIP;
  }
  std::ofstream out(file, std::ios::binary);
  out.close();
  return WRITER_STATUS_OK;
}
writer_status_t
writer_file_1(const fs::path &dir, const env_t &env,
              const std::set<fs::path> &exclude = {} /*not used*/) {
  static constexpr const char *name = ".gitignore";
  fs::path file = dir / name;
  if (fs::exists(file)) {
    std::cerr << "File " << file
              << " already there. It will not be overwritten by rule "
                 "writer_file_1.\n";
    return WRITER_STATUS_SKIP;
  }
  std::ofstream out(file, std::ios::binary);
  {
    constexpr const uint8_t tmp[] = {
        0X3B, 0X0,  0X0,  0X0,  0X78, 0X9C, 0X55, 0XC7, 0X3D, 0XE,  0X0,
        0X10, 0XC,  0X40, 0XE1, 0XDD, 0X2D, 0XAC, 0X6,  0X3D, 0X13, 0X25,
        0X41, 0X24, 0X9A, 0XFE, 0XC4, 0XF5, 0X75, 0X35, 0XBD, 0XF7, 0X41,
        0XB5, 0XB9, 0X1B, 0X4,  0X68, 0X53, 0XD4, 0X93, 0XB1, 0XE0, 0XE8,
        0X3E, 0X62, 0X95, 0XF8, 0XAC, 0X8E, 0X2A, 0X90, 0X42, 0XFC, 0X9D,
        0X2F, 0X17, 0X7A, 0X39, 0X14, 0X13, 0XAA,
    };
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp, sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp, sizeof(tmp));
#endif
    out.write((const char *)dtmp.data(), dtmp.size());
  }
  out.close();
  return WRITER_STATUS_OK;
}
writer_status_t
writer_file_2(const fs::path &dir, const env_t &env,
              const std::set<fs::path> &exclude = {} /*not used*/) {
  static constexpr const char *name = "meson.build";
  fs::path file = dir / name;
  if (fs::exists(file)) {
    std::cerr << "File " << file
              << " already there. It will not be overwritten by rule "
                 "writer_file_2.\n";
    return WRITER_STATUS_SKIP;
  }
  std::ofstream out(file, std::ios::binary);
  {
    constexpr const uint8_t tmp[] = {
        0XE,  0X0,  0X0,  0X0,  0X78, 0X9C, 0X1,  0XE,  0X0,  0XF1,
        0XFF, 0X70, 0X72, 0X6F, 0X6A, 0X65, 0X63, 0X74, 0X28, 0XA,
        0X20, 0X20, 0X20, 0X20, 0X27, 0X24, 0X2,  0X3,  0XD1,
    };
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp, sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp, sizeof(tmp));
#endif
    out.write((const char *)dtmp.data(), dtmp.size());
  }
  WRITE(env.child("project").attribute("name").as_string("undefined")) {
    constexpr const uint8_t tmp[] = {
        0X6F, 0X3,  0X0,  0X0,  0X78, 0X9C, 0XA5, 0X53, 0XCB, 0X6E, 0XDB, 0X30,
        0X10, 0XBC, 0XEB, 0X2B, 0X4,  0XF8, 0X40, 0X19, 0X89, 0XE5, 0XB8, 0XCD,
        0XC9, 0X80, 0X7B, 0X49, 0X73, 0X2A, 0X8A, 0X16, 0X4D, 0X81, 0X1E, 0XC,
        0X83, 0XA0, 0XC9, 0XB5, 0XCC, 0X86, 0X22, 0X9,  0X3E, 0XFA, 0XFA, 0XFA,
        0XE,  0X25, 0X59, 0X31, 0X72, 0X8D, 0XE,  0X2,  0XB8, 0X33, 0XBB, 0X3B,
        0X3B, 0X4B, 0XB2, 0XDB, 0XAA, 0XC6, 0XB7, 0X67, 0XD2, 0X7B, 0X76, 0X18,
        0XF,  0XBF, 0X28, 0X44, 0XED, 0XEC, 0XB6, 0X66, 0X77, 0XED, 0XA6, 0XBD,
        0X63, 0X63, 0XB4, 0XA7, 0XE8, 0X2C, 0X7F, 0XC1, 0X3E, 0XEC, 0XEA, 0X4D,
        0XBB, 0X99, 0X40, 0X45, 0X27, 0X91, 0X4D, 0XE2, 0XCE, 0X27, 0XA0, 0X71,
        0X3B, 0X16, 0XE4, 0X31, 0XA9, 0X9D, 0XBC, 0XB9, 0X79, 0XF7, 0XBE, 0X94,
        0X5E, 0X56, 0X95, 0X50, 0X8A, 0X77, 0XC6, 0X1D, 0X85, 0XE1, 0X22, 0X74,
        0XB9, 0X27, 0X9B, 0X62, 0XB3, 0X67, 0XAB, 0X1F, 0XD6, 0XAD, 0XB2, 0XCD,
        0X91, 0XD4, 0XEA, 0X94, 0XAD, 0X2C, 0X35, 0X90, 0X51, 0X1B, 0X61, 0XBB,
        0X2C, 0X3A, 0X42, 0XB7, 0X22, 0X6F, 0XF9, 0X96, 0X7C, 0X64, 0X57, 0XBD,
        0XB6, 0XFA, 0X1F, 0X57, 0XE4, 0XEB, 0X1D, 0X14, 0X7B, 0XB2, 0X8A, 0XAC,
        0XFC, 0XDB, 0XB0, 0X21, 0X5E, 0X8,  0XFA, 0X54, 0X77, 0X74, 0X99, 0XA2,
        0X61, 0XD2, 0XF5, 0X3E, 0X50, 0X8C, 0X80, 0XCA, 0X90, 0X8,  0X47, 0X64,
        0XA2, 0XDF, 0XC7, 0XEF, 0X8F, 0XF7, 0XFC, 0XE1, 0XCB, 0XE7, 0XAF, 0XDF,
        0X1E, 0X9F, 0X9E, 0XD8, 0XA1, 0X22, 0X13, 0XA9, 0X9E, 0X39, 0XBB, 0X3D,
        0X22, 0X56, 0XE9, 0X53, 0X55, 0X49, 0XA3, 0X91, 0X41, 0X7F, 0X48, 0XE6,
        0X24, 0X8E, 0X86, 0X9A, 0X81, 0XC4, 0X12, 0XDD, 0X4F, 0XC6, 0XED, 0X59,
        0X2F, 0XB4, 0X6D, 0XAF, 0XDC, 0X9F, 0X85, 0X69, 0X2A, 0X3E, 0XCE, 0X9A,
        0X27, 0XD8, 0X68, 0XFB, 0XCC, 0X7F, 0XEB, 0X74, 0X6,  0X36, 0X85, 0XB4,
        0X95, 0X26, 0X2B, 0XE2, 0X4A, 0X7,  0X92, 0XC9, 0X85, 0X31, 0X71, 0X6,
        0X63, 0X12, 0XC6, 0X6C, 0XEB, 0X14, 0X32, 0X8D, 0XA1, 0XB2, 0X19, 0XD8,
        0X7,  0X52, 0X51, 0X5B, 0XF6, 0X32, 0X91, 0X78, 0X2F, 0X30, 0X35, 0X7E,
        0X6B, 0X8,  0XC4, 0X6E, 0XAF, 0X90, 0X33, 0X9,  0X85, 0XD5, 0X8F, 0XFA,
        0X2F, 0XB5, 0X63, 0X3E, 0XA2, 0X27, 0XDC, 0X1D, 0XE7, 0X1,  0X7D, 0XF1,
        0XCA, 0XC1, 0X44, 0X31, 0X15, 0XFB, 0X16, 0X2F, 0X74, 0XEC, 0XAB, 0X45,
        0XFD, 0X98, 0XD6, 0XEC, 0X0,  0X60, 0X32, 0X6A, 0X71, 0X5,  0XF6, 0X94,
        0X84, 0X12, 0X49, 0XC,  0X84, 0X6A, 0XE1, 0XA5, 0XB3, 0X27, 0XB8, 0XA8,
        0X7B, 0XEF, 0X42, 0X6A, 0X98, 0X7F, 0XEE, 0X4A, 0X44, 0X77, 0XA5, 0XEE,
        0X0,  0XB6, 0X1D, 0X59, 0XA,  0X22, 0XC1, 0XDE, 0XA1, 0X11, 0XE4, 0X70,
        0XA3, 0X8F, 0XB7, 0XE3, 0X49, 0X51, 0X94, 0X41, 0XF,  0X82, 0X20, 0XB5,
        0XD4, 0X28, 0X9B, 0XA8, 0X7D, 0X70, 0X3F, 0X61, 0X17, 0XC8, 0XBD, 0X37,
        0XC8, 0X85, 0XEE, 0XA1, 0X88, 0XB,  0X18, 0X65, 0X48, 0XCC, 0X1,  0XB6,
        0XB1, 0X73, 0X4A, 0X3E, 0X6E, 0XD7, 0XEB, 0XE,  0X9E, 0XE7, 0X63, 0X8B,
        0X3B, 0XB1, 0XFE, 0X24, 0X42, 0XE,  0XEE, 0XE1, 0XC,  0XAB, 0X8B, 0X53,
        0X17, 0XFE, 0XFC, 0X36, 0X86, 0XA7, 0XD2, 0X4E, 0XD,  0X2E, 0X4F, 0XA6,
        0X59, 0X82, 0XF6, 0X1F, 0X14, 0X72, 0X12, 0X74,
    };
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp, sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp, sizeof(tmp));
#endif
    out.write((const char *)dtmp.data(), dtmp.size());
  }
  out.close();
  return WRITER_STATUS_OK;
}
writer_status_t
writer_file_3(const fs::path &dir, const env_t &env,
              const std::set<fs::path> &exclude = {} /*not used*/) {
  static constexpr const char *name = "README.md";
  fs::path file = dir / name;
  if (fs::exists(file)) {
    std::cerr << "File " << file
              << " already there. It will not be overwritten by rule "
                 "writer_file_3.\n";
    return WRITER_STATUS_SKIP;
  }
  std::ofstream out(file, std::ios::binary);
  out.close();
  return WRITER_STATUS_OK;
}
writer_status_t
writer_file_4(const fs::path &dir, const env_t &env,
              const std::set<fs::path> &exclude = {} /*not used*/) {
  static constexpr const char *name = "pugixml.wrap";
  fs::path file = dir / name;
  if (fs::exists(file)) {
    std::cerr << "File " << file
              << " already there. It will not be overwritten by rule "
                 "writer_file_4.\n";
    return WRITER_STATUS_SKIP;
  }
  std::ofstream out(file, std::ios::binary);
  {
    constexpr const uint8_t tmp[] = {
        0X42, 0X2,  0X0,  0X0,  0X78, 0X9C, 0X75, 0X52, 0X59, 0X6E, 0X83, 0X30,
        0X10, 0XFD, 0XE7, 0X14, 0XBD, 0X40, 0XF0, 0X82, 0X97, 0X50, 0XA9, 0X27,
        0X89, 0XAA, 0X68, 0X6C, 0XF,  0X60, 0XD5, 0X60, 0X64, 0X96, 0XA4, 0X39,
        0X7D, 0X21, 0X21, 0X5D, 0XA2, 0XF4, 0XCF, 0XD2, 0XBC, 0X5D, 0X3E, 0X9C,
        0X12, 0XF4, 0XBB, 0XCA, 0X7,  0X7C, 0XCF, 0X9C, 0X4F, 0X68, 0XC7, 0X98,
        0X3E, 0X5F, 0XDE, 0X5E, 0XFA, 0XA9, 0XF6, 0XE7, 0X36, 0XEC, 0X58, 0XCE,
        0X44, 0X36, 0XC4, 0X29, 0X59, 0X3C, 0X4E, 0X29, 0X2C, 0X97, 0X66, 0X1C,
        0XFB, 0XE1, 0X95, 0X90, 0XDA, 0X8F, 0XCD, 0X64, 0X72, 0X1B, 0X5B, 0X72,
        0XC1, 0XE9, 0X4C, 0X36, 0X6,  0X81, 0X64, 0X1B, 0X3F, 0X23, 0X99, 0X57,
        0X6A, 0X3E, 0X42, 0XCA, 0XEB, 0XCB, 0X5D, 0X61, 0XF5, 0XE9, 0XA0, 0XC5,
        0X7,  0X83, 0X7,  0X54, 0X3,  0X43, 0XB3, 0X20, 0X14, 0XA3, 0X55, 0XB9,
        0X2F, 0XB4, 0X14, 0X5C, 0X18, 0XA9, 0X98, 0X58, 0X5E, 0XA0, 0XAA, 0X42,
        0X80, 0X28, 0X19, 0X38, 0XE3, 0X1C, 0X0,  0X5A, 0XAA, 0X5,  0X96, 0X54,
        0X8,  0XA9, 0XB5, 0X2B, 0X95, 0X64, 0X8A, 0XA2, 0X65, 0XB4, 0X70, 0X1C,
        0XB3, 0X1E, 0X46, 0XDB, 0X3C, 0XF1, 0X3C, 0XAE, 0X9E, 0X3B, 0X76, 0XBC,
        0XDE, 0XF3, 0X8B, 0XEF, 0X37, 0XE4, 0XDF, 0X7E, 0XEB, 0X30, 0XCE, 0XE4,
        0X2D, 0XE,  0XB1, 0X33, 0X93, 0XF,  0XEE, 0X5A, 0X75, 0XE6, 0XF7, 0XA2,
        0X9B, 0XA,  0XA9, 0X71, 0XBC, 0X29, 0X6D, 0X2A, 0X5B, 0X7A, 0X5E, 0X58,
        0X4,  0X63, 0X9C, 0X36, 0X56, 0X2F, 0XB1, 0XAD, 0X55, 0X96, 0XA3, 0XAA,
        0X14, 0X97, 0X50, 0X1A, 0XBA, 0X17, 0X14, 0XA8, 0XA6, 0XC8, 0X38, 0X97,
        0X6E, 0X2F, 0X5D, 0X61, 0X2D, 0X2F, 0X2D, 0XD7, 0X66, 0X1,  0X52, 0X59,
        0XB2, 0X42, 0X7E, 0X4F, 0X6,  0X21, 0X18, 0XB0, 0X1F, 0XFF, 0XAF, 0XFF,
        0X93, 0X70, 0XCB, 0X4C, 0X12, 0X6,  0X84, 0X1,  0X7,  0XE2, 0XE2, 0XA9,
        0XB,  0X11, 0XDC, 0X63, 0XE6, 0X67, 0XE3, 0XDF, 0XB8, 0XC7, 0X19, 0XD3,
        0XE0, 0X63, 0XB7, 0X58, 0XDD, 0XB0, 0X59, 0X76, 0XE8, 0X53, 0X9C, 0XBD,
        0X5B, 0X3E, 0XC8, 0X46, 0XFB, 0XB5, 0XA4, 0XC3, 0XFE, 0XB,  0X31, 0XC0,
        0XC1, 0X3F,
    };
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp, sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp, sizeof(tmp));
#endif
    out.write((const char *)dtmp.data(), dtmp.size());
  }
  out.close();
  return WRITER_STATUS_OK;
}
writer_status_t
writer_file_5(const fs::path &dir, const env_t &env,
              const std::set<fs::path> &exclude = {} /*not used*/) {
  static constexpr const char *name = "miniz.wrap";
  fs::path file = dir / name;
  if (fs::exists(file)) {
    std::cerr << "File " << file
              << " already there. It will not be overwritten by rule "
                 "writer_file_5.\n";
    return WRITER_STATUS_SKIP;
  }
  std::ofstream out(file, std::ios::binary);
  {
    constexpr const uint8_t tmp[] = {
        0X79, 0X0,  0X0,  0X0,  0X78, 0X9C, 0X1D, 0X8B, 0X41, 0XA,  0XC2, 0X40,
        0XC,  0X45, 0XF7, 0X39, 0X45, 0X2F, 0XA0, 0X83, 0XCB, 0X11, 0X5C, 0X8,
        0XA,  0XDE, 0XA1, 0X14, 0XD1, 0X4E, 0X68, 0X2,  0X9D, 0XC9, 0X90, 0X49,
        0X2B, 0X78, 0XFA, 0XA6, 0X5D, 0XFD, 0XFF, 0X1F, 0XFF, 0XF5, 0X3F, 0XFD,
        0XD4, 0XD3, 0XC4, 0X36, 0XC0, 0XA2, 0X73, 0X77, 0XEB, 0XC8, 0XAC, 0XB6,
        0X6B, 0X8,  0X8E, 0X68, 0XF9, 0X9E, 0X47, 0XC9, 0X41, 0X79, 0XA4, 0X9,
        0XE7, 0X18, 0X63, 0XC8, 0X5C, 0XF8, 0XF,  0X8A, 0X2B, 0X37, 0X96, 0XE2,
        0XF7, 0XD7, 0XF3, 0XFE, 0X80, 0X84, 0XD5, 0XC8, 0XC7, 0X5,  0X32, 0X1A,
        0X49, 0XF2, 0X9A, 0XB1, 0X49, 0X81, 0XBE, 0XAA, 0XAC, 0X9C, 0X70, 0X80,
        0X43, 0XDC, 0XF9, 0X9E, 0X6F, 0X17, 0X36, 0XF1, 0X25, 0X29, 0XAE,
    };
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp, sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp, sizeof(tmp));
#endif
    out.write((const char *)dtmp.data(), dtmp.size());
  }
  out.close();
  return WRITER_STATUS_OK;
}
writer_status_t writer_dir_1(const fs::path &dir, const env_t &env,
                             const std::set<fs::path> &exclude = {},
                             const std::string &_name = {}) {
  static constexpr const char *name = "subprojects";
  fs::path file = dir / (_name.length() != 0 ? _name.c_str() : name);
  fs::create_directories(file);
  if (exclude.find(file) == exclude.end())
    writer_file_4(file, env, exclude);
  if (exclude.find(file) == exclude.end())
    writer_file_5(file, env, exclude);
  return WRITER_STATUS_OK;
}
writer_status_t writer_dir_0(const fs::path &dir, const env_t &env,
                             const std::set<fs::path> &exclude = {},
                             const std::string &_name = {}) {
  static constexpr const char *name = "";
  fs::path file = dir / (_name.length() != 0 ? _name.c_str() : name);
  fs::create_directories(file);
  if (exclude.find(file) == exclude.end())
    writer_file_0(file, env, exclude);
  if (exclude.find(file) == exclude.end())
    writer_file_1(file, env, exclude);
  if (exclude.find(file) == exclude.end())
    writer_file_2(file, env, exclude);
  if (exclude.find(file) == exclude.end())
    writer_file_3(file, env, exclude);
  if (exclude.find(file) == exclude.end())
    writer_dir_1(file, env, exclude);
  return WRITER_STATUS_OK;
}

#ifndef TE4_INTERNAL
int main(int argc, const char *argv[]) {
  if (argc < 3)
    exit(1);
  pugi::xml_document doc;
  doc.load_file((fs::path(getenv("PWD")) / argv[2]).c_str());

  // Default entry point. Change by adding exclusions and further calls if you
  // need to change its behaviour
  writer_dir_0(argv[1], doc.root());
  return 0;
}
#endif
