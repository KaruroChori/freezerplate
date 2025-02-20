/**
 * @file main.cpp
 * @author karurochari
 * @brief Main unit for the freezerplate generator
 * @date 2025-02-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <string_view>
#include <vector>

#include <pugixml.hpp>

#ifdef TE4_COMPRESS
#include "miniz.h"

// Compresses the input data
static std::vector<uint8_t> deflate(std::basic_string_view<uint8_t> data) {
  mz_ulong compressedSize = mz_compressBound(data.size());
  std::vector<uint8_t> compressedData(compressedSize + 4);

  if (mz_compress(compressedData.data() + 4, &compressedSize, data.data(),
                  data.size()) != MZ_OK) {
    throw std::runtime_error("Compression failed");
  }

  ((uint32_t *)compressedData.data())[0] = data.size();
  compressedData.resize(compressedSize + 4);
  return compressedData;
}
#endif

namespace fs = std::filesystem;

struct ctx_t {
  std::ofstream out;
  std::ostringstream fn_table;
  std::string LBRACE = "<?";
  std::string RBRACE = "?>";
  fs::path BASE;

  ctx_t(fs::path name) : out(name, std::ios::binary) {}
};

extern std::string escape_c_str(const std::string &input);
extern std::string escape_meson_str(const std::string &input);
extern std::string escape_xml_str(const std::string &input);
extern bool no_override;

// Define the writer status enumeration
typedef enum {
  WRITER_STATUS_ERROR,
  WRITER_STATUS_OK,
  WRITER_STATUS_SKIP,
} writer_status_t;

// Define the writer function type
typedef writer_status_t (*writer_t)(const char *, size_t);

// Define the environment structure
typedef pugi::xml_node env_t;

// File templates
std::pair<writer_status_t, std::string>
generate_file_function(ctx_t &ctx, const fs::path &source) {
  static int counter = 0;

  // Read file contents
  std::ifstream file(source, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + source.string());
  }

  // Tecchnically not thread safe. But it will be fine
  size_t file_size = file.tellg();
  std::vector<unsigned char> buffer(file_size);
  file.seekg(0);
  file.read((char *)buffer.data(), file_size);
  file.close();

  std::string name = std::format("writer_file_{}", counter++);
  ctx.out << std::format(
      "writer_status_t {}(const fs::path& dir, const env_t& env, const "
      "std::set<fs::path>& exclude={{}} /*not used*/, const std::string& _name "
      "= {{}} /*not used*/){{\n",
      name);
  ctx.fn_table << std::format(R"(fs_tree.emplace("{}",{});)",
                              escape_c_str(fs::relative(source, ctx.BASE)),
                              name);
  ctx.out << std::format("static constexpr int perms = {};",
                         (int)fs::status(source).permissions());
  ctx.out << std::format("static constexpr const char* name  = \"{}\";\n",
                         escape_c_str(source.filename()));
  ctx.out << "fs::path file = dir / (_name.length()!=0?_name.c_str():name);\n";
  ctx.out << "if(fs::exists(file) && no_override){std::cerr<<\"File "
             "\"<<file<<\" already there. It will not be overwritten by rule "
          << name << ".\\n\";return WRITER_STATUS_SKIP;}\n";
  ctx.out << "std::ofstream out(file, std::ios::binary);\n";
  bool mode = 0;
  uint base = 0;
  uint offset = 0;
  for (; offset < buffer.size();) {
    if (mode == 0 && ((buffer.size() - offset >= ctx.LBRACE.length() &&
                       memcmp(buffer.data() + offset, ctx.LBRACE.c_str(),
                              ctx.LBRACE.length()) == 0) ||
                      offset == buffer.size() - 1)) {
      ctx.out << "{";
      ctx.out << "constexpr const uint8_t tmp[] = {";
      // To get the last one
      if (offset == buffer.size() - 1)
        offset++;
#ifdef TE4_COMPRESS
      auto data = deflate(std::basic_string_view<uint8_t>(
          buffer.data() + base, buffer.data() + offset));
#else
      auto data = std::basic_string_view<uint8_t>(buffer.data() + base,
                                                  buffer.data() + offset);
#endif
      for (auto c : data) {
        auto tmp = std::format("{:#X},", c);
        ctx.out.write(tmp.data(), tmp.size());
      }
      ctx.out <<
          R"(};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
})";
      mode = 1;
      if (offset < buffer.size())
        offset += ctx.LBRACE.length();
      else
        break;
      base = offset;
      continue;
    } else if (mode == 1 && ((buffer.size() - offset >= ctx.RBRACE.length() &&
                              memcmp(buffer.data() + offset, ctx.RBRACE.c_str(),
                                     ctx.RBRACE.length()) == 0) ||
                             offset == buffer.size())) {
      // To get the last one
      if (offset == buffer.size() - 1)
        offset++;
      ctx.out.write((const char *)buffer.data() + base, offset - base);

      mode = 0;
      if (offset < buffer.size()) {
        offset += ctx.RBRACE.length();
        // Skip empty lines generated by directives
        if (((base > ctx.LBRACE.length() &&
              buffer[base - ctx.LBRACE.length() - 1] == '\n') ||
             base == ctx.LBRACE.length()) &&
            ((offset < buffer.size() && buffer[offset] == '\n') ||
             offset == buffer.size())) {
          offset++;
        }
      } else
        break;

      base = offset;
      continue;
    } else {
      offset++;
    }
  }
  ctx.out << "out.close();\nfs::permissions(file,fs::perms(perms), "
             "fs::perm_options::replace);\nreturn WRITER_STATUS_OK;\n}\n";
  return {WRITER_STATUS_OK, name};
}

// Mostly binary files which should not be edited but still copied.
std::pair<writer_status_t, std::string>
generate_raw_function(ctx_t &ctx, const fs::path &source) {
  static int counter = 0;

  // Read file contents
  std::ifstream file(source, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + source.string());
  }

  // Tecchnically not thread safe. But it will be fine
  size_t file_size = file.tellg();
  std::vector<unsigned char> buffer(file_size);
  file.seekg(0);
  file.read((char *)buffer.data(), file_size);
  file.close();

  std::string name = std::format("writer_raw_{}", counter++);
  ctx.out << std::format(
      "writer_status_t {}(const fs::path& dir, const env_t& env, const "
      "std::set<fs::path>& exclude={{}} /*not used*/, const std::string& _name "
      "= {{}} /*not used*/){{\n",
      name);
  ctx.fn_table << std::format(R"(fs_tree.emplace("{}",{});)",
                              escape_c_str(fs::relative(source, ctx.BASE)),
                              name);
  ctx.out << std::format("static constexpr int perms = {};",
                         (int)fs::status(source).permissions());
  ctx.out << std::format("static constexpr const char* name  = \"{}\";\n",
                         escape_c_str(source.filename()));
  ctx.out << "fs::path file = dir / (_name.length()!=0?_name.c_str():name);\n";
  ctx.out
      << "if(fs::exists(file) && no_override){std::cerr<<\"File \"<<file<<\" "
         "already there. It will not be overwritten by rule "
      << name << ".\\n\";return WRITER_STATUS_SKIP;}\n";
  ctx.out << "std::ofstream out(file, std::ios::binary);\n";

  ctx.out << "{";
  ctx.out << "constexpr const uint8_t tmp[] = {";
#ifdef TE4_COMPRESS
  auto data =
      deflate(std::basic_string_view<uint8_t>(buffer.begin(), buffer.end()));
#else
  auto data = std::basic_string_view<uint8_t>(buffer.begin(), buffer.end());
#endif
  for (auto c : data) {
    auto tmp = std::format("{:#X},", c);
    ctx.out.write(tmp.data(), tmp.size());
  }
  ctx.out <<
      R"(};
#ifdef TE4_COMPRESS
    auto dtmp = inflate({tmp,sizeof(tmp)});
#else
    auto dtmp = std::basic_string_view<uint8_t>(tmp,sizeof(tmp));
#endif
    out.write((const char*)dtmp.data(),dtmp.size());
})";

  ctx.out
      << "out.close();\n"
         "fs::permissions(file,fs::perms(perms), fs::perm_options::replace);\n"
         "return WRITER_STATUS_OK;\n}\n";
  return {WRITER_STATUS_OK, name};
}

std::pair<writer_status_t, std::string>
generate_link_function(ctx_t &ctx, const fs::path &source) {
  static int counter = 0;
  std::string name = std::format("writer_link_{}", counter++);
  ctx.out << std::format(
      "writer_status_t {}(const fs::path& dir, const env_t& env, const "
      "std::set<fs::path>& exclude={{}} /*not used*/,  const std::string& "
      "_name = {{}} /*not used*/){{\n",
      name);
  ctx.out << std::format("static constexpr int perms = {};",
                         (int)fs::status(source).permissions());
  ctx.fn_table << std::format(R"(fs_tree.emplace("{}",{});)",
                              escape_c_str(fs::relative(source, ctx.BASE)),
                              name);
  ctx.out << std::format("static constexpr const char* name  = \"{}\";\n",
                         escape_c_str(source.filename()));
  ctx.out << "fs::path file = dir / (_name.length()!=0?_name.c_str():name);\n";
  ctx.out
      << "if(fs::exists(file) && no_override){std::cerr<<\"File \"<<file<<\" "
         "already there. "
         "It will not be overwritten by rule "
      << name << ".\\n\";return WRITER_STATUS_SKIP;}\n";
  ctx.out << "auto t = \""
          << escape_c_str(fs::read_symlink(source).lexically_normal().string())
          << "\";\n"; // TODO: escape
  ctx.out
      << "fs::create_symlink(t,file);\n"
         "fs::permissions(file,fs::perms(perms), fs::perm_options::replace);\n"
         "return WRITER_STATUS_OK;\n}\n";
  return {WRITER_STATUS_OK, name};
}

// Function to generate folder functions
std::pair<writer_status_t, std::string>
generate_folder_function(ctx_t &ctx, const fs::path &source,
                         const std::set<fs::path> &exclude = {},
                         const std::set<fs::path> &exclude_ext = {}) {
  static int counter = 0;
  std::string name = std::format("writer_dir_{}", counter++);
  std::vector<std::string> callthese;
  for (const auto &entry : fs::directory_iterator(source)) {
    if (!entry.is_regular_file() && !entry.is_symlink() &&
        !entry.is_directory()) {
      throw std::runtime_error("Unallowed file type in tempalte dir");
    }

    if (exclude.find(entry) != exclude.end()) {
      continue;
    } // Skip unwanted cases of files not to be tracked.

    // Special case to resolve symbolic links.
    if (entry.is_symlink()) {
      auto t = generate_link_function(ctx, entry);
      if (t.first == WRITER_STATUS_ERROR)
        return {WRITER_STATUS_ERROR, ""};
      else
        callthese.push_back(t.second);
      continue;
    }

    // Folders are ignored in this context.
    if (entry.is_directory()) {
      auto t = generate_folder_function(ctx, entry, exclude);
      if (t.first == WRITER_STATUS_ERROR)
        return {WRITER_STATUS_ERROR, ""};
      else
        callthese.push_back(t.second);
      continue;
    }

    // Skip templating for selected extensions
    if (exclude_ext.find(entry.path().extension()) != exclude_ext.end()) {
      auto t = generate_raw_function(ctx, entry);
      if (t.first == WRITER_STATUS_ERROR)
        return {WRITER_STATUS_ERROR, ""};
      else
        callthese.push_back(t.second);
      continue;
    }

    auto t = generate_file_function(ctx, entry);
    if (t.first == WRITER_STATUS_ERROR)
      return {WRITER_STATUS_ERROR, ""};
    else
      callthese.push_back(t.second);
  }

  ctx.out << std::format(
      "writer_status_t {}(const fs::path& dir, const env_t& env, const "
      "std::set<fs::path>& exclude={{}}, const std::string& _name = {{}}){{\n",
      name);
  ctx.out << std::format("static constexpr int perms = {};",
                         (int)fs::status(source).permissions());
  ctx.fn_table << std::format(R"(fs_tree.emplace("{}",{});)",
                              escape_c_str(fs::relative(source, ctx.BASE)),
                              name);
  ctx.out << std::format("static constexpr const char* name  = \"{}\";\n",
                         escape_c_str(source.filename()));
  ctx.out << "fs::path file = dir / (_name.length()!=0?_name.c_str():name);\n";
  ctx.out << "fs::create_directories(file);\n";
  for (auto fn : callthese) {
    ctx.out << std::format(
        "if(exclude.find(file)==exclude.end()){}(file, env, exclude);\n", fn);
  }

  ctx.out
      << "fs::permissions(file,fs::perms(perms), fs::perm_options::replace);\n"
      << "return WRITER_STATUS_OK;\n}\n";

  return {WRITER_STATUS_OK, name};
}

extern writer_status_t writer_dir_0(const fs::path &dir, const env_t &env,
                                    const std::set<fs::path> &exclude = {},
                                    const std::string &_name = {});

extern std::map<fs::path,
                writer_status_t (*)(const fs::path &dir, const env_t &env,
                                    const std::set<fs::path> &exclude,
                                    const std::string &_name)>
    fs_tree;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: freezerplate TEMPLATE_DIR OUT_DIR [CONFIG_FILE]\n";
    exit(1);
  }

  if (argc >= 5 && strcmp(argv[4], "override") == 0) {
    no_override = false;
  }

  fs::create_directories(fs::path(argv[2]));
  auto file = fs::path(argv[2]) / "main.cpp";
  if (fs::exists(file) && no_override) {
    std::cerr << "File " << file
              << " already there. It will not be overwritten\n";
  } else {
    ctx_t ctx(file);
#ifdef TE4_COMPRESS
    ctx.out << "#ifndef TE4_INTERNAL\n#define TE4_COMPRESS\n#endif";
#endif
    ctx.out <<
        R"(
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

)";

    pugi::xml_document doc = {};
    if (argc >= 4 && fs::exists(argv[3]))
      doc.load_file(argv[3]);
    else
      doc.load_string("<project></project>");

    ctx.BASE = argv[1];
    ctx.LBRACE =
        doc.root().child("project").attribute("lbrace").as_string("<?");
    ctx.RBRACE =
        doc.root().child("project").attribute("rbrace").as_string("?>");

    generate_folder_function(ctx, ctx.BASE, {});
    ctx.out <<
        R"(
#ifndef TE4_INTERNAL
int main(int argc, const char* argv[]){
    if(argc<3)exit(1);
    pugi::xml_document doc;
    doc.load_file(/*(fs::path(getenv("PWD"))/argv[2]).c_str()*/argv[2]);
)";
    ctx.out << ctx.fn_table.str();
    ctx.out << R"(
    //Default entry point. Change by adding exclusions and further calls if you need to change its behaviour
    #if !__has_include("impl.frag.cpp")
)";

    for (auto &entry : doc.child("project").child("steps").children()) {
      auto src = entry.attribute("source").as_string(nullptr);
      if (src != nullptr) {
        ctx.out << std::format(
            "fs_tree[\"{}\"](argv[1], doc.root(), {{}}, {{}});\n",
            escape_c_str(src));
      }
    }
    if (!(doc.child("project").child("steps"))) {
      ctx.out << R"(        writer_dir_0(argv[1], doc.root());)" << "\n";
    }
    ctx.out << R"(
    #else
        #include "impl.frag.cpp"
    #endif
)";

    writer_dir_0(argv[2], doc.root());

    ctx.out << R"(
    return 0;
}
#endif
)";
    ctx.out.close();
  }

  return 0;
}
