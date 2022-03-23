
class MenuItem {
  private:
    void (*handler)(MenuItem*);

  public:
    String _name;
    MenuItem* sub;
    MenuItem* parent;
    MenuItem* back;
    MenuItem* next;
    void (*navigate)();
    void (*setvalue)(MenuItem*);
    int tag; // для любой нужды

    MenuItem(String Name, void (*Handler)(MenuItem*)) {
      handler = Handler;
      _name = Name;
      sub = NULL;
      parent = NULL;
      back = NULL;
      next = NULL;
      navigate = NULL;
      setvalue = NULL;
      tag = -1;
    }

    MenuItem(String Name, void (*Handler)(MenuItem*), void (*Navigate)(),  void (*SetValue)(MenuItem*)) {
      handler = Handler;
      _name = Name;
      sub = NULL;
      parent = NULL;
      back = NULL;
      next = NULL;
      navigate = NULL;
      navigate = Navigate;
      setvalue = SetValue;
      tag = -1;
    }

    void Invoke()  {
      if (handler != NULL) {
        (*handler)(this);
      }
    }
     void SetValue()  {
      if (setvalue != NULL) {
        (*setvalue)(this);
      }
    }
};

class MenuManager {
  private:
    MenuItem* first;


  public:
    MenuItem* current;
    MenuManager()  {
      current = NULL;
      first = NULL;
    }
    MenuManager* Append(MenuItem* item)  {
      if (current != NULL) {
        current->next = item;
      } else {
        first = item;
      }

      item->back = current;
      item->parent = current->parent;
      ///item->next = first; // зацикливание
      current = item;

      return this;
    }
    MenuManager* AddSub(MenuItem* item) {
      if (current != NULL) {
        current->sub = item;
      }

      item->parent = current;
      current = item;

      return this;
    };
    MenuItem* Next()  {
      if (current->next != NULL) {
        current = current->next;
      }
      if (current->navigate != NULL) {
        current->navigate();
      }

      return current;
    };
    MenuItem* Back()  {
      if (current->back != NULL) {
        current = current->back;
      }
      if (current->navigate != NULL) {
        current->navigate();
      }

      return current;
    };
    MenuItem* Sub()  {
      if (current->sub != NULL) {
        current = current->sub;
      }

      return current;
    };
    MenuItem* ToParent()  {
      if (current->parent != NULL) {
        current = current->parent;
      }

      return current;
    };
    MenuItem* Find(String Name) {
      for (current = first; current != NULL; current = current->next) {
        //Serial.println(current->_name);
        if (current->_name == Name) {
          return current;
        }
      }

      return NULL;
    }
};
