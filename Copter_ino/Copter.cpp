
#include "Copter.h"



//	Engine
	
		Engine::Engine ( int pin )
			:
				pin		( pin ),
				speed	( 0 )
{

	//this->servo.attach(pin);
	this->setSpeed(0);
	
}

int		Engine::getPin ( void ) const
{

	return this->pin;

}

int		Engine::getSpeed ( void ) const
{

	return this->speed;

}

void	Engine::arm ( void )
{

	this->setSpeed(Engine::SPEED_ARM);
	
}

void	Engine::stop ( void )
{

	this->setSpeed(0);

}

bool	Engine::accelerate ( void )
{

	int speedNew;

	if ( this->speed == Engine::SPEED_ARM )
	{
	
		speedNew = Engine::SPEED_MIN;
		
	}
	
	else
	{
	
		speedNew = this->speed+Engine::SPEED_DELTA;
		
	}

	return this->setSpeed(speedNew);

}

bool	Engine::slow ( void )
{

	return this->setSpeed(this->speed-Engine::SPEED_DELTA);

}

bool	Engine::setSpeed ( int speed )
{

	if ( this->getSpeedValid(speed) )
	{
	
	
		//this->servo.write(speed);
		this->speed = speed;
		
		return true;
	
	}
	
	return false;

}

bool	Engine::getSpeedValid ( int speed ) const
{

	bool actionArm = this->speed == 0 && speed == Engine::SPEED_ARM;
	bool actionLaunch = this->speed == Engine::SPEED_ARM && this->speed < speed;
	bool actionSpeedChange = speed <= Engine::SPEED_MAX && speed >= Engine::SPEED_MIN;
	bool actionStop = speed == 0;
	
	return actionArm || actionLaunch || actionSpeedChange || actionStop;

}




//	ABalancer

		ABalancer::ABalancer ( float* yawPitchRoll )
			:
				yawPitchRoll(yawPitchRoll)
{

}

		ABalancer::~ABalancer ( void )
{

}

void	ABalancer::update ( void )
{

	if ( ! this->getYawBalanced() )
		this->balanceYaw();

	if ( ! this->getPitchBalanced() )
		this->balancePitch();
	
	if ( ! this->getRollBalanced() )
		this->balanceRoll();

}

bool	ABalancer::getBalanced ( void )
{

	return
	(
		this->getYawBalanced()
			&&
		this->getPitchBalanced()
			&&
		this->getRollBalanced()
	);

}

bool	ABalancer::getSteady ( void )
{

	return
	(
		this->getSteadyX()
			&&
		this->getSteadyY()
			&&
		this->getSteadyZ()
	);

}

bool	ABalancer::getYawBalanced ( void ) const
{

	return 1;

}

bool	ABalancer::getPitchBalanced ( void ) const
{

	return fabs(this->yawPitchRoll[1]) < ABalancer::YAW_PITCH_ROLL_ACCURACY;

}

bool	ABalancer::getRollBalanced ( void ) const
{

	return fabs(this->yawPitchRoll[2]) < ABalancer::YAW_PITCH_ROLL_ACCURACY;

}

bool	ABalancer::getSteadyX ( void ) const
{

	return fabs(this->acceleration[0]) < ABalancer::ACCELERATION_ACCURACY;

}

bool	ABalancer::getSteadyY ( void ) const
{

	return fabs(this->acceleration[1]) < ABalancer::ACCELERATION_ACCURACY;

}

bool	ABalancer::getSteadyZ ( void ) const
{

	return fabs(this->acceleration[2]) < ABalancer::ACCELERATION_ACCURACY;

}



//	QuadroBalancer
	
		QuadroBalancer::QuadroBalancer ( QuadroCopter* copter , float* yawPitchRoll )
			:
				ABalancer	( yawPitchRoll ),
				copter		( copter )
{

}

void	QuadroBalancer::balanceYaw ( void )
{

	return;

}

void	QuadroBalancer::balancePitch ( void )
{

	//	Try to slow the engine which is higher
	int engineHigh = ( this->yawPitchRoll[1] < 0 ) ? 3 : 1;
	if ( this->copter->getEngine(engineHigh)->slow() ) return;

	//	Try to accelerate the engine which is lower
	int engineLow = ( this->yawPitchRoll[1] < 0 ) ? 1 : 3;
	if ( this->copter->getEngine(engineLow)->accelerate() ) return;
	
	//	If lower engine works @ maximum speed, that is really bad...
	//	this->copter->stopAllEngines()
	//	this->copter->openParaschute()
}

void	QuadroBalancer::balanceRoll ( void )
{
  	//	Try to slow the engine which is higher
	int engineHigh = ( this->yawPitchRoll[2] < 0 ) ? 2 : 0;
	if ( this->copter->getEngine(engineHigh)->slow() ) return;

	//	Try to accelerate the engine which is lower
	int engineLow = ( this->yawPitchRoll[2] < 0 ) ? 0 : 2;
	if ( this->copter->getEngine(engineLow)->accelerate() ) return;
	
	//	If lower engine works @ maximum speed, that is really bad...
	//	this->copter->stopAllEngines()
	//	this->copter->openParaschute()
}

void	QuadroBalancer::stopX ( void )
{

	//	Sorry, can't yet

}

void	QuadroBalancer::stopY ( void )
{

	//	Sorry, can't yet

}

void	QuadroBalancer::stopZ ( void )
{

	//	Sorry, can't yet

}



//	QuadroCopter

void	QuadroCopter::createEngines ( const int pin0 , const int pin1, const int pin2, const int pin3 )
{

	//	Add four engines
	this->addEngine(0,pin0);
	this->addEngine(1,pin1);
	this->addEngine(2,pin2);
	this->addEngine(3,pin3);
	
}

void	QuadroCopter::createBalancer ( float* yawPitchRoll )
{

	ABalancer* balancer = new QuadroBalancer(this,yawPitchRoll);
	this->addBalancer(balancer);

}
