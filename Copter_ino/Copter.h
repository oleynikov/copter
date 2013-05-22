#pragma once

#include <math.h>
#include <Servo.h>

enum COPTER_COMMANDS
{

	ENGINES_STOP = 48,
  
	ENGINES_ARM,
  
	COPTER_RAISE,
  
	COPTER_DESCEND
  
};

class Engine
{

	public:
	
									Engine ( int pin );
		
		int							getPin ( void ) const;

		int							getSpeed ( void ) const;
		
		void						arm ( void );
		
		void						stop ( void );
		
		bool						accelerate ( void );

		bool						slow ( void );
		
		const static int			SPEED_ARM = 20;
		
		const static int			SPEED_MIN = 35;
		
		const static int			SPEED_MAX = 60;
		
		const static int			SPEED_DELTA = 1;
	
	private:

		bool						setSpeed ( int speed );
		
		bool						getSpeedValid ( int speed ) const;

		int							pin;
		
		int							speed;
		
		Servo						servo;
		
};

class ABalancer
{

	public:
	
									ABalancer ( float* yawPitchRoll );
	
		virtual						~ABalancer ( void );

		void						update ( void );
		
		bool						getBalanced ( void );
		
		virtual bool				getYawBalanced ( void ) const = 0;
		
		virtual bool				getPitchBalanced ( void ) const = 0;
		
		virtual bool				getRollBalanced ( void ) const = 0;
		
		virtual void				balanceYaw ( void ) = 0;
		
		virtual void				balancePitch ( void ) = 0;
		
		virtual void				balanceRoll ( void ) = 0;

		const static float			YAW_PITCH_ROLL_ACCURACY = 0.5;
		
		const static float			ACCELERATION_ACCURACY = 0.5;
		
	protected:

		float*						yawPitchRoll;

		float						acceleration[3];

};

template <int enginesNumber>
class ACopter
{

	public:
	
									ACopter ( void )
										:
											balancer ( 0 ),
											running  ( false )
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
		
		Engine*						getEngine ( int engineId ) const
		{
		
			if ( this->getEngineIdValid(engineId) )
			{

				return this->engines[engineId];
				
			}
			
			return 0;
		
		}
		
		void						armAllEngines ( void )
		{
		
			if ( this->getReadyToLaunch() )
			{
		
				for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
				{
				
					this->engines[engineKey]->arm();
				
				}
				
			}
		
		}
	
		void						stopAllEngines ( void )
		{
		
			for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
			{
			
				this->engines[engineKey]->stop();
			
			}

		}
		
		void						accelerateAllEngines ( void )
		{
		
			for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
			{
			
				this->engines[engineKey]->accelerate();
			
			}

		}
		
		void						slowAllEngines ( void )
		{
		
			for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
			{
			
				this->engines[engineKey]->slow();
			
			}

		}

		void						updateTelemetry ( void )
		{
		
			if
			(
			  this->getEngine(0)->getSpeed() <= Engine::SPEED_MIN
				&& 
			  this->getEngine(1)->getSpeed() <= Engine::SPEED_MIN
				&& 
			  this->getEngine(2)->getSpeed() <= Engine::SPEED_MIN
				&& 
			  this->getEngine(3)->getSpeed() <= Engine::SPEED_MIN
			 )
			 {
			  return; 
			 }
		
			this->balancer->update();
		
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
			);
		
		}
		
		bool						getBalanced ( void ) const
		{
		
			if ( this->balancer )
			{
			
				return this->balancer->getBalanced();
				
			}
			
			return false;
		
		}
		
	protected:
	
		void						addEngine ( int engineId , int enginePin )
		{
			
			this->engines[engineId] = new Engine(enginePin);
		
		}
	
		void						addBalancer ( ABalancer* balancer )
		{
		
			this->balancer = balancer;
		
		}
		
	private:
	
		bool						getEngineIdValid ( int engineId ) const
		{
		
			return
			(
				engineId >= 0
					&&
				engineId < enginesNumber
			);
		
		}
	
		bool						running;
	
		Engine*						engines[enginesNumber];

		ABalancer*					balancer;

};

class QuadroCopter;
class QuadroBalancer
	:
		public ABalancer
{

	public:
	
									QuadroBalancer ( QuadroCopter* copter , float* yawPitchRoll );
	
		bool						getYawBalanced ( void ) const;
		
		bool						getPitchBalanced ( void ) const;
		
		bool						getRollBalanced ( void ) const;
		
		void						balanceYaw ( void );
		
		void						balancePitch ( void );
		
		void						balanceRoll ( void );

	private:
	
		QuadroCopter*				copter;
		
};

class QuadroCopter
	:
		public ACopter<4>
{

	public:
	
		void						createEngines ( const int pin0 , const int pin1, const int pin2, const int pin3 );
		
		void						createBalancer ( float* yawPitchRoll );
		
};
