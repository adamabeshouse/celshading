uniform sampler2D tex;
uniform float iwidth;
uniform float iheight;
const vec4 avgVector = vec4(0.299, 0.587, 0.114, 0);
void main(void) {
		float A00 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(-1.0/iwidth,-1.0/iheight)), avgVector);
		float A10 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(0.0/iwidth,-1.0/iheight)), avgVector);
		float A20 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(1.0/iwidth,-1.0/iheight)), avgVector);
		float A01 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(-1.0/iwidth,0.0/iheight)), avgVector);
		float A11 = dot(texture2D(tex, gl_TexCoord[0].st), avgVector);
		float A21 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(1.0/iwidth,0.0/iheight)), avgVector);
		float A02 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(-1.0/iwidth,1.0/iheight)), avgVector);
		float A12 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(0.0,1.0/iheight)), avgVector);
		float A22 = dot(texture2D(tex, gl_TexCoord[0].st + vec2(1.0/iwidth,1.0/iheight)), avgVector);
		float gx = -1.0*A00 + A20 - 2.0*A01 + 2.0*A21 -1.0*A02 + A22;
		float gy = -1.0*A00 - 2.0*A10 - A20 + A02 + 2.0*A12 + A22;
		float d = sqrt(gx*gx + gy*gy);

		float threshold = 0.1;
		d = min(1.0, d);
		vec4 edge = vec4(1.0,1.0,1.0,0);
		if(d < threshold) {
			edge = vec4(0,0,0,0);
		}
		gl_FragColor = edge;//texture2D(tex, gl_TexCoord[0].st) + edge;
}


