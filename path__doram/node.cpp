class Node
{
public:
  int key;
  int value;
  bool isDummy;
  int pos;
  int leftPos;
  int rightPos;
  int leftKey;
  int rightKey;
  int level;
  int bucketId;

  Node(int key, int value, int isDummy)
  {
    if (isDummy == 1)
    {
      this->key = -1;
      this->value = 0;
      this->isDummy = true;
      this->pos = -1;
      this->leftPos = -1;
      this->rightPos = -1;
      this->leftKey = -1;
      this->rightKey = -1;
      this->level = -1;
      this->bucketId = -1;
    }
    else
    {
      this->key = key;
      this->value = value;
      this->isDummy = false;
      this->pos = -1;
      this->leftPos = -1;
      this->rightPos = -1;
      this->leftKey = -1;
      this->rightKey = -1;
      this->level = -1;
      this->bucketId = -1;
    }
  }
};