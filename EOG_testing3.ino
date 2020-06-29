const int win =2;
const int f = 100;
int n=0;
float period = 1000.0/f;
float sig_vert [win], sig_hor [win];
unsigned long int t = 0;
float filt_sig_vert[win], filt_sig_hor[win];
float threshold_vert = 0.6, threshold_hor = 0.4;
float deriv_sig_vert;
float deriv_sig_hor;
int derivWidth = 5;
char dir = 's';
float conversionFactor = (5.0/1023);
// bits to voltage conversion
float baseline = 2.5;
unsigned long int primed_vert[] = {0,0};
unsigned long int primed_hor[] = {0,0};
unsigned long int timeout = 500;

int R=5, G=6, B=7;


void filterSig (int a[], int b[], int sizA);
float Average (float a[]);
float Derivative (float b[], int sizA);
float Peaks(float c[], int sizA);

float deriv_sig[win];
float peak_trough[win];
int p = 0;
int a = 1;
int large;
int deriv =0;

void setup() {
    t = millis();
    Serial.begin(115200);
    derivWidth = (derivWidth >= win)?win-1:derivWidth;
    pinMode(R, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(B, OUTPUT);
}

void loop() {
// Control Sampling Frequency
    while (millis()<period+t){}
    t=millis();

// Aquire new data points
    sig_vert[n]=analogRead(A0)*conversionFactor-baseline;
    sig_hor[n] = analogRead(A1)*conversionFactor-baseline;
    n++;
    if (n>=win){n=0;}

// Filter and order filtered signal arrays
    filterSig(filt_sig_vert, sig_vert, win);
    filterSig(filt_sig_hor, sig_hor, win);

// Calculate the deriviates
    deriv_sig_vert = Derivative(filt_sig_vert, win);
    deriv_sig_hor = Derivative(filt_sig_hor, win);

// Evaulate Peaks
    if (abs(deriv_sig_vert)>threshold_vert){
      if (primed_vert[(int)(deriv_sig_vert<0)]==0){
        primed_vert[(int)(deriv_sig_vert>0)] = millis();
      }else{
        if (deriv_sig_vert<0){
          dir='w'; //wheelchair goes forward
          digitalWrite(R, LOW);
          digitalWrite(G, HIGH);
          digitalWrite(B, LOW);
        }else{
          dir='s'; //wheelchair stops
          digitalWrite(R, LOW);
          digitalWrite(G, LOW);
          digitalWrite(B, LOW);
        }
      }
    }else if(abs(deriv_sig_hor)>threshold_hor){
      if (primed_hor[(int)(deriv_sig_hor<0)]==0){
        primed_hor[(int)(deriv_sig_hor>0)] = millis();
      }else{
        if (deriv_sig_hor<0){
          dir='d'; //wheelchair goes left
          digitalWrite(R, HIGH);
          digitalWrite(G, LOW);
          digitalWrite(B, LOW);
        }else{
          dir='a'; //wheelchair goes right
          digitalWrite(R, LOW);
          digitalWrite(G, LOW);
          digitalWrite(B, HIGH);
        }
      }
    }

    if (millis()-primed_vert[0] > timeout)
      primed_vert[0] = 0;
    if (millis()-primed_vert[1] > timeout)
      primed_vert[1] = 0;
    if (millis()-primed_hor[0] > timeout)
      primed_hor[0] = 0;
    if (millis()-primed_hor[1] > timeout)
      primed_hor[1] = 0;
    // resets the motor position after timeout

// Print values to screen; 
    Serial.print(threshold_vert);
    Serial.print(",");
    Serial.print(-threshold_vert);
    Serial.print(",");
    Serial.print(filt_sig_vert[win-1]);
    Serial.print(",");
    Serial.print(filt_sig_hor[win-1]);
    Serial.print(",");
    Serial.print(deriv_sig_vert);
    Serial.print(",");
    Serial.println(deriv_sig_hor);
//    Serial.print(" ");
//    Serial.println(peak_trough[n]);

}

void filterSig(float a[], float b[], int sizA){
    for (int i=0; i<sizA-1; i++){
      a[i]=a[i+1];
    }
    a[sizA-1] = Average(b, sizA);
}

float Average (float a[], int sizA){
    float sum = 0;
    for (int i=0; i<sizA; i++){
        sum += a[i];
    }
    return (sum/sizA);
}

float Derivative (float b[], int sizA){
    return (b[sizA] - b[sizA-derivWidth]);
}

float Peaks(float c[], int sizA) {
    if (c[a-1]>0 && c[a+1]< 0)
    p=100;
    else if (c[a-1]<0 && c[a+1]> 0)
    p=-100;

   else
    p=0;
  return(p);
}
