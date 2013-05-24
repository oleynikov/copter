#include "../../Copter.h"
#include <iostream>



bool multiTest_000 ( void )
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

bool multiTest_001 ( void )
{
	QuadroCopter copter;
	copter.createEngines(5,10,20,13);

	return
	(
		copter.getEngine(0)->getPin() == 5
			&&
		copter.getEngine(1)->getPin() == 10
			&&
		copter.getEngine(2)->getPin() == 20
			&&
		copter.getEngine(3)->getPin() == 13
			&&
		copter.getEnginesReady() == 1
			&&
		copter.getBalancerReady() == 0
			&&
		copter.getReadyToLaunch() == 0
	);

}

bool multiTest_002 ( void )
{
	
	float ypr[3] = { 0.49 , 0.49 , 0.49 };
	float accel[3] = { 0.49 , 0.51 , 0.49 };

	QuadroCopter copter;
	copter.createEngines(5,4,3,12);
	copter.createBalancer(ypr,accel);

	return
	(
		copter.getEngine(0)->getPin() == 5
			&&
		copter.getEngine(1)->getPin() == 4
			&&
		copter.getEngine(2)->getPin() == 3
			&&
		copter.getEngine(3)->getPin() == 12
			&&
		copter.getEnginesReady() == 1
			&&
		copter.getBalancerReady() == 1
			&&
		copter.getReadyToLaunch() == 1
			&&
		copter.getBalancer()->getBalanced() == 1
	);

}

bool multiTest_003 ( void )
{

	float ypr[3] = { 0.49 , 0.51 , 0.49 };
	float accel[3] = { 0.49 , 0.51 , 0.49 };

	QuadroCopter copter;
	copter.createEngines(5,7,9,11);
	copter.createBalancer(ypr,accel);
	copter.cmdLaunch();
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
		copter.getReadyToLaunch() == 0
			&&
		copter.getBalancer()->getBalanced() == 0
			&&
		copter.getEngine(0)->getSpeed() == Engine::SPEED_MIN + Engine::SPEED_DELTA * 5
			&&
		copter.getEngine(1)->getSpeed() == Engine::SPEED_MIN + Engine::SPEED_DELTA * 5
			&&
		copter.getEngine(2)->getSpeed() == Engine::SPEED_MIN + Engine::SPEED_DELTA * 5
			&&
		copter.getEngine(3)->getSpeed() == Engine::SPEED_MIN + Engine::SPEED_DELTA * 5
	);

}

bool multiTest_004 ( void )
{

	float ypr[3] = { 0.49 , 0.51 , 0.49 };
	float accel[3] = { 0.49 , 0.51 , 0.49 };

	QuadroCopter copter;
	copter.createEngines(5,7,9,11);
	copter.createBalancer(ypr,accel);
	copter.cmdLaunch();
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
		copter.getReadyToLaunch() == 0
			&&
		copter.getBalancer()->getBalanced() == 0
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

bool test_getYprBalancingSpeed_001 ( void )
{

	float ypr[3] = { 0 , 0 , 0 };
	float accel[3] = { 0 , 0 , 0 };

	QuadroCopter copter;
	copter.createEngines(5,7,9,11);
	copter.createBalancer(ypr,accel);
	
	for ( int i = 0 ; i < 40 ; i ++ )
	{
	
		std::cout << i << " " << copter.getBalancer()->getYprBalancingSpeed(i) << std::endl;
	
	}
//	float balancingSpeed = ;
	
	
	
	return
	(
1	
	
	);

}



int main ( void )
{

	std::cout << ( multiTest_000() 							? "SUCCESS" : "FAIL" ) << std::endl;
	std::cout << ( multiTest_001() 							? "SUCCESS" : "FAIL" ) << std::endl;
	std::cout << ( multiTest_002() 							? "SUCCESS" : "FAIL" ) << std::endl;
	std::cout << ( multiTest_003() 							? "SUCCESS" : "FAIL" ) << std::endl;
	std::cout << ( multiTest_004() 							? "SUCCESS" : "FAIL" ) << std::endl;
	std::cout << ( test_getYprBalancingSpeed_001() 			? "SUCCESS" : "FAIL" ) << std::endl;

	return 0;

}