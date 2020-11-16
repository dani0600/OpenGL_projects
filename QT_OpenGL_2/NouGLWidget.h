#include "MyGLWidget.h"

class NouGLWidget : public MyGLWidget
{
 Q_OBJECT
	   
 public:
  NouGLWidget (QWidget *parent=NULL);
  ~NouGLWidget ();

 public slots: 

 void QTCamera();

 signals: 

 void sigcamera();

 protected:
  void paintGL ();
  void iniCamera ();
  void iniEscena ();
  void viewTransform ();
  void projectTransform ();
  void modelTransformPatricio ();
  void modelTransformPatricio2 ();
  void focusTransform();

  
  // keyPressEvent - Es cridat quan es prem una tecla
  void keyPressEvent (QKeyEvent *event);
  
 private:
 	//Detalls Patricios
 	float escalaPat1 = 2;
 	float escalaPat2 = 3;
 	

 	float px2, pz2;

 	float anglePat1, anglePat2;
 	glm::vec3 posPat1, posPat2;

 	glm::vec3 vrp, obs, up;

 	float fov;

 	bool gopro;

};
