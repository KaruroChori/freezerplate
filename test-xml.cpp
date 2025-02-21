
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

struct xml_path_n {
  const char *path;
  size_t str_len;
  pugi::xml_node operator()(pugi::xml_node ref, pugi::xml_node def = {}) {
    int str_len = strlen(path);
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

struct xml_path_s {
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

xml_path_n operator""_node(const char *str, size_t l) { return {str, l}; }
xml_path_s operator""_attr(const char *str, size_t l) { return {str, l}; }

int main() {
  pugi::xml_document doc = {};
  doc.load_string(
      R"(<project hello="world" wo="errrd"><child>Hello</child></project>)");
  std::cout << "/project~hellow"_attr(doc).value_or("NOOO") << "\n";
  std::cout << "/project~wo"_attr.as<int>(doc, 12) << "\n";

  std::cout << "/project/child~!txt"_attr(doc).value_or("NOOO") << "\n";
}