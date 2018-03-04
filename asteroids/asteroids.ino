#include <Arduboy2.h>
#include <stdio.h>
#include <math.h>

Arduboy2 arduboy;

int game_state = 0;
int buttonA_pressed = 0;
int buttonB_pressed = 0;
int playerx[3];
int playery[3];
int playerx_vel[3];
int playery_vel[3];
int asteroidsx[5];
int asteroidsy[5];
int asteroidsr[5];
int asteroidsflag[5];
int asteroidsdx[5];
int asteroidsdy[5];
int missile[10][3];
int xvel[10] = {0};
int yvel[10] = {0};

void setup(){
  // put your setup code here, to run once:
  arduboy.begin();
  srand(7/8);
  arduboy.setFrameRate(20);
  playerx[0] = 1;
  playerx[1] = 10;
  playerx[2] = 5;
  playery[0] = 1;
  playery[1] = 1;
  playery[2] = 9;
  //set asteroids positions 
  for(int i = 0; i < 5; i++){
    asteroidsx[i] = rand() % 127+20;
    asteroidsy[i] = rand() % 63+20;
    asteroidsr[i] = rand() % 10 + 3;
    asteroidsflag[i] = 0;
  }
  for(int i = 0; i < 5; i++){
    if( (rand() % 100) < 50){
      asteroidsdx[i] = -1;
      asteroidsdy[i] = -1;
    }else{
      asteroidsdx[i] = 1;
      asteroidsdy[i] = 1;  
    }
  }
  for(int i = 0; i < 10; i++){
    missile[i][0] = 0;
    missile[i][1] = 0;
    missile[i][2] = 0;
  }
  arduboy.clear();
  
}

void rotate_main_character(){
  //this function turns the main character 90 degreees clockwise
    int p,p2,p3,q,q2,q3,a1,a2,a3,b1,b2,b3;
    float angle;
    angle = 45*(3.14/180);
    p = playerx[0];
    q = playery[0];
    a1 = p + (playerx[0] - p) * cos(angle) - (playery[0] - q) * sin(angle);
    b1 = q + (playerx[0] - p) * sin(angle) + (playery[0] - q) * cos(angle);
    a2 = p + (playerx[1] - p) * cos(angle) - (playery[1] - q) * sin(angle);
    b2 = q + (playerx[1] - p) * sin(angle) + (playery[1] - q) * cos(angle);
    a3 = p + (playerx[2] - p) * cos(angle) - (playery[2] - q) * sin(angle);
    b3 = q + (playerx[2] - p) * sin(angle) + (playery[2] - q) * cos(angle);
    playerx[0] = a1;
    playerx[1] = a2;
    playerx[2] = a3;
    playery[0] = b1;
    playery[1] = b2;
    playery[2] = b3; 
}

void fire_missile(){
  //this function sets the initial position for the missile as well as how it should travel
  int i;
  for(i = 0; i<10;i++){
    if(missile[i][2] == 0 && playerx[1] > playerx[0] && playerx[1] > playerx[2]){
      missile[i][0] = playerx[1];
      missile[i][1] = playery[1];
      missile[i][2] = 1;
      xvel[i] = 5;
      yvel[i] = 0;
      break;
    }
    if(missile[i][2] == 0 &&  playery[1] > playery[0] && playery[1] > playery[2]){
      missile[i][0] = playerx[1];
      missile[i][1] = playery[1];
      missile[i][2] = 1;
      xvel[i] = 0;
      yvel[i] = 5;
      break;
    }
    if(missile[i][2] == 0 && playerx[1] < playerx[0] && playerx[1] < playerx[2]){
      missile[i][0] = playerx[1];
      missile[i][1] = playery[1];
      missile[i][2] = 1;
      xvel[i] = -5;
      yvel[i] = 0;
      break;
    }
    if(missile[i][2] == 0 && playery[1] < playery[0] && playery[1] < playery[2]){
      missile[i][0] = playerx[1];
      missile[i][1] = playery[1];
      missile[i][2] = 1;
      xvel[i] = 0;
      yvel[i] = -5;
      break;
    }
  } 
}

void missile_motion(){
  //this function increments the motion of the bullet and checks to see whether it is in screen or not
  int i;
  for(i = 0; i < 10; i++){
    if(missile[i][2] == 1){
      arduboy.drawLine(missile[i][0],missile[i][1],missile[i][0]+xvel[i], missile[i][1] + yvel[i]);
      missile[i][0] += xvel[i];
      missile[i][1] += yvel[i];
    }
    if(missile[i][0] >= 700 || missile[i][1] >= 500 || missile[i][0] <= 0 || missile[i][0] <= 0){
      missile[i][0] = 0;
      missile[i][1] = 0;
      missile[i][2] = 0;
      xvel[i] = 0;
      yvel[i] = 0;
    }
  }
}

void missile_collision(){
  int i,j,k;
   for(i = 0; i < 10; i++){
    for(j = 0; j < 5; j++){
      if(missile[i][2] == 1 && missile[i][0] > asteroidsx[j]-asteroidsr[j] && missile[i][0] < asteroidsx[j]+asteroidsr[j] && missile[i][1] > asteroidsy[j] - asteroidsr[j] && missile[i][1] < asteroidsy[j] + asteroidsr[j]){
          if (asteroidsr[j] == 0) {
            asteroidsx[j] = 0;
            asteroidsy[j] = 0;
            asteroidsr[j] = 0;
            missile[i][0] = 0;
            missile[i][1] = 0;
            missile[i][2] = 0;
            asteroidsflag[i] = 1;
          } else {
            asteroidsr[j]--;
            missile[i][0] = 0;
            missile[i][1] = 0;
            missile[i][2] = 0;
          }
      }
    }
  }
}

void loop(){
  // put your main code here, to run repeatedly:
  if(!arduboy.nextFrame()){
    return;
  }
  arduboy.clear();

  switch(game_state){
    case 0:
      //Title Screen
      arduboy.setCursor(0,0);
      arduboy.print("Press A to Begin");

      if(arduboy.pressed(A_BUTTON) and buttonA_pressed == 0){
          buttonA_pressed = 1;
          game_state = 1;
      }
      playerx[0] = 1;
      playerx[1] = 10;
      playerx[2] = 5;
      playery[0] = 1;
      playery[1] = 1;
      playery[2] = 9;
      for (int i = 0; i < 3; i++) {
        playerx_vel[i] = 0;
        playery_vel[i] = 0;
      }
      break;
    case 1:
      //Game Play
      arduboy.fillTriangle(playerx[0],playery[0], playerx[1], playery[1], playerx[2], playery[2], WHITE);
      // move the player
      for (int i = 0; i < 3; i++) {
        playerx[i] += playerx_vel[i];
        playery[i] += playery_vel[i];
      }
      if (playerx[0] > 128 && playerx[1] > 128 && playerx[2] > 128) {
        playerx[0] -= 128;
        playerx[1] -= 128;
        playerx[2] -= 128;
      }
      if (playerx[0] < 0 && playerx[1] < 0 && playerx[2] < 0) {
        playerx[0] += 128;
        playerx[1] += 128;
        playerx[2] += 128;
      }
      if (playery[0] > 64 && playery[1] > 64 && playery[2] > 64) {
        playery[0] -= 64;
        playery[1] -= 64;
        playery[2] -= 64;
      }
      if (playery[0] < 0 && playery[1] < 0 && playery[2] < 0) {
        playery[0] += 64;
        playery[1] += 64;
        playery[2] += 64;
      }
      //this for loop controls the actions of the asteroids
      for(int i = 0; i < 5; i++){
        if (asteroidsx[i] != 0 and asteroidsy[i] != 0 and asteroidsr[i] != 0){
          arduboy.fillCircle(asteroidsx[i],asteroidsy[i],asteroidsr[i],WHITE);
        }
        //start with bounds checking
        if(asteroidsx[i]+asteroidsr[i] >= 128){
          asteroidsdx[i] = -1;
        }
        if(asteroidsx[i]-asteroidsr[i] <= 0){
          asteroidsdx[i] = 1;
        }
        if(asteroidsy[i]+asteroidsr[i] >= 64){
          asteroidsdy[i] = -1;
        }
        if(asteroidsy[i]-asteroidsr[i] <= 0){
          asteroidsdy[i] = 1;
        }
        //checking if any of the points of the sprite fall within the bounds of an asteroid
        for(int j = 0; j < 3; j++){
          if(playerx[j] >= asteroidsx[i]-asteroidsr[i] and playerx[j] <= asteroidsx[i]+asteroidsr[i] and playery[j] <= asteroidsy[i]+asteroidsr[i] and playery[j] >= asteroidsy[i]+asteroidsr[i] and asteroidsflag[i] == 0){
            game_state = 3;
          }
        }
  
       //now that bounds have been checked and directions have been set we can move the asteroids
        if(asteroidsdx[i] == -1){
          asteroidsx[i] = asteroidsx[i] - 1;
        }else{asteroidsx[i] = asteroidsx[i] + 1;}
        if(asteroidsdy[i] == -1){
          asteroidsy[i] = asteroidsy[i] - 1;
        }else{asteroidsy[i] = asteroidsy[i] + 1;}

      }
      missile_motion();
      missile_collision();
      
      //check if all asteroids have been hit and end game
      if(asteroidsr[0] == 0 && asteroidsr[1] == 0 && asteroidsr[2] == 0 && asteroidsr[3] == 0 && asteroidsr[4] == 0){
        game_state = 2;
      }

      if(arduboy.pressed(UP_BUTTON) and playery[0] > 0 and playery[1] > 0 and playery[2] > 0){
        for (int i = 0; i < 3; i++) {
          playery_vel[i]--;
        }
        /*
        playery[0] -= playery_vel[0];
        playery[1] -= playery_vel[1];
        playery[2] -= playery_vel[2];
        */
        //playery[0] = playery[0]-1;
        //playery[1] = playery[1]-1;
        //playery[2] = playery[2]-1;
      }
      if(arduboy.pressed(DOWN_BUTTON) and (playery[0] < 63) and (playery[1] < 63) and (playery[2] < 63)){
        for (int i = 0; i < 3; i++) {
          playery_vel[i]++;
        }
        /*
        playery[0] += playery_vel[0];
        playery[1] += playery_vel[1];
        playery[2] += playery_vel[2];
        */
        //playery[0] = playery[0]+1;
        //playery[1] = playery[1]+1;
        //playery[2] = playery[2]+1;
      }
      if(arduboy.pressed(LEFT_BUTTON) and (playerx[0] > 0) and (playerx[1] > 0) and (playerx[2] > 0)){
        for (int i = 0; i < 3; i++) {
          playerx_vel[i]--;
        }
        /*
        playerx[0] -= playerx_vel[0];
        playerx[1] -= playerx_vel[1];
        playerx[2] -= playerx_vel[2];
        */
        //playerx[0] = playerx[0]-1;
        //playerx[1] = playerx[1]-1;
        //playerx[2] = playerx[2]-1;
      }
      if(arduboy.pressed(RIGHT_BUTTON) and (playerx[0] < 127) and (playerx[1] < 127) and (playerx[2] < 127)){
        for (int i = 0; i < 3; i++) {
          playerx_vel[i]++;
        }
        /*
        playerx[0] += playerx_vel[0];
        playerx[1] += playerx_vel[1];
        playerx[2] += playerx_vel[2];
        */
      }
      if(arduboy.pressed(B_BUTTON)){
        buttonB_pressed = 1;
        rotate_main_character();
      }
      if(arduboy.pressed(A_BUTTON)){
        buttonA_pressed = 1;
        fire_missile();
      }
      break;
    case 2:
      //Win Screen
      arduboy.setCursor(0,0);
      arduboy.print("YOU WIN!!");
      arduboy.setCursor(0,20);
      arduboy.print("Press A to restart");
      if(arduboy.pressed(A_BUTTON) and buttonA_pressed == 0){
          setup();
          buttonA_pressed = 1;
          game_state = 0;
      }
      break;
     case 3:
      arduboy.setCursor(0,0);
      arduboy.print("YOU LOSE!!");
      arduboy.setCursor(0,20);
      arduboy.print("Press A to restart");
      if(arduboy.pressed(A_BUTTON) and buttonA_pressed == 0){
          setup();
          buttonA_pressed = 1;
          game_state = 0;
      }
      break;
  }
  if(arduboy.notPressed(A_BUTTON)){
    buttonA_pressed = 0;
  }
  if(arduboy.notPressed(B_BUTTON)){
    buttonB_pressed = 0;
  }
  arduboy.display();
}
