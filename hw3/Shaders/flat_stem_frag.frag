#version 330
uniform sampler2D MyTexture;

uniform vec4 ka;
uniform vec4 ks;

uniform vec3 light;
uniform vec3 vector;
uniform float alpha;

in vec2 texcoord;
in vec3 pos;

out vec4 outColor;

void main() {

	vec3 norm = -normalize(cross(dFdx(pos.xyz), dFdy(pos.xyz)));
	vec4 color= texture2D(MyTexture, texcoord);
	vec4 kd = vec4(color.rgb,color.a);
	vec3 l = normalize(light - pos);	 
    vec3 e = normalize(vector-pos);
	vec3 gamma = normalize(-reflect(l, norm));
	float lambertian = max(dot(l,norm), 0.0);
    float specular = pow(max(dot(e,gamma), 0.0), alpha);
 	outColor = ka + kd *lambertian + ks * specular;


}