#include "MyMesh.h"
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//circle code from exercise
	std::vector<vector3> vertex;
	GLfloat theta = 0;
	GLfloat delta = static_cast<GLfloat>(2.0 * PI / static_cast<GLfloat>(a_nSubdivisions));
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 temp = vector3(cos(theta) * a_fRadius, sin(theta) * a_fRadius, 0.0f);
		theta += delta;
		vertex.push_back(temp);
	}
	int setHeight = a_fHeight + 0.5;

	for (int i = 0; i < a_nSubdivisions; i++) {
		//create circular base using circle code
		AddTri(ZERO_V3, vertex[(i + 1) % a_nSubdivisions], vertex[i]);
		//based on the circular base, make connections that form a point at the set value for height
		AddTri(vertex[i], vertex[(1 + i) % a_nSubdivisions], vector3(0, 0, setHeight));
	}

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

// I took this course last semester so Cylinder and Cone may be somewhat familiar but I will link it if needed. I did change a lot though and was able to get it functioning better.
// Previous assignment if needed: https://github.com/mstrobeck-png/IGME-309/blob/main/A02/MyMesh.cpp
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	
	//creates top of the cylinder circle
	std::vector<vector3> circle1;

	//creates the bottom of the cylinder circle
	std::vector<vector3> circle2;

	//create walls; rectangles
	std::vector<vector3> walls;


	//taken from circle exercise
	GLfloat theta = 0;
	GLfloat delta = static_cast<GLfloat>(2.0 * PI / static_cast<GLfloat>(a_nSubdivisions));
	
	//sets height to a lower height, avoiding use of negatives in function vector circleTemp2
	float setHeight = -a_fHeight;

	//for the cicular aspect (bottom, top)
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		
		vector3 circleTemp1 = vector3(cos(theta) * a_fRadius, sin(theta) * a_fRadius, 0.0f);
		vector3 circleTemp2 = vector3(cos(theta) * a_fRadius, sin(theta) * a_fRadius, setHeight);

		theta += delta;

		circle1.push_back(circleTemp1);
		circle2.push_back(circleTemp2);
	}

	//create the top & the bottom circles; at this point we have two circles forming the top and bottom of the base
	int i = 0;
	while (i < a_nSubdivisions) {

		//creates first circle
		AddTri(ZERO_V3, circle1[(i + 1) % a_nSubdivisions], circle1[i]);

		//reversing the above function allows for us to see the circle on the opposite side
		AddTri(ZERO_V3, circle1[i],circle1[(i + 1) % a_nSubdivisions]);

		//creates second circle at lower height
		AddTri(vector3(0, 0, setHeight), circle2[(i + 1) % a_nSubdivisions], circle2[i]);

		//then doing the same form of reversal, swap values (y,z) to see circle on opposite side
		AddTri(vector3(0, 0, setHeight), circle2[i], circle2[(i + 1) % a_nSubdivisions]);
		

		//forms the walls from top to the bottom
	 
		//solution based around Maya model & https://open.gl/drawing explaining vertex, shapes, and geometry
		//walls appear to glitch out from time to time
		AddTri(circle1[(i + 1) % a_nSubdivisions], circle1[i], circle2[i]);

		//take the first half of previous function, apply to end but change circle1 to circle2
		AddTri(circle1[(i + 1) % a_nSubdivisions], circle2[i], circle2[(i + 1) % a_nSubdivisions]);

		//AddTri(circle1[(i + 1) % a_nSubdivisions], circle2[(i + 1) % a_nSubdivisions], circle2);
		i++;
	}


	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	
	//same circle code
	GLfloat theta = 0;
	GLfloat delta = static_cast<GLfloat>(2.0 * PI / static_cast<GLfloat>(a_nSubdivisions));

	//unlike most of my other functions, I gave this one  outer and inner type names since the shape was more complex and it allowed me to follow a small sketch I made
	// mapping out the possible path I could follow
	//need three or four vertex
	std::vector<vector3> outerRingTOP;
	std::vector<vector3> innerRingTOP;
	std::vector<vector3> outerRingBOTTOM;
	std::vector<vector3> innerRingBOTTOM;

	//sets height to negative value for lower part of shape
	float setHeightNeg = -a_fHeight;

	//to combine all the "for" functions together for the inital pushing, I combined them based on top and bottom in the same function
	for (int i = 0; i < a_nSubdivisions; i++)
	{
				
		//outer ring, top
		vector3 temp1 = vector3(cos(theta) * a_fOuterRadius, sin(theta) * a_fOuterRadius, a_fHeight);
		
		//inner ring, top
		vector3 temp2 = vector3(cos(theta) * a_fInnerRadius, sin(theta) * a_fInnerRadius, a_fHeight);
		

		//outer ring, bottom
		vector3 temp3 = vector3(cos(theta) * a_fOuterRadius, sin(theta) * a_fOuterRadius, setHeightNeg);

		//outer, bottom
		vector3 temp4 = vector3(cos(theta) * a_fInnerRadius, sin(theta) * a_fInnerRadius, setHeightNeg);
		
		theta += delta;

		outerRingTOP.push_back(temp1);
		innerRingTOP.push_back(temp2);
		
		outerRingBOTTOM.push_back(temp3);
		innerRingBOTTOM.push_back(temp4);
		
	}

	//follow same format as exercise
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//makes the top ring, front side - first set of triangles
		AddTri(outerRingTOP[(i + 1) % a_nSubdivisions], outerRingTOP[i], innerRingTOP[i]);
		//makes top of ring, backside - ensures you can see the shape from the front or back no matter which way it was turned
		AddTri(outerRingTOP[(i + 1) % a_nSubdivisions], innerRingTOP[i], outerRingTOP[i]);

		//makes the top front side - completes the circular shape with second set of triangles
		AddTri(innerRingTOP[(i + 1) % a_nSubdivisions], outerRingTOP[(i + 1) % a_nSubdivisions],innerRingTOP[i]);
		
		//makes the top backside - completes the second set of triangles
		AddTri(innerRingTOP[(i + 1) % a_nSubdivisions], innerRingTOP[i], outerRingTOP[(i + 1) % a_nSubdivisions]);

		//at this point we have the top ring done

		//makes the bottom ring
		//front side - first set triangles
		AddTri(outerRingBOTTOM[(i + 1) % a_nSubdivisions],  outerRingBOTTOM[i], innerRingBOTTOM[i]);
		//completes this side of ring
		AddTri(outerRingBOTTOM[(i + 1) % a_nSubdivisions], innerRingBOTTOM[i], outerRingBOTTOM[i]);
		
		//makes second side of ring
		AddTri(innerRingBOTTOM[(i + 1) % a_nSubdivisions], outerRingBOTTOM[(i + 1) % a_nSubdivisions], innerRingBOTTOM[i]);
		//completes rings
		AddTri(innerRingBOTTOM[(i + 1) % a_nSubdivisions], innerRingBOTTOM[i], outerRingBOTTOM[(i + 1) % a_nSubdivisions]);
	
		//by this point two rings exist, time to connect points


		//use vertex for walls? attempt cubes or keep tying to use triangles?
		//triangles appear easiest when looking at cube shape, connect rings from a to b using triangles, will need to use either 4 or 5 vertex values total
		
		//using excerise circle creation and the lecture from 2/16, this should work when thinking about it following what is given from professor's demos

		//creates outer walls, first set
		AddTri(outerRingBOTTOM[(i + 1) % a_nSubdivisions], outerRingTOP[i], outerRingBOTTOM[i]);

		//finishes outer walls
		AddTri(outerRingBOTTOM[(i + 1) % a_nSubdivisions], outerRingTOP[(i + 1) % a_nSubdivisions], outerRingTOP[i]);

		//creates inner walls, first set
		AddTri(innerRingBOTTOM[(i + 1) % a_nSubdivisions], innerRingTOP[i], innerRingTOP[(i + 1) % a_nSubdivisions]);

		//one set of interior walls not working? commented out
		//AddTri(innerRingBOTTOM[(i + 1) % a_nSubdivisions], innerRingBOTTOM[i], outerRingBOTTOM[i]);
		

	}

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 12;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 12;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	
	GLfloat theta = 0;
	GLfloat delta = static_cast<GLfloat>(2.0 * PI / static_cast<GLfloat>(a_nSubdivisionsB));

	//top of ring vertexs
	std::vector<vector3> vertex;
	std::vector<vector3> vertex2;
	std::vector<vector3> vertex3;
	std::vector<vector3> vertex4;

	//bottom
	std::vector<vector3> vertex5;


	//use subB so we can start at the base of the shape
	for (int i = 0; i < a_nSubdivisionsB; i++)
	{
		//values for the top of the ring
		vector3 temp = vector3(cos(theta) * a_fInnerRadius * 2, sin(theta) * a_fInnerRadius * 2, 0.10f);
		vector3 temp2 = vector3(cos(theta) * a_fOuterRadius * 2, sin(theta) * a_fOuterRadius * 2, 0.10f);
		vector3 temp3 = vector3(cos(theta) * a_fOuterRadius * 2, sin(theta) * a_fOuterRadius * 2, 0.10f);
		vector3 temp4 = vector3(cos(theta) * a_fInnerRadius * 2, sin(theta) * a_fInnerRadius * 2, -0.10f);
		
		//make bottom
		vector3 temp5 = vector3(cos(theta) * a_fInnerRadius * 2, sin(theta) * a_fInnerRadius * 2, 0.20f);

		theta += delta;

		vertex.push_back(temp);
		vertex2.push_back(temp2);
		vertex3.push_back(temp3);
		vertex4.push_back(temp4);

		vertex5.push_back(temp5);

	}

	//create the torus using subdivison b
	for (int i = 0; i < a_nSubdivisionsB; i++)
	{
		//bottom of the torus: SubdivisonB = bottom of top side
		AddTri(vertex2[i], vertex2[(i + 1) % a_nSubdivisionsB], vertex[i]);
		AddTri(vertex[i], vertex2[(i + 1) % a_nSubdivisionsB], vertex[(i + 1) % a_nSubdivisionsB]);
		
		//top: SubdivisionA = top side
		AddTri(vertex4[(i + 1) % a_nSubdivisionsA], vertex3[i], vertex4[i]);
		AddTri(vertex3[(i + 1) % a_nSubdivisionsA], vertex3[i], vertex4[(i + 1) % a_nSubdivisionsA]);

		//make bottom of torus
		AddTri(vertex3[i], vertex3[(i + 1) % a_nSubdivisionsB], vertex5[i]);
		AddTri(vertex5[i], vertex3[(i + 1) % a_nSubdivisionsB], vertex5[(i + 1) % a_nSubdivisionsB]);



	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// Replace this with your code
	std::vector<vector3> vertex;
	std::vector<vector3> vertex2;
	std::vector<vector3> vertex3;
	std::vector<vector3> vertex4;
	std::vector<vector3> vertex5;
	//std::vector<vector3> vertex6;
	GLfloat theta = 0;
	GLfloat delta = static_cast<GLfloat>(2.0 * PI / static_cast<GLfloat>(a_nSubdivisions));
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 temp = vector3(cos(theta) * a_fRadius, sin(theta) * a_fRadius, 0.0f);
		vector3 temp2 = vector3(cos(theta) * a_fRadius * 1.5, sin(theta) * a_fRadius * 1.5, 0.5f);
		vector3 temp3 = vector3(cos(theta) * a_fRadius, sin(theta) * a_fRadius, 1.0f);

		vector3 temp4 = vector3(cos(theta) * a_fRadius / 2, sin(theta) * a_fRadius / 2, -0.2f);
		vector3 temp5 = vector3(cos(theta) * a_fRadius / 2, sin(theta) * a_fRadius / 2, 1.2f);

		theta += delta;
		vertex.push_back(temp);
		vertex2.push_back(temp2);
		vertex3.push_back(temp3);

		vertex4.push_back(temp4);
		vertex5.push_back(temp5);
	}

	for (int i = 0; i < a_nSubdivisions; i++) {
	//top of sphere
	AddTri(ZERO_V3, vertex[(i + 1) % a_nSubdivisions], vertex[i]);
	AddTri(ZERO_V3, vertex[i], vertex[(i + 1) % a_nSubdivisions]);

	//middle section of sphere - make larger by multiplying 1.5 in temp2 function
	AddTri(vector3(0.0f, 0.0f, 0.5f), vertex2[i], vertex2[(i + 1) % a_nSubdivisions]);
	AddTri(vector3(0.0f, 0.0f, 0.5f), vertex2[(i + 1) % a_nSubdivisions], vertex2[i]);

	//bottom of sphere
	AddTri(vector3(0.0f, 0.0f, 1.0f), vertex3[i], vertex3[(i + 1) % a_nSubdivisions]);
	AddTri(vector3(0.0f, 0.0f, 1.0f), vertex3[(i + 1) % a_nSubdivisions], vertex3[i]);


	//making the walls to connect the vertex, connect 
	AddTri(vertex[(i + 1) % a_nSubdivisions], vertex2[i], vertex[i]);
	AddTri(vertex[(i + 1) % a_nSubdivisions], vertex2[i], vertex2[(i + 1) % a_nSubdivisions]);

	AddTri(vertex2[(i + 1) % a_nSubdivisions], vertex[i], vertex2[i]);
	AddTri(vertex2[(i + 1) % a_nSubdivisions], vertex[i], vertex[(i + 1) % a_nSubdivisions]);


	//second set of walls
	AddTri(vertex2[(i + 1) % a_nSubdivisions], vertex3[i], vertex2[i]);
	AddTri(vertex2[(i + 1) % a_nSubdivisions], vertex3[i], vertex3[(i + 1) % a_nSubdivisions]);

	AddTri(vertex2[(i + 1) % a_nSubdivisions], vertex3[i], vertex2[i]);
	AddTri(vertex2[(i + 1) % a_nSubdivisions], vertex3[i], vertex3[(i + 1) % a_nSubdivisions]);


	//the plan was to create walls to make a sphere shape and then add new and smaller circles to help round the shape out in form
	//but it started to become tedious with vertexs


	//new top - smallest
	AddTri(vector3(0.0f, 0.0f, -0.3f), vertex4[(i + 1) % a_nSubdivisions], vertex4[i]);
	AddTri(vector3(0.0f, 0.0f, -0.3f), vertex4[i], vertex4[(i + 1) % a_nSubdivisions]);

	//walls
	AddTri(vertex[(i + 1) % a_nSubdivisions], vertex4[i], vertex[i]);
	AddTri(vertex[(i + 1) % a_nSubdivisions], vertex4[i], vertex4[(i + 1) % a_nSubdivisions]);

	AddTri(vertex4[(i + 1) % a_nSubdivisions], vertex[i], vertex4[i]);
	AddTri(vertex4[(i + 1) % a_nSubdivisions], vertex[i], vertex[(i + 1) % a_nSubdivisions]);

	//new bottom
	AddTri(vector3(0.0f, 0.0f, 1.3f), vertex5[(i + 1) % a_nSubdivisions], vertex5[i]);
	AddTri(vector3(0.0f, 0.0f, 1.3f), vertex5[i], vertex5[(i + 1) % a_nSubdivisions]);

	//bottom walls

	AddTri(vertex3[(i + 1) % a_nSubdivisions], vertex5[i], vertex3[i]);
	AddTri(vertex3[(i + 1) % a_nSubdivisions], vertex5[i], vertex5[(i + 1) % a_nSubdivisions]);

	AddTri(vertex5[(i + 1) % a_nSubdivisions], vertex3[i], vertex5[i]);
	AddTri(vertex5[(i + 1) % a_nSubdivisions], vertex3[i], vertex3[(i + 1) % a_nSubdivisions]);

}



	


	//GenerateCube(a_fRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}