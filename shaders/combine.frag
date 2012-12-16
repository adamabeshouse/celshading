uniform sampler2D tex1;
uniform sampler2D tex2;
void main(void) {
		vec4 newColor = texture2D(tex1, gl_TexCoord[0].st) + texture2D(tex2, gl_TexCoord[0].st);
		newColor.r = min(1.0, newColor.r);
		newColor.g = min(1.0, newColor.g);
		newColor.b = min(1.0, newColor.b);
		gl_FragColor = newColor;
}


