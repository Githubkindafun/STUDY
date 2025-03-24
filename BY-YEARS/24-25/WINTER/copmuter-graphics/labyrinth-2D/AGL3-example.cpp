/*
-grid trójkątów N x N [done]
-ziarno losowe && argument n do ustalania grida [done]
-trójkąt gracza [done]
-stałość poruszania się nie zależna od pc [CHYBA done]

-stałość proporcji wyświetlania [done] [do skomentowania]

-trójkąt kończący [done]
-kolizje [done] [hint kolizja odcinkow i potencjalnie uzycie build-in funckji]
-czas [done] [time juz jest trzeba tylko zmierzyc]

-jasności trójkątów w zależności od gracza [done] [jakies zaleznosci trzeba wykminic]

-ładne tło [done] [jakies shadery do przekminy]

-EXTRA: animacja kończąca [done sor of] [po zakonczeniu zamiast zamekac okno cos odpalic i zamknac oknow]
animacja przez wymnazanie o coraz mniejszy współczynnik skale w shaderach
w while czy coś z if sprawdzającym czy minęło np. 0.5 sekundy
//
*/


// liczba dni [6]


#include <stdlib.h>
#include <stdio.h>


#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>

// dodane na potrzebe zadania
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>




using namespace glm;
using namespace std;



// ==========================================================================
// Collision checks ;> 
// ==========================================================================

void calculateTriangleVertices(float center_x, float center_y, float rotation_angle, float size, vec2 vertices[3]) {
   
   vec2 base_vertices[3] = { // tuaj mamy "template" wierzchołków naszego trójkąta 
      vec2(-0.2f, -0.2f),
      vec2(0.2f, -0.2f),
      vec2(0.0f, 0.8f)
   };

   float cos_angle = cos(rotation_angle);
   float sin_angle = sin(rotation_angle);

   // zasadniczo tutaj odtwarzamy gdzie te wierzchołki powinny być
   for (int i = 0; i < 3; i++) {
      float x = base_vertices[i].x * size;
      float y = base_vertices[i].y * size;

      
      float rotated_x = x * cos_angle - y * sin_angle;
      float rotated_y = x * sin_angle + y * cos_angle;

      
      vertices[i] = vec2(center_x + rotated_x, center_y + rotated_y);
   }
}
float crossProduct(vec2 a, vec2 b) { // cross product do obliczeń
   return a.x * b.y - a.y * b.x;
}

//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect

bool doLinesIntersect(vec2 p1, vec2 p2, vec2 q1, vec2 q2) { // sprawdzamy czy linie sie przecinaja/nachodza na siebie
   vec2 v1 = p2 - p1; // wektor z p1 do p2
   vec2 v2 = q2 - q1; // wektor z q1 do q2
   float v1xv2 = crossProduct(v1, v2);
   vec2 vpq = q1 - p1; // wektor z p1 do q1
   float vpqxv1 = crossProduct(vpq, v1);

   if (v1xv2 == 0 && vpqxv1 == 0) { // pierwsze to czy są równoległe, a oba to wspóliniowe
      return false;
   }

   if (v1xv2 == 0) { // czy równoległe
      return false; 
   }

   float t = crossProduct(vpq, v2) / v1xv2; // opisane w linku zamieszconym powyżej
   float u = crossProduct(vpq, v1) / v1xv2; // 

   return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
}





// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================

class MyTri : public AGLDrawable {
public:
   MyTri() : AGLDrawable(0) {
      setShaders();
   }
   // https://www.youtube.com/watch?v=qjWx1SgeEoM // Tutaj absolutnie czadersko wytłumaczone jak zrobic ciekawe shadery
   void setShaders() {
      float middle = 0.0f;


      compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require

         out vec4 vcolor;
         out vec2 vpos; 

         void main(void) {
            const vec2 vertices[4] = vec2[4](vec2( 0.9, -0.9), 
                                             vec2(-0.9, -0.9),
                                             vec2( 0.9,  0.9),
                                             vec2( -0.9,  0.9)); // 4 wierzchołek
            const vec4 colors[]    = vec4[4](vec4(1.0, 0.0, 0.0, 1.0),
                                             vec4(1.0, 0.0, 1.0, 1.0),
                                             vec4(0.0, 0.0, 1.0, 1.0),
                                             vec4(0.5, 0.6, 0.5, 1.0)); // 4 kolor

            vcolor      = colors[gl_VertexID];
            vpos = vertices[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
         }

      )END", R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require

         in  vec4 vcolor;
         in vec2 vpos;
         out vec4 color;
         layout(location = 0) uniform float time;

         void main(void) {
            color = vcolor;
            
            float red = 0.1 + 0.2 * cos(time * 1.0 + vpos.x * 3.0 + vpos.y * 0.0);
            float green = 0.1 + 0.1 * cos(time * 0.5 + vpos.x * 0.0 + vpos.y * 3.0);
            float blue = 0.1 + 0.2 * cos(time * 0.75 + vpos.x * 2.0 + vpos.y * 2.0);

            color = vec4(
            red * (1.0 - 0.5 * 0.2),
            green * (1.0 - 0.5 * 0.2),
            blue * (1.0 - 0.5 * 0.2),
            1.0
            );
         } 

      )END");
   }
   void draw() {
      bindProgram();
      bindBuffers();

      glUniform1f(0, glfwGetTime());
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   }
};


class MazeTriangle : public AGLDrawable {
public:
   float center_x, center_y;
   float rotation_angle;
   float size;
   vec4* kolorki;
   float player_center_x, player_center_y;
   


   MazeTriangle(float x, float y, float angle, float size, vec4* kolorki) : AGLDrawable(0) {
      this->center_x = x;
      this->center_y = y;
      this->rotation_angle = angle;
      this->size = size;
      this->kolorki = kolorki;
      setShaders();
      setBuffers(); 
   }

   void set_player_center_x(float x) {
      player_center_x = x;
   }
   
   void set_player_center_y(float y) {
      player_center_y = y;
   }

   void set_size(float scale) {
      size = scale;
   }

   void setShaders() {
      compileShaders(R"END(  
         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require

         layout(location = 0) in vec2 pos;

         layout(location = 0) uniform vec2 uniform_center_cords;
         layout(location = 2) uniform float uniform_size;
         layout(location = 3) uniform float uniform_angle;
         layout(location = 4) uniform float dist_to_player;
         layout(location = 5) uniform vec4 uniform_color[3];

         out vec4 vcolor;

         void main(void) {
            float cos_angle = cos(uniform_angle);
            float sin_angle = sin(uniform_angle);

            float rotated_x = pos.x * cos_angle - pos.y * sin_angle;
            float rotated_y = pos.x * sin_angle + pos.y * cos_angle;

            vec2 scaledPos = uniform_center_cords + vec2(rotated_x, rotated_y) * uniform_size;
            gl_Position = vec4(scaledPos, 0.0, 1.0);

            vcolor = uniform_color[gl_VertexID];
            vec3 white = vec3(exp(-8.0*dist_to_player + 1.5) + 1); // do desmosa wpisac i widac
            // w razie czego to współczynnik przy dist oznacza jak dalego od gracza chcemy
            // a to co dodajemy to juz kolor per se
            vcolor.rgb = vcolor.rgb * white;
         }
      )END", R"END(
         #version 330
         #extension GL_ARB_explicit_uniform_location : require

         in vec4 vcolor;
         out vec4 color;

         void main(void) {
            color = vcolor;

         }
      )END");
   }
   void setBuffers() {
      bindBuffers();

      GLfloat vert[3][2] = {
         {-0.2f, -0.2f},
         {0.2f, -0.2f},
         {0.0f, 0.8f}
      };

      glBufferData(GL_ARRAY_BUFFER, 3*2*sizeof(float), vert, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0,                
         2,                
         GL_FLOAT,         
         GL_FALSE,         
         0,                
         (void*)0          
      );

   }

   void draw() {
      bindProgram();
      bindBuffers();

      glUniform2f(0, center_x, center_y);
      glUniform1f(2, size);
      glUniform1f(3, rotation_angle);
      float dist_to_player = sqrt(  pow((player_center_x - center_x), 2) + pow((player_center_y - center_y), 2) );
      glUniform1f(4, dist_to_player);
      glUniform4fv(5, 3, (const GLfloat *)kolorki);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
   }
};

class Player : public AGLDrawable { // mmay naszego gracza
public:
   float center_x, center_y;
   float rotation_angle;
   float size;
   float mov_speed;
   vec4* kolorki;

   Player(float x, float y, float size, vec4* kolorki, float angle=0.0f, float mov_speed=0.1f / 16.0f) : AGLDrawable(0) {
      this->center_x = x;
      this->center_y = y;
      this->rotation_angle = angle;
      this->size = size;
      this->mov_speed = mov_speed;
      this->kolorki = kolorki;
      setShaders();
      setBuffers(); 
   }

   float get_player_center_x() {
      return center_x;
   }

   float get_player_center_y() {
      return center_y;
   }

   void set_player_center_x(float x) {
      center_x = x;
   }

   void set_player_center_y(float y) {
      center_y = y;
   }

   void set_size(float scale) {
      size = scale;
   }

   void AngleNormalization() {
      if (rotation_angle >= M_PI) {
         rotation_angle -= 2 * M_PI;
      }
      else if (rotation_angle <= -M_PI)
      {
         rotation_angle += 2 * M_PI;
      }
   }


   bool checkCollision(const Player* player, const MazeTriangle* triangle) {
   vec2 playerVertices[3];
   vec2 triangleVertices[3];

   ///wyznaczamy współrzędne trójkątów
   calculateTriangleVertices(player->center_x, player->center_y, player->rotation_angle, player->size, playerVertices);
   calculateTriangleVertices(triangle->center_x, triangle->center_y, triangle->rotation_angle, triangle->size, triangleVertices);

   /// sprawdzamy czy mamy kolizje bok do boku trójkąta
   for (int i = 0; i < 3; i++) {
      vec2 p1 = playerVertices[i];
      vec2 p2 = playerVertices[(i + 1) % 3];

      for (int j = 0; j < 3; j++) {
         vec2 q1 = triangleVertices[j];
         vec2 q2 = triangleVertices[(j + 1) % 3];

         if (doLinesIntersect(p1, p2, q1, q2)) {
            return true; 
         }
      }
   }

   return false;
   }


   void forward(double time_delta, vector<MazeTriangle*> *maze_triangles) {
      
      float tmp_x = center_x - mov_speed * sin(rotation_angle) * time_delta;
      float tmp_y = center_y + mov_speed * cos(rotation_angle) * time_delta;


      Player tmp_player(tmp_x, tmp_y, size, kolorki, rotation_angle, mov_speed);
      


      for(int i = 0; i < maze_triangles->size(); i++) {
         if(checkCollision(&tmp_player, (*maze_triangles)[i])) {
            tmp_player.~Player();
            return;
         }
      }

      center_x = tmp_x;
      center_y = tmp_y;

      // cout << center_x << " " << center_y << " " << rotation_angle << endl;
      AngleNormalization();
      // cout << center_x << " " << center_y << " " << rotation_angle << endl;
   };

   void backward(double time_delta,  vector<MazeTriangle*> *maze_triangles) {


      
      float tmp_x = center_x + mov_speed * sin(rotation_angle) * time_delta;
      float tmp_y = center_y - mov_speed * cos(rotation_angle) * time_delta;


      Player tmp_player(tmp_x, tmp_y, size, kolorki, rotation_angle, mov_speed);
      


      for(int i = 0; i < maze_triangles->size(); i++) {
         if(checkCollision(&tmp_player, (*maze_triangles)[i])) {
            tmp_player.~Player();
            return;
         }
      }


      center_x = tmp_x;
      center_y = tmp_y;
      

      // center_x += mov_speed * sin(rotation_angle) * time_delta;
      // center_y -= mov_speed * cos(rotation_angle) * time_delta;
      AngleNormalization();
   };

   void right(float right_turn_radious, double time_delta, vector<MazeTriangle*> *maze_triangles) {


      float tmp_rotation_angle = rotation_angle - right_turn_radious;
      Player tmp_player(center_x, center_y, size, kolorki, tmp_rotation_angle, mov_speed);
      


      for(int i = 0; i < maze_triangles->size(); i++) {
         if(checkCollision(&tmp_player, (*maze_triangles)[i])) {
            tmp_player.~Player();
            return;
         }
      }

      rotation_angle = tmp_rotation_angle;
      AngleNormalization();
   };

   void left(float left_turn_radious, double time_delta,  vector<MazeTriangle*> *maze_triangles) {

      float tmp_rotation_angle = rotation_angle + left_turn_radious;

      Player tmp_player(center_x, center_y, size, kolorki, tmp_rotation_angle, mov_speed);
      


      for(int i = 0; i < maze_triangles->size(); i++) {
         if(checkCollision(&tmp_player, (*maze_triangles)[i])) {
            tmp_player.~Player();
            return;
         }
      }




      rotation_angle = tmp_rotation_angle ;
      AngleNormalization();
   };

   ~Player(){
      
   }

   void setShaders() { 
      compileShaders(R"END(  
         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require

         layout(location = 0) in vec2 pos;

         layout(location = 0) uniform vec2 uniform_center_cords;     
         layout(location = 2) uniform float uniform_size;      
         layout(location = 3) uniform float uniform_angle;     
         layout(location = 4) uniform vec4 uniform_color[3];   

         out vec4 vcolor;

         void main(void) {
            float cos_angle = cos(uniform_angle);
            float sin_angle = sin(uniform_angle);

            float rotated_x = pos.x * cos_angle - pos.y * sin_angle;
            float rotated_y = pos.x * sin_angle + pos.y * cos_angle;

            vec2 scaledPos = uniform_center_cords + vec2(rotated_x, rotated_y) * uniform_size;
            gl_Position = vec4(scaledPos, 0.0, 1.0);

            vcolor = uniform_color[gl_VertexID];
         }
      )END", R"END(
         #version 330
         #extension GL_ARB_explicit_uniform_location : require

         in vec4 vcolor;
         out vec4 color;

         void main(void) {
            color = vcolor;

         }
      )END");
   }
   void setBuffers() {
      bindBuffers();

      GLfloat vert[3][2] = {
         {-0.2f, -0.2f},
         {0.2f, -0.2f},
         {0.0f, 0.8f}
      };

      glBufferData(GL_ARRAY_BUFFER, 3*2*sizeof(float), vert, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0,                
         2,                
         GL_FLOAT,         
         GL_FALSE,         
         0,                
         (void*)0          
      );

   }

   void draw() {
      bindProgram();
      bindBuffers();

      glUniform2f(0, center_x, center_y);
      glUniform1f(2, size);
      glUniform1f(3, rotation_angle);
      glUniform4fv(4, 3, (const GLfloat *)kolorki);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
   }

};






class Exit : public AGLDrawable { // trójkąt "wyjścia"
public:
   float center_x, center_y;
   float rotation_angle;
   float size;
   vec4* kolorki;

   Exit(float x, float y, float size, vec4* kolorki, float angle=0.0f) : AGLDrawable(0) {
      this->center_x = x;
      this->center_y = y;
      this->rotation_angle = angle;
      this->size = size;
      this->kolorki = kolorki;
      setShaders();
      setBuffers(); 
   }


   void set_size(float scale) {
      size = scale;
   }


   void setShaders() {
      compileShaders(R"END(  
         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require

         layout(location = 0) in vec2 pos;

         layout(location = 0) uniform vec2 uniform_center_cords;     
         layout(location = 2) uniform float uniform_size;      
         layout(location = 3) uniform float uniform_angle;     
         layout(location = 4) uniform vec4 uniform_color[3];   

         out vec4 vcolor;

         void main(void) {
            float cos_angle = cos(uniform_angle);
            float sin_angle = sin(uniform_angle);

            float rotated_x = pos.x * cos_angle - pos.y * sin_angle;
            float rotated_y = pos.x * sin_angle + pos.y * cos_angle;

            vec2 scaledPos = uniform_center_cords + vec2(rotated_x, rotated_y) * uniform_size;
            gl_Position = vec4(scaledPos, 0.0, 1.0);

            vcolor = uniform_color[gl_VertexID];
         }
      )END", R"END(
         #version 330
         #extension GL_ARB_explicit_uniform_location : require

         in vec4 vcolor;
         out vec4 color;

         void main(void) {
            color = vcolor;

         }
      )END");
   }
   void setBuffers() {
      bindBuffers();

      GLfloat vert[3][2] = {
         {-0.2f, -0.2f},
         {0.2f, -0.2f},
         {0.0f, 0.8f}
      };

      glBufferData(GL_ARRAY_BUFFER, 3*2*sizeof(float), vert, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0,                
         2,                
         GL_FLOAT,         
         GL_FALSE,         
         0,                
         (void*)0          
      );

   }

   void draw() {
      bindProgram();
      bindBuffers();

      glUniform2f(0, center_x, center_y);
      glUniform1f(2, size);
      glUniform1f(3, rotation_angle);
      glUniform4fv(4, 3, (const GLfloat *)kolorki);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
   }

};




// ==========================================================================
// Collision checks ;> vol.2
// ==========================================================================
bool checkCollisionE(const Player* player, const Exit* triangle) {
    vec2 playerVertices[3];
   vec2 triangleVertices[3];

   calculateTriangleVertices(player->center_x, player->center_y, player->rotation_angle, player->size, playerVertices);
   calculateTriangleVertices(triangle->center_x, triangle->center_y, triangle->rotation_angle, triangle->size, triangleVertices);

   for (int i = 0; i < 3; i++) {
      vec2 p1 = playerVertices[i];
      vec2 p2 = playerVertices[(i + 1) % 3];

      for (int j = 0; j < 3; j++) {
         vec2 q1 = triangleVertices[j];
         vec2 q2 = triangleVertices[(j + 1) % 3];

         if (doLinesIntersect(p1, p2, q1, q2)) {
               return true;
         }
      }
   }

   return false; 
}

// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:

   vector<MazeTriangle*> maze_triangles;
   int grid_size;
   int generation_seed;
   float scaled_size;
   
   MyWin() {};
   MyWin(int _wd, int _ht, const char *name, int vers, int grid_size, int fullscr=0, int generation_seed=0)
      : AGLWindow(_wd, _ht, name, vers, fullscr) {
      this->grid_size = grid_size;
      this->generation_seed = generation_seed;
      InitializeMaze(grid_size, generation_seed);
      };
   virtual void KeyCB(int key, int scancode, int action, int mods);
   void MainLoop();

   void InitializeMaze(int grid_size ,int generation_seed) {
      maze_triangles.clear();

      const float margin = 0.9f;

      float starting_boundry = -margin;
      float distribution = (margin * 2.0) / (grid_size - 1);
      float triangle_size = distribution * 0.5f; 
      
      vec4* kolorki = new vec4[3] {
         vec4(82.0f / 255.0f, 1.0f / 255.0f, 60.0f / 255.0f,  1.0f), 
         vec4(150.0f / 255.0f, 43.0f / 255.0f, 9.0f / 255.0f, 1.0f),
         vec4(161.0f / 225.0f, 73.0f / 225.0f, 72.0f / 225.0f, 1.0f)      
         };

      scaled_size = triangle_size; // dla playera



      srand(generation_seed); // to ten nasz generator "losowych przekreceń"

      for (int i = 0; i < grid_size; i++) { // to do ostatni trojkat jest koncem
         for (int j = 0; j < grid_size; j++) { // a pierwszy to gracz
            
            float x = starting_boundry + (i * distribution);
            float y = starting_boundry + (j * distribution);
            float rotation_angle = (float)(rand())/RAND_MAX * 2.0 * M_PI; // potencjalnie do poprawy

            MazeTriangle *mt = new MazeTriangle(x, y, rotation_angle, triangle_size, kolorki);
            maze_triangles.push_back(mt);
         }
      }
      maze_triangles.erase(maze_triangles.begin()); // robimy miejsce dla gracza
      maze_triangles.pop_back(); // tutaj miejsce dla "wyjścia"
   }



};


// ==========================================================================
// Komentarz do tego, nie używam tego dragmentu kodu, i nie chce już usuwać tego
// jest to pozostałosć z fragmentu z zadania 2 więc już nie dodawałem funkcjonalności do przycisku f.
void MyWin::KeyCB(int key, int scancode, int action, int mods) { // z tego nie korzystam
    AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
       ; // do something
    }
    if (key == GLFW_KEY_HOME  && (action == GLFW_PRESS)) {
       ; // do something
    }
}


// ==========================================================================
void MyWin::MainLoop() {
   ViewportOne(0,0,wd,ht);

   vec4* exit_kolorki = new vec4[3] {
         vec4(245.0f / 255.0f, 198.0f / 255.0f, 0.0f / 255.0f,  1.0f), // 245, 198, 0
         vec4(115.0f / 255.0f, 59.0f / 255.0f, 0.0f / 255.0f,  1.0f), // 115, 59, 0
         vec4(235.0f / 255.0f, 255.0f / 255.0f, 16.0f / 255.0f,  1.0f) // 235, 255, 16
         };

   vec4* player_kolorki = new vec4[3] {
         vec4(3.0f / 255.0f, 69.0f / 255.0f, 37.0f / 255.0f,  1.0f), // 3, 69, 37
         vec4(0.0f / 255.0f, 82.0f / 255.0f, 54.0f / 255.0f, 1.0f),  // 0, 82, 54
         vec4(54.0f / 225.0f, 186.0f / 225.0f, 69.0f / 225.0f, 1.0f) // 54, 186, 69
         };  

   MyTri trian; // nasze tło

   float player_move = 2.0f;
   player_move /= 16.0f;
   float turn_radious = 0.05f;

   Player player(-0.9f, -0.9f, scaled_size, player_kolorki, 0.0f, player_move);
   Exit exit(0.9f, 0.9f, scaled_size, exit_kolorki, 270.f / 2.0f * M_PI);

   double start = glfwGetTime();
   double time = glfwGetTime();
   double time_delta;
   double time_accumulated = 0.0;
   double FPS = 600.0;

   glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
   do {
      glClear( GL_COLOR_BUFFER_BIT );

      // tutaj kod zeby predkosc była taka sama zawsze
      double time_current = glfwGetTime();
      time_delta = time_current - time;
      time = time_current;
      //

      // kolizja z "wyjściem" i animacja końcowa

      if(checkCollisionE(&player, &exit)) {
         double end = glfwGetTime();
         end = end - start;
         cout << "YOU WON! Your run lasted: " << end << " seconds" << endl;

         float now = glfwGetTime();
         float ending = glfwGetTime();
         float last_update = now;
         time_accumulated = 0.0;

         while(ending - now < 0.5) { 
            ending = glfwGetTime();   
            glfwPollEvents();

            if (ending - last_update > 0.01) {
               glClear( GL_COLOR_BUFFER_BIT );

               last_update = ending;
               player.set_size(player.size * 1.1);
               exit.set_size(exit.size * 1.1);

               for(int i = 0; i < maze_triangles.size(); i++) {
                  maze_triangles[i]->set_size(player.size * 1.1);
                  AGLErrors("main-afterdraw-maze-triangle");
               }

               AGLErrors("after trian");
               trian.draw(); //rysujemy nasze tło

               exit.draw();

               AGLErrors("after-trian-exit");



               for(int i = 0; i < maze_triangles.size(); i++) {
                  maze_triangles[i]->draw();
                  AGLErrors("main-afterdraw-maze-triangle");
               }

               AGLErrors("1-afterdraw-player");
               player.draw();
               AGLErrors("main-afterdraw-player");
               glfwSwapBuffers(win()); 
            }
         }

         while(ending - now < 1.2) { // potencjalnie do doania animacja gdzie najpierw sie zwieksza a potem zmniejsza
            ending = glfwGetTime();   
            glfwPollEvents();

            if (ending - last_update > 0.01) {
               glClear( GL_COLOR_BUFFER_BIT );

               last_update = ending;
               player.set_size(player.size * 0.85);
               exit.set_size(exit.size * 0.85);

               for(int i = 0; i < maze_triangles.size(); i++) {
                  maze_triangles[i]->set_size(player.size * 0.85);
                  AGLErrors("main-afterdraw-maze-triangle");
               }

               AGLErrors("after trian");
               trian.draw(); //rysujemy nasze tło

               exit.draw();

               AGLErrors("after-trian-exit");



               for(int i = 0; i < maze_triangles.size(); i++) {
                  maze_triangles[i]->draw();
                  AGLErrors("main-afterdraw-maze-triangle");
               }

               AGLErrors("1-afterdraw-player");
               player.draw();
               AGLErrors("main-afterdraw-player");
               glfwSwapBuffers(win()); 
            }
         }
            
               
         glfwSetWindowShouldClose(win(), 1);
      }

      // --------------------------------------------------------------

      for(int i = 0; i < maze_triangles.size(); i++) {
         maze_triangles[i]->set_player_center_x(player.get_player_center_x());
         maze_triangles[i]->set_player_center_y(player.get_player_center_y());
         AGLErrors("main-afterset-maze-triangle");
      }


      AGLErrors("main-loopbegin");
      
      // malowanie contentu

      trian.draw(); //rysujemy nasze tło

      AGLErrors("after trian");

      exit.draw();

      AGLErrors("after-trian-exit");



      for(int i = 0; i < maze_triangles.size(); i++) {
         maze_triangles[i]->draw();
         AGLErrors("main-afterdraw-maze-triangle");
      }

      AGLErrors("1-beforedraw-player");
      player.draw();
      AGLErrors("main-afterdraw-player");

      // --------------------------------------------------------------

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();
      // glfwWaitEvents();   


      // movement

      if (glfwGetKey(win(), GLFW_KEY_W ) == GLFW_PRESS) {
         if (time_accumulated > 1.0 / FPS) {
            player.forward(time_accumulated + player_move, &maze_triangles);
            time_accumulated = 0.0;
         } else {
            time_accumulated += time_delta;
         }
      }
      else if (glfwGetKey(win(), GLFW_KEY_S ) == GLFW_PRESS) {
         if (time_accumulated > 1.0 / FPS) {
            player.backward(time_accumulated + player_move, &maze_triangles);
            time_accumulated = 0.0;
         } else {
            time_accumulated += time_delta;
         }
      
      }
      else if (glfwGetKey(win(), GLFW_KEY_D ) == GLFW_PRESS) {
         if (time_accumulated > 1.0 / FPS) {
            player.right(turn_radious, time_accumulated, &maze_triangles);
            time_accumulated = 0.0;
         } else {
            time_accumulated += time_delta;
         }
      }
      else if (glfwGetKey(win(), GLFW_KEY_A ) == GLFW_PRESS) {
         if (time_accumulated > 1.0 / FPS) {
            player.left(turn_radious, time_accumulated, &maze_triangles);
            time_accumulated = 0.0;
         } else {
            time_accumulated += time_delta;
         }
      }

      // --------------------------------------------------------------

   } while( glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0 );
}




int main(int argc, char *argv[]) {
   int grid_size = 10; // dałem jakiś bazowy
   int generation_seed = 0; 


   switch(argc) {
      case 1:
         break;
      case 2:
         grid_size = atoi(argv[1]);
         break;
      case 3:
         grid_size = atoi(argv[1]);
         generation_seed = atoi(argv[2]);
         break;
      default:
         throw runtime_error("to much arguments :C");
   }

   MyWin win(600, 600, "Z3 MAZE GAME", 33,grid_size, 0, generation_seed);
   win.MainLoop();
   return 0;
}


