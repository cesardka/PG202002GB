#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

// Limiar da binarização
uniform int limiar;

void main()
{
    //O comando texture é da própria GLSL, e busca a cor do fragmento com a coord de 
    //textura fornecida
    color = texture(ourTexture1, TexCoord);

    //LEMBRE-SE: aqui o valor dos canais de cor está NORMALIZADO (entre 0.0 e 1.0)
    
    // 1e) Binarização de cores
	float media = color.r * 0.2125 + color.g * 0.7154 + color.b * 0.0721;

    float limiarNormalizado = limiar / 255.0;

    if (media < limiarNormalizado)
    {
        color = vec4(0.0, 0.0, 0.0, color.a);
    }
    else
    {
        color = vec4(1.0, 1.0, 1.0, color.a);
    }
	color = vec4(media, media, media, color.a);
}