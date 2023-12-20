#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define FIR_FILTER_LENGTH 5

//Defining the struct to represent the circular buffer
typedef struct{
    float buff[FIR_FILTER_LENGTH]; //for circular buffer array
    uint8_t buffIndex; //Tracking the index of the circular buffer
    float out; //the output value of the circular buffer
} FIRFilter;

//functions prototype
void FIRFilter_init(FIRFilter *fir);
float FIRFilter_calc(FIRFilter *fir, float inputVal);

//define the variable in the stack memory, including declaration. The value of this
//FIR_FILTER_IMPULSE_RESPONSE array is based on a design (the value represent the behaviour of the filter)
static float FIR_FILTER_IMPULSE_RESPONSE[FIR_FILTER_LENGTH]={0.4,0.3,0.2,0.1,0.05}; 

//function to initialise the circular buffer value
void FIRFilter_init(FIRFilter *fir){ //use pointer to FIRFilter variable so that we do not need to copy the memory value (more efficient)
    //clear the buffer of the filter
    for(int i=0;i<FIR_FILTER_LENGTH;i++){
        fir->buff[i]=0.0f;
    }

    //Reset the buffer index
    fir->buffIndex=0;

    //clear filter output
    fir->out=0.0f; //'f' to make is as a float
}

//function to calculate (process) the filter output
float FIRFilter_calc(FIRFilter *fir, float inputVal){

    float out=0.0f;

    /*Implementing CIRCULAR BUFER*/
    //Store the latest sample=inputVal into the circular buffer
    fir->buff[fir->buffIndex]=inputVal;

    //Increase the buffer index. retrun to zero if it reach the end of the index (circular buffer)
    fir->buffIndex++;
    uint8_t sumIndex=fir->buffIndex;
    if(fir->buffIndex==FIR_FILTER_LENGTH){
        fir->buffIndex=0;
    }

    //Compute the filtered sample with convolution
    fir->out=0.0f;
    for(int i=0;i<FIR_FILTER_LENGTH;i++){
        //decrese sum index and warp it if necessary
        if(sumIndex>0){
            sumIndex--;
        }
        else{
            sumIndex=FIR_FILTER_LENGTH-1;
        }
        //The convolution process: Multyply the impulse response with the SHIFTED input sample and add it to the output
        fir->out=fir->out+fir->buff[sumIndex]*FIR_FILTER_IMPULSE_RESPONSE[i];
    }

    //return the filtered data
    return out;

}

int main(int argv, char* argc[]){   
    
    //////////////////// Generating noisy data to be filtered //////////////////////////////////////
    int n=100;
    float input[n];
    float filteredInput[n];

    //Generate the input values
    //const float pi=3.14159265358979;
    //const float f=2.5; //Hz
    for(int i=0;i<n;i++){   
        //float min=0.0f;
        //float max=0.3f; 
        //float noise=min+((float)rand()/(float)RAND_MAX)*(max-min);
        float noise = rand() % 800 + 7;
        input[i]=noise; //one cycle t=i/n=0 to 1     
    }

    
    //////////////////////// FIR filter with circular buffer//////////////////////////////////////
    
    //Declaring the filter struct variable
    FIRFilter fir;

    //Initialise the filter coefficient (the weight)
    FIRFilter_init(&fir);

    //Calculating the filtered values
    for(int i=0;i<n;i++){
        FIRFilter_calc(&fir, input[i]);
        filteredInput[i]=fir.out;
    }
    
 


    for(int i=0;i<n;i++){
        printf("%u\n", i );

        printf( "%6.4lf\n", input[i] );
        printf("%6.4lf\n", filteredInput[i]);
    }
  
}
    
    
