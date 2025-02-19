<?
auto proj_licence = env.child("project").child("licences");
auto proj_author = env.child("project").attribute("author").as_string("Unknown Author");
if(!proj_licence){?>

All rights reserved by <?WRITE(proj_author)?>

<?
}
else for (pugi::xml_node child: proj_licence.children())
    if (child.type() == pugi::node_pcdata)
        WRITE(child.value());?>