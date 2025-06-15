#version 450

//Perlin noise from glNoisse library
//https://github.com/FarazzShaikh/glNoise.git
vec2 _fade(vec2 t) { return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); }
vec3 _fade(vec3 t) { return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); }

vec4 gln_rand4(vec4 p) { return mod(((p * 34.0) + 1.0) * p, 289.0); }

float gln_perlin(vec2 P) {
    vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
    vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
    Pi = mod(Pi, 289.0); // To avoid truncation effects in permutation
    vec4 ix = Pi.xzxz;
    vec4 iy = Pi.yyww;
    vec4 fx = Pf.xzxz;
    vec4 fy = Pf.yyww;
    vec4 i = gln_rand4(gln_rand4(ix) + iy);
    vec4 gx = 2.0 * fract(i * 0.0243902439) - 1.0; // 1/41 = 0.024...
    vec4 gy = abs(gx) - 0.5;
    vec4 tx = floor(gx + 0.5);
    gx = gx - tx;
    vec2 g00 = vec2(gx.x, gy.x);
    vec2 g10 = vec2(gx.y, gy.y);
    vec2 g01 = vec2(gx.z, gy.z);
    vec2 g11 = vec2(gx.w, gy.w);
    vec4 norm =
    1.79284291400159 - 0.85373472095314 * vec4(dot(g00, g00), dot(g01, g01),
    dot(g10, g10), dot(g11, g11));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;
    float n00 = dot(g00, vec2(fx.x, fy.x));
    float n10 = dot(g10, vec2(fx.y, fy.y));
    float n01 = dot(g01, vec2(fx.z, fy.z));
    float n11 = dot(g11, vec2(fx.w, fy.w));
    vec2 fade_xy = _fade(Pf.xy);
    vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
    float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
    return 2.3 * n_xy;
}

#define Step 0.5f
#define NumCols 1000
#define Offset (-999.0 *Step)

layout(push_constant, std430) uniform model {
    mat4 transform;
    vec2 position;
};

layout(location = 0) out vec3 fragColor;

vec2 offsets[6] = vec2[](
    vec2(-Step + Offset, Step + Offset), vec2(Step + Offset, Step + Offset), vec2(-Step + Offset, -Step + Offset),
    vec2(Step + Offset, Step + Offset), vec2(Step + Offset, -Step + Offset), vec2(-Step + Offset, -Step + Offset)
);

vec3 getVertex(uint Row, uint Col, uint square, vec2 pos)
{
    vec2 xy = vec2(2 * Col * Step, 2 * Row * Step) + pos;
    xy += offsets[square];
    return vec3(xy, 5.0 * gln_perlin(xy / 20.0));
}

vec3 colors[6] = vec3[](
    vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0),
    vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)
);

void main() {
    uint gridIndex = gl_VertexIndex / 6;
    uint squareIndex = gl_VertexIndex % 6;
    uint Col = gridIndex % NumCols;
    uint Row = gridIndex / NumCols;
    gl_Position = transform * vec4(getVertex(Row, Col, squareIndex, position), 1.0);
    fragColor = colors[squareIndex];
}