#include "game/Resources.h"
#include "util/DebugLog.h"

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
		DBG_IF(flux::verbose, "BrickLights block not found!");
	}
	if (lightBlockIndex2 == GL_INVALID_INDEX)
	{
		DBG_IF(flux::verbose, "ScenarioLights block not found!");
	}

	// --- portal pipeline ----------------------------------------------------
	// Offscreen target for the opponent's screen. Wider than the playfield's own
	// 1:2, because it captures the grid PLUS a margin of the surrounding screen --
	// see backgroundExpand below.
	const int boardTexWidth = 624;
	const int boardTexHeight = 1024;
	opponentBoard.create(boardTexWidth, boardTexHeight);

	// Where the visible playfield actually lands, derived rather than eyeballed.
	// A brick instance sits at cellSize*column in x and cellSize*sin(82) per row in
	// y, offset by brickUnitMesh's own translation, and the whole mesh is then
	// tilted -8 degrees about X.
	//
	// Only rows 6..25 are framed. Rows 0..5 are the spawn buffer above the
	// playfield: leaving them in frame would show the opponent's next piece before
	// it enters play, which is information their screen does not give them either.
	const float cellSize = 0.030483870967741f;
	const float meshOffsetX = 0.543548387096774f - 0.775f;
	const float meshOffsetY = -0.336816396484375f;
	const int visibleTopRow = 6;

	const float rowRise = cellSize * dsin(82.0f);
	const float playfieldBottom = meshOffsetY - cellSize * 0.5f;
	const float playfieldTop = meshOffsetY + (25 - visibleTopRow) * rowRise + cellSize * 0.5f;

	// A brick's own quad spans [0, cellSize] in x but is CENTRED in y (see
	// brickUnitVertices), so the two axes centre differently: the playfield is
	// 10 whole cells wide starting at the mesh offset, but only half a cell tall
	// either side of it.
	const float gridCenterX = meshOffsetX + 10 * cellSize * 0.5f;
	const float gridCenterYLocal = (playfieldTop + playfieldBottom) * 0.5f;
	const float gridCenterZLocal = cellSize * 0.5f;
	const float gridHalfHeight = (playfieldTop - playfieldBottom) * 0.5f;
	const float gridHalfWidth = 10 * cellSize * 0.5f;
	const float portalViewDistance = 1.0f;

	// The capture is deliberately a little larger than the playfield, and the
	// margin is their screen background rather than empty space.
	//
	// Why: the portal's matter is an ellipse and the window is a rectangle
	// inscribed in it. At the rectangle's sides the ellipse still has room to
	// spare, and at two opposing corners it falls short. The spilling corners get
	// the electric arcs; the spare area at the sides would otherwise be showing
	// nothing, so the surrounding screen fills it. The margin only has to reach the
	// ellipse's bounding box -- it is filler, not subject, so it stays minimal.
	// Kept small on purpose. The background is filler, not subject: it only has to
	// cover the band of ellipse immediately outside the rectangle's sides, and then
	// fade, leaving the ellipse's outer lobes to the matter itself. An earlier,
	// generous margin covered the whole ellipse, which both pulled the opponent's
	// NEXT panel into frame and painted over the matter's swirls entirely.
	const float backgroundExpand = 1.123f;
	const float captureHalfHeight = gridHalfHeight * backgroundExpand;
	const float captureHalfWidth = captureHalfHeight * (float)boardTexWidth / (float)boardTexHeight;
	portalViewHalfHeight = captureHalfHeight;

	// The board is tilted, so a straight-down-Z camera would see it as a trapezoid
	// (its top and bottom sit at different depths). Tilting the view camera by the
	// same -8 degrees puts it square to the playfield and the image comes out flat.
	const glm::mat4 boardTilt = glm::rotate(glm::mat4(1.0f), glm::radians(-8.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::vec3 boardCentre = glm::vec3(boardTilt * glm::vec4(gridCenterX, gridCenterYLocal, gridCenterZLocal, 1.0f));
	const glm::vec3 boardForward = glm::vec3(boardTilt * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	const glm::vec3 boardUp = glm::vec3(boardTilt * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

	portalViewCamera.position = boardCentre - boardForward * portalViewDistance;
	portalViewCamera.orientation = boardForward;
	portalViewCamera.up = boardUp;

	// --- portal placement and size -------------------------------------------
	// Everything about where the portal is and how big it is lives here.
	//
	// `scale` is the one to reach for to resize the whole thing: it is uniform and
	// multiplies the matter, the window, the arcs' thickness and the
	// materialisation's grain together, because every length in portal.frag is in
	// the portal's local units. TR_PORTAL overrides these at startup for quick
	// visual iteration without a rebuild -- see applyPortalOverride in main.cpp.
	//
	// windowHalf and matterRadius change the effect's PROPORTIONS rather than its
	// size, and they interact: the matter's ellipse has to cover the window's short
	// edges and two opposing corners while falling short of the other two, or the
	// electric arcs have nothing to do. Retune those together, and re-run
	// portal.create() afterwards so the proxy quad is rebuilt to match.
	//
	// The yaw is deliberate: face-on the portal would read as a flat billboard, and
	// the point of marching the effect in 3D is that it holds up as you walk round.
	portal.windowHalf = glm::vec2(gridHalfWidth / gridHalfHeight * 0.42f, 0.42f);
	portal.matterRadius = 0.56f;
	portal.position = glm::vec3(1.28f, 0.02f, 0.12f);
	portal.yawDegrees = -25.0f;
	portal.pitchDegrees = 0.0f;
	portal.scale = 1.0f;

	// The grid fills slightly less than the whole window, so the outermost row and
	// column of cells sit on a thin band of the opponent's screen instead of flush
	// against the window's edge. Breathing space only -- the margin is filler, and
	// anything more starts pulling their side panels into frame.
	const float boardInset = 0.93f;
	portal.contentScale = glm::vec2(gridHalfWidth / captureHalfWidth,
	                                gridHalfHeight / captureHalfHeight) / boardInset;

	portal.setTransform();
	portal.create();

	scenarioLights.push_back(Light{ glm::vec4(0.55f - 0.775f, 0.0f, 0.4f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) });
	scenarioLights.push_back(Light{ glm::vec4(camera.position, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.1f, 0.0f, 0.0f, 0.0f) });
	brickLights.push_back(Light{ glm::vec4(0.0f,0.0f,0.1f,0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) });
}


