#include "MyRigidBody.h"
using namespace BTX;
//Allocation

//this function
uint MyRigidBody::SAT(MyRigidBody* const a_pOther)
{
	//TODO: Calculate the SAT algorithm I STRONGLY suggest you use the
	//Real Time Collision detection algorithm for OBB here but feel free to
	//implement your own solution.
	
	/*
	based on the textbook, start with an x, y, z and to hold the rotational values
	* Needs three values, so three objects array, two functions
	*/

	vector3 axOne[] = {
		m_m4ToWorld[0], m_m4ToWorld[1], m_m4ToWorld[2]
	};

	//create the coordinate values that go along the different axises
	//we need the matrix function to take us to the world coordinate system
	//axOne = m_m4ToWorld[0], m_m4ToWorld[1], m_m4ToWorld[2];

	/*axis two will need the same, but does not work calling the same function of
	axTwo = m_m4ToWorld[0], m_m4ToWorld[1], m_m4ToWorld[2]

	therefore we need to use "this" in some form to state we are referring to this one function only;
	*/
	vector3 axTwo[] = {
		 a_pOther->m_m4ToWorld[0],  a_pOther->m_m4ToWorld[1],  a_pOther->m_m4ToWorld[2] //"this->" causes a lack of collison detection half an inch in to other object, use use: "const a_pOther" since it is referenced in collision code
	};

	/*
	create a frame to measure the coordinates of the matrix; use dot product rule
	we need a matrix to multiple our vectors in
	*/

	matrix3 m_translation;

	/*
	we need to run through two different axises and use dot product, so we need two different statments or nested loops
	*/

	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++)
		{
			//https://openframeworks.cc//documentation/glm/detail_func_geometric/#!show_glm::dot
			m_translation[a][b] = glm::dot(axOne[a], axTwo[b]);
			//m_translation[a][b] = axOne[a] * axTwo[b];
		}
	}

	/*
	Create a vector for the translations and math that need to be calculated for the world coordinates and the different axises
	*/

	vector3 translations;

	//create vectors associated with functions to keep an eye on the math without clutter

	/*
	take a_pOther, looks like we need to multiple it by the center point based on the textbook's chapter

	vector3 t_partOne = m_v3Center * a_pOther->m_m4ToWorld; //error with the * key, so

	vector3 t_partOne = vector3(m_v3Center) * a_pOther->m_m4ToWorld; //still an error

	vector3 t_partOne = vector3(m_v3Center, 1.0f) * a_pOther->m_m4ToWorld //issue with vector3 and *
	*/
	 
	
	//Solution to error was to use a vector4 because of matrix
	vector3 t_partOne =a_pOther->m_m4ToWorld * vector4(m_v3Center, 1.0f); //must use 1.0f, 0.5f is creates detection too quick, 1.5 is too slow/half way into object detects collison
	vector3 t_partTwo = m_m4ToWorld * vector4(m_v3Center, 1.0f); //similiar notes here as above


	translations = vector3(t_partTwo - t_partOne);

	//check the nearest point at the absolute value of the translation
	matrix3 m_near;
	for (int i = 0; i < 3; i++)
	{
		//for the value of i and j, find those points in the matrix of m_translation and set equal to m_near at the same points for the new matrix
		for (int j = 0; j < 3; j++)
		{
			//https://openframeworks.cc//documentation/glm/detail_func_common/#!show_glm::abs 
			m_near[i][j] = glm::abs(m_translation[i][j]);
		}
	}

	 
	//calculating the intersection points, create another matrix for the nearby values and then float to hold points and do calcuations
	float calc, a, b;



	//Calculations: take the halfwidth with its corrosponding location and multiply by the matrix we fill above
	//all math is based on matrix, textbook, and any attachments I may include (I drew the matrix out on paper to find answers)

	/*
	Based on the textbook, we need to focus on x,y,z coordinates
	*/


	//Section: a0 and b0
	float a1 = m_near[1][0] * m_v3HalfWidth.z; 
	float a2 = m_near[2][0] * m_v3HalfWidth.y; 

	a = a1 + a2;

	//repeat the above function onto the "a_pOther"  but look at neighboring points
	float b1 = m_near[0][1] * a_pOther->m_v3HalfWidth.z; 
	float b2 = m_near[0][2] * a_pOther->m_v3HalfWidth.y; 

	b = b1 + b2;

	//test for collisions at this point
	//testing matrix locations of translations and m_translations so we can determine the difference
	float testOne = m_translation[1][0] * translations[2];
	float testTwo = m_translation[2][0] * translations[1];

	//if the value is greater than what we get when adding a and b, we need to state there is a collision
	if (testOne - testTwo > a + b)
	{
		return true; //we found a collision
	}



	//Section: a0 and b1
	a1 = m_near[1][1] * m_v3HalfWidth.z; 
	a2 = m_near[2][1] * m_v3HalfWidth.y; 

	a = a1 + a2;

	//repeat the above function onto the "a_pOther" but look at neighboring points
	b1 = m_near[0][0] * a_pOther->m_v3HalfWidth.z; 
	b2 = m_near[0][2] * a_pOther->m_v3HalfWidth.x; 

	b = b1 + b2;

	//test for collisions at this point
	//testing matrix locations of translations and m_translations so we can determine the difference
	testOne = m_translation[1][1] * translations[2];
	testTwo = m_translation[2][1] * translations[1];

	//if the value is greater than what we get when adding a and b, we need to state there is a collision
	if (testOne - testTwo > a + b)
	{
		return true; //we found a collision
	}



	//a0 and b2
	a1 = m_near[1][2] * m_v3HalfWidth.z; 
	a2 = m_near[2][2] * m_v3HalfWidth.y; 

	a = a1 + a2;

	//repeat the above function onto the "a_pOther" but look at neighboring points
	b1 = m_near[0][0] * a_pOther->m_v3HalfWidth.y; 
	b2 = m_near[0][1] * a_pOther->m_v3HalfWidth.x; 

	b = b1 + b2;

	//test for collisions at this point
	//testing matrix locations of translations and m_translations so we can determine the difference
	testOne = m_translation[1][0] * translations[2];
	testTwo = m_translation[2][0] * translations[1];

	//if the value is greater than what we get when adding a and b, we need to state there is a collision
	if (testOne - testTwo > a + b)
	{
		return true; //we found a collision
	}



	//a1 and b0
	a1 = m_near[0][0] * m_v3HalfWidth.z;
	a2 = m_near[2][0] * m_v3HalfWidth.x;

	a = a1 + a2;

	//repeat the above function onto the "a_pOther" but look at neighboring points
	b1 = m_near[1][1] * a_pOther->m_v3HalfWidth.z;
	b2 = m_near[1][2] * a_pOther->m_v3HalfWidth.y;

	b = b1 + b2;

	//test for collisions at this point
	//testing matrix locations of translations and m_translations so we can determine the difference
	testOne = m_translation[2][0] * translations[0]; 
	testTwo = m_translation[0][0] * translations[2];

	//if the value is greater than what we get when adding a and b, we need to state there is a collision
	if (testOne - testTwo > a + b)
	{
		return true; //we found a collision
	}



	//a1 and b1
	a1 = m_near[0][1] * m_v3HalfWidth.z;
	a2 = m_near[2][1] * m_v3HalfWidth.x;

	a = a1 + a2;

	b1 = m_near[1][0] * a_pOther->m_v3HalfWidth.z;
	b2 = m_near[1][2] * a_pOther->m_v3HalfWidth.x;

	b = b1 + b2;

	//test for collisions at this point
	//testing matrix locations of translations and m_translations so we can determine the difference
	testOne = m_translation[0][1] * translations[2];
	testTwo = m_translation[2][1] * translations[0];

	//if the value is greater than what we get when adding a and b, we need to state there is a collision
	if (testOne - testTwo > a + b)
	{
		return true; //we found a collision
	}



	//a1 and b2
	a1 = m_near[0][2] * m_v3HalfWidth.z;
	a2 = m_near[2][2] * m_v3HalfWidth.x;

	a = a1 + a2;

	b1 = m_near[1][0] * a_pOther->m_v3HalfWidth.y;
	b2 = m_near[1][1] * a_pOther->m_v3HalfWidth.x;

	b = b1 + b2;

	//test for collisions at this point
	//testing matrix locations of translations and m_translations so we can determine the difference
	testOne = m_translation[0][2] * translations[2];
	testTwo = m_translation[2][2] * translations[0];

	//if the value is greater than what we get when adding a and b, we need to state there is a collision
	if (testOne - testTwo > a + b)
	{
		return true; //we found a collision
	}



	//a2 and b0
	a1 = m_near[0][0] * m_v3HalfWidth.y;
	a2 = m_near[1][0] * m_v3HalfWidth.x;

	a = a1 + a2;

	b1 = m_near[2][1] * a_pOther->m_v3HalfWidth.z;
	b2 = m_near[2][2] * a_pOther->m_v3HalfWidth.y;

	b = b1 + b2;

	//test for collisions at this point
	//testing matrix locations of translations and m_translations so we can determine the difference
	testOne = m_translation[1][0] * translations[0];
	testTwo = m_translation[0][0] * translations[1];

	//if the value is greater than what we get when adding a and b, we need to state there is a collision
	if (glm::abs(testOne - testTwo) > a + b) //need to use absolute here or else it stays in a state of collision detected
	{
		return true; //we found a collision
	}




	//a2 and b1
	a1 = m_near[0][1] * m_v3HalfWidth.y;
	a2 = m_near[1][1] * m_v3HalfWidth.x;

	a = a1 + a2;

	b1 = m_near[2][0] * a_pOther->m_v3HalfWidth.z;
	b2 = m_near[2][2] * a_pOther->m_v3HalfWidth.x;

	b = b1 + b2;

	//test for collisions at this point
	//testing matrix locations of translations and m_translations so we can determine the difference
	testOne = m_translation[1][1] * translations[0];
	testTwo = m_translation[0][1] * translations[1];

	//if the value is greater than what we get when adding a and b, we need to state there is a collision
	if (glm::abs(testOne - testTwo) > a + b) //need to use absolute here or else it stays in a state of collision detected
	{
		return true; //we found a collision
	}



	//a2 and b2
	a1 = m_near[0][2] * m_v3HalfWidth.y;
	a2 = m_near[1][2] * m_v3HalfWidth.x;

	a = a1 + a2;

	b1 = m_near[2][0] * a_pOther->m_v3HalfWidth.y;
	b2 = m_near[2][1] * a_pOther->m_v3HalfWidth.x;

	b = b1 + b2;

	//test for collisions at this point
	//testing matrix locations of translations and m_translations so we can determine the difference
	testOne = m_translation[1][2] * translations[0];
	testTwo = m_translation[0][2] * translations[1];

	//if the value is greater than what we get when adding a and b, we need to state there is a collision
	if (glm::abs(testOne - testTwo) > a + b) //need to use absolute here or else it stays in a state of collision detected
	{
		return true; //we found a collision
	}


	//if there is no form of collision being detected then return 0 or false
	return false;
}


bool MyRigidBody::IsColliding(MyRigidBody* const a_pOther)
{
	//check if spheres are colliding
	bool bColliding = true;
	
	/*
	* We use Bounding Spheres or ARBB as a pre-test to avoid expensive calculations (SAT)
	* we default bColliding to true here to always fall in the need of calculating
	* SAT for the sake of the assignment.
	*/

	if (bColliding) //they are colliding with bounding sphere
	{
		uint nResult = SAT(a_pOther);

		//NEWLY ADDED
		if (nResult != 0)
		{
			// reset to false
			bColliding = false;
		}
		if (bColliding) //The SAT shown they are colliding
		{
			this->AddCollisionWith(a_pOther);
			a_pOther->AddCollisionWith(this);
		}
		else //they are not colliding
		{
			this->RemoveCollisionWith(a_pOther);
			a_pOther->RemoveCollisionWith(this);
		}

	}
	else //they are not colliding with bounding sphere
	{
		this->RemoveCollisionWith(a_pOther);
		a_pOther->RemoveCollisionWith(this);
	}
	return bColliding;
}
void MyRigidBody::Init(void)
{
	m_pModelMngr = ModelManager::GetInstance();
	m_bVisibleBS = false;
	m_bVisibleOBB = true;
	m_bVisibleARBB = false;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& a_pOther)
{
	std::swap(m_pModelMngr, a_pOther.m_pModelMngr);
	std::swap(m_bVisibleBS, a_pOther.m_bVisibleBS);
	std::swap(m_bVisibleOBB, a_pOther.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, a_pOther.m_bVisibleARBB);

	std::swap(m_fRadius, a_pOther.m_fRadius);

	std::swap(m_v3ColorColliding, a_pOther.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, a_pOther.m_v3ColorNotColliding);

	std::swap(m_v3Center, a_pOther.m_v3Center);
	std::swap(m_v3MinL, a_pOther.m_v3MinL);
	std::swap(m_v3MaxL, a_pOther.m_v3MaxL);

	std::swap(m_v3MinG, a_pOther.m_v3MinG);
	std::swap(m_v3MaxG, a_pOther.m_v3MaxG);

	std::swap(m_v3HalfWidth, a_pOther.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, a_pOther.m_v3ARBBSize);

	std::swap(m_m4ToWorld, a_pOther.m_m4ToWorld);

	std::swap(m_CollidingRBSet, a_pOther.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pModelMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	//Assign the model matrix
	m_m4ToWorld = a_m4ModelMatrix;

	//Calculate the 8 corners of the cube
	vector3 v3Corner[8];
	//Back square
	v3Corner[0] = m_v3MinL;
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3Corner[7] = m_v3MaxL;

	//Place them in world space
	for (uint uIndex = 0; uIndex < 8; ++uIndex)
	{
		v3Corner[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
	}

	//Identify the max and min as the first corner
	m_v3MaxG = m_v3MinG = v3Corner[0];

	//get the new max and min for the global box
	for (uint i = 1; i < 8; ++i)
	{
		if (m_v3MaxG.x < v3Corner[i].x) m_v3MaxG.x = v3Corner[i].x;
		else if (m_v3MinG.x > v3Corner[i].x) m_v3MinG.x = v3Corner[i].x;

		if (m_v3MaxG.y < v3Corner[i].y) m_v3MaxG.y = v3Corner[i].y;
		else if (m_v3MinG.y > v3Corner[i].y) m_v3MinG.y = v3Corner[i].y;

		if (m_v3MaxG.z < v3Corner[i].z) m_v3MaxG.z = v3Corner[i].z;
		else if (m_v3MinG.z > v3Corner[i].z) m_v3MinG.z = v3Corner[i].z;
	}

	//we calculate the distance between min and max vectors
	m_v3ARBBSize = m_v3MaxG - m_v3MinG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//with the max and the min we calculate the center
	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//Get the distance between the center and either the min or the max
	m_fRadius = glm::distance(m_v3Center, m_v3MinL);
}
MyRigidBody::MyRigidBody(MyRigidBody const& a_pOther)
{
	m_pModelMngr = a_pOther.m_pModelMngr;

	m_bVisibleBS = a_pOther.m_bVisibleBS;
	m_bVisibleOBB = a_pOther.m_bVisibleOBB;
	m_bVisibleARBB = a_pOther.m_bVisibleARBB;

	m_fRadius = a_pOther.m_fRadius;

	m_v3ColorColliding = a_pOther.m_v3ColorColliding;
	m_v3ColorNotColliding = a_pOther.m_v3ColorNotColliding;

	m_v3Center = a_pOther.m_v3Center;
	m_v3MinL = a_pOther.m_v3MinL;
	m_v3MaxL = a_pOther.m_v3MaxL;

	m_v3MinG = a_pOther.m_v3MinG;
	m_v3MaxG = a_pOther.m_v3MaxG;

	m_v3HalfWidth = a_pOther.m_v3HalfWidth;
	m_v3ARBBSize = a_pOther.m_v3ARBBSize;

	m_m4ToWorld = a_pOther.m_m4ToWorld;

	m_CollidingRBSet = a_pOther.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& a_pOther)
{
	if (this != &a_pOther)
	{
		Release();
		Init();
		MyRigidBody temp(a_pOther);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };

//--- a_pOther Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* a_pOther)
{

	/*check if the object is already in the colliding set, if
		the object is already there return with no changes */
	
	auto element = m_CollidingRBSet.find(a_pOther);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn'translations find the object so add it
	m_CollidingRBSet.insert(a_pOther);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* a_pOther)
{
	m_CollidingRBSet.erase(a_pOther);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}

void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pModelMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		else
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
		else
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
	}
}

