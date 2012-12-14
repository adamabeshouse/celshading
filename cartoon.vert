varying float intensity;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec3 vertex_norm; 
    vertex_norm = gl_NormalMatrix * gl_Normal;
    vertex_norm = normalize(vertex_norm);
    vec3 light_norm; 
    light_norm = gl_LightSource[0].position - gl_ModelViewMatrix * gl_Vertex;
    light_norm = normalize(light_norm);
    intensity = dot(vertex_norm, light_norm);
}
