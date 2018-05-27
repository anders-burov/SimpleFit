// Runner.cpp : Defines the entry point for the console application.
//

#include <string>
#include <Windows.h>
#include <fstream>

#include "SMPL.h"
#include "OpenPoseTracker.h"

void reconstructBodyFromImage(std::string image, smpl::Body body)
{
	/*
	std::vector<Joint2D> joints2D;
	openpose::regressJoints(image, joints2D);

	smpl::ShapeCoefficients shape;
	smpl::PoseCoefficients pose;
	smpl::Body body;
	smpl::Generator generator;
	smpl::Optimizer optimizer(joints2D);
	optimizer.initialize(shape, pose);

	for (int = 0; i < 1000; i++) {
		generator.run(shape, pose, body);
		optimizer.run(body, shape, pose);
	}
	*/
}

int main(int argc, char** argv)
{
	//reconstructBodyFromImage();
	//MessageBoxW(NULL, (std::wstring(L"Could not open ")).c_str(), L"File error", MB_ICONERROR | MB_OK);

	smpl::ShapeCoefficients shape;
	smpl::PoseCoefficients pose;
	ZeroMemory(&shape, sizeof(shape));
	ZeroMemory(&pose, sizeof(pose));
	Eigen::Vector3f translation(0,0,-5);
	Eigen::Vector3f scaling(1,1,1);

	//
	///*shape[0] = 4.1f;
	//shape[1] = 2.3f;
	//shape[2] = 3.4f;
	//shape[3] = 3.1f;*/

	/*pose(50) = 0.5f;
	pose(52) = 0.6f;
	pose(58) = 0.5f;*/

	//pose[0] = smpl::float3(0.5f, -0.5f, 0.f);
	
	smpl::D3D d3d;

	//smpl::Generator generate(d3d, smpl::Generator::Configuration(std::string("Model")));
	//smpl::Body body = generate(shape, pose);
	//body.Dump("new.obj");

	//OpenPoseTracker joint_tracker(argc, argv);
	//std::vector<float> tracked_joints = joint_tracker("Photos/4.jpg");
	std::vector<float> tracked_joints = { 318.98f,54.0278f,317.706f,103.625f,275.903f,106.226f,210.626f,119.286f,149.272f,128.369f,359.434f,104.888f,429.907f,112.776f,495.202f,121.866f,286.344f,244.583f,274.578f,343.757f,265.453f,440.348f,339.859f,245.872f,341.132f,349.03f,339.872f,442.981f,308.525f,46.1863f,328.075f,46.1329f,296.719f,56.5666f,337.258f,50.1061f };
	smpl::Optimizer optimize(smpl::Optimizer::Configuration(std::string("Model")), 
		smpl::Generator(d3d, smpl::Generator::Configuration(std::string("Model"))), tracked_joints);
	optimize(shape, pose, scaling, translation);

    return 0;
}