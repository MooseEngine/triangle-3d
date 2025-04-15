# Barycentric Coordinates
Barycentric coordinates are a way to express the position of a point P, which lies inside a triangle, as a weighted sum of the triangle's vertices $A$, $B$, and $C$.

Formally, *if*

$$$

P=\alpha{A} + \beta{B} + \gamma{C}

$$$

Then $\alpha$, $\beta$, and $\gamma$ (which are called the *barycentric weights/coordinates*) have the following properties:
* They are non negative if $P$ lies inside or on the edge of the triangle.
* They sum to 1 ($\alpha+\beta+\gamma=1$)

Each weight corresponds to the "influence" of its vertex on the point $P$.

## Computing Barycentric Coordinates
To compute these, one common method is to use the areas (determinants) of sub-triangles. Assuming:
* $A=(A_x, A_y)$
* $B=(B_x, B_y)$
* $C=(C_x, C_y)$

We want to find, for a point $P(P_x, P_y)$, the barycentric coordinates $\alpha$, $\beta$, and $\gamma$. They are:

$$
D = (y_B - y_C)(x_A - x_C) + (x_C - x_B)(y_A - y_C)
$$

$$
\alpha = \frac{(y_B - y_C)(x - x_C) + (x_C - x_B)(y - y_C)}{D}
$$

$$
\beta = \frac{(y_C - y_A)(x - x_C) + (x_A - x_C)(y - y_C)}{D}
$$

$$
\gamma = 1 - \alpha - \beta
$$

$$
P = \alpha A + \beta B + \gamma C
$$

$$
\text{Color}_P = \alpha\,\text{Color}_A + \beta\,\text{Color}_B + \gamma\,\text{Color}_C
$$

Or, in code:
```c
float denom = ((B_y - C_y) * (A_x - C_x) + (C_x - B_x) * (A_y - C_y));

float alpha = ((B_y - C_y) * (P_x - C_x) + (C_x - B_x) * (P_y - C_y)) / denom;
float beta  = ((C_y - A_y) * (P_x - C_x) + (A_x - C_x) * (P_y - C_y)) / denom;
float gamma = 1.0f - alpha - beta;
```

## Interpolating Colors Using Barycentrics

