attribute vec2 VertexLocation;
attribute vec2 TextureLocation;

uniform mat4 WORLD_MATRIX;
uniform mat4 TRANSFORM_MATRIX;

varying vec2 vTextureLocation;

void main() {
	vTextureLocation = TextureLocation;

	highp vec4 vPos = WORLD_MATRIX * TRANSFORM_MATRIX * vec4(VertexLocation, 0.0, 1.0);
    vPos.x = vPos.x - 1.0;
    vPos.y = vPos.y - 1.0;

	gl_Position = vPos;
}