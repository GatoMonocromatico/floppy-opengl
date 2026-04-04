#include<array>

#include"VBO.h"


void create1RoundEdge(std::vector<Vertex>& vertexes, std::vector<GLuint>& indices, std::vector<GLuint> quadrilateralDrawIndices, unsigned int idxVertexEdge1, unsigned int idxVertexEdge2, float roundPercentage)
{
	float i1, j1, k1, i2, j2, k2;

	Vertex& edge1 = vertexes[idxVertexEdge1];
	Vertex& edge2 = vertexes[idxVertexEdge2];
	
	std::array<std::array<unsigned int, 3>, 2> vertsSamePlane;

	int presenceOfEdge1InDrawCalls = 0;

	for (int i = 0; i < 2; i++)
	{
		int amountOfEdgeVertexes = 0;
		GLuint idxSharedEdgeVertex, idxSharedCornerVertex;

		for (int j = 0; j < 3; j++)
		{
			if (quadrilateralDrawIndices[i * 6 + j] == idxVertexEdge1 || quadrilateralDrawIndices[i * 6 + j] == idxVertexEdge2)
			{
				amountOfEdgeVertexes++;
			}
		}

		if (amountOfEdgeVertexes == 1)
		{
			for (int j = 0; j < 3; j++)
			{
				if (quadrilateralDrawIndices[i * 6 + j] == idxVertexEdge1 || quadrilateralDrawIndices[i * 6 + j] == idxVertexEdge2)
				{
					idxSharedEdgeVertex = quadrilateralDrawIndices[i * 6 + j];
				}
			}

			for (int j = 0; j < 3; j++)
			{
				if (quadrilateralDrawIndices[i * 6 + j + 3] != idxVertexEdge1 && quadrilateralDrawIndices[i * 6 + j + 3] != idxVertexEdge2)
				{
					idxSharedCornerVertex = quadrilateralDrawIndices[i * 6 + j + 3];
				}
			}
		}
		else if (amountOfEdgeVertexes == 2)
		{
			{
				for (int j = 0; j < 3; j++)
				{
					if (quadrilateralDrawIndices[i * 6 + j] == idxVertexEdge1 || quadrilateralDrawIndices[i * 6 + j] == idxVertexEdge2)
					{
						idxSharedCornerVertex = quadrilateralDrawIndices[i * 6 + j];
					}
				}

				for (int j = 0; j < 3; j++)
				{
					if (quadrilateralDrawIndices[i * 6 + j + 3] != idxVertexEdge1 && quadrilateralDrawIndices[i * 6 + j + 3] != idxVertexEdge2)
					{
						idxSharedEdgeVertex = quadrilateralDrawIndices[i * 6 + j + 3];
					}
				}
			}
		}
		for (int j = 0; j < 3; j++)
		{
			if (quadrilateralDrawIndices[i * 6 + j] != idxSharedEdgeVertex || quadrilateralDrawIndices[i * 6 + j] == idxSharedCornerVertex)
			{
				vertsSamePlane[i][0];
			}
		}
		for (int j = 0; j < 3; j++)
		{
			if (quadrilateralDrawIndices[i * 6 + j] == idxVertexEdge1 || quadrilateralDrawIndices[i * 6 + j] == idxVertexEdge2)
			{
				idxSharedEdgeVertex = quadrilateralDrawIndices[i * 6 + j];
			}
		}
	}

}