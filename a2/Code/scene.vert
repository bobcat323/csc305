layout(location = SCENE_POSITION_ATTRIB_LOCATION)
in vec4 Position;

layout(location = SCENE_TEXCOORD_ATTRIB_LOCATION)
in vec2 TexCoord;

layout(location = SCENE_NORMAL_ATTRIB_LOCATION)
in vec3 Normal;

uniform mat4 ModelWorld;
uniform mat4 ModelViewProjection;
uniform mat3 Normal_ModelWorld;

in vec3 vertex_color;
out vec3 fragment_color;
out vec2 fragment_texcoord;

void main()
{
    // TODO: Set to MVP * P
        //gl_Position = vec4(0,0,0,1);
        gl_Position = ModelViewProjection * Position;
        // TODO: Pass vertex attributes to fragment shader
        fragment_color = vertex_color;
        fragment_texcoord = TexCoord;
}
