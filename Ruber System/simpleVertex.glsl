/* 
SimpleVertex.glsl

Vertex shader with position, color, normal and ModelViewProject
input and color output.

Mike Barnes
8/17/2013
*/

# version 330 core


in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTexCoord;

out vec2 texCoord;  
out vec3 Position;
out vec3 Normal;
out vec4 color;

uniform bool IsTexture;
uniform mat3 NormalMatrix; // not bound in SolarMain yet
uniform mat4 ModelView; //bound in display in SolarMain
uniform mat4 ModelViewProjection;  // = projection * view * model

void main() {
	gl_Position = ModelViewProjection * vPosition;
	Position = (ModelView * vPosition).xyz;
	Normal = normalize(NormalMatrix * vNormal);
	if (IsTexture)  {  // texture is used, not color
		texCoord = vTexCoord;
		color = vec4(0,0,0,0);
	}
    else {   // color is used, not texture
		texCoord =vec2(0, 0);
		color = vColor;
	}
}