#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Mackenzie Strobeck - mrs8402@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	//allocate the primitive
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateTorus(3.0f, 2.0f, 6, 6, C_RED);

	//allocate the primitive
	m_pMesh2 = new MyMesh();
	m_pMesh2->GenerateTorus(3.0f, 2.0f, 8, 8, C_GREEN);

	m_pMesh3 = new MyMesh();
	m_pMesh3->GenerateCone(0.5f, 1.0f, 6, C_BLUE);

	//create a new camera
	m_pCamera = new MyCamera();

	//projection used
	m_uProjection = 1;
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	//calculate view and projection
	switch (m_uProjection)
	{
	default:
	case 1:
		m_pCamera->ResetCamera();
		break;

	case 2:
		//focus on center, instead of true use false
		m_pCamera->ResetCamera();
		m_pCamera->SetPerspective(false);
		break;

	case 3:
		//make it so you get a side view of the shapes in order from top to bottom: cone, green shape, red shape: use z and x
		m_pCamera->ResetCamera();
		m_pCamera->SetUp(-AXIS_Z);
		//set up the positioning, 5 is too close, 25 is in an okay place, move on x axis
		m_pCamera->SetPosition(25.0f * AXIS_X);
		break;

	case 4:
		//use something that will be up and down based; rotate to face a z axis angle, then find a way to turn it around so we can see the cone in front
		m_pCamera->ResetCamera();
		//m_pCamera->SetUp(-AXIS_Z);
		m_pCamera->SetPosition(-15.0f * AXIS_Z);
		m_pCamera->SetNearFar(vector2(3.0f, 15.0f));
		break;

	case 5:
		//due to similiarity to case 4, alter it so there is no cone
		m_pCamera->ResetCamera();
		m_pCamera->SetPosition(-20.0f * AXIS_Z);
		m_pCamera->SetNearFar(vector2(10.0f, 100.0f));
		break;


	case 6:
		//same as 5 but just green ring and the cone
		m_pCamera->ResetCamera();
		m_pCamera->SetPosition(-15.0f * AXIS_Z);
		m_pCamera->SetNearFar(vector2(2.0f, 12.0f));
		break;

	case 7:
		//order: red outside, green mid, blue back
		m_pCamera->ResetCamera();
		m_pCamera->SetUp(-AXIS_Y);
		//m_pCamera->SetPosition(-20.0f * AXIS_Z);
		//begins to clip at a number such as 10
		m_pCamera->SetNearFar(vector2(8.0f, 100.0f));
		break;
	}

	m_pCamera->CalculateProjectionMatrix();
	m_pCamera->CalculateViewMatrix();

	matrix4 m4Projection = m_pCamera->GetProjectionMatrix();
	matrix4 m4View = m_pCamera->GetViewMatrix();


	//draw the primitive
	m_pMesh1->Render(m4Projection, m4View, glm::rotate(IDENTITY_M4, 1.5708f, AXIS_X));
	m_pMesh2->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3(0.0f, 0.0f, -5.0f)) * glm::rotate(IDENTITY_M4, 1.5708f, AXIS_X));
	m_pMesh3->Render(m4Projection, m4View, glm::translate(vector3(0.0f, 0.0f, -10.0f)));

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}