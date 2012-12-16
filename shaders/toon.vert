varying float lightIntensity;
varying vec4 color;
const vec3 moonDirection = vec3(-1, -1, 0);
void main(void) {
	gl_Position = ftransform();
	vec3 fireDifference = gl_Position.xyz;
	vec3 D = normalize(moonDirection);
	vec3 N = normalize(gl_Normal);
	vec3 F = normalize(fireDifference);
	float moonIntensity = max(0.0, dot(D,N));
	float fireIntensity = max(0.0, dot(F,N));
	lightIntensity = moonIntensity + fireIntensity;
    color = gl_Color;
}
