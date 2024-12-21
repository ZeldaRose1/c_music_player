#include <string>
#include "View.h"
#include "Database.h"


void View::setDatabase(Database d){
    this->d = &d;
}

MainView::MainView(Database d){
    setDatabase(d);
}