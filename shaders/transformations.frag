#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

// Variáveis uniform com parâmetros para os exercícios
uniform int code;

void main()
{
    //O comando texture é da própria GLSL, e busca a cor do fragmento com a coord de 
    //textura fornecida
    color = texture(ourTexture1, TexCoord);// * vec4(ourColor, 1.0);

    //LEMBRE-SE: aqui o valor dos canais de cor está NORMALIZADO (entre 0.0 e 1.0)
    
    // 1a - Exibir imagem grayscale com apenas 1 canal de cor
    if (code == 1) //R
    {
        color = vec4(color.r, color.r, color.r, color.a);
    }
    else if (code == 2) //G
    {
        color = vec4(color.g, color.g, color.g, color.a);
    }
    else if (code == 3) //B
    {
        color = vec4(color.b, color.b, color.b, color.a);
    }
    else if (code == 4)
    {
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }
}