
template <class T, int size>
class Ring {
public:
    T buf[size] = {0};
    int ptr = 0;


    void put(T elem) {
        ptr = (ptr + 1) % size;
        buf[ptr] = elem;
    }

    T getLast(int offset) {
        int ind = (ptr - offset + size) % size;
        return buf[ind];
    }
};