#pragma once

#include <string>

class View{
    private:
        static int view_count;
    public:
        virtual void update();
        // TODO: fix the equality check once the views are distinguished
        bool operator==(const View& rhs) {
            return true;
        }
};

class FullTrackView : public View{
    private:
    public:
        void update();
};

class EditorView : public View{
    public:
        void update();
};

class PlaylistView : public View{
    public:
        void update();
};