#version 130


in vec3 P;
in vec3 N;

out vec3 color_final;

uniform vec3 color = vec3(1.0,0.0,0.0);
uniform vec3 bcolor = vec3(1.0,0.0,1.0);


const vec3 lp1 = vec3(1000,1300,200);
const vec3 lp2 = vec3(-1300,1000,1000);


const float specExp = 200;

void main()
{
	vec3 No = normalize(N);

	vec3 Lu1 = normalize(lp1-P);
	vec3 Lu2 = normalize(lp2-P);
	float lambert= 0.1 + 0.5 * clamp(dot(No,Lu1),0,1) + 0.5 * clamp(dot(No,Lu2),0,1);

	vec3 Re = reflect(-Lu1,No);
	vec3 Ca = normalize(-P);
	float spec = pow( clamp(dot(Re,Ca),0,1), specExp);
	Re = reflect(-Lu2,No);
	spec += pow( clamp(dot(Re,Ca),0,1), specExp);

	if (gl_FrontFacing)
		color_final = color*lambert + vec3(1.0,1.0,1.0)*spec;
	else
		color_final = bcolor*lambert;
}

