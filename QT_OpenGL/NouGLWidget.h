#include "MyGLWidget.h"

class NouGLWidget : public MyGLWidget
{
 Q_OBJECT
	   
 public:
  NouGLWidget (QWidget *parent=NULL);
  ~NouGLWidget ();

 public slots:

 	void movLegoDreta();
 	void movLegoEsquerra();
 	void rotLego();

 signals:



 protected:
  void paintGL ();
  void resizeGL (int w, int h);
  void iniCamera ();
  void iniEscena ();
  void viewTransform ();
  void projectTransform ();
  void modelTransformLego ();
  void modelTransformCub ();
  void focusTransform();
  
  
  // keyPressEvent - Es cridat quan es prem una tecla
  void keyPressEvent (QKeyEvent *event);
  
 private:

 	glm::mat4 legoTG, cubTG;

 	float Yangle, fov;

 	float posLegoX;

 	float left, right, bottom, top;

 	glm::vec3 colFocus;

 	bool ortogonal;

 	glm::vec3 centreBaseCub, posLego, posCub;

};
