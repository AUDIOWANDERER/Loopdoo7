#include <MozziGuts.h>
#include <Sample.h> // Sample template
#include <samples/loop1.h>

#include <LowPassFilter.h>

#define A_SAMPLE_RATE 16000

#define CONTROL_RATE 64

LowPassFilter lpf;

// use: Sample <table_size, update_rate> SampleName (wavetable)
Sample <loop1_table_NUM_CELLS, A_SAMPLE_RATE> aSample(loop1_table_DATA);

void setup(){
  lpf.setResonance(95);

  startMozzi();
  aSample.setFreq((float) loop1_table_SAMPLERATE / (float) loop1_table_NUM_CELLS); // play at the speed it was recorded
  aSample.setLoopingOn();
}

void updateControl(){
  int knob2 = mozziAnalogRead(A0);
  int drifty = mozziAnalogRead(A1);
  int driftvalue = map(drifty, 0, 1023, 0, 12); 
  int driftRange = map(knob2, 0, 1023, 0, driftvalue); // Map knob2 value to the drift range 
  int knob1 = mozziAnalogRead(A2);
  int numValuea = 12; 
  int numValueb = 12; 
  float coefficient = 2.5;  // Adjust this value for the desired rate of decrease
  String driftStringa = "";  // String to store the values
  String driftStringb = "";  // String to store the values
  int drift1 = random(-driftRange, driftRange);// Generate a random drift value between -15 and 15
  int drift2 = random(-driftRange, driftRange);// Generate a random drift value between -15 and 15
  int drifta = pow(drift1, 2) * random (1, 10) + 30;
  int driftb = pow(drift2, 2) * random (1, 10) * 0.5;
   if (drift1 <= 0) {
  drift1 = 0.25;
}
  if (drift2 <= 0) {
  drift2 = 0.25;
}
for (int i = 0; i < numValuea; i++) {
  // Append the value to the string
  driftStringa += String(drifta);
  if (i < numValuea - 1) {
    driftStringa += ",";
  }
}
for (int i = 0; i < numValueb; i++) {
  // Append the value to the string
  driftStringb += String(driftb);
  if (i < numValueb - 1) {
    driftStringb += ",";
}
}

  byte cutoff_freq = (knob1 + drifta) >> 2; // Apply the drift to the knob1 value and range 0-255
  lpf.setCutoffFreq(cutoff_freq);
  int pitchA = mozziAnalogRead(A3);
  int mappedPitchA = map(pitchA + driftb, 0, 1023, 0, 1000); // Apply the drift to the pitchA value
  float aSampleRate = ((float)loop1_table_SAMPLERATE / (float)loop1_table_NUM_CELLS) / (1 + (mappedPitchA / 100.0));
  aSample.setFreq(aSampleRate);
}
 
int updateAudio() {
  // Read the audio sample
  int chn1 = aSample.next(); // Ass
  // Apply the low pass filter to the audio sample
  chn1 = lpf.next(chn1 >> 1);
  return chn1;
}

void loop(){
  audioHook();
}
