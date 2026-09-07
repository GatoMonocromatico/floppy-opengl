#include "game/Resources.h"

glm::vec3 Resources::hexadecimalToRGB(std::string hex, float saturationOffset)
{
	glm::vec3 rgb(0);

	if (hex[0] == '#')
	{
		hex.erase(hex.begin());
	}

	for (int i = 0; i < 3; i++)
	{
		for (int hexadecimalPlace = 0; hexadecimalPlace < 2; hexadecimalPlace++)
		{
			//std::cout << "idx: " << i * 2 + hexadecimalPlace << ", hex size: " << hex.size() << std::endl;
			switch (hex[i * 2 + hexadecimalPlace])
			{
			case '0':
				rgb[i] += 0.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case '1':
				rgb[i] += 1.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case '2':
				rgb[i] += 2.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case '3':
				rgb[i] += 3.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case '4':
				rgb[i] += 4.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case '5':
				rgb[i] += 5.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case '6':
				rgb[i] += 6.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case '7':
				rgb[i] += 7.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case '8':
				rgb[i] += 8.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case '9':
				rgb[i] += 9.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case 'a':
				rgb[i] += 10.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case 'b':
				rgb[i] += 11.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case 'c':
				rgb[i] += 12.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case 'd':
				rgb[i] += 13.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case 'e':
				rgb[i] += 14.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			case 'f':
				rgb[i] += 15.0f * powf(16.0f, 1 - hexadecimalPlace) / 255.0f + saturationOffset;
				break;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		rgb[i] = std::min(rgb[i], 1.0f);
		rgb[i] = std::max(rgb[i], 0.0f);
	}

	return rgb;
}

void Resources::load()
{
	
	//arcadeVertices 
	verts.push_back(std::vector<Vertex>{
		// pos			// color		// tex UV		// mask UV		// normal
		//UI
		/*v<-  0*/Vertex{ glm::vec3(0.0f,	-0.41345f * dsin(82.0f),	-0.41345f * -dcos(82.0f) + 0.00002f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f + 0.15966796875f * 0.0f, 0.239501953125f * 1.0f), glm::vec2(0.15966796875f * 0.0f, 0.239501953125f * 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },	// v<-
			/*^<-  1*/Vertex{ glm::vec3(0.0f,	 0.41345f * dsin(82.0f),	 0.41345f * -dcos(82.0f) + 0.00002f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f + 0.15966796875f * 0.0f, 0.239501953125f * 2.0f), glm::vec2(0.15966796875f * 0.0f, 0.239501953125f * 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },	// ^<-
			/*^->  2*/Vertex{ glm::vec3(1.1f,	 0.41345f * dsin(82.0f),	 0.41345f * -dcos(82.0f) + 0.00002f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f + 0.15966796875f * 1.0f, 0.239501953125f * 2.0f), glm::vec2(0.15966796875f * 1.0f, 0.239501953125f * 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },	// ^->
			/*v->  3*/Vertex{ glm::vec3(1.1f,	-0.41345f * dsin(82.0f),	-0.41345f * -dcos(82.0f) + 0.00002f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f + 0.15966796875f * 1.0f, 0.239501953125f * 1.0f), glm::vec2(0.15966796875f * 1.0f, 0.239501953125f * 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },	// v->
			//monitor
			/*v<-  4*/Vertex{ glm::vec3(0.0f,	-0.41345f * dsin(82.0f),	-0.41345f * -dcos(82.0f) + 0.00001f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f + 0.15966796875f * 0.0f, 0.239501953125f * 0.0f), glm::vec2(0.15966796875f * 0.0f, 0.239501953125f * 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },	// v<-
			/*^<-  5*/Vertex{ glm::vec3(0.0f,	 0.41345f * dsin(82.0f),	 0.41345f * -dcos(82.0f) + 0.00001f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f + 0.15966796875f * 0.0f, 0.239501953125f * 1.0f), glm::vec2(0.15966796875f * 0.0f, 0.239501953125f * 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },	// ^<-
			/*^->  6*/Vertex{ glm::vec3(1.1f,	 0.41345f * dsin(82.0f),	 0.41345f * -dcos(82.0f) + 0.00001f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f + 0.15966796875f * 1.0f, 0.239501953125f * 1.0f), glm::vec2(0.15966796875f * 1.0f, 0.239501953125f * 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },	// ^->
			/*v->  7*/Vertex{ glm::vec3(1.1f,	-0.41345f * dsin(82.0f),	-0.41345f * -dcos(82.0f) + 0.00001f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f + 0.15966796875f * 1.0f, 0.239501953125f * 0.0f), glm::vec2(0.15966796875f * 1.0f, 0.239501953125f * 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },	// v->
			//arcade
			/*v<-  8*/Vertex{ glm::vec3(0.0f - 0.2289833984375f,	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(-0.78113f - 0.0457693359375f) * -dcos(82.0f)),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			/*^<-  9*/Vertex{ glm::vec3(0.0f - 0.2289833984375f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),		(0.78113f - 0.0457693359375f) * -dcos(82.0f)),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			/*^->  10*/Vertex{ glm::vec3(1.558f - 0.2289833984375f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),		(0.78113f - 0.0457693359375f) * -dcos(82.0f)),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^->
			/*v->  11*/Vertex{ glm::vec3(1.558f - 0.2289833984375f, 	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(-0.78113f - 0.0457693359375f) * -dcos(82.0f)),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v->
			// left wall - right side
			/*vfront  12*/Vertex{ glm::vec3(0.0f - 0.2289833984375f,	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f	, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			/*^front  13*/Vertex{ glm::vec3(0.0f - 0.2289833984375f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f + 0.15f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f	, 0.007820136852394f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			/*^back  14*/Vertex{ glm::vec3(0.0f - 0.2289833984375f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),		(0.78113f - 0.0457693359375f) * -dcos(82.0f)),			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f						, 0.007820136852394f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			/*vback  15*/Vertex{ glm::vec3(0.0f - 0.2289833984375f,	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f)),				glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f						, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			// left wall - front side
			/*^  16*/Vertex{ glm::vec3(0.0f - 0.2289833984375f - 0.0457966796875f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f + 0.15f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f * 2.0f, 0.007820136852394f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			/*v  17*/Vertex{ glm::vec3(0.0f - 0.2289833984375f - 0.0457966796875f,	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f * 2.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			// left wall - left side
			/*v  18*/Vertex{ glm::vec3(0.0f - 0.2289833984375f - 0.0457966796875f,	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f - 1.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f * 3.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			/*^  19*/Vertex{ glm::vec3(0.0f - 0.2289833984375f - 0.0457966796875f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f - 1.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f * 3.0f, 0.007820136852394f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			// right wall - left side
			/*vfront  20*/Vertex{ glm::vec3(1.558f - 0.2289833984375f,	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f	, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			/*^front  21*/Vertex{ glm::vec3(1.558f - 0.2289833984375f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f + 0.15f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f	, 0.007820136852394f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			/*^back  22*/Vertex{ glm::vec3(1.558f - 0.2289833984375f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),		(0.78113f - 0.0457693359375f) * -dcos(82.0f)),			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f						, 0.007820136852394f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			/*vback  23*/Vertex{ glm::vec3(1.558f - 0.2289833984375f,	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f)),				glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f						, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			// right wall - front side
			/*^  24*/Vertex{ glm::vec3(1.558f - 0.2289833984375f + 0.0457966796875f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f + 0.157f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f * 2.0f, 0.007820136852394f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			/*v  25*/Vertex{ glm::vec3(1.558f - 0.2289833984375f + 0.0457966796875f,	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f * 2.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			// right wall - right side
			/*v  26*/Vertex{ glm::vec3(1.558f - 0.2289833984375f + 0.0457966796875f,	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f - 1.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f * 3.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			/*^  27*/Vertex{ glm::vec3(1.558f - 0.2289833984375f + 0.0457966796875f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),	(0.78113f + 0.0457693359375f) * dcos(82.0f) + 0.05f - 1.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.65966796875f + 0.003910068426197f * 3.0f, 0.007820136852394f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			// bottom celling
			/*^->  10*/Vertex{ glm::vec3(1.558f - 0.2289833984375f,	(0.78113f - 0.0457693359375f) * dsin(82.0f),		(0.78113f - 0.0457693359375f) * -dcos(82.0f)),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^->
			/*v->  11*/Vertex{ glm::vec3(1.558f - 0.2289833984375f, 	(-0.78113f - 0.0457693359375f) * dsin(82.0f),	(-0.78113f - 0.0457693359375f) * -dcos(82.0f)),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },		// v->

	});

	//portalVertices
	verts.push_back(std::vector<Vertex>{
		// pos			// color		// tex UV		// mask UV		// normal
		Vertex{ glm::vec3(0.0f,	-0.41345f,	0.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.666015625f * 0.0f, 1.0f * 0.0f), glm::vec2(0.146484375f * 0.0f, 0.10986328125f * 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
		Vertex{ glm::vec3(0.0f,	 0.41345f,	0.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.666015625f * 0.0f, 1.0f * 1.0f), glm::vec2(0.146484375f * 0.0f, 0.10986328125f * 6.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
		Vertex{ glm::vec3(1.1f,	 0.41345f,	0.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.666015625f * 1.0f, 1.0f * 1.0f), glm::vec2(0.146484375f * 1.0f, 0.10986328125f * 6.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^->
		Vertex{ glm::vec3(1.1f,	-0.41345f,	0.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.666015625f * 1.0f, 1.0f * 0.0f), glm::vec2(0.146484375f * 1.0f, 0.10986328125f * 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v->
	});

	//brickUnitVertices
	verts.push_back(std::vector<Vertex>{
		//back
		/*v<-  0*/Vertex{ glm::vec3(0.0f,	-0.015241935483870f, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.029296875f * 0.0f), glm::vec2(0.15966796875f * 0.0f, 0.239501953125f * 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			/*^<-  1*/Vertex{ glm::vec3(0.0f,	 0.015241935483870f, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.029296875f * 1.0f), glm::vec2(0.15966796875f * 0.0f, 0.239501953125f * 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			/*^->  2*/Vertex{ glm::vec3(0.030483870967741f,	 0.015241935483870f, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.029296875f * 1.0f, 0.029296875f * 1.0f), glm::vec2(0.15966796875f * 1.0f, 0.239501953125f * 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^->
			/*v->  3*/Vertex{ glm::vec3(0.030483870967741f,	-0.015241935483870f, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.029296875f * 1.0f, 0.029296875f * 0.0f), glm::vec2(0.15966796875f * 1.0f, 0.239501953125f * 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v->
			//front
			/*v<-  4*/Vertex{ glm::vec3(0.0f,	0.015241935483870f, 0.030483870967741f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.029296875f * 0.0f), glm::vec2(0.15966796875f * 0.0f, 0.239501953125f * 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
			/*^<-  5*/Vertex{ glm::vec3(0.030483870967741f,	 0.015241935483870f, 0.030483870967741f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.029296875f * 1.0f), glm::vec2(0.15966796875f * 0.0f, 0.239501953125f * 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^<-
			/*^->  6*/Vertex{ glm::vec3(0.030483870967741f,	 -0.015241935483870f, 0.030483870967741f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.029296875f * 1.0f, 0.029296875f * 1.0f), glm::vec2(0.15966796875f * 1.0f, 0.239501953125f * 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// ^->
			/*v->  7*/Vertex{ glm::vec3(0.0f,	-0.015241935483870f, 0.030483870967741f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.029296875f * 1.0f, 0.029296875f * 0.0f), glm::vec2(0.15966796875f * 1.0f, 0.239501953125f * 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v->

	});

	
	//point
	verts.push_back(std::vector<Vertex>{
		Vertex{ glm::vec3(0.0f,	0, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f },	// v<-
	});
	//arcade
	indices.push_back(std::vector<GLuint>
	{
		9, 8, 10, //arcade machine
			11, 10, 8,
			5, 4, 6, //monitor	
			7, 6, 4,
			1, 0, 2, //ui
			3, 2, 0,
			13, 12, 14, //left wall (right side) arcade machine 
			15, 14, 12,
			12, 13, 16, //left wall (front side) arcade machine 
			17, 12, 16,
			17, 16, 18, //left wall (left side) arcade machine 
			19, 18, 16,

			20, 21, 22, //right wall (left side) arcade machine 
			22, 23, 20,
			21, 20, 24, //right wall (front side) arcade machine 
			20, 25, 24,
			24, 25, 26, //right wall (right side) arcade machine 
			26, 27, 24
	});
	//portalIndices
	indices.push_back(
		std::vector<GLuint>{
		1, 0, 2,
		3, 2, 0
	});
	
	//brickUnitIndices
	indices.push_back(
		std::vector<GLuint>{
		// back
		0, 1, 2, 2, 3, 0,

			// front
			4, 6, 5, 6, 4, 7,

			// left
			4, 1, 7, 1, 0, 7,

			// right
			2, 5, 3, 3, 5, 6,

			// bottom
			0, 3, 6, 0, 6, 7,

			// top
			2, 1, 4, 5, 2, 4

	});

	//Texture portalMaskTex = 0
	texs[0].push_back(Texture("assets/textures/enemy-portal-spawn-anim.png", "mask", GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	//Texture noSpecularTex = 1
	texs[0].push_back(Texture("assets/textures/no-specular.png", "mask", GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	//Texture arcadeMonitorTex = 0
	texs[1].push_back(Texture("assets/textures/arcade-monitor.png", "tex", GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE));
	//Texture arcadeMachineTex = 1
	texs[1].push_back(Texture("assets/textures/arcade.png", "tex", GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE));
	//Texture brickUnitTex = 2
	texs[1].push_back(Texture("assets/textures/brick-unit-skins.png", "tex", GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE));
	
	anims.push_back(std::vector<Animation>{ Animation(1.0f / 16.0f, 36, 1200, 900, 8192, 8192) });

	Mesh arcadeMesh = Mesh(verts[0], indices[0], std::vector<Texture>{texs[0][1], texs[1][1]});
	Mesh portalMesh = Mesh(verts[1], indices[1], std::vector<Texture>{texs[0][0], texs[1][0]});
	Mesh brickUnitMesh = Mesh(verts[2], indices[2], std::vector<Texture>{texs[0][1], texs[1][2]});

	arcadeMesh.translateModel(glm::vec3(-0.775f, 0.0f, 0.0f));

	portalMesh.rotateModel(glm::vec3(1.0f, 0.0f, 0.0f), -glm::radians(5.0f));
	portalMesh.rotateModel(glm::vec3(0.0f, 1.0f, 0.0f), -glm::radians(15.0f));
	portalMesh.translateModel(glm::vec3(-0.16f, 0.0f, 0.0f));
	portalMesh.scaleModel(glm::vec3(0.8f));

	brickUnitMesh.rotateModel(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(-8.0f));
	brickUnitMesh.translateModel(glm::vec3(0.543548387096774f - 0.775f, -0.336816396484375f, 0.00003f));

	meshs.push_back(arcadeMesh);
	meshs.push_back(portalMesh);
	meshs.push_back(brickUnitMesh);
	
	Point lightPoint = Point(verts[3]);
	points.push_back(lightPoint);

	camera.rotateCamera(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(-8.0f));


	shaderProgram[0].Activate();

	glGenBuffers(1, &brickUBO);
	// bind
	glBindBuffer(GL_UNIFORM_BUFFER, brickUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * 64, NULL, GL_DYNAMIC_DRAW);

	GLuint lightBlockIndex1 = glGetUniformBlockIndex(shaderProgram[0].ID, "BrickLights");
	glUniformBlockBinding(shaderProgram[0].ID, lightBlockIndex1, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, brickUBO);

	glGenBuffers(1, &scenarioUBO);
	// bind
	glBindBuffer(GL_UNIFORM_BUFFER, scenarioUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * 64, NULL, GL_DYNAMIC_DRAW);

	GLuint lightBlockIndex2 = glGetUniformBlockIndex(shaderProgram[0].ID, "ScenarioLights");
	glUniformBlockBinding(shaderProgram[0].ID, lightBlockIndex2, 1);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, scenarioUBO);


	shaderProgram[1].Activate();

	// bind
	glBindBuffer(GL_UNIFORM_BUFFER, scenarioUBO);

	lightBlockIndex2 = glGetUniformBlockIndex(shaderProgram[1].ID, "ScenarioLights");
	glUniformBlockBinding(shaderProgram[1].ID, lightBlockIndex2, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, scenarioUBO);

	if (lightBlockIndex1 == GL_INVALID_INDEX)
	{
		std::cout << "BrickLights block not found!" << std::endl;
	}if (lightBlockIndex2 == GL_INVALID_INDEX)
	{
		std::cout << "ScenarioLights block not found!" << std::endl;
	}

	scenarioLights.push_back(Light{ glm::vec4(0.55f - 0.775f, 0.0f, 0.4f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) });
	scenarioLights.push_back(Light{ glm::vec4(camera.position, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.1f, 0.0f, 0.0f, 0.0f) });
	brickLights.push_back(Light{ glm::vec4(0.0f,0.0f,0.1f,0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) });
}


