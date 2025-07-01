# Floor

Most of this discussion will concern `shaders/floor.vert`.

The floor is a procedurally generated surface and is drawn centered around the camera 
(if the camera moves laterally the floor will move with the camera).

The algorithm at its core is very simple.

Construct a grid in row major order (indices increase from left to right, then top to bottom) with N rows and columns.

| 0        | 1            | 2            | ... | N - 1   |
|:---------|:-------------|:-------------|:----|:--------|
| N        | N + 1        | N + 2        | ... | 2N - 1  |
| ...      | ...          | ...          | ... | ...     |
| (N - 1)N | (N - 1)N + 1 | (N - 1)N + 2 | ... | N^2 - 1 |

Then to calculate the row `row = index % N` and column `col = index / N` is easy.

```glsl
    uint Col = gridIndex / NumCols;
    uint Row = gridIndex % NumCols;
```
Lines 36-37.

However, each cell in the grid represents a square and a square can be constructed from two triangles.
Two triangles has 6 vertices, therefore the `gridIndex = gl_VertexIndex / 6;` can be calculated from the vertex index.

From there the position for the current square is calculated.
```glsl
vec2 xy = vec2(2 * Col * Step, 2 * Row * Step) + pos;
```
Line 23.
Where pos is (almost) the position of the camera.


An offset is then added to this position depending on which vertex is being drawn.
```glsl
xy += offsets[square];
```
Line 24.
Where square is the index of the vertex (0-5) for the current square.

These values are then used to calculate perlin noise for z value of the final vertex position.
The color for each vertex cycles between red, green and blue.

Finally the accuracy for the camera position is clipped so that the floor only updates on whole number intervals of the grid size.

```C++
    template<float step>
    static float step_pos(float a) {
        constexpr static float modifier = 2.0f * step;
        a /= modifier;
        return std::floor(a) * modifier;
    }
```
From `src/Floor.cppm` Lines 62-67.
```C++
            push_constant_t push_const{
                transform,
                glm::vec2 {
                    step_pos<0.5f>(global::cameraPosition().x),
                    step_pos<0.5f>(global::cameraPosition().y)
                }
            };
```
From `src/Floor.cppm` Lines 92-98.