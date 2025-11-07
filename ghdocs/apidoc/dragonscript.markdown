---
layout: page
title: DragonScript
permalink: /apidoc/dragonscript/
nav_order: 4
parent: API Documentation
---

# DragonScript Script Module API Documentation

{% assign urlapidoc = site.url | append: site.baseurl | append: "/artifacts/apidoc/dragonscript/" %}
{% assign versions = site.data.apidoc.dragonscript %}

This documentation is relevant for game developers creating a game using the DragonScript Scripting module.
{: .important }

## Latest
{% assign version = versions.first.version %}
[DragonScript Script Module: {{ version }}]({{ urlapidoc }}{{ version }}/index.html){:target="_blank"}

## Previous Versions
{% assign count = versions.size | minus: 1 %}
{% assign versions2 = versions | slice: 1, count %}
{% for each in versions2 %}
  {% assign version = each.version %}
  [DragonScript Script Module: {{ version }}]({{ urlapidoc }}{{ version }}/index.html){:target="_blank"}
{% endfor %}

## Code IDE Scripts

{% assign urlidescripts = site.url | append: site.baseurl | append: "/artifacts/idescripts/dragonscript" %}

These files contain the DragonScript Scripting module scripts alone for use by code IDEs to
provide DragonScript code completion features. The file below contains the list of archive
files an IDE can download together with checksum information to quickly identify changes.
The lines have this format:

```
{version} {file size in bytes} {SHA-256 hash} {file title}
```

To download the respective file append the file title to _{{ urlidescripts }}_.

[DragonScript IDE Scripts List]({{ urlidescripts }}/list.html){:target="_blank"}
