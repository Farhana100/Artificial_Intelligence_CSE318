class Stone{
    int posX;
    int posY;
    int celli;
    int cellj;

public:
    char img;
    Stone()
    {
        this->posX = 0;
        this->posY = 0;
        this->celli = 0;
        this->cellj = 0;
        img = STONE;
    }

    void setPosX(int posX){this->posX = posX;}
    void setPosY(int posY){this->posY = posY;}
    int getPosX(){return posX;}
    int getPosY(){return posY;}

    void setCelli(int Celli){this->celli = celli;}
    void setCellj(int Cellj){this->cellj = cellj;}
    int getCelli(){return celli;}
    int getCellj(){return cellj;}

    void move(int x, int y, int celli, int cellj)
    {
        this->posX = x;
        this->posY = y;
        this->celli = celli;
        this->cellj = cellj;
    }
};
