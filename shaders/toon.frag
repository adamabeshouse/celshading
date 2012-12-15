varying float lightIntensity;
varying vec4 color;
void main(void) {

        float finIntensity;
		finIntensity = float(round(lightIntensity*2))/2.0;
        gl_FragColor = finIntensity * color;
    }
