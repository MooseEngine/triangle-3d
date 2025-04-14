## See `notes/` for a more technical explanation of what this code does. In short:
We have taken our triangle-2d code and extended it to 3d, running it through Model, View, and Projection matrices, and displayed it relative to a camera. This required the creation of some helper files:

### `linalg.*`
This handles all the matrix and vector math, including basics like subtraction, dot and cross products, matrix and vector multiplication. It also includes handling getting the model, view, and projection matrices according to relevant parameters.

It also includes (and will include) the handling of edge functions and possibly barycentric coordinates.

### `model.*`
This handles anything specifically related to the shapes that we're modelling. At the moment, this means only a triangle, but it has code to determine whether a point is within a triangle, and holds the `Triangle` struct.

## Build and run
As with the last project, you can build and run this code with:
```sh
make; ./rasterizer
```

I separated out the building and linking processes in the makefile, so run `make clean` if you want to get rid of the object files. They are `.gitignore`d though.
