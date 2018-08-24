#pragma include "../include/header.glsl"
#pragma include "../include/convolution_3x3.glsl"

vec4 mainFrag() {
    vec3 n[9] = get3x3();

    // https://gist.github.com/Hebali/6ebfc66106459aacee6a9fac029d0115
	vec3 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
  	vec3 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
	vec3 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

    return vec4(sobel, n[4].a);
}

#pragma include "../include/footer.glsl"
