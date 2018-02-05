/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/17/2013
*/

# version 330 core

in vec2 texCoord;
in vec4 color;
in vec3 Position;
in vec3 Normal;
vec3 tempColor = vec3(color) * 0.1f;

uniform vec3 HeadLightPosition;
uniform vec3 HeadLightIntensity;
uniform vec3 PointLightPosition;
uniform vec3 PointLightIntensity;
uniform vec3 AmbientLightIntensity;

uniform bool IsTexture;
uniform bool HeadLightOn;
uniform bool PointLightOn;
uniform bool DebugOn;
uniform bool IsTheSun;
uniform bool AmbientOn;
uniform sampler2D Texture;

vec3 headLightPos = vec3(0.0,0.0,1.0);

vec3 headLightColor = vec3(1.0, 0.0, 0.0); // debug headlight red
vec3 diffuseColor = vec3(0.0, 1.0, 0.0); // debug diffuse green

out vec4 fragColor;

vec3 vLight(vec3 LightPosition, vec3 LightIntensity, int lightType){
	float ambient = 0.0f;
	float diffuse = 0.0f;
	float headLight = 0.0f;
	vec3 n, s;

	n = normalize(Normal);

	if(lightType == 0){  //ambient light
		LightIntensity = LightIntensity * tempColor;
		ambient = 1.0;
	}
	else if(lightType == 1){  //head light
		s = normalize(LightPosition);
		headLight = max(dot(s,n), 0.0);
	}
	else{  //point light
		s = normalize(LightPosition - Position);
		diffuse = max(dot(s,n), 0.0);
	}

	if (DebugOn)
		return headLight * headLightColor + diffuse * diffuseColor + ambient * LightIntensity;
	else
		return ambient * LightIntensity + diffuse * LightIntensity + headLight * LightIntensity;
}

void main() {
	if(IsTheSun && PointLightOn)
		tempColor = vec3(1.0,1.0,0.0);
	else{
		if(PointLightOn)
			tempColor += vLight(PointLightPosition, PointLightIntensity, 2);
		if(HeadLightOn)
			tempColor += vLight(headLightPos, HeadLightIntensity, 1);
		if(AmbientOn)
			tempColor += vLight(HeadLightPosition, HeadLightIntensity, 0);
	}

	if (IsTexture)   // use texture on surface
		fragColor = texture(Texture, texCoord);
    else         // use color for surface
		fragColor = vec4(tempColor, 1.0);
}