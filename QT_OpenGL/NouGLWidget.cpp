#include "NouGLWidget.h"

#include <iostream>

NouGLWidget::NouGLWidget(QWidget *parent) : MyGLWidget(parent) {
  grabKeyboard();
}

NouGLWidget::~NouGLWidget ()
{
}

void NouGLWidget::paintGL() {
  MyGLWidget::paintGL();
}


void NouGLWidget::iniCamera ()
{
  MyGLWidget::iniCamera();
  colFocus = glm::vec3(1.0,1.0,1.0);

  left = -radiEsc; 
  right = radiEsc;
  bottom = -radiEsc;
  top = radiEsc;
  focusTransform();
}

void NouGLWidget::iniEscena ()
{
  MyGLWidget::iniEscena();
  ortogonal = false;
  posLegoX = 7;
  centreBaseCub = glm::vec3(0,0,0);
  posLego = glm::vec3(posLegoX,0,3);
  Yangle = M_PI/2;
  posCub = glm::vec3(1,0,3);
  centreEsc = glm::vec3(4, 1.5, 3);
  radiEsc = distance(centreEsc, glm::vec3(0,0,0));
}

void NouGLWidget::modelTransformLego ()
{
    glm::mat4 legoTG = glm::mat4(1.0f);
    legoTG = glm::translate(legoTG, posLego);
    legoTG = glm::scale(legoTG, glm::vec3(2*escalaLego, 2*escalaLego, 2*escalaLego));
    legoTG = glm::rotate(legoTG, -Yangle, glm::vec3(0,1,0));
    legoTG = glm::translate(legoTG, -centreBaseLego);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &legoTG[0][0]);
}

void NouGLWidget::modelTransformCub ()
{
    glm::mat4 cubTG(1.f);
    cubTG = glm::translate(cubTG, posCub);
 	cubTG = glm::scale(cubTG, glm::vec3(0.5,3,5));
 	cubTG = glm::translate(cubTG, -centreBaseCub);
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &cubTG[0][0]);
}

void NouGLWidget::resizeGL(int w, int h){
	float ample = w;
	float alt = h;
	float raw = float(w)/float(h);
	float radi = radiEsc;

	if(ortogonal){
		rav = raw;
		if(rav < 1.0){
			top = radi/rav;
			bottom = -radi/rav;
			left = -radi; 
			right = radi;
		}
		else{
			top = radi;
			bottom = -radi;
			left = -radi*rav; 
			right = radi*rav;
		}
		projectTransform();
	}

	glViewport(0,0,ample,alt);
}

void NouGLWidget::viewTransform() {
	if(not ortogonal){
		angleX = float(M_PI/6.0);
		glm::mat4 View;
		View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
		View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
		View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
	    View = glm::translate(View, glm::vec3(-centreEsc));
		glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
	} 
	else{
		angleX = float(M_PI/2);
		glm::mat4 View;
		View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
		View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
		View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
	    View = glm::translate(View, glm::vec3(-centreEsc));
		glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
	}
	
}

void NouGLWidget::projectTransform() {
	float zn, zf;
    glm::mat4 Proj;  // Matriu de projecció
    float raw =  width() / (float)height() ;
    //fov = 2*asin(radiEsc/(radiEsc*2));	
	if(ortogonal){
	    
	    zn = 0.1;
	    zf = 20;

	    Proj = glm::ortho(left, right, bottom, top, zn, zf);

	}
	else{
		fov = float(M_PI/3.0);
		zn = radiEsc;
		zf = 3*radiEsc;
		
		if(raw<rav){
		   	fov= atan(tan(0.5*fov)*radiEsc/rav);
		}	
		Proj = glm::perspective(fov, raw, zn, zf);
	}

	
	glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void NouGLWidget::focusTransform(){
	if(posLego.x != 2){
		colFocus = glm::vec3(1.0,1.0,1.0);
		
	}
	else{
		colFocus = glm::vec3(1.0,0.0,0.0);

	}
	glUniform3fv(colFocusLoc,1,&colFocus[0]);
	colFocusLoc = glGetUniformLocation (program->programId(), "colFocus");
	
}

void NouGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_A: {
    	if(posLegoX > 2){
    		posLegoX -= 0.5;
    		posLego.x = posLegoX;
    		
      	}

      	break;

    }
    case Qt::Key_D: {
    	if(posLegoX + 0.5 < 8){
    		posLegoX += 0.5;
    		posLego.x = posLegoX;
    	} 
      	break;
    }
    case Qt::Key_R: {
    	if(Yangle < float(M_PI)) Yangle += float(M_PI);
    	else Yangle = M_PI/2.0;	
    	
      break;
    }
    case Qt::Key_C: {
    	ortogonal = !ortogonal;
    	projectTransform();
      break;
    }
    default: {
      MyGLWidget::keyPressEvent(event);
      break;
    }
  }
  viewTransform();
  focusTransform();
  modelTransformLego();
  update();
}

void NouGLWidget::movLegoDreta(){
	makeCurrent();
	if(posLegoX + 0.5 < 8){
    		posLegoX += 0.5;
    		posLego.x = posLegoX;
    } 
    modelTransformLego();
    focusTransform();
	update();


}
 	
void NouGLWidget::movLegoEsquerra(){
	makeCurrent();
	if(posLegoX > 2){
    	posLegoX -= 0.5;
    	posLego.x = posLegoX;
    }
    modelTransformLego();
    focusTransform();

	update();



}

void NouGLWidget::rotLego(){

	makeCurrent();
	if(Yangle < float(M_PI)) Yangle += float(M_PI);
    else Yangle = M_PI/2.0;	
    modelTransformLego();

	update();


}
  
