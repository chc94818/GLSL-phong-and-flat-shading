#version 330

uniform sampler2D MyTexture1;

uniform vec4 ka;
uniform vec4 ks;

uniform vec3 light;
uniform vec3 vector;
uniform float alpha;

in vec3 c;
in vec3 vertex_view_space;
in vec2 texcoord;
in vec3 pos;

out vec4 outColor;

void main() {
	vec4 color= texture2D(MyTexture1, texcoord);
	vec4 kd = vec4(color.rgb,color.a);
	//vec3 l = normalize(light-pos);				// vec3 lightDir = normalize(lightPos - vertPos);
	//vec3 e = normalize(vector-pos);				//  vec3 viewDir = normalize(-vertPos);		
	//vec3 gamma = normalize(-reflect(l,norm));	//vec3 reflectDir = reflect(-lightDir, normal);
	//outColor = ka + kd *max(dot(l,norm),0) + ks * pow(max(dot(e,gamma),0),alpha);

	 vec3 norm = normalize(cross(dFdx(vertex_view_space), dFdy(vertex_view_space)));
	 vec3 l = -normalize(light - c);
	 vec3 gamma = normalize(-reflect(l, norm));
     vec3 e = normalize(vector-c);
	 float lambertian = max(dot(l,norm), 0.0);
     float specular = pow(max(dot(e,l), 0.0), alpha);

 	outColor = ka + kd *max(dot(l,norm),0) + ks * specular;

	
}