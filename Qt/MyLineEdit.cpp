#include "MyLineEdit.h"

// constructor
MyLineEdit::MyLineEdit(QWidget *parent): QLineEdit(parent) {
}

// implementació slots
void MyLineEdit::tractaReturn() {
    // Implementació de tractaReturn
    emit enviaText (text());
}
