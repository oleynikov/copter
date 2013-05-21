#pragma once



namespace Copter
{



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
		
									Engine ( int pin )
										:
											speed	( 0 ),
											pin		( pin )
			{
			
				this->servo.attach(pin);
				this->setSpeed(0);
				
			}
			
			int						getPin ( void ) const
			{
			
				return this->pin;
			
			}

			int						getSpeed ( void ) const
			{
			
				return this->speed;
			
			}
			
			void					arm ( void )
			{
			
				this->setSpeed(Engine::speedArm);
			
			}
			
			void					stop ( void )
			{
			
				this->setSpeed(0);
			
			}
			
			void					acelerate ( void )
			{
			
				int speedNew;
			
				if ( this->speed == Engine::speedArm )
				{
				
					speedNew = Engine::speedMin;
					
				}
				
				else
				{
				
					speedNew = this->speed+Engine::speedDelta;
					
				}
			
				this->setSpeed(speedNew);
			
			}

			void					slow ( void )
			{
			
				this->setSpeed(this->speed-Engine::speedDelta);
			
			}
			
			const static int		speedArm = 20;
			
			const static int		speedMin = 35;
			
			const static int		speedMax = 180;
		
		private:

			void					setSpeed ( int speed )
			{
			
				if ( this->getSpeedValid(speed) )
				{
				
				
					this->servo.write(speed);
					this->speed = speed;
				
				}
			
			}
			
			bool					getSpeedValid ( int speed ) const
			{
			
				bool actionArm = this->speed == 0 && speed == Engine::speedArm;
				bool actionLaunch = this->speed == Engine::speedArm && this->speed < speed;
				bool actionSpeedChange = speed <= Engine::speedMax && speed >= Engine::speedMin;
				bool actionStop = speed == 0;
				
				return actionArm || actionLaunch || actionSpeedChange || actionStop;
			
			}

			int						pin;
			
			int						speed;
			
			Servo					servo;
			
			const static int		speedDelta = 1;
			
	};
	
	
	
	template <int enginesNumber>
	class Copter
	{
	
		public:
		
									Copter ( void )
			{
			
				this->initiateEnginesArray();
			
			}

			virtual					~Copter ( void )
			{
			
				for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
				{
				
					delete this->engines[engineKey];
				
				}
			
			}
			
			const Engine*			getEngine ( int engineId ) const
			{
			
				if ( this->getEngineIdValid(engineId) )
				{

					return this->engines[engineId];
					
				}
				
				return 0;
			
			}
			
			void					armAllEngines ( void )
			{
			
				if ( this->getEnginesReady() )
				{
			
					for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
					{
					
						this->engines[engineKey]->arm();
					
					}
					
				}
			
			}
		
			void					stopAllEngines ( void )
			{
			
				for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
				{
				
					this->engines[engineKey]->stop();
				
				}

			}
			
			void					acelerateAllEngines ( void )
			{
			
				for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
				{
				
					this->engines[engineKey]->acelerate();
				
				}

			}
			
			void					slowAllEngines ( void )
			{
			
				for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
				{
				
					this->engines[engineKey]->slow();
				
				}

			}
	
		protected:
		
			void					addEngine ( int engineId , int enginePin )
			{
				
				this->engines[engineId] = new Engine(enginePin);
			
			}
		
			virtual void			align ( void ) = 0;
		
		private:
		
			void					initiateEnginesArray ( void )
			{
			
				for ( int engineKey = 0 ; engineKey < enginesNumber ; engineKey++ )
				{
				
					this->engines[engineKey] = 0;
				
				}
			
			}
		
			bool					getEnginesReady ( void ) const
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
		
			bool					getEngineIdValid ( int engineId ) const
			{
			
				return
				(
					engineId >= 0
						&&
					engineId < enginesNumber
				);
			
			}
		
			bool					getIsAligned ( void ) const
			{
			
				//	Copter is aligned 
				return true;
			
			}
		
			Engine*					engines[enginesNumber];

	};
	
	
	
	class QuadroCopter
		:
			public Copter<4>
	{

		public:
		
			void					createEngines ( const int pin0 , const int pin1, const int pin2, const int pin3 )
			{
			
				this->addEngine(0,pin0);
				this->addEngine(1,pin1);
				this->addEngine(2,pin2);
				this->addEngine(3,pin3);

			}
			
		protected:
		
			virtual void			align ( void )
			{
			
				// Align copter
			
			}
			
	};



}
