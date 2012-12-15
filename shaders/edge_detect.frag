uniform sampler2D tex;
const vec3 avgVector = vec3(0.299, 0.587, 0.114);
void main(void) {
	float A00 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(-1,-1)), avgVector);
	float A10 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(0,-1)), avgVector);
	float A20 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(1,-1)), avgVector);
	float A01 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(-1,0)), avgVector);
	float A11 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(0,0)), avgVector);
	float A21 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(1,0)), avgVector);
	float A02 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(-1,1)), avgVector);
	float A12 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(0,1)), avgVector);
	float A22 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(1,1)), avgVector);

	float gx = -1*A00 + A20 - 2*A01 + 2*A21 -1*A02 + A22;
	float gy = -1*A00 - 2*A10 - A20 + A02 + 2*A12 + A22;
	float d = sqrt(gx*gx + gy*gy);
	gl_FragColor.rgb = vec3(d,d,d);
}


