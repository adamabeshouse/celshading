varying float lightIntensity;
varying vec4 color;
const vec3 lightDirection = vec3(1, -1, .3);
void main(void) {
    gl_Position = ftransform();
    vec3 D = normalize(lightDirection);
    vec3 N = normalize(gl_Normal);
    lightIntensity = dot(D,N);
    color = gl_Color;
}
