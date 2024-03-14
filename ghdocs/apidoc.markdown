---
layout: page
title: API Documentation
permalink: /apidoc/
nav_order: 2
---

{% assign urlapidoc = site.url | append: site.baseurl | append: "/artifacts/apidoc/dragengine/" %}
{% assign versions = site.data.apidoc.dragengine %}

# API Documentation

API documentation of  Drag[en]gine Game Engine.

## Latest
{% assign version = versions.first.version %}
[Drag[en]gine Game Engine: {{ version }}]({{ urlapidoc }}{{ version }}/index.html){:target="_blank"}

## Previous Versions
{% assign count = versions.size | minus: 1 %}
{% assign versions2 = versions | slice: 1, count %}
{% for each in versions2 %}
  {% assign version = each.version %}
  [Drag[en]gine Game Engine: {{ version }}]({{ urlapidoc }}{{ version }}/index.html){:target="_blank"}
{% endfor %}



Test 1
{: .highlight }

Test 2
{: .warning }

Test 3
{: .note }

Test 4
{: .important }




This is the base Jekyll theme. You can find out more info about customizing your Jekyll theme, as well as basic Jekyll usage documentation at [jekyllrb.com](https://jekyllrb.com/)

You can find the source code for Minima at GitHub:
[jekyll][jekyll-organization] /
[minima](https://github.com/jekyll/minima)

You can find the source code for Jekyll at GitHub:
[jekyll][jekyll-organization] /
[jekyll](https://github.com/jekyll/jekyll)


[jekyll-organization]: https://github.com/jekyll

Test:

{% for each in site.data.apidoc.dragengine %}
[Drag[en]gine Library: {{ each.version }}]({{ site.url }}{{ site.baseurl }}/artifacts/apidoc/dragengine/{{ each.version }}/index.html){:target="_blank"}
{% endfor %}

Test 2:

{% for each in site.data.apidoc.dragonscript_module %}
[DragonScript Script Module: {{ each.version }}]({{ site.url }}{{ site.baseurl }}/artifacts/apidoc/dragonscript_module/{{ each.version }}/index.html){:target="_blank"}
{% endfor %}

Test 3:

{% for each in site.data.apidoc.dragonscript_language %}
[DragonScript Language: {{ each.version }}]({{ site.url }}{{ site.baseurl }}/artifacts/apidoc/dragonscript_language/{{ each.version }}/index.html){:target="_blank"}
{% endfor %}

