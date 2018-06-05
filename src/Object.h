#pragma once

enum class ObjectType
{
  BUTTON,
  TEXT,
  PASSWORD
};

class Object
{
public:
  int X() const;
  int Y() const;
  ObjectType GetType();
  virtual void Select(bool select) = 0;
  virtual void Select() = 0;
  virtual void Show(bool show) = 0;
  virtual void Move(int x, int y) = 0;
  virtual bool Hidden() const = 0;
protected:
  int x_;
  int y_;
  bool show_;
  ObjectType type_;

  Object(int x, int y, bool show);
};