#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffusePMap;
uniform sampler2D normalPMap;
uniform sampler2D depthPMap;

uniform float heightScale;


vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    // number of depth layers
    const float minLay = 8;
    const float maxLay = 32;
    float numLayers = mix(maxLay, minLay, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float curLayerDepth = 0.0;
    // the amount to shift texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  curTexCoords = texCoords;
    float curDepthMapValue = texture(depthPMap, curTexCoords).r;
      
    while(curLayerDepth < curDepthMapValue)
    {
        // shift texture coordinates along direction of P
        curTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        curDepthMapValue = texture(depthPMap, curTexCoords).r;
        // get depth of next layer
        curLayerDepth += layerDepth;
    }
    // Relief Map
    // halving the offset of texture coordinates and the size of the depth layer
	deltaTexCoords *= 0.5;
	layerDepth *= 0.5;
    // moving in the opposite direction from the point found in Steep PM
	curTexCoords += deltaTexCoords;
	curLayerDepth -= layerDepth;

    // setting the maximum search iterations
	const int reliefSteps = 5;
	int curStep = reliefSteps;
	while (curStep > 0) {
		curDepthMapValue = texture(depthPMap, curTexCoords).r;
		deltaTexCoords *= 0.5;
		layerDepth *= 0.5;
        // if the depth selection is greater than the current layer depth then go to the left half of the interval
		if (curDepthMapValue > curLayerDepth) {
			curTexCoords -= deltaTexCoords;
			curLayerDepth += layerDepth;
		}
        // go to the right half of the interval
		else {
			curTexCoords += deltaTexCoords;
			curLayerDepth -= layerDepth;
		}
		curStep--;
	}

	float lastDepthValue = curDepthMapValue;
    
    return curTexCoords;
}

void main()
{
    // offset texture coordinates with Parallax Mapping
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;
    
    texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);
    if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    // obtain normal from normal map
    vec3 normal = texture(normalPMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
   
    // get diffuse color
    vec3 diffColor = texture(diffusePMap, texCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * diffColor;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diffConst = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diffConst * diffColor;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specConst = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * specConst;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}