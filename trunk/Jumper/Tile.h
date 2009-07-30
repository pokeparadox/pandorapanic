#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED


class Tile : public Sprite
{
    public:
        Tile();


        void setHeight(const int& x){height = x;}
        void setWidth(const int& y){width = y;}
        void setDimensions(const int& x, const int& y){height = x; width = y;}
        void setDimensions(const Vector2di& dimensions){height = dimensions.x; width = dimensions.y;}
        int getHeight(){return height;}
        int getWidth(){return width;}
        void setMovement(const int& x, const int& y){movement.x = x; movement.y = y;}
        void setXMovement(const int& x){movement.x = x;}
        void setYMovement(const int& y){movement.y = y;}
        int getXMovement(){return movement.x;}
        int getYMovement(){return movement.y;}
        int moving;
        int hasblob;

        //bool ontile(
    private:
        int width;
        int height;
        bool heroontile;
        Vector2di movement;

};


#endif // TILE_H_INCLUDED
