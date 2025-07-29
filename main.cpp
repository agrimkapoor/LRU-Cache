// lru_cache.cpp
#include <iostream>
#include <unordered_map>
using namespace std;

struct Node {
    int key, value;
    Node *prev, *next;
    Node(int k, int v): key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    int capacity;
    unordered_map<int, Node*> cache;
    Node *head, *tail;

    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void insert(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

public:
    LRUCache(int cap): capacity(cap) {
        head = new Node(0, 0); // dummy head
        tail = new Node(0, 0); // dummy tail
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            remove(node);
            insert(node);
            return node->value;
        }
        return -1;
    }

    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            remove(cache[key]);
            delete cache[key];
        }
        if (cache.size() == capacity) {
            Node* lru = tail->prev;
            remove(lru);
            cache.erase(lru->key);
            delete lru;
        }
        Node* node = new Node(key, value);
        insert(node);
        cache[key] = node;
    }

    void display() {
        Node* curr = head->next;
        cout << "Cache state: ";
        while (curr != tail) {
            cout << "(" << curr->key << "," << curr->value << ") ";
            curr = curr->next;
        }
        cout << endl;
    }
};

int main() {
    LRUCache cache(3);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);
    cache.display();        // 3,2,1

    cache.get(1);
    cache.display();        // 1,3,2

    cache.put(4, 40);
    cache.display();        // 4,1,3 (2 is evicted)

    return 0;
}
