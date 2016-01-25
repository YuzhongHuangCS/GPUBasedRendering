#version 110

varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;

void main() { 
	gl_Position = ftransform();
	vec3 vert = (gl_ModelViewMatrix * gl_Vertex).xyz;
	normal   = gl_NormalMatrix * gl_Normal;
	lightVec = gl_LightSource[0].position.xyz - vert;
	viewVec  = -vert;
}