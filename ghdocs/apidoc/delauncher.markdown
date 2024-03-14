---
layout: page
title: DELauncher
permalink: /apidoc/delauncher/
nav_order: 2
parent: API Documentation
---

# Drag[en]gine Shared Launcher Library API Documentation

{% assign urlapidoc = site.url | append: site.baseurl | append: "/artifacts/apidoc/delauncher/" %}
{% assign versions = site.data.apidoc.delauncher %}

## Latest
{% assign version = versions.first.version %}
[Drag[en]gine Shared Launcher Library: {{ version }}]({{ urlapidoc }}{{ version }}/index.html){:target="_blank"}

## Previous Versions
{% assign count = versions.size | minus: 1 %}
{% assign versions2 = versions | slice: 1, count %}
{% for each in versions2 %}
  {% assign version = each.version %}
  [Drag[en]gine Shared Launcher Library: {{ version }}]({{ urlapidoc }}{{ version }}/index.html){:target="_blank"}
{% endfor %}
