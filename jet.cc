// OpenGL Jet program.
// ECE8893, Georgia Tech, Fall 2012

#include <iostream>

#ifdef LINUX
//Linux Headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifdef OSX
// MAC Headers
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#ifdef WINDOWS
//Windows Headers
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#endif

#include <fstream>
#include <vector>

using namespace std;

// Classes and constructors for Vertex, Face and Normal starts
class Vertex
{
  public:
    Vertex();                                // Default constructor
    Vertex(GLfloat x, GLfloat y,GLfloat z);  // Construct with initial values

  public:
    GLfloat X;
    GLfloat Y;
    GLfloat Z;
};

Vertex::Vertex()
  : X(0.0), Y(0.0), Z(0.0)
{
}

Vertex::Vertex(GLfloat x, GLfloat y,GLfloat z)
  : X(x), Y(y), Z(z)
{
}

class Normal
{
  public:
    Normal();                                // Default constructor
    Normal(GLfloat i, GLfloat j,GLfloat k);  // Construct with initial values

  public:
    GLfloat I;
    GLfloat J;
    GLfloat K;

};

Normal::Normal()
  : I(0.0), J(0.0), K(0.0)
{
}

Normal::Normal(GLfloat i, GLfloat j,GLfloat k)
  : I(i), J(j), K(k)
{
}

class Face
{
  public:
    Face();                                         // Default constructor
    Face(uint32_t *vi, uint32_t *vni, uint32_t mi); // Construct with initial values

  public:
    uint32_t Vi[3];
    uint32_t Vni[3];
    uint32_t Mi;
};

Face::Face()
{
  Vi[0] = Vi[1] = Vi[2] = Vni[0] = Vni[1] = Vni[2] = Mi = 0;
}

Face::Face(uint32_t *vi, uint32_t *vni, uint32_t mi)
{
  Vi[0] = vi[0];
  Vi[1] = vi[1];
  Vi[2] = vi[2];

  Vni[0] = vni[0];
  Vni[1] = vni[1];
  Vni[2] = vni[2];

  Mi = mi;
}

class Material
{
  public:
    Material();
    Material(GLfloat Tr, GLfloat Ns, GLfloat Ar, GLfloat Ag, GLfloat Ab, GLfloat Dr, GLfloat Dg, GLfloat Db,
                   GLfloat Er, GLfloat Eg, GLfloat Eb, GLfloat Sr, GLfloat Sg, GLfloat Sb);

  public:
    GLfloat transparency;
    GLfloat shiny;

    GLfloat ar;
    GLfloat ag;
    GLfloat ab;

    GLfloat dr;
    GLfloat dg;
    GLfloat db;

    GLfloat er;
    GLfloat eg;
    GLfloat eb;

    GLfloat sr;
    GLfloat sg;
    GLfloat sb;
};

Material::Material()
  : transparency(0.0), shiny(0.0), ar(0.0), ag(0.0), ab(0.0), dr(0.0), dg(0.0), db(0.0), er(0.0), eg(0.0), eb(0.0), sr(0.0), sg(0.0), sb(0.0)
{
}

Material::Material(GLfloat Tr, GLfloat Ns, GLfloat Ar, GLfloat Ag, GLfloat Ab, GLfloat Dr, GLfloat Dg, GLfloat Db,
                   GLfloat Er, GLfloat Eg, GLfloat Eb, GLfloat Sr, GLfloat Sg, GLfloat Sb)
  : transparency(Tr), shiny(Ns), ar(Ar), ag(Ag), ab(Ab), dr(Dr), dg(Dg), db(Db), er(Er), eg(Eg), eb(Eb), sr(Sr), sg(Sg), sb(Sb)
{
}
// Classes and constructors for Vertex, Face and Normal ends


vector<Material> materials;
vector<Vertex>   verticies;
vector<Normal>   normals;
vector<Face>     triangles;


GLfloat updateRate = 30;

GLfloat angleX, angleY, angleZ;
GLfloat deltaX, deltaY, deltaZ;

bool rotx, roty, rotz;

GLfloat Scale = 1.2;

uint16_t no_light_sources = 0;

bool wiredraw = false;

void setMaterial(int materialId)
{
  float specular[4];
  float ambient[4];
  float diffuse[4];
  float emmisive[4];
  float shiny;

  // Ugly but works

  specular[0] = materials[materialId].sr;
  specular[1] = materials[materialId].sg;
  specular[2] = materials[materialId].sb;
  specular[3] = 1 - materials[materialId].transparency;

  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

  ambient[0] = materials[materialId].ar;
  ambient[1] = materials[materialId].ag;
  ambient[2] = materials[materialId].ab;
  ambient[3] = 1 - materials[materialId].transparency;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

  diffuse[0] = materials[materialId].dr;
  diffuse[1] = materials[materialId].dg;
  diffuse[2] = materials[materialId].db;
  diffuse[3] = 1 - materials[materialId].transparency;

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

  emmisive[0] = materials[materialId].er;
  emmisive[1] = materials[materialId].eg;
  emmisive[2] = materials[materialId].eb;
  emmisive[3] = 1 - materials[materialId].transparency;

  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmisive);

  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &materials[materialId].shiny);
}

void drawModel()
{
  // Your code to draw the model here
  if(true == wiredraw)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  glBegin(GL_TRIANGLES);

  for(uint32_t i=0; i<triangles.size(); i++)
  {
    setMaterial(triangles[i].Mi);

    glNormal3f(normals[triangles[i].Vni[0]-1].I, normals[triangles[i].Vni[0]-1].J, normals[triangles[i].Vni[0]-1].K);
    glVertex3f(verticies[triangles[i].Vi[0]-1].X, verticies[triangles[i].Vi[0]-1].Y, verticies[triangles[i].Vi[0]-1].Z);

    glNormal3f(normals[triangles[i].Vni[1]-1].I, normals[triangles[i].Vni[1]-1].J, normals[triangles[i].Vni[1]-1].K);
    glVertex3f(verticies[triangles[i].Vi[1]-1].X, verticies[triangles[i].Vi[1]-1].Y, verticies[triangles[i].Vi[1]-1].Z);

    glNormal3f(normals[triangles[i].Vni[2]-1].I, normals[triangles[i].Vni[2]-1].J, normals[triangles[i].Vni[2]-1].K);
    glVertex3f(verticies[triangles[i].Vi[2]-1].X, verticies[triangles[i].Vi[2]-1].Y, verticies[triangles[i].Vi[2]-1].Z);
  }

  glEnd();
}


void init(void)
{ // Called from main
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
}


void timer(int)
{
  // Adjust rotation angles as needed here
  if(true == rotx)
  {
    angleX += deltaX;
  }
  if(true == roty)
  {
    angleY += deltaY;
  }
  if(true == rotz)
  {
    angleZ += deltaZ;
  }

  // Then tell glut to redisplay
  glutPostRedisplay();
  // And reset tht timer
  glutTimerFunc(1000.0 / updateRate, timer, 0);
}

void display(void)
{
  //glClearColor(0.0, 0.0, 0.0, 1.0); // black background
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0, 15.0, -25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  // rotations and scaling here
  glScalef(Scale, Scale, Scale);

  glRotatef(angleX, 1.0, 0.0, 0.0);
  glRotatef(angleY, 0.0, 1.0, 0.0);
  glRotatef(angleZ, 0.0, 0.0, 1.0);

  GLfloat ambientcolor[] = {0.4, 0.4, 0.4, 1.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientcolor);

  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
  glDisable(GL_LIGHT2);
  glDisable(GL_LIGHT3);
  glDisable(GL_LIGHT4);
  glDisable(GL_LIGHT5);

  GLfloat lightpos[]   = {100, 100, 100, 1.0};

  for(uint16_t i=0; i<no_light_sources; i++)
  {
    if(0 == i)
    {
      glEnable(GL_LIGHT0);
      GLfloat lightcolor[] = {0.8, 0.0, 0.0, 1.0};
      glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);
      glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    }

    if(1 == i)
    {
      glEnable(GL_LIGHT1);
      GLfloat lightcolor[] = {0.5, 1, 0.0, 1.0};
      glLightfv(GL_LIGHT1, GL_DIFFUSE, lightcolor);
      glLightfv(GL_LIGHT1, GL_POSITION, lightpos);
    }

    if(2 == i)
    {
      glEnable(GL_LIGHT2);
      GLfloat lightcolor[] = {1.0, 1.0, 0.0, 1.0};
      glLightfv(GL_LIGHT2, GL_DIFFUSE, lightcolor);
      glLightfv(GL_LIGHT2, GL_POSITION, lightpos);
    }

    if(3 == i)
    {
      glEnable(GL_LIGHT3);
      GLfloat lightcolor[] = {0.0, 1.0, 0.5, 1.0};
      glLightfv(GL_LIGHT3, GL_DIFFUSE, lightcolor);
      glLightfv(GL_LIGHT3, GL_POSITION, lightpos);
    }

    if(4 == i)
    {
      glEnable(GL_LIGHT4);
      GLfloat lightcolor[] = {0.0, 0.0, 1.0, 1.0};
      glLightfv(GL_LIGHT4, GL_DIFFUSE, lightcolor);
      glLightfv(GL_LIGHT4, GL_POSITION, lightpos);
    }

    if(5 == i)
    {
      glEnable(GL_LIGHT5);
      GLfloat lightcolor[] = {1.0, 0.2, 0.0, 1.0};
      glLightfv(GL_LIGHT5, GL_DIFFUSE, lightcolor);
      glLightfv(GL_LIGHT5, GL_POSITION, lightpos);
    }
  }

  // Draw the emodel
  drawModel();
  // Swap the double buffers
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}



void keyboard (unsigned char key, int x, int y)
{
  // Keystroke processing here
  if('q' == key)
  {
    exit(0);
  }
  else if('x' == key)
  {
    if(true == rotx)
    {
      rotx = false;
    }
    else
    {
      rotx = true;
      deltaX = 72.0/updateRate;
    }
  }
  else if('y' == key)
  {
    if(true == roty)
    {
      roty = false;
    }
    else
    {
      roty = true;
      deltaY = 72.0/updateRate;
    }
  }
  else if('z' == key)
  {
    if(true == rotz)
    {
      rotz = false;
    }
    else
    {
      rotz = true;
      deltaZ = 72.0/updateRate;
    }
  }
  else if('S' == key)
  {
    Scale *= 1.01;
  }
  else if('s' == key)
  {
    Scale *= 0.99;
  }
  else if('L'== key)
  {
    if(no_light_sources < 6)
      no_light_sources++;
  }
  else if('l' == key)
  {
    if(no_light_sources > 0)
      no_light_sources--;
  }
  else if('w' == key)
  {
    if(false == wiredraw)
    {
      wiredraw = true;
    }
    else
    {
      wiredraw = false;
    } 
  }
}


void loadModel()
{
  //open file
  ifstream ifs ("jet.obj");

  if (!ifs) return;

  string line;
  uint32_t current_material;
  while(ifs)
  {
    getline(ifs, line);

    //process material
    if(line[0] == 'u')
    {
      sscanf(line.c_str(), "%*s %d", &current_material);
    }

    //process vertex
    else if(line[0] == 'v' && line[1] == ' ')
    {
      Vertex vert;
      sscanf(line.c_str(), "%*s %f %f %f", &vert.X, &vert.Y, &vert.Z);
      verticies.push_back(vert);
    }

    //process normal
    else if(line[0] == 'v' && line[1] == 'n')
    {
      Normal normal;
      sscanf(line.c_str(), "%*s %f %f %f", &normal.I, &normal.J, &normal.K);
      normals.push_back(normal);
    }

    //process face
    else if(line[0] == 'f')
    {
      Face tri;
      sscanf(line.c_str(), "%*s %d/%*d/%d %d/%*d/%d %d/%*d/%d" , &tri.Vi[0], &tri.Vni[0], &tri.Vi[1], &tri.Vni[1], &tri.Vi[2], &tri.Vni[2]);
      tri.Mi = current_material;
      triangles.push_back(tri);
    }
  }
}

void loadMaterials()
{
  //open file
  ifstream ifs ("jet.mtl");

  if (!ifs) return;

  string line;
  while(ifs)
  {
    getline(ifs, line);

    Material mat;

    //process shininess
    getline(ifs, line);
    sscanf(line.c_str(), "%*s %f", &mat.shiny);

    //process transperancy
    getline(ifs, line);
    sscanf(line.c_str(), "%*s %f", &mat.transparency);

    //process ambient lightling
    getline(ifs, line);
    sscanf(line.c_str(), "%*s %f %f %f", &mat.ar, &mat.ag, &mat.ab);

    //process diffuse lightling
    getline(ifs, line);
    sscanf(line.c_str(), "%*s %f  %f %f", &mat.dr, &mat.dg, &mat.db);

    //process specular lighting
    getline(ifs, line);
    sscanf(line.c_str(), "%*s %f  %f %f", &mat.sr, &mat.sg, &mat.sb);

    //process emissive lightling
    getline(ifs, line);
    sscanf(line.c_str(), "%*s %f %f %f", &mat.er, &mat.eg, &mat.eb);

    materials.push_back(mat);

    getline(ifs, line);
  }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jet Display");
    loadModel(); // Uncomment when the model reader is coded.
    loadMaterials();  // Uncomment when the materials reader is coded
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc (keyboard);
    glutReshapeFunc(reshape);
    // Compute the update rate here...
    glutTimerFunc(1000.0 / updateRate, timer, 0);
    glutMainLoop();
    return 0;
}

