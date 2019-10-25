// A normal map fragment shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 cameraPosition;

uniform vec3 Ia;  // Light ambient
uniform vec3 Id;  // Light diffuse
uniform vec3 Is;  // Light specular;
uniform vec3 lightDirection;

uniform vec3 Jd;  // Light 2 diffuse
uniform vec3 Js;  // Light 2 specular
uniform vec3 light2Direction;

out vec4 fragColour;

uniform vec3 Ka;  // Material ambient
uniform vec3 Kd;  // Material diffuse
uniform vec3 Ks;  // Material specular
uniform float specularPower;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

void main() {

		vec3 N = normalize(vNormal);
		vec3 L = normalize(lightDirection);
		vec3 L2 = normalize(light2Direction);
		vec3 T = normalize(vTangent);
		vec3 B = normalize(vBiTangent);
	
		mat3 TBN = mat3(T,B,N);
	
		vec3 texDiffuse = texture( diffuseTexture, vTexCoord ).rgb;
		vec3 texSpecular = texture( specularTexture, vTexCoord ).rgb;
		vec3 texNormal = texture( normalTexture, vTexCoord ).rgb;
	
		N = TBN * (texNormal * 2 - 1);
	
		// Calculate lambert term
		float lambertTerm = max( 0, dot( N, -L ) );
		float lambertTerm2 = max( 0, dot( N, -L2 ) );
	
		// Calculate view vector and reflection vector
		vec3 V = normalize(cameraPosition - vPosition.xyz);
		vec3 R = reflect( L, N );
		vec3 R2 = reflect(L2, N);
	
		// Calculate specular term
		float specularTerm = pow( max( 0, dot( R,V ) ), specularPower );
		float specularTerm2 = pow( max( 0, dot( R2,V ) ), specularPower );
	
		// Calculate each light property
		vec3 ambient = Ia * Ka;
		vec3 diffuse = ((Id * Kd * lambertTerm )+ (Jd * Kd  * lambertTerm2 ))* texDiffuse;
		vec3 specular = ((Is * Ks * specularTerm) + (Js * Ks * specularTerm2))* texSpecular ;
	
		fragColour = vec4(ambient + diffuse + specular, 1);
}