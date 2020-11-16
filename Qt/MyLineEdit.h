#include <QLineEdit>

class MyLineEdit: public QLineEdit{
    Q_OBJECT     //IMPORTANT
    
    public:
        MyLineEdit (QWidget *parent);
    public slots:      // IMPORTANT
        void tractaReturn ();
    signals:     // IMPORTANT
        void enviaText (const QString &);
};
