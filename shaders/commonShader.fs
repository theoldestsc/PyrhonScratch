#version 440
out vec4 FragColor;
flat in vec4 we;
in vec4 id;
in vec3 interpolatedNormal;
in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;

void main()
{    
    vec4 nonNegativeWeights = max(we, vec4(0.0));
    //vec4 wColor = vec4(nonNegativeWeights.x, nonNegativeWeights.y, nonNegativeWeights.z, nonNegativeWeights.w);
    //vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0)); // Example light direction
    //float diffuseIntensity = max(dot(interpolatedNormal, lightDirection), 0.0);
    FragColor = texture(material.texture_diffuse1, TexCoords);
    //FragColor = diffuseIntensity * wColor;
    FragColor = we;
}