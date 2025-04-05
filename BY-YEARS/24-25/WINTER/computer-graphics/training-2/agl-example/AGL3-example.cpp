// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
/*
https://www.javatpoint.com/defining-a-circle-using-polar-coordinates : aproksymacja kolka za pomoca wspolzednych biegonowych
https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl : 



*/
#include <stdlib.h>
#include <stdio.h>


#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>

// dodane na potrzebe zadania
#include <cmath>

// ==========================================================================
// Collision check ..........................................................
// ==========================================================================

/*
w zasadzie idea jest prosta, jakbysmy rzeczywiscie robili grę
To co będziemy chcieli to chcieli byśmy kalkulowac dystans między
krzyzykiem (cyzi dokładniej 2 prostymi krzyzyka) a środkiem kolka
jeżeli dystans będzie <= r no to mamy kolizje wpp jej nie mamy
*/

float two_points_line_length(float x1, float y1, float x2, float y2) { // to uzywam do wyliczania dlugosci lini
   return sqrt(  pow((x2 - x1), 2) + pow((y2 - y1), 2)    );
 }



// bool Collision(float tx, float ty, float cx, float cy, float r, float half_cross) {
// tutaj próbowałem zrobić tak aby kólko po zetknięciu się z "bokami" krzyzyka też dawało detekcje
// lecz po przemyśleniach skojarzeine z gra z treści polecenia przypomniało mi że krzyżyk sygnalizuje oczywiście
// celownik a kółko jest celem dlategoż zostałem przy takowym rozwiązaniu.
   

//    r = r/16.0f;
//    half_cross = half_cross / 16.0f;
//    float dist = two_points_line_length(cx, cy, tx, ty);

//    // float x_dist = fabs( cy - ty ) / 16.0f; //
//    // float y_dist = fabs( cx - tx ) / 16.0f;
//    // std::cout << x_dist << std::endl;
//    // std::cout << y_dist << std::endl;



//    // std::cout << tx << " " << ty << std::endl;
   
//    // teraz posprawdzamy sobie warunki
//    if(dist <= r) { 
//       std::cout << "x radius " << dist << " cx " << cx << " tx " << tx << " " << half_cross << std::endl;

//       if(two_points_line_length(cx, cy, tx + half_cross, ty) && two_points_line_length(cx, cy, tx - half_cross, ty)) {
//       return true;
//       }
//    }

//    // float y_dist = fabs(cx - tx);
//    if(dist <= r) {
//       std::cout << "y radius " << dist << " cy " << cy << " ty " << ty << std::endl;

//       if((cy >= ty + half_cross) && (cy <= ty - half_cross)) {
//       return true;
//       }
//    }
//    return false;

//  }



bool Collision(float tx, float ty, float cx, float cy, float r, float half_cross) {

   // float x_cross_arm = x_line_length / 32.0f; 
   // float y_cross_arm = y_line_length / 32.0f; 

   r = r / 16.0f; // promień
   half_cross = half_cross / 16.0f; // kawałek
   // float x_dist = fabs( cy - ty ); //
   // float y_dist = fabs( cx - tx );
   float dist = two_points_line_length(cx, cy, tx, ty);
   // float y_dist = two_points_line_length(cx, cy, tx, ty);
   // std::cout << x_dist << std::endl;
   // std::cout << y_dist << std::endl;


   // std::cout << tx << " " << ty << std::endl;
   
   // teraz posprawdzamy sobie warunki
   if(dist <= r ) { 
      std::cout << "dist " << dist << " cx " << cx << " tx " << tx << std::endl;
      // if(x_dist + ) 
      // if((r >= tx + x_cross_arm) && r <= tx - x_cross_arm) {
      return true;
      // }
   }

   // float y_dist = fabs(cx - tx);
   // if(y_dist <= r) {
   //    std::cout << "y radius " << y_dist << " cy " << cy << " ty " << ty << std::endl;

   //    if((r >= ty + y_cross_arm) && r <= ty - y_cross_arm) {
   //    return true;
   //    }
   // }

   // float val = two_points_line_length(cx, cy, tx, ty);
   // if (val <= r) {return true;}

   return false;

 }






// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================
class MyTri : public AGLDrawable {
public:
   MyTri() : AGLDrawable(0) {
      setShaders();
   }
   void setShaders() {
      float middle = 0.0f; // bo to kwadrat moge zmienic jak zle
      // (x1 + x2) / 2 , (y1 + y2) / 2 
      // komentarz:
      // Znalazłem bardzo interesująca ksiązkę https://thebookofshaders.com/
      //out vec2 vpos; // tutaj dodaje "to co na wyjście" dokładniej pozycje wierzchołków
      //...
      //vpos = vertices[gl_VertexID]; // gl_VertexID treść pytania o jaki pkt chodzi (nr pkt 0-3)
      //...
      //float center_to_pixel = length(vpos); // odległość od środka (0.0)
      //float dist_to_edge = 0.707107f - center_to_pixel; ( sqrt(2) / 2 ) // c_t_p to nam da odległość pixela od krawędzi
      //flot dist_percent = dist_to_edge / 0.707107f ;  // procent drogi pixela do srodka



      compileShaders(R"END(

         #version 330 
         out vec4 vcolor;
         out vec2 vpos; 
         void main(void) {
            const vec2 vertices[4] = vec2[4](vec2( 0.9, -0.9), 
                                             vec2(-0.9, -0.9),
                                             vec2( 0.9,  0.9),
                                             vec2( -0.9,  0.9)); // 4 wierzchołek
            const vec4 colors[]    = vec4[4](vec4(1.0, 0.0, 0.0, 1.0),
                                             vec4(0.0, 1.0, 0.0, 1.0),
                                             vec4(0.0, 0.0, 1.0, 1.0),
                                             vec4(1.0, 0.0, 1.0, 1.0)); // 4 kolor

            vcolor      = colors[gl_VertexID];
            vpos = vertices[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
         }

      )END", R"END(

         #version 330 
         in  vec4 vcolor;
         in vec2 vpos;
         out vec4 color;

         void main(void) {
            color = vcolor;
            float center_to_pixel = length(vpos); // odległość pixela od środka
            float dist_to_edge = sqrt(2) - center_to_pixel ; // odległośc od krawędzi "prawie dokładnie"

            //float dist_percent = dist_to_edge / sqrt(2) ;

            vec3 white = vec3(exp(-center_to_pixel * 7.0f)); //jak cos to przez kontrast z bialym
            //eksponenta (z -) bo przy 0 da nam 1 a przy -inf da 0
            // czyli im blizej środka tym blizej naszego celu

            //vec3 color_gradient = color.rgb + white; // -

            color.rgb = color.rgb * 0.8f + white; // tutaj dotykamy tylko wspolzedne rgb bez alfy / opacity
            
            //color.rgb = color_gradient; //-
            //color_gradient.rgb = color.rgb * dist_percent * 3.0f; // od 1 na krancu do inf w środku // -
            //olor_gradient.w = color.w; // -
            //color = color_gradient; // -
         } 

      )END");
   }
   void draw() {
      bindProgram();
      // glDrawArrays(GL_QUADS, 0, 4); // tak nie dziala
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   }
};


// ==========================================================================
// Drawable object with some data in buffer and vertex/fragment programs
// ==========================================================================
class MyCross : public AGLDrawable {
public:
   float half_hand;
   // int x_line_length;
   // int y_line_length;

   MyCross() : AGLDrawable(0) { // konstruktor
      setShaders(); // setuping shaders
      setBuffers(); // setuping geometry data
   }
   void setShaders() {
      compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec2 pos; 
         layout(location = 0) uniform float scale;
         layout(location = 1) uniform vec2  center;
         out vec4 vtex;

         void main(void) {
            vec2 p = (pos * scale + center);
            gl_Position = vec4(p, 0.0, 1.0);
         }

      )END", R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         layout(location = 3) uniform vec3  circle_color;
         out vec4 color;

         void main(void) {
            color = vec4(circle_color,1.0);
         } 

      )END");
   }
   void setBuffers() { 
      bindBuffers();
      GLfloat vert[4][2] = {  // Cross lines
         { -1,  0  },
         {  1,  0  },
         {  0, -1 },
         {  0,  1 }
      };


      half_hand = two_points_line_length(vert[3][0], vert[3][1], vert[2][0], vert[2][1]);

      // //sqrt( (x2 - x1)^2 + (y2 - y1)^2)
      // x_line_length = 
      // // fabs(vert[1][0] - vert[0][0]);
      // two_points_line_length(
      //    vert[0][0], vert[0][1], // x1
      //    vert[1][0], vert[1][1] // x2
      // ) ;

      // y_line_length = 
      // // fabs(vert[3][1] - vert[2][1]);
      // two_points_line_length(
      //    vert[2][0], vert[2][1],
      //    vert[3][0], vert[3][1]
      // ) ;


      glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), vert, GL_STATIC_DRAW );
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0,                 // attribute 0, must match the layout in the shader.
         2,                  // size
         GL_FLOAT,           // type
         GL_FALSE,           // normalized?
         0,//24,             // stride
         (void*)0            // array buffer offset
      );
   }
   void draw(float tx, float ty) {
      bindProgram();
      bindBuffers();
      glUniform1f(0, 1.0/16);  // scale  in vertex shader
      glUniform2f(1, tx, ty);  // center in vertex shader
      glUniform3f(3, circle_color[0],circle_color[1],circle_color[2]);

      glDrawArrays(GL_LINES, 0, 4);
   }
   void setColor(float r, float g, float b){
      circle_color[0]=r;circle_color[1]=g;circle_color[2]=b;
   }
 private:
   GLfloat circle_color[3] = { 0.0, 1.0, 0.0 };
};
 // tutaj trzeba to przerobić na kółko

class MyCircle : public AGLDrawable {
public:
   MyCircle(int points_a, float x, float y, float r, double R, double G, double B) : AGLDrawable(0) {
   //   MyCircle(int points_a) : AGLDrawable(0) {
      this->points_a = points_a;
      this->x = x;
      this->y = y;
      this->r = r;
      this->R = R;
      this->G = G;
      this->B = B;
      setShaders();
      setBuffers();
   }
   void setShaders() {
      compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec2 pos;
         layout(location = 0) uniform float scale;
         layout(location = 1) uniform vec2  center;
         out vec4 vtex;

         void main(void) {
            vec2 p = (pos * scale + center);
            gl_Position = vec4(p, 0.0, 1.0);
         }

      )END", R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         layout(location = 3) uniform vec3  circle_color;
         out vec4 color;

         void main(void) {
            color = vec4(circle_color,1.0);
         } 

      )END");
   }
   void setBuffers() { 
      bindBuffers();
      //int points_amount = 50;
      //zobaczmy czy to zadziała
      GLfloat vert[points_a][2];
      // float x = 0.0;                    // x cord
      // float y = 0.0;                      // y cord
      // double r = 10;                  // circle radius
      float theta = 0.0;                // theta
      double pi_times_two = 2.0f * M_PI; // full circle / 2 * pi

      for(int i = 0; i < points_a; i++) {
         theta = pi_times_two * (float(i) / float(points_a)); // to jest nasz krok
         x = r * cos(theta);
         y = r * sin(theta);
         vert[i][0] = x;
         vert[i][1] = y;
      }
      

      glBufferData(GL_ARRAY_BUFFER, points_a*2*sizeof(float), vert, GL_STATIC_DRAW );
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0,                 // attribute 0, must match the layout in the shader.
         2,                  // size
         GL_FLOAT,           // type
         GL_FALSE,           // normalized?
         0,//24,             // stride
         (void*)0            // array buffer offset
      );
   }
   void draw(float tx, float ty) {
      bindProgram();
      bindBuffers();
      glUniform1f(0, 1.0/16);  // scale  in vertex shader
      glUniform2f(1, tx, ty);  // center in vertex shader
      glUniform3f(3, circle_color[0],circle_color[1],circle_color[2]);

      // glDrawArrays(GL_LINES, 0, points_a);
      glDrawArrays(GL_LINE_LOOP, 0, points_a); // przeczytałem ze mozna jest wiecej rzeczy poza liniami i ze tutaj loop sie nada lepiej
   }
   void setColor(float r, float g, float b){
      circle_color[0]=r;circle_color[1]=g;circle_color[2]=b;
   }
 private:
   GLfloat circle_color[3] = { 1.0, 0.0, 0.0 };
   int points_a; 
   float x;                   
   float y;                      
   double r;
   double R;
   double G;
   double B;  
};


// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:
    MyWin() {};
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void MainLoop();
};


// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods) {
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
   
   float   tx=0.0f, ty=0.5f;
   float   cx = -0.8f, cy = 0.0f;
   float circle_radious = 0.5f; 
   // circle_radious = circle_radious / 16.0f;

   MyTri   trian;
   MyCross cross;
   MyCircle circle(500, cx, cy, circle_radious, 1.0, 0.0, 0.0);
   
   float half_hand = cross.half_hand;
   

   // MyCircle circle(50);
   do {
      glClear( GL_COLOR_BUFFER_BIT );
   
      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      trian.draw();
      cross.draw(tx, ty); // tutaj przekazywane sa wspolrzedne w kazdym obrocie petli
      circle.draw(cx, cy); // dlatego to basicly sie rusza teraz zostaje wykminic jak to przekminic zeby 
                              // zeby to ladnie dzialalo i zrobic ta detekcje po tym zostanie ten prostokat do namalowania
      
      // dobra zobaczmy czy się stykają
      if(Collision(tx, ty, cx, cy, circle_radious , half_hand)) {
         // std::cout << half_hand << " " << circle_radious;
         // GLFWwindowclosefun(win());
         std::cout << "wow " << two_points_line_length(cx, cy, tx, ty) << " " <<  circle_radious / 16.0f<< std::endl;
         // glfwDestroyWindow(win());
         glfwSetWindowShouldClose(win(), 1);
         
      }


      AGLErrors("main-afterdraw");


      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();
      //glfwWaitEvents();   

      if (glfwGetKey(win(), GLFW_KEY_F ) == GLFW_PRESS) { // wciskanie o odpala
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wciskanie f gasi
      } else if (glfwGetKey(win(), GLFW_KEY_O ) == GLFW_PRESS) { 
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      } else if (glfwGetKey(win(), GLFW_KEY_D ) == GLFW_PRESS) { // tutaj dodalem "wsad"
         cx += 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_A ) == GLFW_PRESS) {
         cx -= 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_W ) == GLFW_PRESS) {
         cy += 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_S ) == GLFW_PRESS) {
         cy -= 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) { // tutaj dodalem "wsad"
         tx += 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS) {
         tx -= 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
         ty += 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
         ty -= 0.01;
      }

   } while( glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0 );
}


int main(int argc, char *argv[]) {
   MyWin win;
   win.Init(800,600,"AGL3 example",0,33);
   win.MainLoop();
   return 0;
}