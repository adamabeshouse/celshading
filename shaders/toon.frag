varying float lightIntensity;
varying vec4 color;
void main(void) {

        float finIntensity;
		finIntensity = float(round(lightIntensity*2))/2.0;
		gl_FragColor = finIntensity * color + 0.5*color;
		/*gl_FragColor.r = min(1.0, gl_FragColor.r);
		gl_FragColor.g = min(1.0, gl_FragColor.g);
		gl_FragColor.b = min(1.0, gl_FragColor.b);*/
    }
