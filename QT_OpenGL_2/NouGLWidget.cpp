#include "NouGLWidget.h"

#include <iostream>

NouGLWidget::NouGLWidget(QWidget *parent) : MyGLWidget(parent) {
  grabKeyboard();
}

NouGLWidget::~NouGLWidget ()
{
}

void NouGLWidget::paintGL() {
	#ifdef __APPLE__
	GLint vp[4];
	glGetIntegerv (GL_VIEWPORT, vp);
	ample = vp[2];
	alt = vp[3];
#endif

	  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
	  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
	  //  glViewport (0, 0, ample, alt);
	  
	  // Esborrem el frame-buffer i el depth-buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  //--------------------------------------------------------
	  // Activem el VAO per a pintar el terra
	glBindVertexArray (VAO_Terra);  
	  // pintem terra
	modelTransformTerra ();
	glDrawArrays(GL_TRIANGLES, 0, 12);

	  //--------------------------------------------------------
	  // Activem el VAO per a pintar el Patricio
	glBindVertexArray (VAO_Pat);
	  // pintem el Patricio
	modelTransformPatricio();
	glDrawArrays(GL_TRIANGLES, 0, patModel.faces().size()*3);
	  // pintem el Patricio2
	modelTransformPatricio2();
	glDrawArrays(GL_TRIANGLES, 0, patModel.faces().size()*3);

	  //--------------------------------------------------------
	glBindVertexArray(0);
}

void NouGLWidget::iniCamera ()
{	
	MyGLWidget::iniCamera();
	angleX = float(M_PI)/6;	
	obs = posPat2 + glm::vec3(0.0,0.5,0.0);
	up = glm::vec3(0,1,0);
	
}

void NouGLWidget::iniEscena ()
{
	posPat1 = glm::vec3(2,0,1);
	posPat2 = glm::vec3(4,0,6);

	px2 = 4.0f;
	pz2 = 6.0f;
	
	anglePat1 = float(M_PI)/2;
 	anglePat2 = float(M_PI);

  	MyGLWidget::iniEscena();
  	centreEsc = glm::vec3(4,2,4);
  	radiEsc = distance(centreEsc, glm::vec3(0,0,0));

  	gopro = false;
}

void NouGLWidget::focusTransform(){
  	
  	glm::vec3 posFocus = glm::vec4(1,1,1,1);

    glUniform4fv(posFocusLoc,1, &posFocus[0]);  
     
  }

void NouGLWidget::modelTransformPatricio ()
{
	glm::mat4 patTG = glm::mat4(1.0f);
	patTG = glm::translate(patTG, posPat1);
	patTG = glm::rotate(patTG, anglePat1, glm::vec3(0,1,0));
    patTG = glm::scale(patTG, glm::vec3(escalaPat1*escalaPat,escalaPat1*escalaPat, escalaPat1*escalaPat));
    patTG = glm::translate(patTG, -centreBasePat);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &patTG[0][0]);
}

void NouGLWidget::modelTransformPatricio2(){
	posPat2 = glm::vec3(px2,0,pz2);
    glm::mat4 patTG = glm::mat4(1.0f);
	patTG = glm::translate(patTG, posPat2);
	patTG = glm::rotate(patTG, anglePat2, glm::vec3(0,1,0));
    patTG = glm::scale(patTG, glm::vec3(escalaPat2*escalaPat,escalaPat2*escalaPat, escalaPat2*escalaPat));
    patTG = glm::translate(patTG, -centreBasePat);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &patTG[0][0]);
    if(gopro){
    	viewTransform();
    	projectTransform();
    }
    	

}

void NouGLWidget::viewTransform() {
 	
    
    if(!gopro){
	    glm::mat4 View(1.0f);
        View = glm::translate(View, glm::vec3(0,0,-(radiEsc*2)));
        View = glm::rotate(View, angleX, glm::vec3(1,0,0));
        View = glm::rotate(View, -angleY, glm::vec3(0,1,0));
        View = glm::translate(View, glm::vec3(-centreEsc));
        glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
    }
    else{
    	glm::mat4 View2(1.0f); // Matriu GoPro
    	obs = posPat2 + glm::vec3(0.0,3.5,0.0);
    	vrp = obs + glm::vec3(0,0, -posPat2.z);
    	View2 = glm::lookAt(obs, vrp, up);
    	glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View2[0][0]);

    }
    focusTransform();
}

void NouGLWidget::projectTransform() {

	float zn, zf;
    glm::mat4 Proj;  // Matriu de projecció
	float raw =  width() / (float)height() ;
	
	if(gopro){
        fov = M_PI/2.0;
        raw = rav;
        zn = 0.1;
        zf = 20;
    }
    else{
    	fov = float(M_PI/3.0);
		zn = radiEsc;
		zf = 3*radiEsc;

		
		if(raw<rav){
	    	fov= atan(tan(0.5*fov)*radiEsc/raw);
		}	
    }


	Proj = glm::perspective(fov, raw, zn, zf);

	glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
	
}



void NouGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: {
    	if (pz2 + 0.5 < 8.0) pz2 += 0.5;
	  	modelTransformPatricio2();
      break;
    }
    case Qt::Key_W: {
      	if (pz2 - 0.5 > 0.0) pz2 -= 0.5;
	  	modelTransformPatricio2();
      break;
    }
    case Qt::Key_A: {
    	if (px2 - 0.5 > 0.0) px2 -= 0.5;
	  	modelTransformPatricio2();
      break;
    }
    case Qt::Key_D: {
    	if (px2 + 0.5 < 8.0) px2 += 0.5;
	  	modelTransformPatricio2();
      break;
    }
    case Qt::Key_C: {
	    gopro = !gopro;
	    emit sigcamera();
	    viewTransform();
	    projectTransform();
      break;
    }
    default: {
      MyGLWidget::keyPressEvent(event);
      break;
    }
  }
  update();
}

void NouGLWidget::QTCamera(){
	makeCurrent();
	gopro = !gopro;
	viewTransform();
	projectTransform();
	update();
}
  
