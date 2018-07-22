precision mediump float;

varying vec2 vTextureLocation;
uniform sampler2D TextureSample;

void main() {
    gl_FragColor = texture2D(TextureSample, vTextureLocation);
}