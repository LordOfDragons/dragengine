---
layout: page
title: DEIGDE
permalink: /apidoc/deigde/
nav_order: 3
parent: API Documentation
---

# Drag[en]gine IGDE Shared Library API Documentation

{% assign urlapidoc = site.url | append: site.baseurl | append: "/artifacts/apidoc/deigde/" %}
{% assign versions = site.data.apidoc.deigde %}

## Latest
{% assign version = versions.first.version %}
[Drag[en]gine IGDE Shared Library: {{ version }}]({{ urlapidoc }}{{ version }}/index.html){:target="_blank"}

## Previous Versions
{% assign count = versions.size | minus: 1 %}
{% assign versions2 = versions | slice: 1, count %}
{% for each in versions2 %}
  {% assign version = each.version %}
  [Drag[en]gine IGDE Shared Library: {{ version }}]({{ urlapidoc }}{{ version }}/index.html){:target="_blank"}
{% endfor %}
