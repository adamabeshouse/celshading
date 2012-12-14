varying float lightIntensity;
varying vec4 color;
void main(void) {

        float finIntensity;
        if(lightIntensity>.9){
            finIntensity=.95;
        }
        else if(lightIntensity>.70){
            finIntensity= .75;
        }
        else if(lightIntensity>.45){
            finIntensity = .5;
        }
        else{
            finIntensity = .25;
        }
        gl_FragColor = finIntensity * color;
    }
