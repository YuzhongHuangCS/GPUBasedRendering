varying float LightIntensity;

void main()
{	
	gl_FragColor = vec4(0.8*LightIntensity, 0.2*LightIntensity, 0.2*LightIntensity, 1.0);
}
