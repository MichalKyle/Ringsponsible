typedef uint8_t timer_ticks;
const uint16_t SCH_PERIOD_MICRO_SEC = 10000;
const uint16_t SCH_PERIOD = SCH_PERIOD_MICRO_SEC * 16; // 16 MHz clock so 800 ticks is 50us
const uint8_t MAX_RM_FUNCTIONS = 5;

struct RMFunction
{
public:
  timer_ticks deadline;
  timer_ticks period;
  timer_ticks nextArrival;
  bool isRunning;
  bool shouldRun;
  void(*function)(void);

  RMFunction():period(0), nextArrival(0), isRunning(false), shouldRun(false) {};
};

class RMScheduler
{
private:
  timer_ticks currTimerTicks;
  timer_ticks nextArrival;
  RMFunction functions[MAX_RM_FUNCTIONS];

  uint8_t functions_length()
  {
    uint8_t count = 0;

    for (uint8_t i = 0; i < MAX_RM_FUNCTIONS; ++i)
    {
      if (functions[i].period != 0)
      {
        ++count;
      }
    }

    return count;
  }
public:
  RMScheduler() : currTimerTicks(0), nextArrival(0) {};

  bool addNewFunction(uint32_t periodInMicroseconds, void(*callback)(void), uint32_t relativeDeadline = 0)
  {
    timer_ticks periodInTimerTicks = (periodInMicroseconds * 16) / (SCH_PERIOD+1);
    timer_ticks deadlineInTimerTicks = (relativeDeadline * 16) / (SCH_PERIOD+1);

    if (relativeDeadline == 0)
    {
      deadlineInTimerTicks = periodInTimerTicks;
    }

    uint8_t i = 0;
    if (functions_length() != 0)
    {
      while (functions[i].period != 0 && i < MAX_RM_FUNCTIONS && functions[i].deadline < deadlineInTimerTicks)
        i++;
      if (i == MAX_RM_FUNCTIONS || functions_length() == MAX_RM_FUNCTIONS)
        return false;
      for (uint8_t j = MAX_RM_FUNCTIONS - 2; j >= i; j--)
        int k = 0;
        //functions[j + 1] = functions[j];
    }
    functions[i].function = callback;
    functions[i].period = periodInTimerTicks;
    functions[i].deadline = deadlineInTimerTicks;
    functions[i].nextArrival = 0;

    if (nextArrival > functions[i].nextArrival)
    {
      nextArrival = functions[i].nextArrival;
    }
    
    return true;
  }


  void incTicks() 
  {
    digitalWrite(0, HIGH);
    currTimerTicks++;

    if (currTimerTicks == nextArrival)
    {
      timer_ticks currArrivalTime = nextArrival;
      currTimerTicks = 0;
      nextArrival = 0;
      --nextArrival; // Rolls over to the latest possible arrival

      // Search for the runnable functions
      uint8_t i = 0;
      while (functions[i].period != 0 && i < MAX_RM_FUNCTIONS)
      {
        functions[i].nextArrival -= currArrivalTime;

        if (functions[i].nextArrival == 0)
        {
          functions[i].nextArrival = functions[i].period;
          functions[i].shouldRun = true;
        }
        
        if (functions[i].nextArrival < nextArrival)
        {
          nextArrival = functions[i].nextArrival;
        }

        ++i;
      }
      digitalWrite(0, LOW);
      tryRunFunctions();
    }
    digitalWrite(0, LOW);
  }

  void tryRunFunctions()
  {
    uint8_t i = 0;
    while (functions[i].period != 0 && i < MAX_RM_FUNCTIONS)
    {
      if (functions[i].shouldRun)
      {
        functions[i].shouldRun = false;
        if (functions[i].isRunning)
        {
          // ERROR, function did not finish before it's deadline.
          // In this case continue with the previous iteration until it is finished and skip this period.
        }
        else
        {
          uint8_t j = 0;
          while (functions[j].period != 0 && j < MAX_RM_FUNCTIONS)
          {
            digitalWrite(j+12, LOW);
            ++j;
          }
          
          functions[i].isRunning = true;
          digitalWrite(i+12, HIGH);
          (*functions[i].function)();
          digitalWrite(i+12, LOW);
          functions[i].isRunning = false;

          j=0;
          while (functions[j].period != 0 && j < MAX_RM_FUNCTIONS)
          {
            if (functions[j].isRunning)
              digitalWrite(j+12, HIGH);

             ++j;
          }
        }
      }
      else if (functions[i].isRunning)
      {
        return;
      }
      
      i++;
    }
  }
};

RMScheduler gTheScheduler;

void task_one()
{
  delay(100);
}

void task_two()
{
  task_one();
  task_one();
}

void setup() {
  
  gTheScheduler.addNewFunction(10000000, task_one);
  gTheScheduler.addNewFunction(60000000, task_two);
  
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1H = 0;
  TCNT1L = 0;

  OCR1A = SCH_PERIOD; // 16 MHz clock so 800 ticks is 50us
  OCR1B = SCH_PERIOD >> 8;

  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << WGM12); // CTC mode
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupts

  interrupts();             // enable all interrupts
  
  //gTheScheduler.addNewFunction(4000, task_two);
}

ISR(TIMER1_COMPA_vect)
{
  interrupts();
  gTheScheduler.incTicks();
}

void loop() {
  // put your main code here, to run repeatedly:
  while(true)
  {
    
  };
}
