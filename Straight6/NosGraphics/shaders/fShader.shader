#version 330

in vec2 uv;
in vec3 outNorm;
in vec3 posW;
in float fog_amount;
in vec4 fragPosLight;

uniform vec3 fog_colour;
uniform sampler2D sampler;
uniform sampler2D shadowMap;
uniform vec3 viewPos;

out vec4 fragmentColour;

uniform vec3 lightColour;
uniform vec3 ambient;
uniform vec3 lightPos;
const float shininess = 64.0;
const float specularStrength = 0.2;

//https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	vec4 texColour = texture(sampler, uv).rgba;

	vec3 normaliseNormal = normalize(outNorm);

	vec3 lightDir = normalize(lightPos - posW);

	float diffIllumination = max(dot(normaliseNormal, lightDir), 0.0);
	
	float inShadow = 0.0;
	vec3 lightCords = fragPosLight.xyz / fragPosLight.w;
    if(lightCords.z <= 1.0)
    {
        float bias = 0.005;
        lightCords = (lightCords + 1.0) / 2.0;
        float currentDepth = lightCords.z;

        int sampleRad = 3;
        vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
        float randomOffset = 0.001; // Adjust the random offset strength as needed

        for(int y = -sampleRad; y <= sampleRad; y++)
        {   
            for(int x = -sampleRad; x <= sampleRad; x++)
            {
                // Generate random offset
                vec2 randomOffsetVec = vec2(
                    rand(lightCords.xy + vec2(x, y) * pixelSize), rand(lightCords.yx + vec2(y, x) * pixelSize)) * randomOffset;

                float closestDepth = texture(shadowMap, lightCords.xy + vec2(x, y) * pixelSize + randomOffsetVec).r;   
                if(currentDepth > closestDepth + bias)
                    inShadow += 1.0;    
            }
        }
        inShadow /= pow((sampleRad * 2 + 1), 2);
    }


	vec3 diffuse = (diffIllumination * lightColour) * (1.0 - inShadow);
	vec3 viewDir = normalize(viewPos - posW);
	vec3 reflectionDir = reflect(-lightDir, normaliseNormal);

	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), shininess);

	vec3 specular = (specularStrength * spec * lightColour) * (1.0 - inShadow);

	vec3 finalColour = (ambient + diffuse) * texColour.rgb + specular;

	vec4 ligthing = vec4(finalColour, texColour.a);
	fragmentColour = mix(ligthing, vec4(fog_colour, 1), fog_amount);
}