void main() {
    vec3 n[9] = get3x3();

    vec3 col = main3x3(n);

    outputColor = vec4(col, 1.0);
}