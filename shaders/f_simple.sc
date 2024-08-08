$input v_color0, v_texcoord0

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
    float edge_distance = min(min(v_texcoord0.x, 1.0 - v_texcoord0.x), min(v_texcoord0.y, 1.0 - v_texcoord0.y));
    if (edge_distance < 0.05)
	{
	    v_color0.b=0.5;
	}

    vec3 rgb = hsv2rgb(v_color0.rgb);
 
    gl_FragColor = vec4(rgb, v_color0.a);
}