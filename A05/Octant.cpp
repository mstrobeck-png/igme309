#include "Octant.h"
using namespace BTX;
//  Octant
uint Octant::m_uOctantCount = 0;
uint Octant::m_uMaxLevel = 3;
uint Octant::m_uIdealEntityCount = 5;


Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{	
	
	//set the inital values that will be used while the program runs
	//this section only to be used on root node, taking a size, and taking a center to create new space
	
	Init();
	m_uOctantCount = 0; //will store the number of octants instantiated
	m_uMaxLevel = a_nMaxLevel;//will store the maximum level an octant can go to
	m_uIdealEntityCount = a_nIdealEntityCount; //will tell how many Entities this object will contain ideally (may have more or less)
	m_uID = m_uOctantCount;

	m_pRoot = this;
	m_lChild.clear();

	//we need a rigid body and a list to keep track of values

	//create a list
	std::vector<vector3> listMaximum;

	//create a rigid body
	RigidBody* ptr_rigidbody;


	//based on the blog https://anteru.net/blog/2008/loose-octrees/ for loose octrees, we should look into creating an object list
	//to help keep track of the largest possible values in each display

	//create a maximum value to keep calling for the largest possible values
	vector3 v3_maximum;

	//the object list to keep track of maximum values in the tree
	int i = 0;
	while (i < m_pEntityMngr->GetEntityCount()) {
		//while looking for the total entity count, we can call for the rigid bodies of those entities from the entity manager
		ptr_rigidbody = m_pEntityMngr->GetEntity(i)->GetRigidBody();

		//set the v3_maximum to the function in Octant.h, GetMaxGlobal() so we can keep calling to check it for the maximum global
		v3_maximum = ptr_rigidbody->GetMaxGlobal();

		//to keep track of the largest in the list, we need to push_back the maximum value
		listMaximum.push_back(v3_maximum);
		i++;
	}
	
	//now that we have the maximum list and the rigid bodies from those values, we can start identifying
	//the maximum values for the octant as well as set the rigid bodies of the maxmimum list equal to a new rigid body
	//attached to our pointer ptr_rigidbodies

	ptr_rigidbody = new RigidBody(listMaximum);

	//in order to utlize the size, center, min, and max values we need to run through to find out if 
	//the size is less than or equal to the halfwidth. Without this check, it does not give us the yellow box
	//or the subdivisions

	//The following will set up the values of the octant, make sure the are right, the rigid body at start
	m_fSize = ptr_rigidbody->GetHalfWidth().x * 2;
	m_v3Center = ptr_rigidbody->GetCenterLocal();

	//to make the min and max values work, we need to use the vector3 of the m_fSize instead of the rigidbody halfwidths as this doesnt affect anything properly
	m_v3Min = m_v3Center - vector3(m_fSize);
	m_v3Max = m_v3Center + vector3(m_fSize);
	
	m_uOctantCount++; //When we add an octant we increment the count
	ConstructTree(m_uMaxLevel); //Construct the children
}
bool Octant::IsColliding(uint a_uRBIndex)
{
	//Get how many objects there are in the world
	//If the index given is larger than the number of elements in the bounding object there is no collision
	//As the Octree will never rotate or scale this collision is as easy as an Axis Alligned Bounding Box
	//Get all vectors in global space (the octant ones are already in Global)
	
	//set the rigid body we want to check to ensure min and max points align in the bounding box 
	RigidBody* ptr_rigidbody = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody();


	//referred to: https://www.kitware.com/octree-collision-imstk/ where i found the idea of a "loose octree" and through further reading, found out we need to compare the min and max values
	//therefore based on the Octant.h file, we should use the v3_max and v3_min against the global
	int i = 0;
	while (i < a_uRBIndex) {
		//compare the current min and max of values against the global to determine if a collison occurs or not
		//to optimize function, we use an || or an "OR" operator to use less space and return quicker results: https://eklitzke.org/stl-iterators-and-performance-revisited; helped me understand how to improve function i have blocked out below this
		if (m_v3Max.x < ptr_rigidbody->GetMinGlobal().x || m_v3Min.x > ptr_rigidbody->GetMaxGlobal().x) {
			return false; //no collison on this axis
		}
		if (m_v3Max.y < ptr_rigidbody->GetMinGlobal().y || m_v3Min.y > ptr_rigidbody->GetMaxGlobal().y) {
			return false; //no collison on this axis
		}
		if (m_v3Max.z < ptr_rigidbody->GetMinGlobal().z || m_v3Min.z > ptr_rigidbody->GetMaxGlobal().z) {
			return false; //no collison on this axis
		}
		//collision is found if true
		else {
			return true;
		}
		i++;
	}

	/*

	//this function slows down the process drastically, making it reach about 30 seconds to render
	//int i = 0;
	while (i < 1) {
		//check the maxs
		if (m_v3Max.x < ptr_rigidbody->GetMinGlobal().x) {
			return false;
		}
		if (m_v3Max.y < ptr_rigidbody->GetMinGlobal().y) {
			return false;
		}
		if (m_v3Max.z < ptr_rigidbody->GetMinGlobal().z) {
			return false;
		}
		else {
			return true;
		}

		//check the mins 
		if (m_v3Min.x > ptr_rigidbody->GetMaxGlobal().x) {
			return false;
		}
		if (m_v3Min.y > ptr_rigidbody->GetMaxGlobal().y) {
			return false;
		}
		if (m_v3Min.z > ptr_rigidbody->GetMaxGlobal().z) {
			return false;
		}
		i++;
	}
	
	*/

	//return true; // for the sake of startup code
}
void Octant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// Display the specified octant
	int i = 0; //start at 0, cycle through each one
	while (i < m_uChildren) {
		//for every child we cycle through, display the color that is assigned by a_v3color
		m_pChild[i]->Display(a_v3Color); //the assigned color to the child object
		i++;
	}

	//I tried doing this with a for loop and also an if statement but for some reason my computer prefered the "while" statement; it ran strange but I think it is just because my computer is not great
}
void Octant::Display(vector3 a_v3Color)
{
	//this is meant to be a recursive method, in starter code will only display the root
	//repeat the function from Display to obtain color
	int i = 0;
	while (i < m_uChildren) {
		//for every child we cycle through, display the yellow
		m_pChild[i]->Display(a_v3Color); //by default this color should be the yellow
		i++;
	}

	// render the wire cube
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
}


void Octant::Subdivide(void)
{

	//If this node has reach the maximum depth return without changes
	if (m_uLevel >= m_uMaxLevel)
		return;

	//If this node has been already subdivided return without changes
	if (m_uChildren != 0)
		return;

	//Subdivide the space and allocate 8 children
	//the number of children based on the rubric, video, and other areas of the code is eight so set the number of children to 8.
	m_uChildren = 8; //setting it equal will help us fill an array for each child and create a new octant


	//creating the children
	//all children will follow the same sort of function with minor changes based on: https://en.wikipedia.org/wiki/Octant_(solid_geometry) I used this to sort of figure out where to add or subtract values and also where to find certain points; but most importantly I used the Octant.h to manually figure it out on paper
	//to reamin consistent with one another, the division with 4 allows for the most accurate change in values, anything more or less does not do what is needed well
	m_pChild[0] = new Octant(vector3(m_v3Center.x - (m_fSize / 4), m_v3Center.y + (m_fSize / 4), m_v3Center.z + (m_fSize / 4)), m_fSize / 2); 
	m_pChild[1] = new Octant(vector3(m_v3Center.x - (m_fSize / 4), m_v3Center.y + (m_fSize / 4), m_v3Center.z - (m_fSize / 4)), m_fSize / 2);

	m_pChild[2] = new Octant(vector3(m_v3Center.x - (m_fSize / 4), m_v3Center.y - (m_fSize / 4), m_v3Center.z + (m_fSize / 4)), m_fSize / 2);
	m_pChild[3] = new Octant(vector3(m_v3Center.x - (m_fSize / 4), m_v3Center.y - (m_fSize / 4), m_v3Center.z - (m_fSize / 4)), m_fSize / 2);

	
	m_pChild[4] = new Octant(vector3(m_v3Center.x + (m_fSize / 4), m_v3Center.y + (m_fSize / 4), m_v3Center.z + (m_fSize / 4)), m_fSize / 2);
	m_pChild[5] = (new Octant(vector3(m_v3Center.x + (m_fSize / 4), m_v3Center.y + (m_fSize / 4), m_v3Center.z - (m_fSize / 4)), m_fSize / 2));

	m_pChild[6] = new Octant(vector3(m_v3Center.x + (m_fSize / 4), m_v3Center.y - (m_fSize / 4), m_v3Center.z + (m_fSize / 4)), m_fSize / 2);
	m_pChild[7] = new Octant(vector3(m_v3Center.x + (m_fSize / 4), m_v3Center.y - (m_fSize / 4), m_v3Center.z - (m_fSize / 4)), m_fSize / 2);


	//create a while loop to cycle through the roots, parents, and the level system 
	int i = 0;
	while (m_uChildren > i) {
		//need to use the child object with the root variable, parent, and ulevel; set them equal to the current root, parent, level
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this; 

		//increase the level by 1
		m_pChild[i]->m_uLevel = m_uLevel + 1;

		/*
		 * static uint m_uIdealEntityCount; //will tell how many Entities this object will contain ideally (may have more or less)
		 * given this will tell us how many entities are ideal for an object, we could use ContainsAtLeast to set it equal to the ideal amount of entities
		 */

		if (m_pChild[i]->ContainsAtLeast(m_uIdealEntityCount)) {
			//once we know the ideal value, if there is too many, we will subdivide
			m_pChild[i]->Subdivide();
		}
		i++;
	}
}


bool Octant::ContainsAtLeast(uint a_nEntities)
{
	//You need to check how many entity objects live within this octant
	
	//create an int to keep track of entities
	int trackEntities = 0; //currently zero

	//loop through the entities and check their collision with one another
	int i = 0;
	//run through the entity manager to find how many entities there are
	while (i < m_pEntityMngr->GetEntityCount()) {
		//check the collison of the entities found, then add the entities into trackEntities
		if (IsColliding(i)) {
			trackEntities++;
			//if our tracked entities are equal to the number of entities in the maanger, then return true and finish the statement
			if (trackEntities >= a_nEntities) {
				return true;
			}
		}
		//I got the idea for this while loop when looking over the professor's c19_entity and c20_entityManager
		i++;
	}	
	//return false; //return something for the sake of start up code
}
void Octant::AssignIDtoEntity(void)
{

	//Recursive method
	//Have to traverse the tree and make sure to tell the entity manager
	//take the first entity and tell it its on this space (m_pEntityMngr->AddDimension(i, m_uID);)
	//what octant (space) each object is at -> this tells us we need to use the adddimension

	int i = 0;
	while (m_pEntityMngr->GetEntityCount() > i) {
		//take the pre-existing function and apply it here to cycle through
		//this will help us add an identity to the octant
		m_pEntityMngr->AddDimension(i, m_uID); //"space" this means we look into dimensional placement
		m_EntityList.push_back(i); //push the dimension location into the entity list we have
		i++;
	}
}



//-------------------------------------------------------------------------------------------------------------------
// You can assume the following is fine and does not need changes, you may add onto it but the code is fine as is
// in the proposed solution.
void Octant::Init(void)
{
	m_uChildren = 0;

	m_fSize = 0.0f;

	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (uint n = 0; n < 8; n++)
	{
		m_pChild[n] = nullptr;
	}
}
void Octant::Swap(Octant& other)
{
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);
	for (uint i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}
void Octant::Release(void)
{
	//this is a special kind of release, it will only happen for the root
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}
void Octant::ConstructTree(uint a_nMaxLevel)
{
	//If this method is tried to be applied to something else
	//other than the root, don't.
	if (m_uLevel != 0)
		return;

	m_uMaxLevel = a_nMaxLevel; //Make sure you mark which is the maximum level you are willing to reach
	m_uOctantCount = 1;// We will always start with one octant

	//If this was initialized before make sure its empty
	m_EntityList.clear();//Make sure the list of entities inside this octant is empty
	KillBranches();
	m_lChild.clear();

	//If we have more entities than those that we ideally want in this octant we subdivide it
	if (ContainsAtLeast(m_uIdealEntityCount))
	{
		Subdivide();
	}
	AssignIDtoEntity();//Add octant ID to Entities
	ConstructList();//construct the list of objects
}
//The big 3
Octant::Octant(vector3 a_v3Center, float a_fSize)
{
	//Will create the octant object based on the center and size but will not construct children
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	m_uOctantCount++;
}
Octant::Octant(Octant const& other)
{
	m_uChildren = other.m_uChildren;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_pParent = other.m_pParent;

	m_pRoot, other.m_pRoot;
	m_lChild, other.m_lChild;

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}
Octant& Octant::operator=(Octant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		Octant temp(other);
		Swap(temp);
	}
	return *this;
}
Octant::~Octant() { Release(); };
//Accessors
float Octant::GetSize(void) { return m_fSize; }
vector3 Octant::GetCenterGlobal(void) { return m_v3Center; }
vector3 Octant::GetMinGlobal(void) { return m_v3Min; }
vector3 Octant::GetMaxGlobal(void) { return m_v3Max; }
uint Octant::GetOctantCount(void) { return m_uOctantCount; }
bool Octant::IsLeaf(void) { return m_uChildren == 0; }
Octant* Octant::GetParent(void) { return m_pParent; }
//--- other methods
Octant * Octant::GetChild(uint a_nChild)
{
	//if its asking for more than the 8th children return nullptr, as we don't have one
	if (a_nChild > 7) return nullptr;
	return m_pChild[a_nChild];
}
void Octant::KillBranches(void)
{
	/*This method has recursive behavior that is somewhat hard to explain line by line but
	it will traverse the whole tree until it reaches a node with no children and
	once it returns from it to its parent it will kill all of its children, recursively until
	it reaches back the node that called it.*/
	for (uint nIndex = 0; nIndex < m_uChildren; nIndex++)
	{
		m_pChild[nIndex]->KillBranches();
		delete m_pChild[nIndex];
		m_pChild[nIndex] = nullptr;
	}
	m_uChildren = 0;
}
void Octant::DisplayLeaves(vector3 a_v3Color)
{
	/*
	* Recursive method
	* it will traverse the tree until it find leaves and once found will draw them
	*/
	uint nLeafs = m_lChild.size(); //get how many children this will have (either 0 or 8)
	for (uint nChild = 0; nChild < nLeafs; nChild++)
	{
		m_lChild[nChild]->DisplayLeaves(a_v3Color);
	}
	//Draw the cube
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color);
}
void Octant::ClearEntityList(void)
{
	/*
	* Recursive method
	* It will traverse the tree until it find leaves and once found will clear its content
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ClearEntityList();
	}
	m_EntityList.clear();
}
void Octant::ConstructList(void)
{
	/*
	* Recursive method
	* It will traverse the tree adding children
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ConstructList();
	}
	//if we find a non-empty child add it to the tree
	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
}
