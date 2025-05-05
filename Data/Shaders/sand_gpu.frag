#version 130
uniform sampler2D u_current_state;

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    float value = texture2D(u_current_state, uv).r;

    // Just pass through white pixels, black otherwise
    if (value > 0.5)
    gl_FragColor = vec4(1.0);
    else
    gl_FragColor = vec4(0.0);
}