template<typename T> class List{
    struct Node{
        T data;
        Node* next;
        Node(T c) : data(c), next(nullptr) { }
    };
private:
    Node *_head;
    Node *_tail;
    int _size;
public:
    List() : _head(nullptr), _tail(nullptr) { }

    ~List(){
        Node *curr = _head;
        while(curr != nullptr){
            Node *temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    void push_back(T c){
        Node *node = new Node(c);
        if(!_head)
            _head = _tail = node;
        else{
            _tail->next = node;
            _tail = node;
        }
    }

    void insert_at(T c, int index){
        Node *node = new Node(c);
        if(index == 0){
            node->next = _head;
            _head = node;
        }
        else{
            Node *curr = _head;
            for(int i = 0; i < index - 1; i++)
                curr = curr->next;
            node->next = curr->next;
            curr->next = node;
        } 
    }

    T get(int index){
        Node *curr = _head;
        for(int i = 0; i < index; i++)
            curr = curr->next;
        return curr->data;
    }

    int size() { return _size; }

    std::string to_string(){
        std::string str;
        Node *curr = _head;
        while(curr != nullptr){
            str += curr->data;
            curr = curr->next;
        }
        return str;
    }

    void print(){
        Node *curr = _head;
        while(curr != nullptr){
            std::cout << curr->data;
            curr = curr->next;
        }
        std::cout << std::endl;
    }
};