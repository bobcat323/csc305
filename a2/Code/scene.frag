uniform vec3 CameraPos;

uniform vec3 Ambient;
uniform vec3 Diffuse;
uniform vec3 Specular;
uniform float Shininess;

uniform int HasDiffuseMap;
uniform sampler2D DiffuseMap;

in vec3 fragment_color;
in vec2 fragment_texcoord;
out vec4 FragColor;

void main()
{
    // TODO: Replace with Phong shading
        //sample from the texture using GLSL's texture function
        //vec3 diffuseMap = texture(DiffuseMap, fragment_texcoord).rgb;

        vec3 diffuseMap;
        if(HasDiffuseMap != 0){
            diffuseMap = texture(DiffuseMap, fragment_texcoord).rgb;
        }else{
            diffuseMap = vec3(1.0);
        }
        //FragColor = fragment_color;
        FragColor = vec4(diffuseMap,0);
}
