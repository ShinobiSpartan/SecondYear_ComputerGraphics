// Classic Phong fragment shader
#version 410

 in vec4 vPosition;
 in vec3 vNormal;
 
 uniform vec3 cameraPosition;
 
 uniform vec3 Ia; // Ambient light colour
 uniform vec3 Id; // Diffuse light colour
 uniform vec3 Is; // Specular light colour
 uniform vec3 LightDirection;
 
 out vec4 FragColour;
 
 uniform vec3 Ka; // Ambient material colour
 uniform vec3 Kd; // Diffuse material colour
 uniform vec3 Ks; // Specular material colour
 uniform float specularPower; // Material specular power
 
 void main() {
		// Ensure normal and light direction are normalised
		vec3 N = normalize(vNormal);
		vec3 L = normalize(LightDirection);
		
		// Calculate lambert term (negate light direction)
		float lambertTerm = max( 0, min( 1, dot( N, -L ) ) );
		
		// Calculate view vector and reflection vector
		vec3 V = normalize(cameraPosition - vPosition.xyz);
		vec3 R = reflect( L,N );
		
		// Calculate specular term
		float specularTerm = pow( max( 0, dot( R,V ) ), specularPower );
		
		// Calculate each colour property
		vec3 ambient = Ia * Ka;
		vec3 diffuse = Id * Kd * lambertTerm;
		vec3 specular = Is * Ks * specularTerm;
		
		// Output final colour
		FragColour = vec4(ambient + diffuse + specular, 1);
 }