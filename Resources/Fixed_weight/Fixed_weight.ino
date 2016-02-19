/******************************************************************
 * Include necessary libraries
 ******************************************************************/
#include <math.h>
#include "NewPing.h"
/******************************************************************
 * Network Configuration - customized per network 
 ******************************************************************/
const int PatternCount =1;
const int InputNodes = 5;
const int HiddenNodes = 10;
const int OutputNodes = 4;
const float LearningRate = 0.3;
const float Momentum = 0.9;
const float InitialWeightMax = 0.5;
const float Success = 0.0001;
float Input[PatternCount][InputNodes];
float Target[PatternCount][OutputNodes];
/******************************************************************
 * Get distance and calculate for input
 ******************************************************************/
#define MAX_DISTANCE 400 
 
  double leftDistance, frontDistance, rightDistance,leftdownDistance,rightdownDistance;
  int lefttarget,righttarget,fronttarget,backtarget;
  
  int leftMotor[] = {2, 3};
  int rightMotor[] = {5, 4};
 
NewPing leftSonar(9, 8, MAX_DISTANCE);
NewPing frontSonar (28, 29, MAX_DISTANCE);
NewPing rightSonar (13, 12, MAX_DISTANCE);
NewPing rightdownSonar (50, 51, MAX_DISTANCE);
NewPing leftdownSonar (52, 53, MAX_DISTANCE);

void updateDistance()
{
    Serial.println("Updating distances: ");
    leftDistance = leftSonar.ping() / US_ROUNDTRIP_CM;
    frontDistance = frontSonar.ping() / US_ROUNDTRIP_CM;
    rightDistance = rightSonar.ping() / US_ROUNDTRIP_CM;
    leftdownDistance = leftdownSonar.ping() / US_ROUNDTRIP_CM;
    rightdownDistance = rightdownSonar.ping() / US_ROUNDTRIP_CM;

    Serial.println("Front: " + String(frontDistance) + " cm");
    Serial.println("Left: " + String(leftDistance) + " cm");
    Serial.println("Right: " + String(rightDistance) + " cm");
    Serial.println("Leftdown: " + String(leftdownDistance) + " cm");
    Serial.println("Rightdown: " + String(rightdownDistance) + " cm");
}
void calculateinputdistance()
{
  updateDistance();
  if(leftdownDistance<10 && rightdownDistance<10)
    {
      if(frontDistance>30)
     { 
      lefttarget=0;
      righttarget=0;
      fronttarget=1;
      backtarget=0;
      }
    else if(frontDistance<30)
    {
      if(leftDistance>rightDistance)
      { 
      lefttarget=1;
      righttarget=0;
      fronttarget=0;
      backtarget=0;
      }
    else if(leftDistance<rightDistance)
    { 
      lefttarget=0;
      righttarget=1;
      fronttarget=0;
      backtarget=0;
      }
    }
    }
   else if(leftdownDistance<10 && rightdownDistance>10)
    {lefttarget=0;
      righttarget=1;
      fronttarget=0;
      backtarget=0;  
    }
   else if(leftdownDistance>10 && rightdownDistance<10)
    {
      lefttarget=0;
      righttarget=1;
      fronttarget=0;
      backtarget=0;
    }
   else if (leftdownDistance>10 && rightdownDistance>10)
   {
      lefttarget=0;
      righttarget=1;
      fronttarget=0;
      backtarget=0;
   } 
   }
void inputtarget()
{
  calculateinputdistance();
  
  Input[0][0] = leftDistance;
  Input[0][1] = frontDistance;
  Input[0][2] = rightDistance;
  Input[0][3] = leftdownDistance;
  Input[0][4] = rightdownDistance;
  
  Target[0][0] = lefttarget;
  Target[0][1] = fronttarget;
  Target[0][2] = righttarget;
  Target[0][3] = backtarget;
  
  
 // Input[PatternCount][InputNodes]={left,right,front};
  //Target[PatternCount][OutputNodes]={left,right,front};
  
}

/******************************************************************
 * End Network Configuration
 ******************************************************************/
int i, j, p, q, r;
int ReportEvery1000;
int RandomizedIndex[PatternCount];
long  TrainingCycle;
float Rando;
float Error;
float Accum;


float Hidden[HiddenNodes];
float Output[OutputNodes];
float HiddenWeights[InputNodes+1][HiddenNodes];
float OutputWeights[HiddenNodes+1][OutputNodes];
float HiddenDelta[HiddenNodes];
float OutputDelta[OutputNodes];
float ChangeHiddenWeights[InputNodes+1][HiddenNodes];
float ChangeOutputWeights[HiddenNodes+1][OutputNodes];

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(3));
  ReportEvery1000 = 1;
  for( p = 0 ; p < PatternCount ; p++ ) {    
    RandomizedIndex[p] = p ;
  } 
  for (int i = 0; i < 2; i++){
        pinMode(leftMotor[i], OUTPUT);
        pinMode(rightMotor[i], OUTPUT);  
  }}
void toTerminal()
{

  for( p = 0 ; p < PatternCount ; p++ ) { 
    Serial.println(); 
    Serial.print ("  Training Pattern: ");
    Serial.println (p);      
    Serial.print ("  Input ");
    for( i = 0 ; i < InputNodes ; i++ ) {
      Serial.print (Input[p][i], DEC);
      Serial.print (" ");
    }
    Serial.print ("  Target ");
    for( i = 0 ; i < OutputNodes ; i++ ) {
      Serial.print (Target[p][i], DEC);
      Serial.print (" ");
    }
/******************************************************************
* Compute hidden layer activations
******************************************************************/

    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[InputNodes][i] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += Input[p][j] * HiddenWeights[j][i] ;
      }
      Hidden[i] = 1.0/(1.0 + exp(-Accum)) ;
    }

/******************************************************************
* Compute output layer activations and calculate errors
******************************************************************/

    for( i = 0 ; i < OutputNodes ; i++ ) {    
      Accum = OutputWeights[HiddenNodes][i] ;
      for( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum += Hidden[j] * OutputWeights[j][i] ;
      }
      Output[i] = 1.0/(1.0 + exp(-Accum)) ; 
    }
    Serial.print ("  Output ");
    for( i = 0 ; i < OutputNodes ; i++ ) {       
      Serial.print (Output[i], 5);
      Serial.print (" ");
    }
  }
}  

void loop (){

inputtarget();
/******************************************************************
* Initialize HiddenWeights and ChangeHiddenWeights 
******************************************************************/
HiddenWeights[0][0]=-0.123846022463959;
HiddenWeights[0][1]=-0.00943216170181901;
HiddenWeights[0][2]=0.0529408461862648;
HiddenWeights[0][3]=-0.123864861930569;
HiddenWeights[0][4]=0.0113203978845675;
HiddenWeights[0][5]=-0.0209666967752174;
HiddenWeights[0][6]=0.260817369960935;
HiddenWeights[0][7]=0.0291801603463181;
HiddenWeights[0][8]=0.0687477758262452;
HiddenWeights[0][9]=-0.864956860786283;

HiddenWeights[1][0]=0.179099982170336;
HiddenWeights[1][1]=0.190785920164527;
HiddenWeights[1][2]=-0.0744938157672159;
HiddenWeights[1][3]=0.179125725651186;
HiddenWeights[1][4]=-0.212801209888234;
HiddenWeights[1][5]=0.0875463811271163;
HiddenWeights[1][6]=-0.132303846236798;
HiddenWeights[1][7]=-0.0852946696659487;
HiddenWeights[1][8]=-0.0188061560562580;
HiddenWeights[1][9]=-0.0188061560562580;

HiddenWeights[2][0]=-0.00476675710696819;
HiddenWeights[2][1]=-0.0541091777140061;
HiddenWeights[2][2]=0.00695561747720111;
HiddenWeights[2][3]=-0.00476836570700222;
HiddenWeights[2][4]=0.0574879315133481;
HiddenWeights[2][5]=0.314365219824837;
HiddenWeights[2][6]=0.0667069941885243;
HiddenWeights[2][7]=0.0445408186051293;
HiddenWeights[2][8]=-0.0460329218589721;
HiddenWeights[2][9]=0.00258787072128847;


HiddenWeights[3][0]=0.272066753019938;
HiddenWeights[3][1]=0.850182470535221;
HiddenWeights[3][2]=0.209714254537584;
HiddenWeights[3][3]=-0.424980325760586;
HiddenWeights[3][4]=-0.242110712630647;
HiddenWeights[3][5]=0.0126711577044135;
HiddenWeights[3][6]=-0.0241007773642907;
HiddenWeights[3][7]=0.122445369549045;
HiddenWeights[3][8]=0.0949081343997307;
HiddenWeights[3][9]=0.531629709341710;

HiddenWeights[4][0]=-0.226277786889064;
HiddenWeights[4][1]=-1.07942633839939;
HiddenWeights[4][2]=0.00374740660295069;
HiddenWeights[4][3]=-0.226097517221839;
HiddenWeights[4][4]=1.12138471351235;
HiddenWeights[4][5]=-0.958152167190698;
HiddenWeights[4][6]=-0.599777076292437;
HiddenWeights[4][7]=-0.0936665737257557;
HiddenWeights[4][8]=-0.0527183458031945;
HiddenWeights[4][9]=2.26661910952386;

  
/******************************************************************
* Initialize OutputWeights and ChangeOutputWeights
******************************************************************/
OutputWeights[0][0]=-0.500056800850571;
OutputWeights[0][1]=0.500057086503142;
OutputWeights[0][2]=2.43680532615379e-07;
OutputWeights[0][3]=-5.29333104318573e-07;

OutputWeights[1][0]=0.499975812510385;
OutputWeights[1][1]=-0.499976500989936;
OutputWeights[1][2]=-0.500000126618779;
OutputWeights[1][3]=0.500000815098330;

OutputWeights[2][0]=0.000139421696241365;
OutputWeights[2][1]=-0.000139147698381058;
OutputWeights[2][2]=-1.74941539084963e-07;
OutputWeights[2][3]=-9.90563211608096e-08;

OutputWeights[3][0]=0.500074842730003;
OutputWeights[3][1]=-0.500074650708225;
OutputWeights[3][2]=-2.99427916670697e-07;
OutputWeights[3][3]=1.07406138182355e-07;

OutputWeights[4][0]=0.499993545348272;
OutputWeights[4][1]=-0.499941196329069;
OutputWeights[4][2]=-0.500052642014435;
OutputWeights[4][3]=0.500000292995232;

OutputWeights[5][0]=-2.00765286920690e-07;
OutputWeights[5][1]=0.500052648036074;
OutputWeights[5][2]=-0.500052433776244;
OutputWeights[5][3]=-1.34945432432261e-08;

OutputWeights[6][0]=6.41759346347621e-06;
OutputWeights[6][1]=-0.500059665715155;
OutputWeights[6][2]=0.500053023157881;
OutputWeights[6][3]=2.24963810352230e-07;

OutputWeights[7][0]=-6.45664492142652e-05;
OutputWeights[7][1]=6.46381440627136e-05;
OutputWeights[7][2]=-2.08508840804720e-07;
OutputWeights[7][3]=1.36813992286779e-07;

OutputWeights[8][0]=-5.76519959392238e-05;
OutputWeights[8][1]=5.76846518190429e-05;
OutputWeights[8][2]=7.46977759354211e-08;
OutputWeights[8][3]=-1.07353655702976e-07;

OutputWeights[9][0]=-0.499994065766919;
OutputWeights[9][1]=-5.91485259262892e-05;
OutputWeights[9][2]=0.500052981564082;
OutputWeights[9][3]=2.32728762916888e-07;
 
  Serial.println("Initial/Untrained Outputs: ");
  toTerminal();
/******************************************************************
* Begin training 
******************************************************************/

  for( TrainingCycle = 1 ; TrainingCycle < 2147483647 ; TrainingCycle++) 
  {    

/******************************************************************
* Randomize order of training patterns
******************************************************************/

    for( p = 0 ; p < PatternCount ; p++) {
      q = random(PatternCount);
      r = RandomizedIndex[p] ; 
      RandomizedIndex[p] = RandomizedIndex[q] ; 
      RandomizedIndex[q] = r ;
    }
    Error = 0.0 ;
/******************************************************************
* Cycle through each training pattern in the randomized order
******************************************************************/
    for( q = 0 ; q < PatternCount ; q++ ) {    
      p = RandomizedIndex[q];

/******************************************************************
* Compute hidden layer activations
******************************************************************/

      for( i = 0 ; i < HiddenNodes ; i++ ) {    
        Accum = HiddenWeights[InputNodes][i] ;
        for( j = 0 ; j < InputNodes ; j++ ) {
          Accum += Input[p][j] * HiddenWeights[j][i] ;
        }
        Hidden[i] = 1.0/(1.0 + exp(-Accum)) ;
      }

/******************************************************************
* Compute output layer activations and calculate errors
******************************************************************/

      for( i = 0 ; i < OutputNodes ; i++ ) {    
        Accum = OutputWeights[HiddenNodes][i] ;
        for( j = 0 ; j < HiddenNodes ; j++ ) {
          Accum += Hidden[j] * OutputWeights[j][i] ;
        }
        Output[i] = 1.0/(1.0 + exp(-Accum)) ;   
        OutputDelta[i] = (Target[p][i] - Output[i]) * Output[i] * (1.0 - Output[i]) ;   
        Error += 0.5 * (Target[p][i] - Output[i]) * (Target[p][i] - Output[i]) ;
      }

/******************************************************************
* Backpropagate errors to hidden layer
******************************************************************/

      for( i = 0 ; i < HiddenNodes ; i++ ) {    
        Accum = 0.0 ;
        for( j = 0 ; j < OutputNodes ; j++ ) {
          Accum += OutputWeights[i][j] * OutputDelta[j] ;
        }
        HiddenDelta[i] = Accum * Hidden[i] * (1.0 - Hidden[i]) ;
      }


/******************************************************************
* Update Inner-->Hidden Weights
******************************************************************/


      for( i = 0 ; i < HiddenNodes ; i++ ) {     
        ChangeHiddenWeights[InputNodes][i] = LearningRate * HiddenDelta[i] + Momentum * ChangeHiddenWeights[InputNodes][i] ;
        HiddenWeights[InputNodes][i] += ChangeHiddenWeights[InputNodes][i] ;
        for( j = 0 ; j < InputNodes ; j++ ) { 
          ChangeHiddenWeights[j][i] = LearningRate * Input[p][j] * HiddenDelta[i] + Momentum * ChangeHiddenWeights[j][i];
          HiddenWeights[j][i] += ChangeHiddenWeights[j][i] ;
        }
      }

/******************************************************************
* Update Hidden-->Output Weights
******************************************************************/

      for( i = 0 ; i < OutputNodes ; i ++ ) {    
        ChangeOutputWeights[HiddenNodes][i] = LearningRate * OutputDelta[i] + Momentum * ChangeOutputWeights[HiddenNodes][i] ;
        OutputWeights[HiddenNodes][i] += ChangeOutputWeights[HiddenNodes][i] ;
        for( j = 0 ; j < HiddenNodes ; j++ ) {
          ChangeOutputWeights[j][i] = LearningRate * Hidden[j] * OutputDelta[i] + Momentum * ChangeOutputWeights[j][i] ;
          OutputWeights[j][i] += ChangeOutputWeights[j][i] ;
        }
      }
    }

/******************************************************************
* Every 1000 cycles send data to terminal for display
******************************************************************/
    ReportEvery1000 = ReportEvery1000 - 1;
    if (ReportEvery1000 == 0)
    {
      Serial.println(); 
      Serial.println(); 
      Serial.print ("TrainingCycle: ");
      Serial.print (TrainingCycle);
      Serial.print ("  Error = ");
      Serial.println (Error, 5);

      toTerminal();

      if (TrainingCycle==1)
      {
        ReportEvery1000 = 999;
      }
      else
      {
        ReportEvery1000 = 1000;
      }
    }    
/******************************************************************
* If error rate is less than pre-determined threshold then end
******************************************************************/
    if( Error < Success ) break ;  
  }
  Serial.println ();
  Serial.println(); 
  Serial.print ("TrainingCycle: ");
  Serial.print (TrainingCycle);
  Serial.print ("  Error = ");
  Serial.println (Error, 5);

  toTerminal();

  Serial.println ();  
  Serial.println ();
  Serial.println ("Training Set Solved! ");
  Serial.println ("--------");
  Serial.println ();
  Serial.println (); 
  
 /* for( i = 0 ; i < HiddenNodes ; i++ )
  {
    for( j = 0 ; j < InputNodes ; j++ )
    {
      Serial.println(HiddenWeights[j][i]);
    }} */
  ReportEvery1000 = 1;
  double O=max(max(Output[0],Output[1]),max(Output[2],Output[3]));
  Serial.println(O);
  if(O==Output[0])
  Serial.println("Target1");
 else if(O==Output[1])
  Serial.println("Target2");
 else if(O==Output[2])
  Serial.println("Target3");
 else if(O==Output[3])
  Serial.println("Target4");
}


