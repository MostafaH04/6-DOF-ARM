class Positions{
  private:
    long positions[6];

  public:
    Positions();
    void setPosition(long motor, long position);

    void setPositions(long pos1, long pos2, long pos3, long pos4, long pos5, long pos6);

    void setPositionsAngles(long angle1, long angle2, long angle3, long angle4, long angle5, long angle6);

    long *getPositions();
};