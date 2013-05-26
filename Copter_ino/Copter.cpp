
#include "Copter.h"



//	Engine
	
				Engine::Engine ( int pin )
					:
						pin		( pin ),
						speed	( 0 ),
						status	( ENGINE_STATUS_STOPPED )
{

	this->servo.attach(pin);
	this->setSpeed(0);
	
}

int				Engine::getPin ( void ) const
{

	return this->pin;

}

int				Engine::getSpeed ( void ) const
{

	return this->speed;

}

EngineStatus	Engine::getStatus ( void ) const
{

	return this->status;

}

bool			Engine::arm ( void )
{

	return this->setSpeed(SPEED_ARM);
	
}

void			Engine::stop ( void )
{

	this->setSpeed(0);
	this->status = ENGINE_STATUS_STOPPED;

}

bool			Engine::slow ( void )
{

	bool success = this->setSpeed(this->speed-Engine::SPEED_DELTA);
	
	if ( ! success )
	{
	
		this->setSpeed(SPEED_MIN);
	
	}

	return success;

}

bool			Engine::accelerate ( void )
{

	float speedNew;

	if ( this->speed == Engine::SPEED_ARM )
	{
	
		speedNew = SPEED_MIN;
		this->status = ENGINE_STATUS_RUNNING;
		
	}
	
	else
	{
	
		speedNew = this->speed+Engine::SPEED_DELTA;
		
	}
	
	bool success = this->setSpeed(speedNew);
	
	if ( ! success )
	{
	
		this->setSpeed(SPEED_MAX);
	
	}

	return success;

}

bool			Engine::accelerate ( int delta )
{

	bool success = this->setSpeed(this->speed+delta);
	
	if ( ! success )
	{
	
		if ( delta > 0 )
		{
		
			this->setSpeed(SPEED_MAX);
			
		}
			
		else
		{
		
			this->setSpeed(SPEED_MIN);
		
		}
	
	}
	
	return success;

}

bool			Engine::setSpeed ( int speed )
{

	if ( this->getSpeedValid(speed) )
	{
	
		this->servo.write(speed);
		this->speed = speed;
		
		return true;
	
	}
	
	return false;

}

bool			Engine::getSpeedValid ( int speed ) const
{

	bool stopEngine = speed == 0;
	bool armStoppedEngine = this->speed == 0 && speed == SPEED_ARM;
	bool launchArmedEngine = this->speed == SPEED_ARM && speed == SPEED_ARM + SPEED_DELTA;
	bool changeRunningEngineSpeed = this->status == ENGINE_STATUS_RUNNING && speed >= SPEED_MIN && speed <= Engine::SPEED_MAX;
	
	return
	(
		stopEngine
			||
		armStoppedEngine
			||
		launchArmedEngine
			||
		changeRunningEngineSpeed
	);

}



//	ABalancer

				ABalancer::ABalancer ( float* yawPitchRoll , float* acceleration )
					:
						enabled			( false ),
						yawPitchRoll	( yawPitchRoll ),
						acceleration	( acceleration )
{

}

				ABalancer::~ABalancer ( void )
{

}

void			ABalancer::toggleEnabled ( void )
{

	this->enabled = ! this->enabled;

}

void			ABalancer::updateTelemetry ( void )
{

	if ( ! this->enabled )
		return;

	if ( ! this->getYawBalanced() )
		this->balanceYaw();

	if ( ! this->getPitchBalanced() )
		this->balancePitch();
	
	if ( ! this->getRollBalanced() )
		this->balanceRoll();

}

bool			ABalancer::getBalanced ( void )
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

bool			ABalancer::getSteady ( void )
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

bool			ABalancer::getYawBalanced ( void ) const
{

	return 1;

}

bool			ABalancer::getPitchBalanced ( void ) const
{

	return fabs(this->yawPitchRoll[1]) < ABalancer::YAW_PITCH_ROLL_ACCURACY;

}

bool			ABalancer::getRollBalanced ( void ) const
{

	return fabs(this->yawPitchRoll[2]) < ABalancer::YAW_PITCH_ROLL_ACCURACY;

}

float			ABalancer::getYprBalancingSpeed ( float deflectionAngle ) const
{

	return sqrt(deflectionAngle);

}

bool			ABalancer::getSteadyX ( void ) const
{

	return fabs(this->acceleration[0]) < ABalancer::ACCELERATION_ACCURACY;

}

bool			ABalancer::getSteadyY ( void ) const
{

	return fabs(this->acceleration[1]) < ABalancer::ACCELERATION_ACCURACY;

}

bool			ABalancer::getSteadyZ ( void ) const
{

	return fabs(this->acceleration[2]) < ABalancer::ACCELERATION_ACCURACY;

}



//	QuadroBalancer
	
				QuadroBalancer::QuadroBalancer ( QuadroCopter* copter , float* yawPitchRoll , float* acceleration )
					:
						ABalancer					( yawPitchRoll , acceleration ),
						copter						( copter )
{

	this->yprBalancingCoefficient = 0.31623;

}

void			QuadroBalancer::balanceYaw ( void )
{

	return;

}

void			QuadroBalancer::balancePitch ( void )
{

	float pitchAbs = fabs(this->yawPitchRoll[1]);
	float engineSpeedDelta = this->getYprBalancingSpeed(pitchAbs);

	//	Try to slow the engine which is higher
	int engineHigh = ( this->yawPitchRoll[1] < 0 ) ? 3 : 1;
        if ( this->copter->getEngine(engineHigh)->accelerate(engineSpeedDelta*-1) ) ;

	//	Try to accelerate the engine which is lower
	int engineLow = ( this->yawPitchRoll[1] < 0 ) ? 1 : 3;
        this->copter->getEngine(engineLow)->accelerate(engineSpeedDelta);
	
}

void			QuadroBalancer::balanceRoll ( void )
{
  this->copter->getEngine(0)->accelerate(this->yawPitchRoll[2]);
  
/*
	float rollAbs = fabs(this->yawPitchRoll[2]);
	float engineSpeedDelta = this->getYprBalancingSpeed(rollAbs);

  	//	Try to slow the engine which is higher
	int engineHigh = ( this->yawPitchRoll[2] < 0 ) ? 2 : 0;
	if ( this->copter->getEngine(engineHigh)->accelerate(engineSpeedDelta*-1) ) ;

	//	Try to accelerate the engine which is lower
	int engineLow = ( this->yawPitchRoll[2] < 0 ) ? 0 : 2;
	this->copter->getEngine(engineLow)->accelerate(engineSpeedDelta);
*/


}

void			QuadroBalancer::stopX ( void )
{

	//	Sorry, can't yet

}

void			QuadroBalancer::stopY ( void )
{

	//	Sorry, can't yet

}

void			QuadroBalancer::stopZ ( void )
{

	//	Sorry, can't yet

}



//	QuadroCopter

void			QuadroCopter::createEngines ( const int pin0 , const int pin1, const int pin2, const int pin3 )
{

	//	Add four engines
	this->createEngine(0,pin0);
	this->createEngine(1,pin1);
	this->createEngine(2,pin2);
	this->createEngine(3,pin3);
	
}

void			QuadroCopter::createBalancer ( float* yawPitchRoll , float* acceleration )
{

	this->balancer = new QuadroBalancer(this,yawPitchRoll,acceleration);

}
