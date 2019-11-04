{%- extends 'base.tpl' -%}
{% from 'mathjax.tpl' import mathjax %}


{%- block header -%}
<!DOCTYPE html>
<html>
<head>
{%- block html_head -%}
<meta charset="utf-8" />
{% set nb_title = nb.metadata.get('title', '') or resources['metadata']['name'] %}
<title>{{nb_title}} - Notes Portal</title>

<link rel="icon" type="image/gif" href="https://lihd1003.github.io/assets/images/logo.png">
<link rel="stylesheet" href="https://lihd1003.github.io/assets/css/vendor.css" />
<link rel="stylesheet" href="https://lihd1003.github.io/assets/css/style.css" />
<link href="https://fonts.googleapis.com/css?family=Roboto&display=swap" rel="stylesheet">
<script src="https://kit.fontawesome.com/98fa07784c.js"></script>

{% block ipywidgets %}
{%- if "widgets" in nb.metadata -%}
<script>
(function() {
  function addWidgetsRenderer() {
    var mimeElement = document.querySelector('script[type="application/vnd.jupyter.widget-view+json"]');
    var scriptElement = document.createElement('script');
    var widgetRendererSrc = '{{ resources.ipywidgets_base_url }}@jupyter-widgets/html-manager@*/dist/embed-amd.js';
    var widgetState;

    // Fallback for older version:
    try {
      widgetState = mimeElement && JSON.parse(mimeElement.innerHTML);

      if (widgetState && (widgetState.version_major < 2 || !widgetState.version_major)) {
        widgetRendererSrc = '{{ resources.ipywidgets_base_url }}jupyter-js-widgets@*/dist/embed.js';
      }
    } catch(e) {}

    scriptElement.src = widgetRendererSrc;
    document.body.appendChild(scriptElement);
  }

  document.addEventListener('DOMContentLoaded', addWidgetsRenderer);
}());
</script>
{%- endif -%}
{% endblock ipywidgets %}

<style type="text/css">
/* Overrides of notebook CSS for static HTML export */

div#notebook {
  overflow: visible;
  border-top: none;
}

{%- if resources.global_content_filter.no_prompt-%}
div#notebook-container{
  padding: 6ex 12ex 8ex 12ex;
}
{%- endif -%}

@media print {
  div.cell {
    display: block;
    page-break-inside: avoid;
  } 
  div.output_wrapper { 
    display: block;
    page-break-inside: avoid; 
  }
  div.output { 
    display: block;
    page-break-inside: avoid; 
  }
}
</style>

<!-- Custom stylesheet, it must be in the same directory as the html file -->
<link rel="stylesheet" href="https://lihd1003.github.io/UofT-Course-Material-Repo/table_of_content/custom.css">

<!-- Loading mathjax macro -->
{{ mathjax() }}
{%- endblock html_head -%}
</head>
{%- endblock header -%}

{% block body %}
<body>
	<!-- header -->
   <header class="header-sticky header-dark">
    <div class="container">
      <nav class="navbar navbar-expand-lg navbar-dark">
        <a class="navbar-brand" href="./index.html">
          <img class="navbar-logo navbar-logo-light" src="https://lihd1003.github.io/assets/images/logo.png" alt="Logo">
          <img class="navbar-logo navbar-logo-dark" src="https://lihd1003.github.io/assets/images/logo.png" alt="Logo">
        </a>

        <div class="collapse navbar-collapse" id="navbarSupportedContent">
          <ul class="navbar-nav align-items-center mr-auto">
          </ul>

          <ul class="navbar-nav align-items-center mr-0">
            <li class="nav-item">
              <a href="https://github.com/lihd1003" class="nav-link waves-effect waves-light" target="_blank">
                <i class="icon-github mr-2"></i>GitHub
              </a>
            </li>
            <li class="nav-item">
              <a href="https://github.com/lihd1003/UofT-Course-Material-Repo" class="nav-link waves-effect waves-light" target="_blank">
                <i class="icon-star mr-2"></i>Star the Repo
              </a>
            </li>
          </ul>
        </div>
      </nav>
    </div>
  </header>

<section class="hero hero-with-header text-white" data-top-top="transform: translateY(0px);" data-top-bottom="transform: translateY(250px);">
    <div class="image image-overlay" style="background-image:url(https://lihd1003.github.io/assets/images/black.jpg)"></div>
    <div class="container">
      <div class="row align-items-center">
        <div class="col text-shadow">
{% set nb_title = nb.metadata.get('title', '') or resources['metadata']['name'] %}
          <h1 class="mb-0">{{nb_title}}</h1>
        </div>
      </div>
    </div>
 </section>
 <section class="bg-white sec" id="project">
  <div tabindex="-1" id="notebook" class="border-box-sizing">
    <div class="container" id="notebook-container">
{{ super() }}
    </div>
  </div>
</section>
  <script src="https://lihd1003.github.io/assets/js/vendor.js"></script>
  <script src="https://lihd1003.github.io/assets/js/app.js"></script>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
  <script>
    $("table").addClass("table table-lined")
  </script>
</body>
{%- endblock body %}

{% block footer %}
{{ super() }}
</html>
{% endblock footer %}
