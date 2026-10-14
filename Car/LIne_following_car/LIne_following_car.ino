/*
# Line Following Robot car

## Project Overview
This project is a line-following robot built using an Arduino Nano, an L298N motor driver, two DC gear motors, and a 5-channel IR sensor array.

The robot detects a black line on a white surface and adjusts its movement based on the sensor readings.

## Hardware Components
- Arduino Nano
- L298N Motor Driver
- 5-Channel IR Line Tracking Sensor
- 2 DC Gear Motors
- Robot Car Chassis and Wheels
- 2 × 18650 Batteries

## Pin Configuration

### L298N Motor Driver
| Arduino Pin | L298N Connection |
|-------------|------------------|
|     D7      |      IN1         |
|     D8      |      IN2         |
|     D9      |      IN3         |
|     D10     |      IN4         |
|     D5      |      ENA         |
|     D6      |      ENB         |
----------------------------------

### IR Sensor Array

| Sensor | Arduino Pin |
|--------|-------------|
|   S1   |    D2       |
|   S2   |    D3       |
|   S3   |    D4       |
|   S4   |    A0       |
|   S5   |    A1       |
------------------------

## Features
- Black-line detection on a white surface
- Forward movement and directional turning
- Adjustable motor speed using PWM
- Reverse movement when the line is lost
- Direction-based rotation to search for the line
- Configurable reverse and search timeouts

## Line Recovery Logic
When all five sensors detect white, the robot enters line-search mode. It first moves backward for a configured period, then rotates according to the last known line direction.

If a sensor detects black again, the search mode ends and normal line-following resumes.

A search timeout stops the robot if the line cannot be found within the configured time.

## Important Notes
- Sensor logic: Black = LOW (0), White = HIGH (1).
- `BACK_TIME` controls the reverse duration.
- `ROTATE_TIMEOUT` sets the maximum rotation-search duration.
- Search behavior and motor speeds may require tuning for different tracks.
- The current implementation should be tested for intersections, wide black lines, and sharp curves.

## Future Improvements
- PID-based line-following control
- Better handling of intersections and sharp turns
- Improved line-loss recovery
- Wheel encoders for speed and distance measurement

## Project Status
Development and testing in progress.
*/

int In1 = 7;
int In2 = 8;
int In3 = 9;
int In4 = 10;

int Ena = 5;
int Enb = 6;

// Sensor pins
int S1 = 2;
int S2 = 3;
int S3 = 4;
int S4 = A0;
int S5 = A1;

int s1, s2, s3, s4, s5;

// -1 = Left, 1 = Right
int lastDirection = 1;

bool searching = false;
bool backingUp = false;
bool searchFailed = false;

unsigned long backStartTime = 0;
unsigned long searchStartTime = 0;

const unsigned long BACK_TIME = 180;
const unsigned long ROTATE_TIMEOUT = 4000;

void setup()
{
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);

  pinMode(Ena, OUTPUT);
  pinMode(Enb, OUTPUT);

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  Serial.begin(9600);

  stop();
  delay(3000);
}

void loop()
{
  s1 = digitalRead(S1);
  s2 = digitalRead(S2);
  s3 = digitalRead(S3);
  s4 = digitalRead(S4);
  s5 = digitalRead(S5);

  Serial.print(s1); Serial.print(" ");
  Serial.print(s2); Serial.print(" ");
  Serial.print(s3); Serial.print(" ");
  Serial.print(s4); Serial.print(" ");
  Serial.println(s5);

  // Stop if the search safety timeout was reached
  if (searchFailed)
  {
    stop();
    return;
  }

  // LINE SEARCH MODE
  if (searching)
  {
    // Any sensor detects black: line found again
    if (s1 == 0 || s2 == 0 || s3 == 0 ||
        s4 == 0 || s5 == 0)
    {
      searching = false;
      backingUp = false;
      stop();
      return;
    }

    // Step 1: Move backward briefly
    if (backingUp)
    {
      back();

      if (millis() - backStartTime >= BACK_TIME)
      {
        backingUp = false;
        searchStartTime = millis();
      }

      return;
    }

    // Step 2: Rotate in the last known direction
    if (lastDirection == -1)
    {
      left();
    }
    else
    {
      right();
    }

    // Safety timeout
    if (millis() - searchStartTime >= ROTATE_TIMEOUT)
    {
      stop();
      searching = false;
      searchFailed = true;
    }

    return;
  }

  // Center sensor detects black
  if (s1 == 1 && s2 == 1 && s3 == 0 &&
      s4 == 1 && s5 == 1)
  {
    forward();
  }

  // Left side
  else if ((s1 == 1 && s2 == 1 && s3 == 1 &&
            s4 == 0 && s5 == 1) ||
           (s1 == 1 && s2 == 1 && s3 == 1 &&
            s4 == 1 && s5 == 0))
  {
    lastDirection = -1;
    left();
  }

  // Right side
  else if ((s1 == 1 && s2 == 0 && s3 == 1 &&
            s4 == 1 && s5 == 1) ||
           (s1 == 0 && s2 == 1 && s3 == 1 &&
            s4 == 1 && s5 == 1))
  {
    lastDirection = 1;
    right();
  }

  // All white: line lost
  else if (s1 == 1 && s2 == 1 && s3 == 1 &&
           s4 == 1 && s5 == 1)
  {
    searching = true;
    backingUp = true;
    backStartTime = millis();
    back();
  }

  // All black: stop for safety
  else if (s1 == 0 && s2 == 0 && s3 == 0 &&
           s4 == 0 && s5 == 0)
  {
    stop();
  }

  // Other mixed patterns
  else
  {
    forward();
  }
}

// function call

// Ena, Enb motor power

void forward()
{
  analogWrite(Ena, 145);
  analogWrite(Enb, 145);

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}

void back()
{
  analogWrite(Ena, 105);
  analogWrite(Enb, 105);

  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
}

void left()
{
  analogWrite(Ena, 150);
  analogWrite(Enb, 150);

  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}

void right()
{
  analogWrite(Ena, 150);
  analogWrite(Enb, 150);

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
}

void stop()
{
  analogWrite(Ena, 0);
  analogWrite(Enb, 0);

  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}
