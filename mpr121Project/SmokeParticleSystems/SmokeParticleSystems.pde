import processing.serial.*;

ArrayList<ParticleSystem> ParticleSystems = new ArrayList<ParticleSystem>();
int numOfSystems = 10;
IntList gotSmoke = new IntList();

String myString = null;
Serial myPort;  // The serial port

void setup() {
  //size(1682, 1188);
  fullScreen();
  colorMode(RGB);
  PImage img = loadImage("texture.png");
  for (int n = 0; n < numOfSystems; n++) {
    ParticleSystems.add(new ParticleSystem(0, new PVector(0, 0), img));
    gotSmoke.append(0);
  }
  frameRate(20);

  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 9600);
  //myPort.clear();
}

void draw() {
  while (myPort.available() > 0) {
    myString = myPort.readStringUntil('\n');
    if (myString != null) {
      if (myString.length() >= 12) {
        char[] allChars = new char[myString.length()];
        for (int i = 0; i < myString.length(); i++) {
          allChars[i] = myString.charAt(i);
          println(allChars[i]);
        }
        for (int i = 0; i < allChars.length; i++) {
          println(allChars[i]);
          if (allChars[i] == '0') {
            gotSmoke.set(i, 0);
            //println("xxxxxxxxx");
          } else if (allChars[i] == '1') {
            gotSmoke.set(i, 1);
            //println("ooooooooooo");
          }
        }
      }
    }
  }




  background(0);

  translate(width/2, height/2);
  // Calculate a "wind" force based on mouse horizontal position
  float dx = map(mouseX, 0, width, -0.5, 0.5);
  float dy = map(mouseY, 0, height, -0.5, 0.5);
  PVector wind = new PVector(dx, dy);

  for (int i = 0; i < numOfSystems; i++) {
    pushMatrix();
    rotate(radians(i*360/numOfSystems));
    translate(0, height/2-120);
    ParticleSystems.get(i).applyForce(wind);
    ParticleSystems.get(i).run(i*20%150+100, i*45%120+100, i*60%100+100, map(mouseX, 0, width, 0.5, 1.5)); //param: r, g, b, size
    if (gotSmoke.get(i) == 1) {
      ParticleSystems.get(i).addParticle();
    }
    //ellipse(0, 0, 50, 50); //for painting circles
    popMatrix();
  }
}

// Renders a vector object 'v' as an arrow and a position 'loc'
void drawVector(PVector v, PVector loc, float scayl) {
  pushMatrix();
  float arrowsize = 4;
  // Translate to position to render vector
  translate(loc.x, loc.y);
  stroke(255);
  // Call vector heading function to get direction (note that pointing up is a heading of 0) and rotate
  rotate(v.heading());
  // Calculate length of vector & scale it to be bigger or smaller if necessary
  float len = v.mag()*scayl;
  // Draw three lines to make an arrow (draw pointing up since we've rotate to the proper direction)
  line(0, 0, len, 0);
  line(len, 0, len-arrowsize, +arrowsize/2);
  line(len, 0, len-arrowsize, -arrowsize/2);
  popMatrix();
}


// A class to describe a group of Particles
// An ArrayList is used to manage the list of Particles 

class ParticleSystem {

  ArrayList<Particle> particles;    // An arraylist for all the particles
  PVector origin;                   // An origin point for where particles are birthed
  PImage img;

  ParticleSystem(int num, PVector v, PImage img_) {
    particles = new ArrayList<Particle>();              // Initialize the arraylist
    origin = v.copy();                                   // Store the origin point
    img = img_;
    for (int i = 0; i < num; i++) {
      particles.add(new Particle(origin, img));         // Add "num" amount of particles to the arraylist
    }
  }

  void run(float r, float g, float b, float amp) {
    for (int i = particles.size()-1; i >= 0; i--) {
      Particle p = particles.get(i);
      p.run(r, g, b, amp);
      if (p.isDead()) {
        particles.remove(i);
      }
    }
  }

  // Method to add a force vector to all particles currently in the system
  void applyForce(PVector dir) {
    for (Particle p : particles) {
      p.applyForce(dir);
    }
  }  

  void addParticle() {
    particles.add(new Particle(origin, img));
  }
}


/*
A simple Particle class, renders the particle as an image
 */
class Particle {
  PVector loc;
  PVector vel;
  PVector acc;
  float lifespan;
  PImage img;

  Particle(PVector l, PImage img_) {
    acc = new PVector(0, 0);
    float vx = randomGaussian()*0.5;
    float vy = randomGaussian()*0.5 - 5.0;
    vel = new PVector(vx, vy);
    loc = l.copy();
    lifespan = 100.0;
    img = img_;
  }

  void run(float r, float g, float b, float amp) {
    update();
    render(r, g, b, amp);
  }

  // Method to apply a force vector to the Particle object
  // Note we are ignoring "mass" here
  void applyForce(PVector f) {
    acc.add(f);
  }  

  // Method to update position
  void update() {
    vel.add(acc);
    loc.add(vel);
    lifespan -= 2.5;
    acc.mult(0); // clear Acceleration
  }

  // Method to display
  void render(float r, float g, float b, float amp) {
    pushMatrix();
    scale(amp);
    imageMode(CENTER);
    tint(r, g, b, lifespan);
    image(img, loc.x, loc.y);
    popMatrix();
  }

  // Is the particle still useful?
  boolean isDead() {
    if (lifespan <= 0.0) {
      return true;
    } else {
      return false;
    }
  }
}

void mousePressed() {
  //gotSmoke = !gotSmoke;
}

/*
For up to 10 circles only
 */
void keyPressed() {
  if (key == '1') {
    if (gotSmoke.get(0) == 1) {
      gotSmoke.set(0, 0);
    } else {
      gotSmoke.set(0, 1);
    }
  } else if (key == '2') {
    if (gotSmoke.get(1) == 1) {
      gotSmoke.set(1, 0);
    } else {
      gotSmoke.set(1, 1);
    }
  } else if (key == '3') {
    if (gotSmoke.get(2) == 1) {
      gotSmoke.set(2, 0);
    } else {
      gotSmoke.set(2, 1);
    }
  } else if (key == '4') {
    if (gotSmoke.get(3) == 1) {
      gotSmoke.set(3, 0);
    } else {
      gotSmoke.set(3, 1);
    }
  } else if (key == '5') {
    if (gotSmoke.get(4) == 1) {
      gotSmoke.set(4, 0);
    } else {
      gotSmoke.set(4, 1);
    }
  } else if (key == '6') {
    if (gotSmoke.get(5) == 1) {
      gotSmoke.set(5, 0);
    } else {
      gotSmoke.set(5, 1);
    }
  } else if (key == '7') {
    if (gotSmoke.get(6) == 1) {
      gotSmoke.set(6, 0);
    } else {
      gotSmoke.set(6, 1);
    }
  } else if (key == '8') {
    if (gotSmoke.get(7) == 1) {
      gotSmoke.set(7, 0);
    } else {
      gotSmoke.set(7, 1);
    }
  } else if (key == '9') {
    if (gotSmoke.get(8) == 1) {
      gotSmoke.set(8, 0);
    } else {
      gotSmoke.set(8, 1);
    }
  } else if (key == '0') {
    if (gotSmoke.get(9) == 1) {
      gotSmoke.set(9, 0);
    } else {
      gotSmoke.set(9, 1);
    }
  }
}