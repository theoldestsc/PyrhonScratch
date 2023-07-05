#version 440

const int MAX_BONES = 100;

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoord;



layout (location = 3) in vec4 weights;
layout (location = 4) in ivec4 s_vIDs;


uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform mat4 gBones[MAX_BONES];

out vec2 TexCoords;
out vec4 we;
out vec4 id;
out vec3 interpolatedNormal;

void main()
{
    TexCoords = vertex_texcoord;

    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < 4 ; i++)
    {
        if(s_vIDs[i] == -1) 
            continue;
        if(s_vIDs[i] >= MAX_BONES) 
        {
            totalPosition = vec4(vertex_position, 1.0f);
            break;
        }
        vec4 localPosition = gBones[s_vIDs[i]] * vec4(vertex_position, 1.0f);
        totalPosition += localPosition * weights[i];
    }
    
    //mat4 boneMatrix = gBones[int(s_vIDs.x)] * weights.x;
    //boneMatrix += gBones[int(s_vIDs.y)] * weights.y;
    //boneMatrix += gBones[int(s_vIDs.z)] * weights.z;
    //boneMatrix += gBones[int(s_vIDs.w)] * weights.w;
    //boneMatrix = gBones[int(s_vIDs.x)];

    interpolatedNormal = normalize(mat3(transpose(inverse(ModelMatrix))) * vertex_normal);

    mat4 viewModel = ViewMatrix * ModelMatrix;
    gl_Position = ProjectionMatrix * viewModel * totalPosition;
    we = weights;
    id = s_vIDs;

}

