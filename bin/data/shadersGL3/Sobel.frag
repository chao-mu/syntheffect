#pragma include "common/frag.glsl"
#pragma include "common/convolution_frag.glsl"

void main() {
    vec3 n[9] = getTextures();

    // https://gist.github.com/Hebali/6ebfc66106459aacee6a9fac029d0115
	vec3 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
  	vec3 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
	vec3 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

    outputColor = vec4(sobel.rgb, 1.0);
}