#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
    //O comando texture é da própria GLSL, e busca a cor do fragmento com a coord de 
    //textura fornecida
    color = texture(ourTexture1, TexCoord);

    //LEMBRE-SE: aqui o valor dos canais de cor está NORMALIZADO (entre 0.0 e 1.0)
    
    // 1b) - Exibir imagem grayscale com média ponderada
    // R * 0.2125
	// G * 0.7154
	// B * 0.0721
	float media = color.r * 0.2125 + color.g * 0.7154 + color.b * 0.0721;
	color = vec4(media, media, media, color.a);
}