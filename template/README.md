# <?WRITE_XML("/project~name"_attr(env).value_or("undefined"))?>

Custom template builder.

## Description

<?
auto proj_description = env.child("project").child("description");
for (pugi::xml_node child: proj_description.children()){
    if (child.type() == pugi::node_pcdata)
        WRITE(child.value());
}?>

## How to compile

## How to use

## Licence
