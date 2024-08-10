$input a_position, a_color0
$output v_color0

#include <bgfx_shader.sh>
#include "shaderlib.sh"

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 0.0, 1.0) );
    float hue = a_color0.x + 0.5;
    vec3 rgb_color = hsv2rgb(vec3(hue, a_color0.yz));
    v_color0 = vec4(rgb_color, a_color0.a);
}