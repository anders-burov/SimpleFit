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
	smpl::PoseAxisAngleCoefficients pose;
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
	smpl::PoseAxisAngleCoefficients pose;
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

	//smpl::Generator generate(d3d, smpl::Generator::Configuration(std::string("Model")));
	//smpl::Body body = generate(shape, pose);
	//body.Dump("new.obj");

	OpenPoseTracker joint_tracker(argc, argv);
	std::vector<float> tracked_joints = joint_tracker("Photos/all/test1.png");
	
	// andrei - tpose test0.png
	// std::vector<float> tracked_joints = { 318.98f,54.0278f,317.706f,103.625f,275.903f,106.226f,210.626f,119.286f,149.272f,128.369f,359.434f,104.888f,429.907f,112.776f,495.202f,121.866f,286.344f,244.583f,274.578f,343.757f,265.453f,440.348f,339.859f,245.872f,341.132f,349.03f,339.872f,442.981f,308.525f,46.1863f,328.075f,46.1329f,296.719f,56.5666f,337.258f,50.1061f };
	
	// andrei - surrender test1.png
	//std::vector<float> tracked_joints = { 338.471f,47.4714f,334.632f,101.036f,294.157f,102.323f,236.761f,142.781f,224.997f,86.5996f,376.387f,101.036f,431.221f,147.955f,457.312f,90.5693f,303.306f,236.732f,295.431f,337.224f,287.609f,432.552f,359.44f,241.937f,360.735f,339.893f,362.068f,436.41f,328.039f,38.3885f,343.779f,38.3787f,309.87f,47.5075f,352.873f,46.2211f };

	// jake - tpose test20.png
	//std::vector<float> tracked_joints = { 266.814f,87.9533f,265.497f,129.689f,221.071f,128.426f,163.683f,128.365f,111.452f,119.294f,309.855f,129.763f,362.053f,128.376f,412.994f,121.864f,234.159f,257.647f,225.048f,339.847f,222.379f,409.041f,292.866f,260.223f,291.539f,339.872f,298.024f,410.337f,260.216f,80.1354f,275.943f,80.1616f,247.181f,89.3096f,286.358f,90.535f };

	// yawar
	//std::vector<float> tracked_joints = { 318.919f,67.071f,319.01f,103.593f,287.611f,106.203f,235.396f,119.28f,181.876f,119.223f,351.603f,99.7549f,403.814f,106.217f,458.638f,98.4039f,304.607f,217.145f,308.477f,295.482f,317.683f,371.14f,345.07f,214.568f,345.057f,295.478f,339.801f,371.195f,309.835f,59.2867f,325.454f,57.9994f,300.675f,67.0261f,332.028f,61.8331f };

	/*for (uint i = 0; i < smpl::COCO_JOINT_COUNT; i++)
	{
		tracked_joints[2 * i] -= 50;
	}*/
	smpl::Optimizer optimize(smpl::Optimizer::Configuration(std::string("Model")), 
		smpl::Generator(smpl::Generator::Configuration(std::string("Model"))), tracked_joints);
	optimize(std::string("Photos/test1.png"), shape, pose, scaling, translation);

	system("pause");

    return 0;
}