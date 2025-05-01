uniform sampler2D texture;

void main() {
    // texCoords hold sand cell coords (i, j)
    vec2 texCoords = gl_TexCoord[0].xy;

    // Normalize to texture size
    vec2 uv = texCoords / vec2(textureSize(texture, 0));
    vec4 color = texture2D(texture, uv);

    gl_FragColor = color;
}