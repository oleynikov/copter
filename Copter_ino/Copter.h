#pragma once

#include <math.h>
#include <Servo.h>
#include <Arduino.h>



enum EngineStatus
{

	ENGINE_STATUS_STOPPED,
	
	ENGINE_STATUS_RUNNING

};

class Engine
{

	public:
	
									Engine ( int pin );
		
		int							getPin ( void ) const;

		float						getSpeed ( void ) const;
		
		EngineStatus				getStatus ( void ) const;
		
		bool						arm ( void );
		
		void						stop ( void );

		bool						accelerate ( float delta );
		
		bool						accelerate ( void );

		bool						slow ( void );
		
		const static int			SPEED_ARM = 20;
		
		const static int			SPEED_MIN = 35;
		
		const static int			SPEED_MAX = 50;
		
		const static int			SPEED_DELTA = 1;
	
	protected:

		
		bool						getSpeedValid ( float speed ) const;
		
		bool						setSpeed ( float speed );

		int							pin;
		
		float						speed;
		
		EngineStatus				status;
		
		Servo						servo;
		
};

class ABalancer
{

	public:
	
									ABalancer ( float* yawPitchRoll , float* acceleration );
	
		virtual						~ABalancer ( void );
		
		void						toggleEnabled ( void );

		void						updateTelemetry ( void );

		bool						getBalanced ( void );
		
		bool						getSteady ( void );

		float						getYprBalancingSpeed ( float deflectionAngle ) const;

		const static float			YAW_PITCH_ROLL_ACCURACY = 0.5;
		
		const static float			ACCELERATION_ACCURACY = 0.5;
		
	protected:

		bool						getYawBalanced ( void ) const;
		
		bool						getPitchBalanced ( void ) const;
		
		bool						getRollBalanced ( void ) const;
		
		bool						getSteadyX ( void ) const;

		bool						getSteadyY ( void ) const;

		bool						getSteadyZ ( void ) const;

		virtual void				balanceYaw ( void ) = 0;
		
		virtual void				balancePitch ( void ) = 0;
		
		virtual void				balanceRoll ( void ) = 0;
		
		virtual void				stopX ( void ) = 0;

		virtual void				stopY ( void ) = 0;

		virtual void				stopZ ( void ) = 0;
		
		bool						enabled;
	
		float*						yawPitchRoll;
		
		float*						acceleration;

		float						yprBalancingCoefficient;

};

enum CopterCommand
{

	COPTER_STOP = 48,
  
	COPTER_LAUNCH,
	
	COPTER_RAISE,
  
	COPTER_DESCEND,
	
	BALANCER_TOGGLE_ENABLED
  
};

template <int enginesNumber>
class ACopter
{

	public:
	
									ACopter ( void )
										:
											balancer	( 0 )
		{
		
			//	Zero-initiate engines array
			for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
			{
			
				this->engines[engineKey] = 0;
			
			}
		
		}

		virtual						~ACopter ( void )
		{
		
			//	Delete engines
			for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
			{
			
				delete this->engines[engineKey];
			
			}
			
			//	Delete the balancer
			if ( this->balancer )
			{
			
				delete this->balancer;
				
			}
		
		}
		
		void						cmdStop ( void )
		{
		
			for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
			{
			
				this->engines[engineKey]->stop();
			
			}
			
		}
		
		void						cmdLaunch ( void )
		{
			
			if ( this->getReadyToLaunch() )
			{
	
				for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
				{
				
					this->engines[engineKey]->arm();
				
				}
				
				delay(3000);
				
				this->cmdRaise();
			}
		
		}
		
		void						cmdRaise ( void )
		{
		
			for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
			{
			
				this->engines[engineKey]->accelerate();
			
			}
		
		}
		
		void						cmdDescend ( void )
		{
		
			for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
			{
			
				this->engines[engineKey]->slow();
			
			}
		
		}
		
		Engine*						getEngine ( int engineId ) const
		{
		
			if ( this->getEngineIdValid(engineId) )
			{

				return this->engines[engineId];
				
			}
			
			return 0;
		
		}
		
		ABalancer*					getBalancer ( void ) const
		{
		
			return this->balancer;
		
		}
		
		bool						getEnginesReady ( void ) const
		{
	
			for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
			{
			
				if( this->engines[engineKey] == 0 )
				{
				
					return false;
					
				}
			
			}
			
			return true;
		
		}
	
		bool						getBalancerReady ( void ) const
		{
		
			return this->balancer;
		
		}
	
		bool						getReadyToLaunch ( void ) const
		{
		
			return
			(
				this->getEnginesReady()
					&&
				this->getBalancerReady()
					&&
				this->getEngine(0)->getStatus() == ENGINE_STATUS_STOPPED
					&&
				this->getEngine(1)->getStatus() == ENGINE_STATUS_STOPPED
					&&
				this->getEngine(2)->getStatus() == ENGINE_STATUS_STOPPED
					&&
				this->getEngine(3)->getStatus() == ENGINE_STATUS_STOPPED
			);
		
		}
		
	protected:
	
		void						createEngine ( int engineId , int enginePin )
		{
			
			this->engines[engineId] = new Engine(enginePin);
		
		}
	
		bool						getEngineIdValid ( int engineId ) const
		{
		
			return
			(
				engineId >= 0
					&&
				engineId < enginesNumber
			);
		
		}

		Engine*						engines[enginesNumber];

		ABalancer*					balancer;

};

class QuadroCopter;
class QuadroBalancer
	:
		public ABalancer
{

	public:
	
									QuadroBalancer ( QuadroCopter* copter , float* yawPitchRoll , float* acceleration );
	
		bool						getYawBalanced ( void ) const;
		
		bool						getPitchBalanced ( void ) const;
		
		bool						getRollBalanced ( void ) const;
		
		void						balanceYaw ( void );
		
		void						balancePitch ( void );
		
		void						balanceRoll ( void );
		
		void						stopX ( void );

		void						stopY ( void );

		void						stopZ ( void );
		
	protected:
	
		QuadroCopter*				copter;
		
};

class QuadroCopter
	:
		public ACopter<4>
{

	public:
	
		void						createEngines ( const int pin0 , const int pin1, const int pin2, const int pin3 );
		
		void						createBalancer ( float* yawPitchRoll , float* acceleration );
		
};
