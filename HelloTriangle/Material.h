#pragma once

//Esta classe futuramente pode ser estendida para ter as outras propriedades de um material
//Como por exemplo coef. ambiente, difuso e especular

class Material
{
public:
	Material() { texID = -1; }

	unsigned int texID; //identificador da textura
protected:
	//Decidi colocar o atributo de ID como público no momento
};

