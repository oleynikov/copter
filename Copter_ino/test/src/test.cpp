#include "../../Copter.h"
#include <iostream>



bool test_QuadroCopterConstructor_000 ( void )
{

	QuadroCopter copter;
	
	return
	(
		copter.getEngine(0) == 0
			&&
		copter.getEngine(1) == 0
			&&
		copter.getEngine(2) == 0
			&&
		copter.getEngine(3) == 0
			&&
		copter.getEnginesReady() == 0
			&&
		copter.getBalancerReady() == 0
			&&
		copter.getReadyToLaunch() == 0
	);

}

bool test_QuadroCopterConstructor_001 ( void )
{
	QuadroCopter copter;
	copter.createEngines(1,2,3,4);

	return
	(
		copter.getEngine(0)->getPin() == 1
			&&
		copter.getEngine(1)->getPin() == 2
			&&
		copter.getEngine(2)->getPin() == 3
			&&
		copter.getEngine(3)->getPin() == 4
			&&
		copter.getEnginesReady() == 1
			&&
		copter.getBalancerReady() == 0
			&&
		copter.getReadyToLaunch() == 0
	);

}

bool test_QuadroCopterConstructor_002 ( void )
{
	
	float ypr[3] = { 0.49 , 0.49 , 0.49 };

	QuadroCopter copter;
	copter.createEngines(1,2,3,4);
	copter.createBalancer(ypr);

	return
	(
		copter.getEngine(0)->getPin() == 1
			&&
		copter.getEngine(1)->getPin() == 2
			&&
		copter.getEngine(2)->getPin() == 3
			&&
		copter.getEngine(3)->getPin() == 4
			&&
		copter.getEnginesReady() == 1
			&&
		copter.getBalancerReady() == 1
			&&
		copter.getReadyToLaunch() == 1
			&&
		copter.getBalanced() == 1
	);

}

bool test_QuadroCopterSpeedChange_001 ( void )
{

	float ypr[3] = { 0.49 , 0.51 , 0.49 };

	QuadroCopter copter;
	copter.createEngines(5,7,9,11);
	copter.createBalancer(ypr);
	copter.cmdArm();
	copter.cmdRaise();
	copter.cmdRaise();
	copter.cmdRaise();
	copter.cmdRaise();
	copter.cmdRaise();

	return
	(
		copter.getEngine(0)->getPin() == 5
			&&
		copter.getEngine(1)->getPin() == 7
			&&
		copter.getEngine(2)->getPin() == 9
			&&
		copter.getEngine(3)->getPin() == 11
			&&
		copter.getEnginesReady() == 1
			&&
		copter.getBalancerReady() == 1
			&&
		copter.getReadyToLaunch() == 1
			&&
		copter.getBalanced() == 0
			&&
		copter.getEngine(0)->getSpeed() == Engine::SPEED_MIN + Engine::SPEED_DELTA * 4
			&&
		copter.getEngine(1)->getSpeed() == Engine::SPEED_MIN + Engine::SPEED_DELTA * 4
			&&
		copter.getEngine(2)->getSpeed() == Engine::SPEED_MIN + Engine::SPEED_DELTA * 4
			&&
		copter.getEngine(3)->getSpeed() == Engine::SPEED_MIN + Engine::SPEED_DELTA * 4
	);

}

bool test_QuadroCopterSpeedChange_002 ( void )
{

	float ypr[3] = { 0.49 , 0.51 , 0.49 };

	QuadroCopter copter;
	copter.createEngines(5,7,9,11);
	copter.createBalancer(ypr);
	copter.cmdArm();
	copter.cmdRaise();
	copter.cmdRaise();
	copter.cmdDescend();
	copter.cmdDescend();
	copter.cmdDescend();
	copter.cmdDescend();
	copter.cmdDescend();

	return
	(
		copter.getEngine(0)->getPin() == 5
			&&
		copter.getEngine(1)->getPin() == 7
			&&
		copter.getEngine(2)->getPin() == 9
			&&
		copter.getEngine(3)->getPin() == 11
			&&
		copter.getEnginesReady() == 1
			&&
		copter.getBalancerReady() == 1
			&&
		copter.getReadyToLaunch() == 1
			&&
		copter.getBalanced() == 0
			&&
		copter.getEngine(0)->getSpeed() == Engine::SPEED_MIN
			&&
		copter.getEngine(1)->getSpeed() == Engine::SPEED_MIN
			&&
		copter.getEngine(2)->getSpeed() == Engine::SPEED_MIN
			&&
		copter.getEngine(3)->getSpeed() == Engine::SPEED_MIN
	);

}



int main ( void )
{

	std::cout << ( test_QuadroCopterConstructor_000() 							? "SUCCESS" : "FAIL" ) << std::endl;
	std::cout << ( test_QuadroCopterConstructor_001() 							? "SUCCESS" : "FAIL" ) << std::endl;
	std::cout << ( test_QuadroCopterConstructor_002() 							? "SUCCESS" : "FAIL" ) << std::endl;
	std::cout << ( test_QuadroCopterSpeedChange_001() 							? "SUCCESS" : "FAIL" ) << std::endl;
	std::cout << ( test_QuadroCopterSpeedChange_002() 							? "SUCCESS" : "FAIL" ) << std::endl;

	return 0;

}