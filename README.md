# molecular-gfx

This is a 3D graphics library with a (to my knowledge) unique approach.

## Principles Of Operation

molecular-gfx revolves around the idea of dynamic scoping. C++ uses lexical scoping, which means that you can access
variables depending on where they where defined. In contrast, dynamic scoping lets you access variables depending on
where your code was _called_ from. Pseudocode follows:

```c++
void f()
{
    int i = 5;
    g();
}

void g()
{
    std::cout << i; // Can access i here!
}
```

Further reading: https://en.wikipedia.org/wiki/Scope_(computer_science)#Lexical_scoping_vs._dynamic_scoping

This principle is used to form a kind of generalized scene graph, where not only transforms are inherited along the
tree, but all sorts of data.

At the leafs of the tree, where actual rendering takes place, all variables defined at this point are collected and fed
to a shader generator. The shader generator attempts to use as many of the defined variables as possible to build a
complete shader program out of individual functions. See [its project page](https://github.com/cmdrf/molecular-programgenerator)
for more information.

A simple render graph could look like this (pseudocode!):

```c++
void Render()
{
    Matrix4 projection = PerpectiveProjection();
    Matrix4 view = LookTo(Vector3(1, 2, 3), Vector3(2, 3, 4));
    RenderMesh1();
    RenderMesh2();
}

void RenderMesh1()
{
    Vector3 diffuseColor(1, 0, 0);
    VertexBuffer vertexPosition = LoadMesh("cube.obj");
    DrawMesh(); // Shader generator generates program using projection, view, diffuseColor and vertexPosition
}

void RenderMesh2()
{
    Texture diffuseTexture = LoadTexture("car.jpg");
    VertexBuffer vertexPosition = LoadMesh("car.obj");
    DrawMesh(); // Shader generator generates program using projection, view, diffuseTexture and vertexPosition
}
```
