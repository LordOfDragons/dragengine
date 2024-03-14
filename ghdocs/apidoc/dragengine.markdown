---
layout: page
title: Drag[en]gine
permalink: /apidoc/dragengine/
nav_order: 1
parent: API Documentation
---

# Drag[en]gine Library API Documentation

{% assign urlapidoc = site.url | append: site.baseurl | append: "/artifacts/apidoc/dragengine/" %}
{% assign versions = site.data.apidoc.dragengine %}

## Latest
{% assign version = versions.first.version %}
[Drag[en]gine Library: {{ version }}]({{ urlapidoc }}{{ version }}/index.html){:target="_blank"}

## Previous Versions
{% assign count = versions.size | minus: 1 %}
{% assign versions2 = versions | slice: 1, count %}
{% for each in versions2 %}
  {% assign version = each.version %}
  [Drag[en]gine Library: {{ version }}]({{ urlapidoc }}{{ version }}/index.html){:target="_blank"}
{% endfor %}
