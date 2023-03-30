#include "MyCamera.h"
using namespace BTX;
//  MyCamera
void MyCamera::SetPositionTargetAndUpward(vector3 a_v3Position, vector3 a_v3Target, vector3 a_v3Upward)
{
	//TODO:: replace the super call with your functionality
	//Tip: Changing any positional vector forces you to calculate new directional ones
	super::SetPositionTargetAndUpward(a_v3Position, a_v3Target, a_v3Upward);

	//After changing any vectors you need to recalculate the MyCamera View matrix.
	//While this is executed within the parent call above, when you remove that line
	//you will still need to call it at the end of this method
	CalculateView();
}

/* ALL OTHER RESOURCES USED ARE CLASS LECTURES UNLESS DOCUMENTED ELSEWHERE */

//I took this course last semester and it was the last homework I did before withdrawing. It was never graded and I had made a lot of mistakes beforehand.
//That reference can be found at: https://github.com/mstrobeck-png/IGME-309/blob/main/A03_Camera/MyCamera.cpp it even states I last touched it October 31st, 2022.
void MyCamera::MoveForward(float a_fDistance)
{
	//Tips:: Moving will modify both positional and directional vectors,
	//		 here we only modify the positional.
	//       The code below "works" because we wrongly assume the forward 
	//		 vector is going in the global -Z but if you look at the demo 
	//		 in the _Binary folder you will notice that we are moving 
	//		 backwards and we never get closer to the plane as we should 
	//		 because as we are looking directly at it.

	//This section needed to be changed based on rubric because world space vs camera space
	//m_v3Position += vector3(0.0f, 0.0f, -a_fDistance);
	//m_v3Target += vector3(0.0f, 0.0f, -a_fDistance);


	//take pre-existing values of m_v3Position and m_v3Target and instead of vectors, use the other functions times a_fDistance
	//CONTROLS: w & s; move backwards and forward, forward should pull us closer to ground(according to rubric)
	m_v3Position += m_v3Forward * a_fDistance;
	m_v3Target += m_v3Forward * a_fDistance;

}
void MyCamera::MoveVertical(float a_fDistance) //vertical is up and down
{
	//Tip:: Look at MoveForward
	//this makes q = down, e = up
	//m_v3Position += vector3(0.0f, a_fDistance, 0.0f);
	//m_v3Target += vector3(0.0f, a_fDistance, 0.0f);

	//m_v3Position += vector3(0.0f, -a_fDistance, 0.0f);
	//m_v3Target += vector3(0.0f, -a_fDistance, 0.0f);


	//using position and taget, change the height the camera is viewing from; use negatives
	//CONTROLS: e & q is up and down
	m_v3Position -= m_v3Upward * a_fDistance;
	m_v3Target -= m_v3Upward * a_fDistance;
}
void MyCamera::MoveSideways(float a_fDistance) //horizontal or sideways
{
	//Tip:: Look at MoveForward
	//m_v3Position += vector3(a_fDistance, 0.0f, 0.0f);
	//m_v3Target	 += vector3(a_fDistance, 0.0f, 0.0f);


	//Since we are moving left and right based on the functions in MyCamera.h and the above function of MoveVertical, we can use m_v3Position and m_v3Target again
	//CONTROLS: a & d, move left and right
	m_v3Position += m_v3Rightward * a_fDistance;
	m_v3Target += m_v3Rightward * a_fDistance;

}
void MyCamera::CalculateView(void)
{
	//Tips:: Directional vectors will be affected by the orientation in the quaternion
	//		 After calculating any new vector one needs to update the View Matrix
	//		 Camera rotation should be calculated out of the m_v3PitchYawRoll member
	//		 it will receive information from the main code on how much these orientations
	//		 have change so you only need to focus on the directional and positional 
	//		 vectors. There is no need to calculate any right click process or connections.

	m_m4View = glm::lookAt(m_v3Position, m_v3Target, m_v3Upward);

	//control speed to prevent my motion sickness, you can remove if you want
	m_v3PitchYawRoll *= 0.75f;

	//The first step is to create quaternions so that when changing the view, we can combine the quaternions and make it easier to alter both the pitch and yaw
	quaternion q_Pitch;
	quaternion q_Yaw;

	//Reference Material: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#quaternions
	//use angleAxis from glm: uses a normalized axis and an angle to build upon a quaternion
	//assign x and y values for the angleAxis to have an axis to rotate upon
	//https://www.mathworks.com/help/map/ref/globe.graphics.geographicglobe.campitch.html this helped me understand camera direction for pitch and yaw
	q_Pitch = glm::angleAxis(m_v3PitchYawRoll.x, m_v3Rightward); 
	q_Yaw = glm::angleAxis(m_v3PitchYawRoll.y, m_v3Upward);
	//when coming the two, we see clear movement looking up, down, and left and right


	//now that we have the two quaternions we need, we have to create a quaternion for combining the two of them so multiplication. Could be done with either * or glm:cross probably
	quaternion q_PitchYaw;
	q_PitchYaw = q_Pitch * q_Yaw;

	//After having the two combined, we can start to initialize values
	//From looking at the rubric in hints, we should look into using rightward and forward
	//both rightward and forward set the forward of camera based on MyCamera.h
	m_v3Rightward = glm::cross(m_v3Forward, m_v3Upward); //must use cross here, just multiplying does absolutely nothing 
	
	//since we have our cross set up, we now need a rotation so use glm::rotate with our combined variable
	//https://open.gl/transformations used to help me determine the values we use for the rotation
	//q_PitchYaw includes both our x and y scale and forward directs the camera forward
	m_v3Forward = glm::rotate(q_PitchYaw, m_v3Forward);
	
	//updating our changes made in the code when used, so changing our target direction we need to view
	//when facing a specific angle and then the ability to also look upwards or above our "heads"
	
	//change the way we move based on the direction of our target and our position
	m_v3Target = m_v3Forward + m_v3Position;

	//m_v3Above = What is above the camera position
	//based on our position and the way we look up
	m_v3Above = m_v3Position + m_v3Upward;

}
//You can assume that the code below does not need changes unless you expand the functionality
//of the class or create helper methods, etc.
void MyCamera::Init(vector3 a_v3Position, vector3 a_v3Target, vector3 a_v3Upward)
{
	m_bFPS = true;
	m_nMode = BTXs::eCAMERAMODE::CAM_PERSP;
	m_fFOV = 45.0f;
	m_v2NearFar = vector2(0.001f, 1000.0f);
	m_v3PitchYawRoll = vector3(0.0f);
	SystemSingleton* pSystem = SystemSingleton::GetInstance();
	vector2 v3WidthHeigh(	static_cast<float>(pSystem->GetWindowWidth()),
							static_cast<float>(pSystem->GetWindowHeight()));
	SetWidthAndHeightOfDisplay(v3WidthHeigh);

	return SetPositionTargetAndUpward(a_v3Position, a_v3Target, a_v3Upward);
}
void MyCamera::Swap(MyCamera& other)
{
	std::swap(m_bFPS, other.m_bFPS);

	std::swap(m_nMode, other.m_nMode);

	std::swap(m_fFOV, other.m_fFOV);

	std::swap(m_v2NearFar, other.m_v2NearFar);

	std::swap(m_v3Position, other.m_v3Position);
	std::swap(m_v3Target, other.m_v3Target);
	std::swap(m_v3Above, other.m_v3Above);

	std::swap(m_v3Forward, other.m_v3Forward);
	std::swap(m_v3Upward, other.m_v3Upward);
	std::swap(m_v3Rightward, other.m_v3Rightward);

	std::swap(m_v3PitchYawRoll, other.m_v3PitchYawRoll);

	std::swap(m_m4Projection, other.m_m4Projection);
	std::swap(m_m4View, other.m_m4View);
}
void MyCamera::Release(void){}
//The big 3
MyCamera::MyCamera()
{
	Init(vector3(0.0f, 0.0f, 5.0f), vector3(0.0f, 0.0f, 0.0f), vector3(0.0f, 1.0f, 0.0f));
}
MyCamera::MyCamera(vector3 a_v3Position, vector3 a_v3Target, vector3 a_v3Upward)
{
	Init(a_v3Position, a_v3Target, a_v3Upward);
}
MyCamera::MyCamera(MyCamera const& other)
{
	m_bFPS = other.m_bFPS;

	m_nMode = other.m_nMode;

	m_fFOV = other.m_fFOV;

	m_v2NearFar = other.m_v2NearFar;

	m_v3Position = other.m_v3Position;
	m_v3Target = other.m_v3Target;
	m_v3Above = other.m_v3Above;

	m_v3Forward = other.m_v3Forward;
	m_v3Upward = other.m_v3Upward;
	m_v3Rightward = other.m_v3Rightward;

	m_v3PitchYawRoll = other.m_v3PitchYawRoll;

	m_m4Projection = other.m_m4Projection;
	m_m4View = other.m_m4View;
}
MyCamera& MyCamera::operator=(MyCamera const& other)
{
	if (this != &other)
	{
		Release();
		Init(other.m_v3Position, other.m_v3Target, other.m_v3Upward);
		MyCamera temp(other);
		Swap(temp);
	}
	return *this;
}
MyCamera::~MyCamera(){ Release(); };
//Accessors
vector3 MyCamera::GetPosition(void){ return m_v3Position; }
vector3 MyCamera::GetForward(void) { return m_v3Forward; }
vector3 MyCamera::GetUpward(void) { return m_v3Upward; }
vector3 MyCamera::GetRightward(void) { return m_v3Rightward; }
void MyCamera::SetForward(vector3 a_v3Input) { m_v3Forward = a_v3Input; }
void MyCamera::SetUpward(vector3 a_v3Input) { m_v3Upward = a_v3Input; }
void MyCamera::SetRightward(vector3 a_v3Input) { m_v3Rightward = a_v3Input; }
matrix4 MyCamera::GetViewMatrix(void){ CalculateView(); return m_m4View; }
matrix4 MyCamera::GetProjectionMatrix(void){ CalculateProjection(); return m_m4Projection; }
void MyCamera::SetNearFarPlanes(float a_fNear, float a_fFar){ m_v2NearFar = vector2(a_fNear, a_fFar); }
void MyCamera::SetFOV(float a_fFOV){ m_fFOV = a_fFOV; }
void MyCamera::SetFPS(bool a_bFPS){ m_bFPS = a_bFPS; }
void MyCamera::SetCameraMode(BTXs::eCAMERAMODE a_nMode){ /*Removed to simplify assignment*/ }
BTXs::eCAMERAMODE MyCamera::GetCameraMode(void){ return m_nMode; }
void MyCamera::SetPosition(vector3 a_v3Position)
{
	return SetPositionTargetAndUpward(a_v3Position, m_v3Target, m_v3Upward);
}
void MyCamera::SetTarget(vector3 a_v3Target)
{
	return SetPositionTargetAndUpward(m_v3Position, a_v3Target, m_v3Upward);
}
matrix4 MyCamera::GetMVP(matrix4 a_m4ModelToWorld)
{
	CalculateProjection();
	CalculateView();
	return m_m4Projection * m_m4View * a_m4ModelToWorld;
}
matrix4 MyCamera::GetVP(void)
{
	CalculateProjection();
	CalculateView();
	return m_m4Projection * m_m4View;
}
//--- Non Standard Singleton Methods
void MyCamera::CalculateProjection(void)
{
	SystemSingleton* pSystem = SystemSingleton::GetInstance();
	vector2 v3WidthHeigh(	static_cast<float>(pSystem->GetWindowWidth()),
							static_cast<float>(pSystem->GetWindowHeight()));
	SetWidthAndHeightOfDisplay(v3WidthHeigh);
	float fRatio = v3WidthHeigh.x / v3WidthHeigh.y;
	m_m4Projection = glm::perspective(m_fFOV, fRatio, m_v2NearFar.x, m_v2NearFar.y);
}

void MyCamera::ChangePitch(float a_fDegree)
{
	m_v3PitchYawRoll.x += a_fDegree;
}
void MyCamera::ChangeYaw(float a_fDegree)
{
	m_v3PitchYawRoll.y += a_fDegree;
}
void MyCamera::ChangeRoll(float a_fDegree)
{
	m_v3PitchYawRoll.z += a_fDegree;
}

void MyCamera::ResetCamera(void)
{
	m_v3PitchYawRoll = vector3(0.0f);

	m_v3Position = vector3(0.0f, 0.0f, 10.0f);
	m_v3Target = vector3(0.0f, 0.0f, 9.0f);
	m_v3Above = vector3(0.0f, 1.0f, 10.0f);

	m_v3Forward = vector3(0.0f, 0.0f, -1.0f);
	m_v3Upward = vector3(0.0f, 1.0f, 0.0f);
	m_v3Rightward = vector3(1.0f, 0.0f, 0.0f);

}
void MyCamera::SetWidthAndHeightOfDisplay(vector2 a_v2WidthHeight)
{
	m_fWidth = a_v2WidthHeight.x;
	m_fHeight = a_v2WidthHeight.y;
}