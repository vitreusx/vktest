flextGL
=======

[![Build Status](https://circleci.com/gh/mosra/flextgl.svg?style=shield)](https://circleci.com/gh/mosra/flextgl)
[![Coverage Status](https://codecov.io/gh/mosra/flextgl/branch/master/graph/badge.svg)](https://codecov.io/gh/mosra/flextgl)

flextGL is an OpenGL and Vulkan header and loader generator.

It is a bit different than other comparable systems:

-   Gives complete control over exposed version and extensions
-   Bindings directly generated from the OpenGL / Vulkan registry `gl.xml` /
    `vk.xml` files
-   For OpenGL, optionally exports only core-profile functions
-   For Vulkan, it's possible to adapt the templates to generate separate
    instance-specific and device-specific loader for faster runtime
    performance, or switch from globally visible symbols to local ones
    ([blog post with details](https://blog.magnum.graphics/hacking/simple-efficient-vulkan-loading-with-flextgl/))
-   Only requested extensions are loaded
-   Flexible python template system for source generation
-   Source templates easy to adapt to project requirements
-   (Optionally) specify only particular functions to be loaded

You will need the following dependencies:

-   [Python 3.x](http://python.org)
-   [Wheezy Template](http://packages.python.org/wheezy.template)

### What's new?

-   **October 2019:** Breaking change -- the GL `<KHR/khrplatform.h>` include
    is now required to be specified by the template instead of being added
    implicitly from the `gl.xml` definition. This is done in order to allow
    more flexibility and is consistent with how `vk_platform.h` is handled.
    See commit bef1e12c98562fa87b698751a75d9da84e11c98d for details.
-   **May 2018:** Vulkan support, 100% test coverage,
    [@mosra](https://github.com/mosra) took over the maintainership from
    [@ginkgo](https://github.com/ginkgo)
-   **January 30 2015:** Adding function whitelist and `lite` template
-   **August 7 2014:** Adding support for generating OpenGL ES loading code
-   **July 10 2014:** flextGL now parses `gl.xml` instead of the deprecated
    `.spec` files.

Source tree
-----------

-   `flextGLgen.py`

    > The generator script.

-   `flext.py`

    > The parsing code

-   `templates/`

    > The sub-directories in here contain the different template sets. You can
    > add your own template by simply creating a new folder in there.

-   `profiles/`

    > Some example profile files to give you an idea on how to write your own.
    > `profiles/exampleProfile.txt` contains a lot of comments to get you up to
    > speed.

-   `spec/` (generated)

    > This directory is automatically created by the script to store the
    > downloaded OpenGL and Vulkan XML spec files.

Usage
-----

You create your loader code by writing a profile file and passing it to the
script.

Here is what a typical profile might look like:

    version 3.3 core
    extension EXT_texture_filter_anisotropic optional
    extension ARB_tesselation_shader optional

This requests  an OpenGL core profile and  the extensions for anisotropic
filtering and tesselation shaders. Those extensions were requested as
`optional`. This means that a missing extension won't cause an error. Instead,
the programmer will have to check before using it. This can be easily done by
testing a generated global variable. For OpenGL ES a typical profile might look
like:

    version 3.0 es
    extension OES_standard_derivatives optional
    extension OES_vertex_array_object optional

Because the world is far from ideal, *certain vendors* don't bother upstreaming
their extensions. To handle that, one can supply additional spec URLs to get
merged to the main one. Local files passed to `extraspec` also work, in case
it's needed to manually add an entrypoint defined nowhere else. If the file
path is relative, it's resolved relative to the profile file location.

    version 2.0 es

    extraspec https://raw.githubusercontent.com/google/angle/master/scripts/gl_angle_ext.xml

    # This one is from the vanilla gl.xml
    extension OES_vertex_array_object optional
    # This one ... thanks for the unnecessary extra pain, Google
    extension ANGLE_multi_draw optional

A Vulkan profile, whitelisting only some particular functions, can look like:

    version 1.1 vulkan

    extension KHR_swapchain optional

    begin functions
        CreateInstance
        CreateBuffer
        CmdDraw
        CreateSwapchainKHR
    end functions

In case of Vulkan it's also possible to generate the output from a particular
release by saying e.g. `version 1.1.73 vulkan`. That is useful in case you need
to match exactly the release version required by
[vulkan.hpp](https://github.com/KhronosGroup/Vulkan-Hpp), for example.

The profile file is then passed to the script like this:

    $ python flextGLgen.py -D generated -t <template-of-your-choice> profile.txt

This will create the requested source code and put it in the `generated/`
directory.

The best way to work with flextGL is to integrate it in your build-system. The
example project demonstrates this for Make and CMake [here](https://github.com/ginkgo/flextGL-example).

Generated API for OpenGL
------------------------

The builtin OpenGL templates all generate the following API:

* `int flextInit()`
  > Initializes the OpenGL functions after context creation.

* `FLEXT_MAJOR_VERSION`
  > The OpenGL major version defined in the profile file.

* `FLEXT_MINOR_VERSION`
  > The OpenGL minor version defined in the profile file.

* `FLEXT_CORE_PROFILE`
  > Boolean variable.  Is GL_TRUE,  if the profile  file defined  a core
  > profile.

* `FLEXT_<extension-name>`
  > Generated global  variables for checking if a  specific extension is
  > supported.

Take a look at the [example program](https://github.com/ginkgo/flextGL-example)
to get an idea on how it's used.

Note that it's possible to supply your own template with your own API that
might or might not resemble the above.

Generated API for Vulkan
------------------------

For simplicity, the default template generates all function pointers globally
and loads them as instance-specific. See
[this blog post](https://blog.magnum.graphics/hacking/simple-efficient-vulkan-loading-with-flextgl/)
for other options. Apart from Vulkan APIs, two functions are defined:

-   `void flextVkInit()`

    >   Initializes all global function pointers (pointers not depending on a
    >   particular VkInstance). Call this before doing anything else.

-   `void flextVkInitInstance(VkInstance instance)`

    >   Initializes all instance-specific function pointers using given
    >   `instance` pointer. Call this once you have created a VkInstance.

Note that it's possible to supply your own template with your own API that
might or might not resemble the above.

Templates
---------

At the moment, there are three template sets available:

-   `compatible`

    > This loads the extensions using a framework-agnostic method with WGL
    > AGL or GLX. This is probably a sensible default for most people.

-   `glfw3`, `glfw3-es`

    > These use GLFW 3 functions for loading and testing for OpenGL extensions
    > either for desktop OpenGL or OpenGL ES. In these templates, the
    > `flextInit()` function takes a pointer to `GLFWwindow`.

-   `lite`

    > A variant of the `compatible` template that generates more compact code.
    > It has not been thoroughly tested yet, though.

*   `vulkan`

    > A basic loader for Vulkan.

*   `vulkan-dynamic`

    > A variant of the `vulkan` template that loads the Vulkan library dynamically.
    > No static linking to Vulkan is necessary and the signature of `flextVkInit` is changed to `bool flextVkInit()`.
    > This lets you additionally check for the availability of Vulkan itself
    > and display an error message or fallback to another API if not available.

Installing Wheezy Template on Windows
-------------------------------------

If you have Python 3.4+ installed you should be able to install Wheezy using pip.

    $ pip install --user wheezy.template

The `--user` does a local install in your home-folder. You can omit it if you want
to do a system-wide installation.

If you have an older version of Python or don't have pip for some reason, then
you need to [install](https://pip.pypa.io/en/latest/installing.html) it first.

Credits
-------

-   Thomas Weber ([@ginkgo](https://github.com/ginkgo)) is the original author.
-   The `compatible` template uses a few code snippets from Slavomir Kaslev's
    gl3w OpenGL core profile loader for portable loading of procedures and
    checking minor/major version in OpenGL < 3.0.
-   Mykhailo Parfeniuk ([@sopyer](https://github.com/sopyer)) provided most of
    the `gl.xml` parsing code.
-   Vladimír Vondruš ([@mosra](https://github.com/mosra)) added support for
    OpenGL ES and Vulkan loader generation.
-   Leander Beernaert ([@LeanderBB](https://github.com/LeanderBB)) did various
    fixes on the compatible template.
-   [@eternalrain](https://github.com/eternalrain) added support for function
    white-lists and created the `lite` template.
-   The Khronos Group Inc. ([https://www.khronos.org/vulkan/](https://www.khronos.org/vulkan/))
    for Vulkan itself and the dynamic Vulkan library loader code.
-   Tolga Mizrak ([@to-miz](https://github.com/to-miz)) added the dynamic
    Vulkan loader template variant.

License
-------

flextGL is licensed under the MIT/Expat license, see the [COPYING](COPYING)
file for details.
