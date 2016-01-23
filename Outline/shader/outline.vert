void main()
{
    vec4 n = gl_ModelViewMatrix * vec4(gl_Normal.xyz,0);
    vec4 direct = gl_ModelViewMatrix * gl_Vertex;
	
	vec4 nn = normalize(n);
	vec4 ndirect = normalize(direct);

    float intensityn = dot(ndirect.xyz,nn.xyz);

    if(intensityn>-0.1&& intensityn < 0.1)
        gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
    else{
		gl_FrontColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
     
	gl_Position = ftransform();
}
