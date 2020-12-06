#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture3;

void main()
{
    //O comando texture é da própria GLSL, e busca a cor do fragmento com a coord de 
    //textura fornecida
    color = texture(ourTexture1, TexCoord);
    
    vec4 colorMask = texture(ourTexture3, TexCoord);

    // 1e) - Efeito Polaroid
    color = mix(color, colorMask, colorMask.a);
    color = vec4(color.rgb, 1.0);
}