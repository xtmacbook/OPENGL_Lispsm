#ifndef PriorityVectorH
#define PriorityVectorH
//---------------------------------------------------------------------------
#include <vector>
#include <algorithm>
//---------------------------------------------------------------------------
template<class T, class Compare = std::less<T> >
class PriorityVector  {
protected:
    std::vector<T> h;
    Compare comp;
    typedef std::vector<T>::size_type Index;
    Index hfirst(const Index id) { return id*2+1; }
    Index hsecond(const Index id) { return hfirst(id)+1; }
    Index father(const Index id) { return ((id-1)/2); }
public:

    PriorityVector(const Compare& vComp = Compare()): comp(vComp) { }
    T& operator[](Index id) { return h[id]; }
    void clear() {  h.clear(); }
    bool empty() const { return h.empty(); }
    void pop() { deleteElement(0); }
    const Index size() const { return h.size(); }
    const T& first() const { return h.front(); }
    T& first() { return h.front(); }

    void push(const T& element) {
        Index i;
        h.resize(h.size()+1);
        i = h.size()-1;
        while((i > 0) && comp(h[father(i)],element)) {
            h[i] = h[father(i)];
            i = father(i);
        }
        h[i] = element;
    }


    void deleteElement(const unsigned id) {
        unsigned position = id;
        while(position < h.size()) {
            unsigned childpos = hfirst(position);
            if(childpos < h.size()) {
                if(childpos+1 < h.size()) {
                    if(!comp(h[childpos+1],h[childpos])) {
                        childpos++;
                    }
                }
                if(!comp(h.back(),h[childpos])) {
                    h[position] = h.back();
                    h.resize(h.size()-1);
                    return;
                }
                else {
                    h[position] = h[childpos];
                    position = childpos;
                }
            }
            h.resize(h.size()-1);
        }
    }

    bool valid() const {
        for(Index i = 0; i+1 < size(); i++) {
            if(hfirst(i) < size()) {
                if(comp(h[i],h[hfirst(i)])) {
                    return false;
                }
            }
            if(hsecond(i) < size()) {
                if(comp(h[i],h[hsecond(i)])) {
                    return false;
                }
            }
        }
        return true;
    }
};
#endif
