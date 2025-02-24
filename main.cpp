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
#include <unistd.h>
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
  std::vector<std::pair<fs::path, std::string>> callthese;
  for (const auto &entry : fs::directory_iterator(source)) {
    if (!entry.is_regular_file() && !entry.is_symlink() &&
        !entry.is_directory()) {
      throw std::runtime_error("Unallowed file type in template dir");
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
        callthese.push_back({fs::relative(entry, ctx.BASE), t.second});
      continue;
    }

    // Folders are ignored in this context.
    if (entry.is_directory()) {
      auto t = generate_folder_function(ctx, entry, exclude);
      if (t.first == WRITER_STATUS_ERROR)
        return {WRITER_STATUS_ERROR, ""};
      else
        callthese.push_back({fs::relative(entry, ctx.BASE), t.second});
      continue;
    }

    // Skip templating for selected extensions
    if (exclude_ext.find(entry.path().extension()) != exclude_ext.end()) {
      auto t = generate_raw_function(ctx, entry);
      if (t.first == WRITER_STATUS_ERROR)
        return {WRITER_STATUS_ERROR, ""};
      else
        callthese.push_back({fs::relative(entry, ctx.BASE), t.second});
      continue;
    }

    auto t = generate_file_function(ctx, entry);
    if (t.first == WRITER_STATUS_ERROR)
      return {WRITER_STATUS_ERROR, ""};
    else
      callthese.push_back({fs::relative(entry, ctx.BASE), t.second});
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
        "if(exclude.find(\"{}\")==exclude.end()){}(file, env, exclude);\n",
        escape_c_str(fn.first), fn.second);
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

extern void g_inits();

void printHelp(const char *arg0) {
  std::cout << "Usage: " << arg0
            << " [options] source "
               "dest [env-file]\n"
            << "Options:\n"
            << "  -f, --force        Enable the override flag. Existing files "
               "will be overwritten.\n"
            << "  -h, --help         Display this help message and exit.\n"
            << "\n"
            << "Arguments:\n"
            << "  source       Path to the directory containing the "
               "template files.\n"
            << "  destination  Path to the directory where the "
               "generated code will be saved.\n"
            << "  env-file     Name of the environment file in XML "
               "format. If omitted, content will be read from stdin.\n"
            << "More info using man pages\n";
}

int main(int argc, char *argv[]) {
  g_inits();

  std::string source_folder;
  std::string dest_folder;
  std::string src_file;
  bool no_bundle = false;

  pugi::xml_document doc = {};

  int opt;
  while ((opt = getopt(argc, argv, "fhs")) != -1) {
    switch (opt) {
    case 'f':
      no_override = false;
      break;
    case 's':
      no_bundle = true;
      break;
    case 'h':
    case '?':
      printHelp(argv[0]);
      return 1;
    }
  }

  // Check positional arguments
  if (optind < argc) {
    source_folder = argv[optind++];
  } else {
    std::cerr << "Error: Source directory is required." << std::endl;
    return 1;
  }

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

  fs::create_directories(fs::path(dest_folder));
  auto file = fs::path(dest_folder) / "main.cpp";
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
    inline static std::string STAMP() { return timestamp; }

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
)";

    ctx.BASE = source_folder;
    ctx.LBRACE =
        doc.root().child("project").attribute("lbrace").as_string("<?");
    ctx.RBRACE =
        doc.root().child("project").attribute("rbrace").as_string("?>");

    generate_folder_function(ctx, ctx.BASE, {});
    ctx.out <<
        R"(
void g_inits(){
  TIME::initialize();
)";
    ctx.out << ctx.fn_table.str();
    ctx.out << R"(
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

)";
    ctx.out << ctx.fn_table.str();
    ctx.out << R"(
    //Default entry point. Change by adding exclusions and further calls if you need to change its behaviour
    #if !__has_include("body.frag.cpp")
)";

    for (auto &entry : doc.child("project").child("steps").children()) {
      auto src = entry.attribute("source").as_string(nullptr);
      if (src != nullptr) {
        ctx.out << std::format(
            "fs_tree[\"{}\"](dest_folder, doc.root(), {{}}, {{}});\n",
            escape_c_str(src));
      }
    }
    if (!(doc.child("project").child("steps"))) {
      ctx.out << R"(        writer_dir_0(dest_folder, doc.root());)" << "\n";
    }
    ctx.out << R"(
    #else
        #include "body.frag.cpp"
    #endif
)";

    // Check to only generate the main.cpp, used only internally when running
    // freezerplate on itself.
    if (!no_bundle) {
      writer_dir_0(dest_folder, doc.root(), {"man/page.1"});
      fs_tree.at("man/page.1")(
          fs::path(dest_folder) / "man", doc.root(), {},
          std::string(
              doc.child("project").attribute("name").as_string("undefined")) +
              ".1");
    }
    ctx.out << R"(
    return 0;
}
#endif
)";
    ctx.out.close();
  }

  return 0;
}
