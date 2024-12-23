#include <string>
#include "../view/View.h"
#include "../database/Database.h"


void View::setDatabase(Database d){
    this->d = &d;
}

MainView::MainView(Database d){
    setDatabase(d);
}