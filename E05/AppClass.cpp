#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "MacKenzie Strobeck - mrs8402@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 30.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);


	//init the mesh
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_BLACK);

}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Create a model matrix out of the arcball quaternion
	matrix4 m4Model = ToMatrix4(m_qArcBall);

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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f, 2.0f, 2.0f));
	static float value = 0.0f;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));


	//could not figure out how to move in both directions when approaching edge of window
	//move right
	value += 0.01f;
	
	//move left
	//value -= 0.01f;


	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model = m4Scale * m4Translate;

	
	//creating spacey dude; may be repetitive but felt better than having multiple sections of code
	for (int i = 0; i <= 2; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3(value, (0.0f + i), 0.0f)));
	}

	for (int i = 0; i <= 1; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 1.0f), (2.0f + i), 0.0f)));
	}

	for (int i = 0; i <= 4; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 2.0f), (0.0f + i), 0.0f)));
	}

	m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 2.0f), 6.0f, 0.0f)));

	for (int i = 0; i <= 1; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 3.0f), (5.0f - i), 0.0f)));
	}

	for (int i = 0; i <= 1; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 3.0f), (2.0f - i), 0.0f)));
	}
	
	for (int i = 0; i <= 1; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 3.0f + i), -1.0f, 0.0f)));
	}

	for (int i = 0; i <= 1; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 3.0f + i), -1.0f, 0.0f)));
	}

	for (int i = 0; i <= 3; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 4.0f), (1.0f + i), 0.0f)));
	}

	for (int i = 0; i <= 3; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 5.0f), (1.0f + i), 0.0f)));
	}

	for (int i = 0; i <= 3; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 6.0f), (1.0f + i), 0.0f)));
	}

	m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 8.0f), 6.0f, 0.0f)));
	

	for (int i = 0; i <= 1; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 7.0f), (5.0f - i), 0.0f)));
	}

	for (int i = 0; i <= 1; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 7.0f), (1.0f + i), 0.0f)));
	}

	for (int i = 0; i <= 1; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 6.0f + i), -1.0f, 0.0f)));
	}

	for (int i = 0; i <= 4; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 8.0f), (0.0f + i), 0.0f)));
	}

	for (int i = 0; i <= 1; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 9.0f), (2.0f + i), 0.0f)));
	}
	
	for (int i = 0; i <= 2; i++) {
			m_pMesh->Render(m4Projection, m4View, glm::translate(IDENTITY_M4, vector3((value + 10.0f), (0.0f + i), 0.0f)));
	}


	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

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
	SafeDelete(m_pMesh);
	//release GUI
	ShutdownGUI();
}