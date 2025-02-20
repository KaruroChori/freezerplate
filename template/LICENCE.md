<?
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
if(!proj_licence){?>

All rights reserved by <?WRITE(proj_author)?>

<?
}
else for (pugi::xml_node child: proj_licence.children())
    if (child.type() == pugi::node_pcdata)
        WRITE(child.value());?>
